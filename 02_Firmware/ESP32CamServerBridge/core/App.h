#ifndef CORE_APP_H
#define CORE_APP_H

#include <Arduino.h>

#include "comms/wifi/WiFiStation.h"
#include "config/Constants.h"
#include "drivers/camera/CameraDriver.h"
#include "state/SystemState.h"

class App {
 public:
  void begin() {
    Serial.begin(Constants::kSerialBaudRate);

    state_.bootMillis = millis();
    state_.lastStatusLogMillis = 0;

    Serial.println();
    Serial.println("ESP32CamServerBridge bring-up starting");
    Serial.print("Firmware version: ");
    Serial.println(Constants::kFirmwareVersion);

    state_.cameraReady = cameraDriver_.begin(&state_.lastError);
    state_.wifiConfigured = wifiStation_.isConfigured();
    wifiStation_.begin();
    state_.wifiConnected = wifiStation_.isConnected();
    state_.localIp = wifiStation_.localIp();
    state_.appReady = state_.cameraReady;

    if (state_.cameraReady) {
      Serial.println("Camera initialization: OK");
    } else {
      Serial.print("Camera initialization: FAILED - ");
      Serial.println(state_.lastError);
    }

    if (state_.wifiConfigured) {
      Serial.println("WiFi configuration: FOUND");
      if (state_.wifiConnected) {
        Serial.print("WiFi connected. IP: ");
        Serial.println(state_.localIp);
      } else {
        Serial.println("WiFi connection: IN PROGRESS");
      }
    } else {
      Serial.println("WiFi configuration: MISSING");
      Serial.println("Create config/SecretsLocal.h from config/SecretsLocal.example.h");
    }
  }

  void update() {
    const unsigned long now = millis();
    maintainWiFi_(now);
    wifiStation_.update();
    logStatusIfDue_(now);
  }

 private:
  void maintainWiFi_(unsigned long now) {
    state_.wifiConnected = wifiStation_.isConnected();
    state_.localIp = wifiStation_.localIp();

    if (!state_.wifiConfigured || state_.wifiConnected) {
      return;
    }

    if ((now - state_.lastWifiAttemptMillis) < Constants::kWifiConnectAttemptIntervalMs) {
      return;
    }

    state_.lastWifiAttemptMillis = now;
    Serial.println("Retrying WiFi connection...");
    wifiStation_.connect();
  }

  void logStatusIfDue_(unsigned long now) {
    if ((now - state_.lastStatusLogMillis) < Constants::kStatusLogIntervalMs) {
      return;
    }

    state_.lastStatusLogMillis = now;

    Serial.print("uptime_ms=");
    Serial.print(now - state_.bootMillis);
    Serial.print(", camera_ready=");
    Serial.print(state_.cameraReady ? "true" : "false");
    Serial.print(", wifi_configured=");
    Serial.print(state_.wifiConfigured ? "true" : "false");
    Serial.print(", wifi_connected=");
    Serial.print(state_.wifiConnected ? "true" : "false");
    Serial.print(", ip=");
    Serial.print(state_.wifiConnected ? state_.localIp : String("-"));
    Serial.print(", free_heap=");
    Serial.println(ESP.getFreeHeap());
  }

  CameraDriver cameraDriver_;
  WiFiStation wifiStation_;
  SystemState state_;
};

#endif
