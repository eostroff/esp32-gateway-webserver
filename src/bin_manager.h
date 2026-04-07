#pragma once
#include <Arduino.h>
#include <Preferences.h>
#include <WebSocketsServer.h>
#include "config.h"

extern WebSocketsServer ws;

struct Bin {
  uint8_t id;
  String name;

  float raw_weight;

  long offset;
  float scale;

  float weight_per_item;
  int stock;

  bool online;
  unsigned long last_update;
};

class BinManager {
public:
  void begin();
  void updateWeight(uint8_t id, float raw);
  void tare(uint8_t id, long raw);
  void calibrate(uint8_t id, float known_weight, long raw);
  void autoWPI(uint8_t id, int count);
  void save(uint8_t id);

  String toJSON();
  Bin* getBin(uint8_t id);

private:
  Bin bins[MAX_BINS];
  Preferences prefs;

  void computeStock(Bin &b);
  void load();
};