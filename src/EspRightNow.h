#include <cstddef>
#include <WiFi.h>
#include <esp_now.h>
#include "stdint.h"

struct EspNowDataBuffer {
  String sBuffer;
  uint32_t readIndex = 0;  // Track current read position
  uint32_t writeIndex = 0; // Track current write position
  bool hasData = false;
};

struct Esp_Now_ESP32 {
    EspNowDataBuffer mainBuffer;
    esp_now_peer_info_t peerInfo;
    uint8_t* peerMacAddress;

    // Static wrapper to call the actual member function
    // Compatible with both old and new Arduino ESP32 core versions
    #if ESP_ARDUINO_VERSION >= ESP_ARDUINO_VERSION_VAL(2, 0, 0)
    // For Arduino ESP32 Core 2.x+ (uses esp_now_recv_info_t)
    static void on_data_recv_static(const esp_now_recv_info_t* recv_info, const uint8_t* data, int length) {
      if (instance != nullptr) {
        instance->on_data_recv(recv_info->src_addr, data, length);
      }
    }
    #else
    // For older Arduino ESP32 Core versions (direct MAC parameter)
    static void on_data_recv_static(const uint8_t* mac, const uint8_t* data, int length) {
      if (instance != nullptr) {
        instance->on_data_recv(mac, data, length);
      }
    }
    #endif
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
      // Add null terminator to ensure proper string handling
      char tempBuffer[length + 1];
      memcpy(tempBuffer, data, length);
      tempBuffer[length] = '\0';
      
