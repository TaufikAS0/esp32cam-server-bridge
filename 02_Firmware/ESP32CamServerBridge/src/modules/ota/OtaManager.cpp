#include "OtaManager.h"

#include "../../config/Settings.h"
#include "../../state/SystemState.h"
#include "../network/WifiManager.h"

#include <ArduinoOTA.h>

namespace {

bool otaStarted = false;
String currentHostname = "solar-cam";
String currentMessage = "Waiting for WiFi.";

void configureOta() {
  const AppConfig &config = configGet();
  currentHostname = configSanitizeHostname(config.otaHostname);

  ArduinoOTA.setPort(OTA_PORT);
  ArduinoOTA.setHostname(currentHostname.c_str());
  ArduinoOTA.setMdnsEnabled(true);
  ArduinoOTA.setRebootOnSuccess(true);

  if (config.otaPassword.isEmpty()) {
    ArduinoOTA.setPassword(nullptr);
  } else {
    ArduinoOTA.setPassword(config.otaPassword.c_str());
  }

  ArduinoOTA.onStart([]() {
    runtimeState().otaMessage = "Arduino OTA upload in progress.";
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    const unsigned int percent = total == 0 ? 0 : (progress * 100U) / total;
    runtimeState().otaMessage = String("Arduino OTA upload: ") + percent + "%";
  });

  ArduinoOTA.onEnd([]() {
    runtimeState().otaMessage = "Arduino OTA upload complete. Rebooting.";
  });

  ArduinoOTA.onError([](ota_error_t error) {
    runtimeState().otaReady = false;
    runtimeState().otaMessage = String("Arduino OTA error: ") + error;
  });
}

}  // namespace

void otaBegin() {
  runtimeState().otaReady = false;
  runtimeState().otaMessage = currentMessage;
}

void otaLoop() {
  if (!wifiIsConnected()) {
    if (otaStarted) {
      ArduinoOTA.end();
      otaStarted = false;
    }
    runtimeState().otaReady = false;
    runtimeState().otaMessage = "Waiting for WiFi.";
    return;
  }

  if (!otaStarted) {
    configureOta();
    ArduinoOTA.begin();
    otaStarted = true;
    currentMessage = String("Arduino OTA ready at ") + currentHostname + ".local:" + OTA_PORT;
    runtimeState().otaReady = true;
    runtimeState().otaMessage = currentMessage;
  }

  ArduinoOTA.handle();
}

bool otaIsReady() {
  return otaStarted && wifiIsConnected();
}

String otaHostname() {
  return currentHostname;
}

String otaMessage() {
  return runtimeState().otaMessage;
}
