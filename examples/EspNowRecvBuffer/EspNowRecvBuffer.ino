#include <EspRightNow.h>
Esp_Now_ESP32 espnow;
uint8_t mac[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //Replace with sending board mac address

void setup() {
  // put your setup code here, to run once:
  espnow.begin((uint8_t*)mac);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Old version
  //if (espnow.cbuffer != "") {
  //  Serial.print(espnow.cbuffer);
  //  espnow.cbuffer = "";
  //}
  //delay(20);
  //Now is:
  if (espnow.available()) {
    Serial.print(espnow.readString(false)) //readString param oneSecondDelay: if true, it will wait one second before responce like standard Serial.readString
  }
  delay(10); //Small delay to prevent program from running too fast
}
