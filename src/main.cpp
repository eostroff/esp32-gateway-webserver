#include <Arduino.h>
#include "bin_manager.h"
#include "espnow_handler.h"
#include "web_server.h"

BinManager binManager;
ESPNowHandler espNowHandler;
WebHandler webHandler;

void setup() {
  Serial.begin(115200);

  binManager.begin();
  espNowHandler.begin(&binManager);
  webHandler.begin(&binManager);
}

void loop() {
  webHandler.loop();
}