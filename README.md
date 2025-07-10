# About the EspRightNow library
This is a library deticated to making ESP-NOW technology on the ESP32... easier. Currently it only works on ESP32-based devices.
    It uses a single .h file and struct instead of class which requires two files which results in more efficient compiling/upload.
# Functions
The functions used in this library are similar to those used in the standard Arduino Serial library.
## Initialization
To initialize the ESP-NOW WiFi system, #include the EspRightNow library at the top, then underneath, before the Void Setup function, define an ESP Now object (e.g. Esp_Now_ESP32 esp_now) and your other board MAC address as a uint8_t mac[6] type pointer, and in the Void Setup function just call the begin function like esp_now.begin((uint8_t*)mac);.
## Sending functions
Sending functions are just like regular Arduino Serial functions (Serial.print() or Serial.println(), etc.) and if the payload is over 250 bytes, it will be sent in chunks. It supports most variable types (most integers, unsigned integers, Arduino strings, and C strings); These functions are blocking but they happen very fast anyway.
## Receiving functions
Receiving functions are also just like the regular Serial functions. It has functions: void flush(), int read(), String readString(bool oneSecondDelay = false), String readStringUntil(char terminator), size_t readBytes(char *buffer, size_t length /*to read all bytes use int available()*/, bool oneSecondDelay = false), size_t readBytesUntil(char* buffer, char terminator), and int available(). Do NOT manually change variables like 'mainBuffer' to prevent messing up data stored.
## Usage example
```
#include <EspRightNow.h>
Esp_Now_ESP32 espnow;
uint8_t mac[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //Replace with your other board's MAC address
uint32_t currentTime = 0;

void setup() {
    Serial.begin(115200);
    espnow.begin((uint8_t*)mac); //Start ESP-NOW
    Serial.println("ESP-NOW Test");
    currentTime = millis();
}
void loop() {
  if (millis() - currentTime >= 5000) {
    currentTime = millis();
    espnow.println("Hello World!");
  }
    if (espnow.available()) {
      Serial.println(espnow.readString(false));
    }
}
```
