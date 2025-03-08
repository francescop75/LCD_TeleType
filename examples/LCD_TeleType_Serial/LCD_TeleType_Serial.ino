#include <LCD_TeleType.h>

#define SERIAL_SPEED 2400
#define SERIAL_TIMEOUT 200

LCDTeleType lcd(0x27, 4, 20);

void setup() {

  Serial.begin(SERIAL_SPEED);
  Serial.setTimeout(SERIAL_TIMEOUT);

  while (!Serial) {
    // Not really necessary with onboard USB serial, but who knows...
  }

  lcd.begin();
  lcd.prompt();
}

void loop() {

  String buffer;

  if (Serial.available()) {

    while (Serial.available()) {

      buffer = Serial.readString();
      lcd.write(buffer);
    }
  }

  delay(50);
}
