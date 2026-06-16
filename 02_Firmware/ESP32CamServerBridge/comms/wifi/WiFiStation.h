#ifndef COMMS_WIFI_WIFI_STATION_H
#define COMMS_WIFI_WIFI_STATION_H

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include "config/Constants.h"

#if __has_include("config/SecretsLocal.h")
#include "config/SecretsLocal.h"
#define HAS_WIFI_SECRETS 1
#else
namespace SecretsLocal {
constexpr char kWifiSsid[] = "";
constexpr char kWifiPassword[] = "";
}
#define HAS_WIFI_SECRETS 0
#endif

class WiFiStation {
 public:
  WiFiStation() : server_(80) {}

  bool isConfigured() const {
    return HAS_WIFI_SECRETS && strlen(SecretsLocal::kWifiSsid) > 0;
  }

  void begin() {
    if (!isConfigured()) {
      return;
    }

    WiFi.mode(WIFI_STA);
    WiFi.setHostname(Constants::kDeviceName);
    server_.on("/", [this]() {
      String body = "device=";
      body += Constants::kDeviceName;
      body += "\nstatus=ok";
      body += "\nip=";
      body += WiFi.localIP().toString();
      body += "\nrssi=";
      body += String(WiFi.RSSI());
      server_.send(200, "text/plain", body);
    });
    connect();
  }

  void connect() {
    if (!isConfigured()) {
      return;
    }

    WiFi.disconnect(false, true);
    WiFi.begin(SecretsLocal::kWifiSsid, SecretsLocal::kWifiPassword);
  }

  void update() {
    if (WiFi.status() == WL_CONNECTED) {
      if (!serverStarted_) {
        server_.begin();
        serverStarted_ = true;
      }
      server_.handleClient();
    }
  }

  bool isConnected() const {
    return WiFi.status() == WL_CONNECTED;
  }

  String localIp() const {
    return isConnected() ? WiFi.localIP().toString() : String("");
  }

  long rssi() const {
    return isConnected() ? WiFi.RSSI() : 0;
  }

 private:
  WebServer server_;
  bool serverStarted_ = false;
};

#endif
