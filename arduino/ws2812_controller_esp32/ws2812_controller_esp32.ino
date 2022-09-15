/*
*This example works for ESP32 and uses the NeoPixelBus library instead of the one bundle
*
*/
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <NeoPixelBus.h>

// Set to the number of LEDs in your LED strip
#define NUM_LEDS 60
// Maximum number of packets to hold in the buffer. Don't change this.
#define BUFFER_LEN 1024

//NeoPixelBus settings
const uint8_t PixelPin = 3;  // make sure to set this to the correct pin, ignored for Esp8266(set to 3 by default for DMA)

// Wifi and socket settings
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
unsigned int localPort = 7777;
char packetBuffer[BUFFER_LEN];

uint8_t N = 0;

WiFiUDP port;
// Network information
// IP must match the IP in config.py
IPAddress ip(192, 168, 0, 10);
// Set gateway to your router's gateway
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> ledstrip(NUM_LEDS, PixelPin);

void setup() {
    Serial.begin(115200);
    WiFi.config(ip, gateway, subnet);
    WiFi.begin(ssid, password);
    // Connect to wifi and print the IP address over serial
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    port.begin(localPort);
    ledstrip.Begin();//Begin output
    ledstrip.Show();//Clear the strip for use
}

void loop() {
    // Read data over socket
    int packetSize = port.parsePacket();
    // If packets have been received, interpret the command
    if (packetSize) {
        int len = port.read(packetBuffer, BUFFER_LEN);
        for (int j = 0)
        for(int i = 0; i < len; i+=4) {
            packetBuffer[len] = 0;
            N = packetBuffer[i];
            RgbColor pixel((uint8_t)packetBuffer[i+1], (uint8_t)packetBuffer[i+2], (uint8_t)packetBuffer[i+3]);//color
            ledstrip.SetPixelColor(N, pixel);//N is the pixel number
        }
        ledstrip.Show();
    }
        if (millis() - secondTimer >= 1000U) {
            secondTimer = millis();
            fpsCounter = 0;
        }
}
