#include "web_server.h"
#include "bin_manager.h"
#include <LittleFS.h>

WebSocketsServer ws{81};

void WebHandler::begin(BinManager* manager) {
  bm = manager;

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("ESP32-Inventory", "12345678");

  setupRoutes();

  ws.begin();
  ws.onEvent([this](uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    if (type == WStype_CONNECTED) {
      String json = bm->toJSON();
      ws.sendTXT(num, (const uint8_t*)json.c_str(), json.length());
    }
  });

  server.begin();
}

void WebHandler::setupRoutes() {

  if (LittleFS.begin()) {
    server.serveStatic("/", LittleFS, "/index.html");
  } else {
    server.on("/", HTTP_GET, [this]() {
      server.send(500, "text/plain", "LittleFS mount failed");
    });
  }

  server.on("/bins", HTTP_GET, [this]() {
    server.send(200, "application/json", bm->toJSON());
  });

  server.on("/tare", HTTP_POST, [this]() {
    int id = server.arg("id").toInt();
    bm->tare(id, bm->getBin(id)->raw_weight);
    server.send(200);
  });

  server.on("/config", HTTP_POST, [this]() {
    JsonDocument doc;
    deserializeJson(doc, server.arg("plain"));

    int id = doc["id"];
    String name = doc["name"];
    float wpi = doc["weight_per_item"];

    Bin* b = bm->getBin(id);
    b->name = name;
    b->weight_per_item = wpi;

    bm->save(id);

    server.send(200, "text/plain", "OK");
  });

  server.on("/calibrate", HTTP_POST, [this]() {
    JsonDocument doc;
    deserializeJson(doc, server.arg("plain"));

    int id = doc["id"];
    float known = doc["known_weight"];

    bm->calibrate(id, known, bm->getBin(id)->raw_weight);

    server.send(200);
  });

  server.on("/auto_wpi", HTTP_POST, [this]() {
    JsonDocument doc;
    deserializeJson(doc, server.arg("plain"));

    int id = doc["id"];
    int count = doc["count"];

    bm->autoWPI(id, count);
    server.send(200);
  });

  server.onNotFound([this]() {
    server.send(404, "text/plain", "Not found");
  });
}

void WebHandler::loop() {
  server.handleClient();
  ws.loop();
}