#include <Servo.h>

Servo miServo; // Creamos el objeto para controlar el servo

void setup() {
  miServo.attach(9); // Vinculamos el servo al pin digital 9
}

void loop() {
  // Gira de 0 a 180 grados progresivamente
  for (int pos = 0; pos <= 180; pos += 1) {
    miServo.write(pos);
    delay(15); // Espera 15 ms para que el motor alcance la posición
  }

  delay(1000); // Pausa de 1 segundo en 180°

  // Regresa de 180 a 0 grados
  for (int pos = 180; pos >= 0; pos -= 1) {
    miServo.write(pos);
    delay(15);
  }

  delay(1000); // Pausa de 1 segundo en 0°
}