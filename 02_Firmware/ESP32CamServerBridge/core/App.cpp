#include "core/App.h"

#include <Arduino.h>

#include "config/Constants.h"

void App::begin() {
  Serial.begin(Constants::kSerialBaudRate);

  state_.bootMillis = millis();
  state_.lastStatusLogMillis = 0;

  Serial.println();
  Serial.println("ESP32CamServerBridge bring-up starting");
  Serial.print("Firmware version: ");
  Serial.println(Constants::kFirmwareVersion);

  state_.cameraReady = cameraDriver_.begin(&state_.lastError);
  state_.appReady = state_.cameraReady;

  if (state_.cameraReady) {
    Serial.println("Camera initialization: OK");
  } else {
    Serial.print("Camera initialization: FAILED - ");
    Serial.println(state_.lastError);
  }
}

void App::update() {
  const unsigned long now = millis();
  logStatusIfDue_(now);
}

void App::logStatusIfDue_(unsigned long now) {
  if ((now - state_.lastStatusLogMillis) < Constants::kStatusLogIntervalMs) {
    return;
  }

  state_.lastStatusLogMillis = now;

  Serial.print("uptime_ms=");
  Serial.print(now - state_.bootMillis);
  Serial.print(", camera_ready=");
  Serial.print(state_.cameraReady ? "true" : "false");
  Serial.print(", free_heap=");
  Serial.println(ESP.getFreeHeap());
}
