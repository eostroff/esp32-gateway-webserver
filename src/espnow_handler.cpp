#include "espnow_handler.h"

BinManager* ESPNowHandler::bm = nullptr;

void ESPNowHandler::begin(BinManager* manager) {
  bm = manager;

  WiFi.mode(WIFI_AP_STA);
  esp_now_init();

  esp_now_register_recv_cb(onReceive);
}

void ESPNowHandler::onReceive(const uint8_t * mac, const uint8_t *data, int len) {
  SensorPacket pkt;
  memcpy(&pkt, data, sizeof(pkt));

  if (bm) {
    bm->updateWeight(pkt.bin_id, pkt.raw_weight);
  }
}