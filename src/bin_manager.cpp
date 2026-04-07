#include "bin_manager.h"

void BinManager::begin() {
  prefs.begin("bins", false);

  for (int i = 0; i < MAX_BINS; i++) {
    bins[i].id = i;
    bins[i].name = "Bin " + String(i);
    bins[i].scale = 1.0;
    bins[i].weight_per_item = 1.0;
  }

  load();
}

void BinManager::updateWeight(uint8_t id, float raw) {
  Bin &b = bins[id];
  b.raw_weight = raw;
  b.last_update = millis();
  b.online = true;

  computeStock(b);
}

void BinManager::computeStock(Bin &b) {
  float grams = (b.raw_weight - b.offset) / b.scale;
  float net = grams;

  if (b.weight_per_item > 0)
    b.stock = round(net / b.weight_per_item);
}

void BinManager::tare(uint8_t id, long raw) {
  bins[id].offset = raw;
  save(id);
}

void BinManager::calibrate(uint8_t id, float known_weight, long raw) {
  Bin &b = bins[id];
  b.scale = (raw - b.offset) / known_weight;
  save(id);
}

void BinManager::autoWPI(uint8_t id, int count) {
  Bin &b = bins[id];
  float grams = (b.raw_weight - b.offset) / b.scale;
  b.weight_per_item = grams / count;
  save(id);
}

Bin* BinManager::getBin(uint8_t id) {
  return &bins[id];
}

String BinManager::toJSON() {
  String json = "[";

  for (int i = 0; i < MAX_BINS; i++) {
    Bin &b = bins[i];

    if (i) json += ",";

    json += "{";
    json += "\"id\":" + String(b.id) + ",";
    json += "\"name\":\"" + b.name + "\",";
    json += "\"weight\":" + String(b.raw_weight) + ",";
    json += "\"stock\":" + String(b.stock) + ",";
    json += "\"weight_per_item\":" + String(b.weight_per_item) + ",";
    json += "\"online\":" + String(b.online ? "true" : "false");
    json += "}";
  }

  json += "]";
  return json;
}

void BinManager::save(uint8_t id) {
  Bin &b = bins[id];

  prefs.putFloat(("s" + String(id)).c_str(), b.scale);
  prefs.putLong(("o" + String(id)).c_str(), b.offset);
  prefs.putFloat(("w" + String(id)).c_str(), b.weight_per_item);
  prefs.putString(("n" + String(id)).c_str(), b.name);
}

void BinManager::load() {
  for (int i = 0; i < MAX_BINS; i++) {
    bins[i].scale = prefs.getFloat(("s" + String(i)).c_str(), 1.0);
    bins[i].offset = prefs.getLong(("o" + String(i)).c_str(), 0);
    bins[i].weight_per_item = prefs.getFloat(("w" + String(i)).c_str(), 1.0);
    bins[i].name = prefs.getString(("n" + String(i)).c_str(), "Bin " + String(i));
  }
}