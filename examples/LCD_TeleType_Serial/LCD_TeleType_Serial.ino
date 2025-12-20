/**
 * Whatever come in on the onboard USB serial, is forwarded to the LCD display.
 */

#include <LCD_TeleType.h>

#define SERIAL_SPEED 300
#define SERIAL_TIMEOUT 250

// LCDTeleType lcd(0x27, // LCD display I2C address.
//                 4,    // LCD display rows.
//                 20,   // LCD display columns.
//                 15,   // Bell simulation - flash interval.
//                 35    // Bell simulation - flash repetitions.
// );
LCDTeleType lcd(0x27, // LCD display I2C address.
                2,    // LCD display rows.
                40,   // LCD display columns.
                25,   // Bell simulation - flash interval.
                35    // Bell simulation - flash repetitions.
);

void setup() {

  Serial.begin(SERIAL_SPEED);
  Serial.setTimeout(SERIAL_TIMEOUT);

  while (!Serial) {
    // Not really necessary with onboard USB serial, but who knows...
    delay(500);
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
