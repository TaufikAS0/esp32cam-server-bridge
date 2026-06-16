#pragma once

#include <Arduino.h>
#include "Constants.h"

struct AppConfig {
  String wifiSsid;
  String wifiPassword;
  String backendBaseUrl;
  String ingestApiKey;
  String deviceId;
  String lineId;
  String otaHostname;
  String otaPassword;
  uint16_t captureIntervalSec;
  uint16_t heartbeatIntervalSec;
  uint8_t jpegQuality;
  uint8_t frameSizeCode;
  bool uploadEnabled;
};

void configBegin();
AppConfig &configGet();
bool configSave(const AppConfig &updated, String &message);
void configResetToDefaults(String &message);

String configNormalizeBaseUrl(const String &value);
String configSanitizeHostname(const String &value);
String configDefaultApSsid();
String configApPassword();
String configFrameSizeLabel(uint8_t code);