      mainBuffer.sBuffer += String(tempBuffer);
      mainBuffer.writeIndex = mainBuffer.sBuffer.length();
      mainBuffer.hasData = true;
    }

  public:
    // Static pointer to the instance of the struct
    static Esp_Now_ESP32* instance;

    // Constructor to initialize the instance pointer
    Esp_Now_ESP32() {
      instance = this;
    }

    bool begin(uint8_t* peerMacAddr) {
      mainBuffer.sBuffer.reserve(200);  // Fixed typo: was "sbuffer"
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

    // Print functions with proper ESP-NOW sending
    void print(int val) {
      String string = String(val);
      sendData(string.c_str(), string.length());
    }
    void print(uint8_t val) {
      String string = String(val);
      sendData(string.c_str(), string.length());
    }
    void print(int8_t val) {
      String string = String(val);
      sendData(string.c_str(), string.length());
    }
    void print(uint16_t val) {
      String string = String(val);
      sendData(string.c_str(), string.length());
    }
    void print(int16_t val) {
      String string = String(val);
      sendData(string.c_str(), string.length());
    }
    void print(uint32_t val) {
      String string = String(val);
      sendData(string.c_str(), string.length());
    }
    // Remove conflicting long/unsigned long overloads on ESP32
    // since long and uint32_t are the same type
    #if !defined(ESP32)
    void print(long val) {
      String string = String(val);
      sendData(string.c_str(), string.length());
    }
    void print(unsigned long val) {
      String string = String(val);
      sendData(string.c_str(), string.length());
    }
    #endif
    void print(String val) {
      sendData(val.c_str(), val.length());
    }
    void print(const char* val) {
      sendData(val, strlen(val));
    }
    
    // Add println variants for convenience
    void println(int val) { print(val); print("\n"); }
    void println(uint8_t val) { print(val); print("\n"); }
    void println(int8_t val) { print(val); print("\n"); }
    void println(uint16_t val) { print(val); print("\n"); }
    void println(int16_t val) { print(val); print("\n"); }
    void println(uint32_t val) { print(val); print("\n"); }
    #if !defined(ESP32)
    void println(long val) { print(val); print("\n"); }
    void println(unsigned long val) { print(val); print("\n"); }
    #endif
    void println(String val) { print(val); print("\n"); }
    void println(const char* val) { print(val); print("\n"); }
    void println() { print("\n"); } // Just a newline

    // READ FUNCTIONS - Similar to Serial interface
    
    // Check if data is available to read
    int available() {
      return mainBuffer.writeIndex - mainBuffer.readIndex;
    }
    
    // Read a single character
    int read() {
      if (mainBuffer.readIndex >= mainBuffer.writeIndex) {
        return -1; // No data available
      }
      
      char c = mainBuffer.sBuffer.charAt(mainBuffer.readIndex);
      mainBuffer.readIndex++;
      
      // Clean up buffer if we've read everything
      if (mainBuffer.readIndex >= mainBuffer.writeIndex) {
        cleanupBuffer();
      }
      
      return (int)c;
    }
    
    // Read multiple characters into a buffer
    size_t readBytes(char* buffer, size_t length) {
      size_t bytesRead = 0;
      
      while (bytesRead < length && mainBuffer.readIndex < mainBuffer.writeIndex) {
        buffer[bytesRead] = mainBuffer.sBuffer.charAt(mainBuffer.readIndex);
        mainBuffer.readIndex++;
        bytesRead++;
      }
      
      // Clean up buffer if we've read everything
      if (mainBuffer.readIndex >= mainBuffer.writeIndex) {
        cleanupBuffer();
      }
      
      return bytesRead;
    }
    size_t readBytesUntil(const char* buffer, char terminator) {
      String baseResult = readStringUntil(terminator);
      size_t length = baseResult.length();
      buffer = baseResult.c_str();
      return length;
    }
    
    // Read until a terminator character is found
    String readStringUntil(char terminator) {
      String result = "";
      
      while (mainBuffer.readIndex < mainBuffer.writeIndex) {
        char c = mainBuffer.sBuffer.charAt(mainBuffer.readIndex);
        mainBuffer.readIndex++;
        
        if (c == terminator) {
          break;
        }
        result += c;
      }
      
      // Clean up buffer if we've read everything
      if (mainBuffer.readIndex >= mainBuffer.writeIndex) {
        cleanupBuffer();
      }
      
      return result;
    }
    
    // Read all available data as String
    String readString(bool oneSecondDelay = false) {
      if (oneSecondDelay) {
        delay(1000); // Mimic Serial.readString() behavior
      }
      
      String result = mainBuffer.sBuffer.substring(mainBuffer.readIndex);
      mainBuffer.readIndex = mainBuffer.writeIndex;
      
      cleanupBuffer();
      return result;
    }
    
    // Peek at the next character without consuming it
    int peek() {
      if (mainBuffer.readIndex >= mainBuffer.writeIndex) {
        return -1; // No data available
      }
      return (int)mainBuffer.sBuffer.charAt(mainBuffer.readIndex);
    }
    
    // Find a character in the buffer
    int find(char target) {
      for (uint32_t i = mainBuffer.readIndex; i < mainBuffer.writeIndex; i++) {
        if (mainBuffer.sBuffer.charAt(i) == target) {
          return i - mainBuffer.readIndex; // Return relative position
        }
      }
      return -1; // Not found
    }
    
    // Flush the buffer - clear all data
    void flush() {
      mainBuffer.sBuffer = "";
      mainBuffer.readIndex = 0;
      mainBuffer.writeIndex = 0;
      mainBuffer.hasData = false;
    }
    
private:
    // ESP-NOW maximum payload size is 250 bytes
    static const size_t ESP_NOW_MAX_PAYLOAD = 250;
    
    // Send data with proper chunking for large messages
    void sendData(const char* data, size_t length) {
      if (length == 0) return;
      
      size_t sent = 0;
      while (sent < length) {
        size_t chunkSize = min(length - sent, ESP_NOW_MAX_PAYLOAD);
        
        esp_err_t result = esp_now_send(
          (const uint8_t*)peerMacAddress, 
          (const uint8_t*)(data + sent), 
          chunkSize
        );
        
        if (result != ESP_OK) {
          // Handle error - you might want to add error callback here
          break;
        }
        
        sent += chunkSize;
        
        // Small delay between chunks to prevent overwhelming the receiver
        if (sent < length) {
          delay(10);
        }
      }
    }
    
    // Clean up the buffer by removing already read data
    void cleanupBuffer() {
      if (mainBuffer.readIndex > 0) {
        // Remove the read portion from the buffer
        mainBuffer.sBuffer = mainBuffer.sBuffer.substring(mainBuffer.readIndex);
        mainBuffer.writeIndex -= mainBuffer.readIndex;
        mainBuffer.readIndex = 0;
        
        // If buffer is empty, reset everything
        if (mainBuffer.writeIndex == 0) {
          mainBuffer.sBuffer = "";
          mainBuffer.hasData = false;
        }
      }
    }
};

// Initialize the static instance pointer
Esp_Now_ESP32* Esp_Now_ESP32::instance = nullptr;
