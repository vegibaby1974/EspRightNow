#include <EspRightNow.h>
Esp_Now_ESP32 espnow;
uint8_t mac[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00); //Replace with recieving board MAC address

void setup() {
  // put your setup code here, to run once:
  espnow.begin((uint8_t*)mac);
}

void loop() {
  // put your main code here, to run repeatedly:
  espnow.print(Hello World!!!); //Print a message
  delay(5000); //5 second delay in between message
}
