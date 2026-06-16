#include "core/App.h"

namespace {
App app;
}

void setup() {
  app.begin();
}

void loop() {
  app.update();
}
