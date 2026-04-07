#pragma once
#include <WebServer.h>
#include "bin_manager.h"

class WebHandler {
public:
  void begin(BinManager* manager);

private:
  WebServer server{80};
  BinManager* bm;

  void setupRoutes();
};