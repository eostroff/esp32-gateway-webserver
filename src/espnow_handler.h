#pragma once
#include <esp_now.h>
#include <WiFi.h>
#include "bin_manager.h"

struct SensorPacket {
  uint8_t bin_id;
  float raw_weight;
};

class ESPNowHandler {
public:
  void begin(BinManager* manager);

private:
  static BinManager* bm;
  static void onReceive(const uint8_t*, const uint8_t*, int);
};