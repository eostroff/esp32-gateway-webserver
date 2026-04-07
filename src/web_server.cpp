#include "web_server.h"

void WebHandler::begin(BinManager* manager) {
  bm = manager;

  WiFi.softAP("ESP32-Inventory", "12345678");

  setupRoutes();
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
    int id = server.arg("id").toInt();
    bm->getBin(id)->name = server.arg("name");
    bm->getBin(id)->weight_per_item = server.arg("weight_per_item").toFloat();
    server.send(200);
  });

  server.on("/calibrate", HTTP_POST, [this]() {
    int id = server.arg("id").toInt();
    float known = server.arg("known_weight").toFloat();
    bm->calibrate(id, known, bm->getBin(id)->raw_weight);
    server.send(200);
  });

  server.on("/auto_wpi", HTTP_POST, [this]() {
    int id = server.arg("id").toInt();
    int count = server.arg("count").toInt();
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