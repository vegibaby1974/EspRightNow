#include <EspRightNow.h>
Esp_Now_ESP32 espnow;
uint8_t mac[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; //Replace with your recieving board MAC address - if one of your 6 number is 9c then you would put 0x9c and like that for the other numbers.
//This example can be paired with EspNowRecvBuffer on the other side of communication

void setup() {
  // put your setup code here, to run once:
  espnow.begin((uint8_t*)mac);
}

void loop() {
  // put your main code here, to run repeatedly:
  espnow.print("Hello World!");
  delay(5000);
}
