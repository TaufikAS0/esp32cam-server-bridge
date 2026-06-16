#include "Settings.h"

#include <ESP.h>
#include <Preferences.h>

namespace {

constexpr char PREF_NAMESPACE[] = "solarcam";

AppConfig appConfig;

String chipSuffix() {
  char buffer[7];
  const uint64_t chipId = ESP.getEfuseMac();
  snprintf(buffer, sizeof(buffer), "%06llX", chipId & 0xFFFFFFULL);
  return String(buffer);
}

AppConfig defaultConfig() {
  AppConfig config;
  config.wifiSsid = "HardwareControl";
  config.wifiPassword = "jayaabadi100";
  config.backendBaseUrl = "http://192.168.1.100/api/v1";
  config.ingestApiKey = "";
  config.deviceId = "CP_Panel_Surya";
  config.lineId = "CP_Panel_Surya";
  config.otaHostname = "cp-panel-surya-cam";
  config.otaPassword = "";
  config.captureIntervalSec = 60;
  config.heartbeatIntervalSec = 30;
  config.jpegQuality = 12;
  config.frameSizeCode = 2;
  config.uploadEnabled = true;
  return config;
}

bool isFrameSizeValid(uint8_t code) {
  return code <= 5;
}

}  // namespace

String configNormalizeBaseUrl(const String &value) {
  String normalized = value;
  normalized.trim();

  while (normalized.endsWith("/")) {
    normalized.remove(normalized.length() - 1);
  }

  if (normalized.isEmpty()) {
    return defaultConfig().backendBaseUrl;
  }

  String lower = normalized;
  lower.toLowerCase();

  if (lower.endsWith("/api/v1")) {
    return normalized;
  }
  if (lower.endsWith("/api")) {
    return normalized + "/v1";
  }
  return normalized + "/api/v1";
}

String configSanitizeHostname(const String &value) {
  String input = value;
  input.trim();
  input.toLowerCase();

  String output;
  output.reserve(input.length());
  bool lastWasDash = false;

  for (size_t index = 0; index < input.length(); ++index) {
    const char current = input.charAt(index);
    const bool alphaNumeric =
        (current >= 'a' && current <= 'z') || (current >= '0' && current <= '9');

    if (alphaNumeric) {
      output += current;
      lastWasDash = false;
      continue;
    }

    if (!lastWasDash && output.length() > 0) {
      output += '-';
      lastWasDash = true;
    }
  }

  while (output.startsWith("-")) {
    output.remove(0, 1);
  }
  while (output.endsWith("-")) {
    output.remove(output.length() - 1);
  }

  if (output.isEmpty()) {
    output = "solar-cam";
  }

  if (output.length() > 63) {
    output.remove(63);
  }

  return output;
}

String configDefaultApSsid() {
  return "SolarCamSetup-" + chipSuffix();
}

String configApPassword() {
  return "jayaabadi100";
}

String configFrameSizeLabel(uint8_t code) {
  switch (code) {
    case 0:
      return "QVGA";
    case 1:
      return "VGA";
    case 2:
      return "SVGA";
    case 3:
      return "XGA";
    case 4:
      return "SXGA";
    case 5:
      return "UXGA";
    default:
      return "SVGA";
  }
}

