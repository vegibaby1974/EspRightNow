#include <EspRightNow.h>
Esp_Now_ESP32 espnow;
uint8_t mac[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00); //Replace with recieving board MAC address

void setup() {
  // put your setup code here, to run once:
  espnow.begin((uint8_t*)mac);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  espnow.println("Hello World!!!"); //Print a message
  Serial.println("Message sent");
  delay(5000); //5 second delay in between message
}
