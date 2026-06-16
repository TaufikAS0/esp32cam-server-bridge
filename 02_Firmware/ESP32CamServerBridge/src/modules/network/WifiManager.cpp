#include "WifiManager.h"

#include "../../config/Settings.h"
#include "../../state/SystemState.h"

#include <DNSServer.h>
#include <WiFi.h>

namespace {

DNSServer dnsServer;
bool accessPointActive = false;
unsigned long stationAttemptStartedAtMs = 0;
unsigned long lastReconnectAttemptAtMs = 0;

void refreshRuntimeNetworkState() {
  RuntimeState &state = runtimeState();
  state.stationConnected = WiFi.status() == WL_CONNECTED;
  state.stationSsid = state.stationConnected ? WiFi.SSID() : "-";
  state.stationIp = state.stationConnected ? WiFi.localIP().toString() : "-";
  state.stationRssi = state.stationConnected ? WiFi.RSSI() : 0;
  state.accessPointActive = accessPointActive;
  state.accessPointSsid = accessPointActive ? configDefaultApSsid() : "-";
  state.accessPointIp = accessPointActive ? WiFi.softAPIP().toString() : "-";
}

void startAccessPoint() {
  if (accessPointActive) {
    refreshRuntimeNetworkState();
    return;
  }

  WiFi.mode(WIFI_AP_STA);
  const String apSsid = configDefaultApSsid();
  const String apPassword = configApPassword();
  WiFi.softAP(apSsid.c_str(), apPassword.c_str());
  dnsServer.start(53, "*", WiFi.softAPIP());
  accessPointActive = true;
  refreshRuntimeNetworkState();
}

void stopAccessPoint() {
  if (!accessPointActive) {
    return;
  }

  dnsServer.stop();
  WiFi.softAPdisconnect(true);
  accessPointActive = false;
  refreshRuntimeNetworkState();
}

void beginStationConnection() {
  const AppConfig &config = configGet();
  if (config.wifiSsid.isEmpty()) {
    startAccessPoint();
    return;
  }

  WiFi.mode(accessPointActive ? WIFI_AP_STA : WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(false);
  WiFi.setSleep(false);
  const String host = configSanitizeHostname(config.otaHostname);
  WiFi.setHostname(host.c_str());
  WiFi.begin(config.wifiSsid.c_str(), config.wifiPassword.c_str());
  stationAttemptStartedAtMs = millis();
  lastReconnectAttemptAtMs = stationAttemptStartedAtMs;
}

}  // namespace

void wifiBegin() {
  WiFi.mode(WIFI_MODE_NULL);
  delay(100);
  beginStationConnection();
  refreshRuntimeNetworkState();
}

void wifiLoop() {
  if (accessPointActive) {
    dnsServer.processNextRequest();
  }

  const bool connected = WiFi.status() == WL_CONNECTED;
  refreshRuntimeNetworkState();

  if (connected) {
    stopAccessPoint();
    refreshRuntimeNetworkState();
    return;
  }

  if (!accessPointActive && stationAttemptStartedAtMs > 0 &&
      millis() - stationAttemptStartedAtMs >= WIFI_CONNECT_TIMEOUT_MS) {
    startAccessPoint();
  }

  if (millis() - lastReconnectAttemptAtMs >= WIFI_RETRY_INTERVAL_MS) {
    beginStationConnection();
  }
}

bool wifiIsConnected() {
  return WiFi.status() == WL_CONNECTED;
}

bool wifiIsAccessPointActive() {
  return accessPointActive;
}

String wifiStationIp() {
  return wifiIsConnected() ? WiFi.localIP().toString() : String("-");
}

String wifiAccessPointIp() {
  return accessPointActive ? WiFi.softAPIP().toString() : String("-");
}

String wifiModeLabel() {
  if (wifiIsConnected()) {
    return "Station";
  }
  if (accessPointActive) {
    return "Setup AP";
  }
  return "Connecting";
}
