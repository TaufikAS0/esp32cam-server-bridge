#include "BackendHttpClient.h"

#include "../../config/Settings.h"
#include "../../drivers/camera/CameraDriver.h"
#include "../../state/SystemState.h"
#include "../../modules/network/WifiManager.h"

#include <HTTPClient.h>
#include <WiFiClient.h>

namespace {

unsigned long lastSnapshotAttemptAtMs = 0;
unsigned long lastHeartbeatAttemptAtMs = 0;
bool firstSnapshotPending = true;
bool firstHeartbeatPending = true;

String jsonEscape(const String &value) {
  String escaped;
  escaped.reserve(value.length() + 8);

  for (size_t index = 0; index < value.length(); ++index) {
    const char current = value.charAt(index);
    switch (current) {
      case '\"':
        escaped += "\\\"";
        break;
      case '\\':
        escaped += "\\\\";
        break;
      case '\n':
        escaped += "\\n";
        break;
      case '\r':
        escaped += "\\r";
        break;
      case '\t':
        escaped += "\\t";
        break;
      default:
        escaped += current;
        break;
    }
  }

  return escaped;
}

String urlEncode(const String &value) {
  String encoded;
  char buffer[4];

  for (size_t index = 0; index < value.length(); ++index) {
    const uint8_t current = static_cast<uint8_t>(value.charAt(index));
    const bool isSafe =
        (current >= 'a' && current <= 'z') || (current >= 'A' && current <= 'Z') ||
        (current >= '0' && current <= '9') || current == '-' || current == '_' ||
        current == '.' || current == '~';

    if (isSafe) {
      encoded += static_cast<char>(current);
      continue;
    }

    snprintf(buffer, sizeof(buffer), "%%%02X", current);
    encoded += buffer;
  }

  return encoded;
}

String normalizedBaseUrl() {
  return configNormalizeBaseUrl(configGet().backendBaseUrl);
}

String deviceId() {
  return configGet().deviceId;
}

String lineId() {
  const String configuredLineId = configGet().lineId;
  return configuredLineId.isEmpty() ? deviceId() : configuredLineId;
}

void addSharedHeaders(HTTPClient &http) {
  http.setConnectTimeout(8000);
  http.setTimeout(15000);
  http.addHeader("Accept", "application/json");

  if (!configGet().ingestApiKey.isEmpty()) {
    http.addHeader("X-API-Key", configGet().ingestApiKey);
  }
}

void recordUploadResult(bool ok, const String &message, int httpCode, size_t bytes) {
  RuntimeState &state = runtimeState();
  state.lastUploadOk = ok;
  state.lastUploadMessage = message;
  state.lastUploadHttpCode = httpCode;
  state.lastUploadBytes = bytes;
  state.lastUploadAtMs = millis();
}

void recordHeartbeatResult(bool ok, const String &message, int httpCode) {
  RuntimeState &state = runtimeState();
  state.lastHeartbeatOk = ok;
  state.lastHeartbeatMessage = message;
  state.lastHeartbeatHttpCode = httpCode;
  state.lastHeartbeatAtMs = millis();
}

String buildHeartbeatPayload() {
  const RuntimeState &state = runtimeState();
  String payload;
  payload.reserve(512);

  payload += "{";
  payload += "\"schema_version\":\"solar-backend-r01\",";
  payload += "\"source_mode\":\"esp32\",";
  payload += "\"device_id\":\"" + jsonEscape(deviceId()) + "\",";
  payload += "\"line_id\":\"" + jsonEscape(lineId()) + "\",";
  payload += "\"running\":true,";
  payload += "\"active_scenario\":\"camera_snapshot_uploader\",";
  payload += "\"backend_base_url\":\"" + jsonEscape(normalizedBaseUrl()) + "\",";
  payload += "\"last_send_ok\":";
  payload += state.lastUploadOk ? "true" : "false";
  payload += ",";
  payload += "\"last_send_at_ms\":";
  payload += String(state.lastUploadAtMs == 0 ? millis() : state.lastUploadAtMs);
  payload += "}";

  return payload;
}

bool intervalElapsed(unsigned long lastAtMs, unsigned long intervalMs) {
  return lastAtMs == 0 || millis() - lastAtMs >= intervalMs;
}

}  // namespace

