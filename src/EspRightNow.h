#include <cstddef>
#include <WiFi.h>
#include <esp_now.h>
#include "stdint.h"

struct Esp_Now_ESP32 {
    String sbuffer;
    char* cBuffer = "";
    esp_now_peer_info_t peerInfo;
    uint8_t* peerMacAddress;

    // Static wrapper to call the actual member function
    static void on_data_recv_static(const uint8_t* mac, const uint8_t* data, int length) {
      if (instance != nullptr) {
        instance->on_data_recv(mac, data, length);
      }
    }
    static void on_data_send_static(const uint8_t* mac, esp_now_send_status_t result) {
      if (instance != nullptr) {
        instance->on_data_send(mac, result);
      }
    }
    void on_data_send(const uint8_t* mac, esp_now_send_status_t result) {
      //Nothing to do here
    }

    // Actual member function to handle received data
    void on_data_recv(const uint8_t* mac, const uint8_t* data, int length) {
      sbuffer += String((char*)data);
      cBuffer = (char*)sbuffer.c_str();
    }

  public:
    // Static pointer to the instance of the struct
    static Esp_Now_ESP32* instance;

    // Constructor to initialize the instance pointer
    Esp_Now_ESP32() {
      instance = this;
    }

    bool begin(uint8_t* peerMacAddr) {
      sbuffer.reserve(200);
      peerMacAddress = peerMacAddr;
      WiFi.mode(WIFI_STA);
      esp_err_t errData = esp_now_init();
      if (errData == ESP_ERR_ESPNOW_INTERNAL) {
        return false;
      }
      errData = esp_now_register_send_cb(on_data_send_static);
      if (errData != ESP_OK) {
        return false;
      }
      // Register the static wrapper as the callback
      memcpy(peerInfo.peer_addr, peerMacAddr, 6);
      peerInfo.channel = 0;  
      peerInfo.encrypt = false;
      errData = esp_now_add_peer(&peerInfo);
      if (errData != ESP_OK) {
        return false;
      }
      errData = esp_now_register_recv_cb(on_data_recv_static);
      if (errData != ESP_OK) {
        return false;
      }
      return true;
    }
    //Print with standard int aka int32_t aka 32 bit signed integer
    void print(int val) {
      String string = String(val);
      const char * actualData = string.c_str();
      esp_now_send((const uint8_t*)peerMacAddress, (const uint8_t*)actualData, 232);
    }
    //Print with 8 bit unsigned integer
    void print(uint8_t val) {
      String string = String(val);
      const char * actualData = string.c_str();
      esp_now_send((const uint8_t*)peerMacAddress, (const uint8_t*)actualData, 232);
    }
    //Print with 8 bit signed integer
    void print(int8_t val) {
      String string = String(val);
      const char * actualData = string.c_str();
      esp_now_send((const uint8_t*)peerMacAddress, (const uint8_t*)actualData, 232);
    }
    //Print with 16 bit unsigned integer
    void print(uint16_t val) {
      String string = String(val);
      const char * actualData = string.c_str();
      esp_now_send((const uint8_t*)peerMacAddress, (const uint8_t*)actualData, 232);
    }
    //Print with 16 bit signed integer
    void print(int16_t val) {
      String string = String(val);
      const char * actualData = string.c_str();
      esp_now_send((const uint8_t*)peerMacAddress, (const uint8_t*)actualData, 232);
    }
    //Print with 32 bit unsigned integer aka 'uint'
    void print(uint32_t val) {
      String string = String(val);
      const char * actualData = string.c_str();
      esp_now_send((const uint8_t*)peerMacAddress, (const uint8_t*)actualData, 232);
    }
    void print(long val) {
      String string = String(val);
      const char * actualData = string.c_str();
      esp_now_send((const uint8_t*)peerMacAddress, (const uint8_t*)actualData, 232);
    }
    void print(unsigned long val) {
      String string = String(val);
      const char * actualData = string.c_str();
      esp_now_send((const uint8_t*)peerMacAddress, (const uint8_t*)actualData, 232);
    }
    //Print software created unsigned 64 bit integers
    /*
    void print(uint64_t val) {
      String string = String(val);
      const char * actualData = string.c_str();
      esp_now_send((const uint8_t*)peerMacAddress, (const uint8_t*)actualData, 232);
    }
    //Print software created signed 64 bit integers
    void print(int64_t val) {
      String string = String(val);
      const char * actualData = string.c_str();
      esp_now_send((const uint8_t*)peerMacAddress, (const uint8_t*)actualData, 232);
    }
    */
    //Print arduino::String
    void print(String val) {
      String string = String(val);
      const char * actualData = string.c_str();
      esp_now_send((const uint8_t*)peerMacAddress, (const uint8_t*)actualData, 232);
    }
    //Print const char array
    void print(const char* val) {
      esp_now_send((const uint8_t*)peerMacAddress, (const uint8_t*)val, 232);
    }
    //READ to arduino::String
};

// Initialize the static instance pointer
Esp_Now_ESP32* Esp_Now_ESP32::instance = nullptr;