/*
  ECONOMY LAB — V1
  Tres ranuras físicas para insertar monedas.
  Cada ranura acciona un microswitch conectado a un pin digital.
  Arduino envía G, R o A por USB Serial a la página web.

  Conexiones (Arduino UNO):
    Ranura CRECIMIENTO:     microswitch entre D2 y GND
    Ranura REDISTRIBUCIÓN:  microswitch entre D3 y GND
    Ranura APOYO DIRECTO:   microswitch entre D4 y GND

  Se usa INPUT_PULLUP: no hacen falta resistencias externas.
  El switch debe cerrar el circuito al ser presionado.
*/

const byte PIN_GROWTH = 2;
const byte PIN_REDISTRIBUTE = 3;
const byte PIN_SUPPORT = 4;

const unsigned long DEBOUNCE_MS = 45;

struct Slot {
  byte pin;
  char code;
  int lastReading;
  int stableState;
  unsigned long lastChange;
};

Slot slots[] = {
  {PIN_GROWTH, 'G', HIGH, HIGH, 0},
  {PIN_REDISTRIBUTE, 'R', HIGH, HIGH, 0},
  {PIN_SUPPORT, 'A', HIGH, HIGH, 0}
};

const byte SLOT_COUNT = sizeof(slots) / sizeof(slots[0]);

void setup() {
  Serial.begin(9600);

  for (byte i = 0; i < SLOT_COUNT; i++) {
    pinMode(slots[i].pin, INPUT_PULLUP);
    int reading = digitalRead(slots[i].pin);
    slots[i].lastReading = reading;
    slots[i].stableState = reading;
    slots[i].lastChange = millis();
  }

  Serial.println("ECONOMY_LAB_READY");
}

void loop() {
  unsigned long now = millis();

  for (byte i = 0; i < SLOT_COUNT; i++) {
    int reading = digitalRead(slots[i].pin);

    if (reading != slots[i].lastReading) {
      slots[i].lastReading = reading;
      slots[i].lastChange = now;
    }

    if ((now - slots[i].lastChange) > DEBOUNCE_MS &&
        reading != slots[i].stableState) {
      slots[i].stableState = reading;

      // INPUT_PULLUP: LOW significa que el switch está presionado.
      if (slots[i].stableState == LOW) {
        Serial.println(slots[i].code);
      }
    }
  }
}