void backendClientBegin() {
  firstSnapshotPending = true;
  firstHeartbeatPending = true;
}

void backendClientLoop() {
  const AppConfig &config = configGet();
  if (!wifiIsConnected()) {
    return;
  }

  if (firstHeartbeatPending ||
      intervalElapsed(lastHeartbeatAttemptAtMs, static_cast<unsigned long>(config.heartbeatIntervalSec) * 1000UL)) {
    String heartbeatMessage;
    backendClientSendHeartbeatNow(heartbeatMessage);
    firstHeartbeatPending = false;
  }

  if (config.uploadEnabled &&
      (firstSnapshotPending ||
       intervalElapsed(lastSnapshotAttemptAtMs, static_cast<unsigned long>(config.captureIntervalSec) * 1000UL))) {
    String snapshotMessage;
    backendClientCaptureNow(snapshotMessage);
    firstSnapshotPending = false;
  }
}

bool backendClientCaptureNow(String &message) {
  lastSnapshotAttemptAtMs = millis();

  if (!configGet().uploadEnabled) {
    message = "Snapshot upload is disabled.";
    recordUploadResult(false, message, 0, 0);
    return false;
  }

  if (!wifiIsConnected()) {
    message = "WiFi is not connected.";
    recordUploadResult(false, message, 0, 0);
    return false;
  }

  camera_fb_t *frame = nullptr;
  String captureMessage;
  if (!cameraCaptureFrame(frame, captureMessage)) {
    message = captureMessage;
    recordUploadResult(false, message, 0, 0);
    return false;
  }

  WiFiClient client;
  HTTPClient http;
  const String url = normalizedBaseUrl() + "/devices/" + urlEncode(deviceId()) + "/camera-snapshot";
  http.begin(client, url);
  addSharedHeaders(http);
  http.addHeader("Content-Type", "image/jpeg");

  const int httpCode = http.POST(frame->buf, frame->len);
  const bool ok = httpCode >= 200 && httpCode < 300;
  String resultMessage;

  if (ok) {
    resultMessage =
        String("Snapshot uploaded successfully. HTTP ") + httpCode + ", " + frame->len + " bytes.";
  } else if (httpCode > 0) {
    resultMessage = String("Snapshot upload failed. HTTP ") + httpCode + ".";
  } else {
    resultMessage = String("Snapshot upload failed: ") + http.errorToString(httpCode);
  }

  recordUploadResult(ok, resultMessage, httpCode, frame->len);
  message = resultMessage;

  http.end();
  cameraReleaseFrame(frame);
  return ok;
}

bool backendClientSendHeartbeatNow(String &message) {
  lastHeartbeatAttemptAtMs = millis();

  if (!wifiIsConnected()) {
    message = "WiFi is not connected.";
    recordHeartbeatResult(false, message, 0);
    return false;
  }

  WiFiClient client;
  HTTPClient http;
  const String url = normalizedBaseUrl() + "/devices/" + urlEncode(deviceId()) + "/heartbeat";
  http.begin(client, url);
  addSharedHeaders(http);
  http.addHeader("Content-Type", "application/json");

  const String payload = buildHeartbeatPayload();
  const int httpCode = http.POST(payload);
  const bool ok = httpCode >= 200 && httpCode < 300;

  if (ok) {
    message = String("Heartbeat sent successfully. HTTP ") + httpCode + ".";
  } else if (httpCode > 0) {
    message = String("Heartbeat failed. HTTP ") + httpCode + ".";
  } else {
    message = String("Heartbeat failed: ") + http.errorToString(httpCode);
  }

  recordHeartbeatResult(ok, message, httpCode);
  http.end();
  return ok;
}
