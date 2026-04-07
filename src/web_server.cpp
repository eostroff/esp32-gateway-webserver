#include "web_server.h"

void WebHandler::begin(BinManager* manager) {
  bm = manager;

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

  server.on("/bins", HTTP_GET, [this]() {
    server.send(200, "application/json", bm->toJSON());
  });

  server.on("/tare", HTTP_POST, [this]() {
    int id = server.arg("id").toInt();
    bm->tare(id, bm->getBin(id)->raw_weight);
    server.send(200);
  });

  server.on("/config", HTTP_POST, [this]() {
    StaticJsonDocument<256> doc;
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
    StaticJsonDocument<256> doc;
    deserializeJson(doc, server.arg("plain"));

    int id = doc["id"];
    float known = doc["known_weight"];

    bm->calibrate(id, known, bm->getBin(id)->raw_weight);

    server.send(200);
  });

  server.on("/auto_wpi", HTTP_POST, [this]() {
    StaticJsonDocument<256> doc;
    deserializeJson(doc, server.arg("plain"));

    int id = doc["id"];
    int count = doc["count"];

    bm->autoWPI(id, count);
    server.send(200);
  });

  // Serve UI
  server.on("/", HTTP_GET, [this]() {
    server.send(200, "text/html", "<h1>Upload UI here</h1>");
  });

  server.onNotFound([this]() {
    server.send(404, "text/plain", "Not found");
  });
}