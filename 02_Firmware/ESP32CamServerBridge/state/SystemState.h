#ifndef STATE_SYSTEM_STATE_H
#define STATE_SYSTEM_STATE_H

#include <Arduino.h>

struct SystemState {
  bool cameraReady = false;
  bool appReady = false;
  bool wifiConfigured = false;
  bool wifiConnected = false;
  unsigned long bootMillis = 0;
  unsigned long lastStatusLogMillis = 0;
  unsigned long lastWifiAttemptMillis = 0;
  String lastError;
  String localIp;
};

#endif
