/**
 * Whatever come in on the UDP port, is forwarded to the LCD display.
 */

#include <LCD_TeleType.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#define LED_GPIO 8

/*
 * WiFi setup.
 */
#define WIFI_ESSID "OSCR.wifi"
#define WIFI_KEY "1q2w3e.azsxdc"

/*
 * UDP setup.
 */
#define UDP_PORT 4675

/*
 * LCD display setup.
 */
LCDTeleType lcd(0x27, // LCD display I2C address.
                4,    // LCD display rows.
                20,   // LCD display columns.
                15,   // Bell simulation - flash interval.
                35    // Bell simulation - flash repetitions.
);
// LCDTeleType lcd(0x27, // LCD display I2C address.
//                 2,    // LCD display rows.
//                 40,   // LCD display columns.
//                 25,   // Bell simulation - flash interval.
//                 35    // Bell simulation - flash repetitions.
// );

/*
 * Globals.
 */
WiFiUDP udp;
char buffer[255] = {};

void setup() {

    lcd.begin();
    lcd.prompt();
    setupWiFi();
    setupUDP();

    // DEBUG
    // Serial.begin(9600);
    // while (!Serial) {
    //     ;
    // }
    // Serial.println("DEBUG");
}

void setupWiFi() {

    WiFi.begin(WIFI_ESSID, WIFI_KEY);
    WiFi.setAutoReconnect(true);

    lcd.write((char *)"Connecting");
    while (WiFi.status() != WL_CONNECTED) {

        lcd.write((char *)".");
        delay(2000);
        delay(1000);
    }

    lcd.write((char *)"\rIP: ");
    lcd.write(WiFi.localIP().toString());
    lcd.write('\n');
}

void setupUDP() {

    itoa(UDP_PORT, buffer, 10);
    udp.begin(UDP_PORT);
    lcd.write((char *)"Port: ");
    lcd.write(buffer);
    lcd.write('\n');
    memset(buffer, 0, 255);
    lcd.write(buffer);
}

void loop() {

    int bufferSize = udp.parsePacket();
    if (bufferSize) {

        int len = udp.read(buffer, 255);
        if (len > 0) {
            buffer[len] = 0;
        }
        lcd.write(buffer);

        // DEBUG
        // Serial.print("[");
        // Serial.print(buffer);
        // Serial.println("]");
    }

    delay(50);
}
