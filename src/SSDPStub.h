// SSDP Stub - Empty implementation when SSDP is disabled
#ifndef SSDPStub_h
#define SSDPStub_h

#ifndef ENABLE_SSDP

#include <Arduino.h>
#include <WiFi.h>

// Stub class that does nothing - used when ENABLE_SSDP is not defined
class SSDPClassStub {
 public:
  bool isStarted = false;

  bool begin() { return true; }
  void loop() {}
  void end() {}

  void setHTTPPort(uint16_t) {}
  void setSchemaURL(uint8_t, const char*) {}
  void setChipId(uint8_t, uint32_t) {}
  void setDeviceType(uint8_t, const char*) {}
  void setName(uint8_t, const char*) {}
  void setModelName(uint8_t, const char*) {}
  void setModelNumber(uint8_t, const char*) {}
  void setModelURL(uint8_t, const char*) {}
  void setManufacturer(uint8_t, const char*) {}
  void setManufacturerURL(uint8_t, const char*) {}
  void setURL(uint8_t, const char*) {}
  void setActive(uint8_t, bool) {}
  void schema(WiFiClient) {}
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SSDP)
extern SSDPClassStub SSDP;
#endif

#endif // ENABLE_SSDP

#endif // SSDPStub_h