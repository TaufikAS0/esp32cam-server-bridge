#pragma once

#include <Arduino.h>

struct RuntimeState {
  unsigned long bootAtMs;
  bool cameraReady;
  String cameraMessage;

  bool stationConnected;
  String stationSsid;
  String stationIp;
  int stationRssi;

  bool accessPointActive;
  String accessPointSsid;
  String accessPointIp;

  bool otaReady;
  String otaMessage;

  bool lastUploadOk;
  String lastUploadMessage;
  int lastUploadHttpCode;
  size_t lastUploadBytes;
  unsigned long lastUploadAtMs;

  bool lastHeartbeatOk;
  String lastHeartbeatMessage;
  int lastHeartbeatHttpCode;
  unsigned long lastHeartbeatAtMs;
};

void runtimeStateBegin();
RuntimeState &runtimeState();
