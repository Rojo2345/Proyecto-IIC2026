/*
  EL PRECIO DEL PROGRESO — Firmware V1 calibrado
  Arduino Leonardo
  Servo crecimiento: D9
  Servo desigualdad (Gini): D10

  Protocolo USB Serial a 9600 baudios:
    La página envía: crecimiento,gini\n
    Ejemplo: 3.60,0.385

  El firmware responde:
    READY,2_SERVOS
    OK,anguloCrecimiento,anguloGini

  IMPORTANTE:
  - Alimenta los dos MG90S desde una fuente regulada externa de 5 V,
    idealmente de 2 A o más (según las especificaciones de los servos).
  - Une GND de la fuente externa con GND del Arduino.
  - NO alimentes ambos servos desde el pin 5V del Leonardo.
  - Conecta los cables de señal: crecimiento a D9 y Gini a D10.
*/

#include <Servo.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const byte PIN_SERVO_GROWTH = 9;
const byte PIN_SERVO_GINI = 10;

Servo growthServo;
Servo giniServo;

char inputBuffer[48];
byte inputIndex = 0;

// Puntos medidos físicamente en el tablero.
// Los valores de entrada están ordenados de menor a mayor.
const float growthValues[] = {
  0.0, 1.0, 2.0, 2.5, 3.0, 4.0, 5.0
};
const float growthAngles[] = {
  170.0, 140.0, 110.0, 90.0, 70.0, 45.0, 15.0
};

const float giniValues[] = {
  0.30, 0.35, 0.40, 0.45, 0.50
};
const float giniAngles[] = {
  170.0, 143.0, 90.0, 45.0, 9.0
};

const byte GROWTH_POINTS = sizeof(growthValues) / sizeof(growthValues[0]);
const byte GINI_POINTS = sizeof(giniValues) / sizeof(giniValues[0]);

float interpolateAngle(
  float value,
  const float values[],
  const float angles[],
  byte count
) {
  if (value <= values[0]) return angles[0];
  if (value >= values[count - 1]) return angles[count - 1];

  for (byte i = 0; i < count - 1; i++) {
    if (value <= values[i + 1]) {
      float ratio = (value - values[i]) / (values[i + 1] - values[i]);
      return angles[i] + ratio * (angles[i + 1] - angles[i]);
    }
  }

  return angles[count - 1];
}

void moveGauges(float growth, float gini) {
  // Limitar los datos al rango físico de las escalas.
  growth = constrain(growth, 0.0, 5.0);
  gini = constrain(gini, 0.30, 0.50);

  int growthAngle = (int)round(
    interpolateAngle(growth, growthValues, growthAngles, GROWTH_POINTS)
  );

  int giniAngle = (int)round(
    interpolateAngle(gini, giniValues, giniAngles, GINI_POINTS)
  );

  growthServo.write(growthAngle);
  giniServo.write(giniAngle);

  // Respuesta que reconoce la página web.
  Serial.print("OK,");
  Serial.print(growthAngle);
  Serial.print(",");
  Serial.println(giniAngle);
}

void processLine(char *line) {
  // En Arduino AVR, sscanf con %f puede no estar habilitado.
  // Separamos la línea por la coma y convertimos cada campo con strtod.
  char *comma = strchr(line, ',');

  if (comma == NULL) {
    Serial.println("ERR,FORMATO,FALTA_COMA");
    return;
  }

  *comma = '\0';

  char *endGrowth;
  char *endGini;

  float growthValue = (float)strtod(line, &endGrowth);
  float giniValue = (float)strtod(comma + 1, &endGini);

  // Ignorar espacios al final de cada campo.
  while (*endGrowth == ' ' || *endGrowth == '\t') endGrowth++;
  while (*endGini == ' ' || *endGini == '\t') endGini++;

  if (endGrowth == line || endGini == comma + 1 ||
      *endGrowth != '\0' || *endGini != '\0') {
    Serial.print("ERR,FORMATO,");
    Serial.println(line);
    return;
  }

  moveGauges(growthValue, giniValue);
}

void setup() {
  Serial.begin(9600);

  growthServo.attach(PIN_SERVO_GROWTH);
  giniServo.attach(PIN_SERVO_GINI);

  // Posición inicial centrada: crecimiento 2,5 y Gini 0,40.
  moveGauges(2.5, 0.40);

  delay(300);
  Serial.println("READY,2_SERVOS");
}

void loop() {
  while (Serial.available() > 0) {
    char incoming = (char)Serial.read();

    if (incoming == '\r') {
      continue;
    }

    if (incoming == '\n') {
      inputBuffer[inputIndex] = '\0';

      if (inputIndex > 0) {
        processLine(inputBuffer);
      }

      inputIndex = 0;
    } else {
      if (inputIndex < sizeof(inputBuffer) - 1) {
        inputBuffer[inputIndex++] = incoming;
      } else {
        inputIndex = 0;
        Serial.println("ERR,LINEA_DEMASIADO_LARGA");
      }
    }
  }
}
