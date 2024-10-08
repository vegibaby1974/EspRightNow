# EspRightNow
    This is a library deticated to making ESP-NOW technology on the ESP32... easier.
    It uses a single .h file and struct instead of class which requires two files which results in more efficient compiling/upload.
    It is like Serial with functions:
##    Esp_Now_ESP32::begin(uint8_t* macAddress)
    This function will initiate ESPNOW on ESP32, add the peer enable recieving / sending buffer.
### Parameters
    Parameter 'uint8_t* macAddress' is a 6 byte CHARACTER array. If using uint8_t mac[] {} kind then you have to do ___.begin((uint8_t*)uint8_t[]); The next version will support that varible type.
### Returns
Will return a true/false bool type;
##    Esp_Now_ESP32::print(parameter type will vary)
    This function prints output to the other ESP NOW board;
### Parameters
    The parameter varies including unsigned integers from 8 - 32 bits, signed integers 8-32 bits, Arduino String, and char* character array.
### Returns
Returns a void, check if connected functionallity not implemented yet.
## Esp_Now_ESP32::cbuffer
    This is a field which is a buffer that holds the data the OTHER esp32 board has sent to the current board. It is in the type of char array (char*)
    WARNING 'while (____.cbuffer == "")' will block the ESPNOW buffer from working and will never break out of the while loop
## Esp_Now_ESP32::sbuffer
    This is a field which is like 'cbuffer' but is in Arduino String format.
