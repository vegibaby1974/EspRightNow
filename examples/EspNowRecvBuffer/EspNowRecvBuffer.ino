#include <EspRightNow.h>
Esp_Now_ESP32 espnow;
uint8_t mac[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //Replace with sending board mac address

void setup() {
  // put your setup code here, to run once:
  espnow.begin((uint8_t*)mac);
  Serial.begin(115200, SERIAL_8N1, RX, TX, false, 20000UL);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (espnow.cbuffer != "") {
    Serial.print(espnow.cbuffer);
    espnow.cbuffer = "";
  }
  delay(20);
}
