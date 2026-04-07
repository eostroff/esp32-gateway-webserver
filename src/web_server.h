#pragma once
#include <WebServer.h>
#include <ArduinoJson.h>
#include <WebSocketsServer.h>
#include <WiFi.h>

class BinManager;

extern WebSocketsServer ws;

class WebHandler {
public:
  void begin(BinManager* manager);
  void loop();

private:
  WebServer server{80};
  BinManager* bm;

  void setupRoutes();
};