void configBegin() {
  appConfig = defaultConfig();

  Preferences preferences;
  if (!preferences.begin(PREF_NAMESPACE, true)) {
    return;
  }

  appConfig.wifiSsid = preferences.getString("wifi_ssid", appConfig.wifiSsid);
  appConfig.wifiPassword = preferences.getString("wifi_pass", appConfig.wifiPassword);
  appConfig.backendBaseUrl = configNormalizeBaseUrl(
      preferences.getString("backend_url", appConfig.backendBaseUrl));
  appConfig.ingestApiKey = preferences.getString("api_key", appConfig.ingestApiKey);
  appConfig.deviceId = preferences.getString("device_id", appConfig.deviceId);
  appConfig.lineId = preferences.getString("line_id", appConfig.lineId);
  appConfig.otaHostname = configSanitizeHostname(
      preferences.getString("ota_host", appConfig.otaHostname));
  appConfig.otaPassword = preferences.getString("ota_pass", appConfig.otaPassword);
  appConfig.captureIntervalSec =
      preferences.getUInt("capture_sec", appConfig.captureIntervalSec);
  appConfig.heartbeatIntervalSec =
      preferences.getUInt("heartbeat_sec", appConfig.heartbeatIntervalSec);
  appConfig.jpegQuality = preferences.getUChar("jpeg_q", appConfig.jpegQuality);
  appConfig.frameSizeCode = preferences.getUChar("frame_size", appConfig.frameSizeCode);
  appConfig.uploadEnabled = preferences.getBool("upload_en", appConfig.uploadEnabled);

  preferences.end();

  if (appConfig.deviceId.isEmpty()) {
    appConfig.deviceId = defaultConfig().deviceId;
  }
  if (appConfig.lineId.isEmpty()) {
    appConfig.lineId = appConfig.deviceId;
  }
  if (appConfig.captureIntervalSec < 5) {
    appConfig.captureIntervalSec = 5;
  }
  if (appConfig.heartbeatIntervalSec < 5) {
    appConfig.heartbeatIntervalSec = 5;
  }
  if (appConfig.jpegQuality < 6 || appConfig.jpegQuality > 30) {
    appConfig.jpegQuality = defaultConfig().jpegQuality;
  }
  if (!isFrameSizeValid(appConfig.frameSizeCode)) {
    appConfig.frameSizeCode = defaultConfig().frameSizeCode;
  }
}

AppConfig &configGet() {
  return appConfig;
}

bool configSave(const AppConfig &updated, String &message) {
  AppConfig sanitized = updated;

  sanitized.wifiSsid.trim();
  sanitized.wifiPassword.trim();
  sanitized.backendBaseUrl = configNormalizeBaseUrl(sanitized.backendBaseUrl);
  sanitized.ingestApiKey.trim();
  sanitized.deviceId.trim();
  sanitized.lineId.trim();
  sanitized.otaHostname = configSanitizeHostname(sanitized.otaHostname);
  sanitized.otaPassword.trim();

  if (sanitized.deviceId.isEmpty()) {
    message = "Device ID cannot be empty.";
    return false;
  }

  if (sanitized.lineId.isEmpty()) {
    sanitized.lineId = sanitized.deviceId;
  }

  if (sanitized.captureIntervalSec < 5 || sanitized.captureIntervalSec > 86400) {
    message = "Capture interval must be between 5 and 86400 seconds.";
    return false;
  }

  if (sanitized.heartbeatIntervalSec < 5 || sanitized.heartbeatIntervalSec > 3600) {
    message = "Heartbeat interval must be between 5 and 3600 seconds.";
    return false;
  }

  if (sanitized.jpegQuality < 6 || sanitized.jpegQuality > 30) {
    message = "JPEG quality must be between 6 and 30.";
    return false;
  }

  if (!isFrameSizeValid(sanitized.frameSizeCode)) {
    message = "Frame size value is not valid.";
    return false;
  }

  Preferences preferences;
  if (!preferences.begin(PREF_NAMESPACE, false)) {
    message = "Failed to open Preferences storage.";
    return false;
  }

  preferences.putString("wifi_ssid", sanitized.wifiSsid);
  preferences.putString("wifi_pass", sanitized.wifiPassword);
  preferences.putString("backend_url", sanitized.backendBaseUrl);
  preferences.putString("api_key", sanitized.ingestApiKey);
  preferences.putString("device_id", sanitized.deviceId);
  preferences.putString("line_id", sanitized.lineId);
  preferences.putString("ota_host", sanitized.otaHostname);
  preferences.putString("ota_pass", sanitized.otaPassword);
  preferences.putUInt("capture_sec", sanitized.captureIntervalSec);
  preferences.putUInt("heartbeat_sec", sanitized.heartbeatIntervalSec);
  preferences.putUChar("jpeg_q", sanitized.jpegQuality);
  preferences.putUChar("frame_size", sanitized.frameSizeCode);
  preferences.putBool("upload_en", sanitized.uploadEnabled);
  preferences.end();

  appConfig = sanitized;
  message = "Settings saved successfully.";
  return true;
}

void configResetToDefaults(String &message) {
  AppConfig defaults = defaultConfig();
  if (configSave(defaults, message)) {
    message = "Defaults restored successfully.";
  }
}
