#pragma once
#include <WebServer.h>
#include <ArduinoJson.h>
#include <WebSocketsServer.h>
#include "bin_manager.h"

WebSocketsServer ws{81};

class WebHandler {
public:
  void begin(BinManager* manager);

private:
  WebServer server{80};
  BinManager* bm;

  void setupRoutes();
};