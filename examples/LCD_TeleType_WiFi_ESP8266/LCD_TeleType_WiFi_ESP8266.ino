/**
 * Whatever come in on the onboard USB serial, is forwarded to the LCD display.
 */

#include <ESP8266WiFi.h>
#include <LCD_TeleType.h>
#include <WiFiUdp.h>

/*
 * WiFi setup.
 */
#define WIFI_ESSID "<ESSID>"
#define WIFI_KEY "<password>"

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
bool firstRun = true;

void setup() {

    lcd.begin();
    lcd.prompt();
    setupWiFi();
    setupUDP();

    // Serial.begin(9600);
    // Serial.println("DEBUG CONSOLE");
}

void setupWiFi() {

    WiFi.begin(WIFI_ESSID, WIFI_KEY);
    WiFi.setAutoReconnect(true);

    // lcd.write((char *)"Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        lcd.write((char *)"...");
        delay(1000);
        lcd.write((char *)"\b\b\b");
    }

    lcd.write((char *)"IP: ");
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

        if (firstRun) {
            firstRun = false;
            lcd.write('\f');
        }

        int len = udp.read(buffer, 255);
        if (len > 0) {
            buffer[len] = 0;
        }
        lcd.write(buffer);
        
        // Serial.print("[");
        // Serial.print(buffer);
        // Serial.println("]");
    }

    delay(50);
}
