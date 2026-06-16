#include "PortalServer.h"

#include "../../config/Settings.h"
#include "../../config/Version.h"
#include "../../drivers/camera/CameraDriver.h"
#include "../../modules/server_sync/ServerSync.h"
#include "../../state/SystemState.h"
#include "../network/WifiManager.h"
#include "../ota/OtaManager.h"

#include <Update.h>
#include <WebServer.h>

namespace {

WebServer server(HTTP_PORT);
bool webServerStarted = false;
bool otaUploadOk = false;
String otaUploadMessage = "No web OTA upload yet.";

void sendNoCacheHeaders() {
  server.sendHeader("Cache-Control", "no-store, max-age=0");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "0");
}

String htmlEscape(const String &value) {
  String output;
  output.reserve(value.length() + 8);

  for (size_t index = 0; index < value.length(); ++index) {
    const char current = value.charAt(index);
    switch (current) {
      case '&':
        output += "&amp;";
        break;
      case '<':
        output += "&lt;";
        break;
      case '>':
        output += "&gt;";
        break;
      case '\"':
        output += "&quot;";
        break;
      case '\'':
        output += "&#39;";
        break;
      default:
        output += current;
        break;
    }
  }

  return output;
}

String jsonEscape(const String &value) {
  String output;
  output.reserve(value.length() + 8);

  for (size_t index = 0; index < value.length(); ++index) {
    const char current = value.charAt(index);
    switch (current) {
      case '\"':
        output += "\\\"";
        break;
      case '\\':
        output += "\\\\";
        break;
      case '\n':
        output += "\\n";
        break;
      case '\r':
        output += "\\r";
        break;
      case '\t':
        output += "\\t";
        break;
      default:
        output += current;
        break;
    }
  }

  return output;
}

String statusClass(bool ok) {
  return ok ? "ok" : "warn";
}

String yesNo(bool value) {
  return value ? "Yes" : "No";
}

String uptimeText(unsigned long startedAtMs) {
  const unsigned long seconds = (millis() - startedAtMs) / 1000UL;
  const unsigned long hours = seconds / 3600UL;
  const unsigned long minutes = (seconds % 3600UL) / 60UL;
  const unsigned long secs = seconds % 60UL;

  char buffer[40];
  snprintf(buffer, sizeof(buffer), "%luh %lum %lus", hours, minutes, secs);
  return String(buffer);
}

String agoText(unsigned long atMs) {
  if (atMs == 0) {
    return "never";
  }

  const unsigned long seconds = (millis() - atMs) / 1000UL;
  if (seconds < 60) {
    return String(seconds) + "s ago";
  }
  if (seconds < 3600) {
    return String(seconds / 60UL) + "m ago";
  }
  return String(seconds / 3600UL) + "h ago";
}

String frameSizeOptions(uint8_t selectedCode) {
  String html;
  for (uint8_t code = 0; code <= 5; ++code) {
    html += "<option value=\"" + String(code) + "\"";
    if (code == selectedCode) {
      html += " selected";
    }
    html += ">" + configFrameSizeLabel(code) + "</option>";
  }
  return html;
}

String rootPageHtml() {
  const AppConfig &config = configGet();
  const RuntimeState &state = runtimeState();

  String html;
  html.reserve(16000);

  html += F(
      "<!DOCTYPE html><html><head><meta charset=\"UTF-8\">"
      "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">"
      "<title>ESP32-CAM Solar Monitor</title>"
      "<style>"
      "body{margin:0;font-family:Segoe UI,Arial,sans-serif;background:#0b1220;color:#e5eefb;}"
      ".wrap{max-width:1180px;margin:0 auto;padding:24px;}"
      ".hero,.card{background:rgba(13,22,40,.9);border:1px solid rgba(130,165,255,.18);"
      "border-radius:22px;box-shadow:0 18px 40px rgba(0,0,0,.28);}"
      ".hero{padding:24px 28px;margin-bottom:18px;}"
      ".hero h1{margin:0 0 6px;font-size:30px;}"
      ".hero p{margin:0;color:#9fb3d1;line-height:1.5;}"
      ".grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(240px,1fr));gap:16px;margin-bottom:16px;}"
      ".card{padding:18px;}"
      ".card h2{margin:0 0 10px;font-size:18px;}"
      ".kv{display:flex;justify-content:space-between;gap:12px;padding:6px 0;border-bottom:1px solid rgba(130,165,255,.12);}"
      ".kv:last-child{border-bottom:0;}"
      ".k{color:#89a0c5;}"
      ".v{font-weight:600;text-align:right;}"
      ".ok{color:#65d88f;}.warn{color:#ffd166;}.bad{color:#ff6b6b;}"
      ".actions{display:flex;flex-wrap:wrap;gap:12px;margin:18px 0;}"
      "button,.linkbtn{background:#3d7eff;color:#fff;border:0;border-radius:14px;padding:12px 16px;"
      "font-weight:700;cursor:pointer;text-decoration:none;display:inline-block;}"
      ".ghost{background:#1f2b43;}.danger{background:#c44536;}.accent{background:#00a896;}"
      ".layout{display:grid;grid-template-columns:1.1fr .9fr;gap:16px;align-items:start;}"
      ".panel{background:rgba(13,22,40,.9);border:1px solid rgba(130,165,255,.18);border-radius:22px;padding:18px;}"
      ".panel h2{margin:0 0 12px;font-size:20px;}"
      ".preview{width:100%;border-radius:18px;border:1px solid rgba(130,165,255,.18);background:#02060f;min-height:280px;object-fit:cover;}"
      "form.gridform{display:grid;grid-template-columns:repeat(auto-fit,minmax(220px,1fr));gap:14px;}"
      ".field{display:flex;flex-direction:column;gap:6px;}"
      "label{font-size:13px;color:#96abd0;font-weight:700;}"
      "input,select{padding:12px 14px;border-radius:12px;border:1px solid rgba(130,165,255,.18);"
      "background:#0f1a2c;color:#e5eefb;}"
      ".checkbox{display:flex;align-items:center;gap:10px;padding-top:24px;}"
      ".note{margin-top:12px;padding:12px 14px;border-radius:14px;background:rgba(61,126,255,.1);color:#c9d7ee;line-height:1.5;}"
      ".foot{margin-top:14px;color:#8ba0c4;font-size:13px;}"
      "@media (max-width: 960px){.layout{grid-template-columns:1fr;}}"
      "</style></head><body><div class=\"wrap\">");

  html += "<section class=\"hero\">";
  html += "<h1>ESP32-CAM Solar Monitor Uploader</h1>";
  html += "<p>Setup portal, backend snapshot sender, live preview, and OTA update in one device.</p>";
  html += "<div class=\"foot\">Firmware " + String(FW_VERSION) + " | Build " + FW_BUILD_DATE + " " +
          FW_BUILD_TIME + " | Uptime " + uptimeText(state.bootAtMs) + "</div>";
  html += "</section>";

  html += "<section class=\"grid\">";

  html += "<div class=\"card\"><h2>Network</h2>";
  html += "<div class=\"kv\"><span class=\"k\">Mode</span><span class=\"v\">" + wifiModeLabel() + "</span></div>";
  html += "<div class=\"kv\"><span class=\"k\">WiFi SSID</span><span class=\"v\">" +
          htmlEscape(state.stationSsid) + "</span></div>";
  html += "<div class=\"kv\"><span class=\"k\">Station IP</span><span class=\"v\">" +
          htmlEscape(state.stationIp) + "</span></div>";
  html += "<div class=\"kv\"><span class=\"k\">Setup AP</span><span class=\"v\">" +
          htmlEscape(state.accessPointSsid) + " / " + htmlEscape(state.accessPointIp) + "</span></div>";
  html += "</div>";

  html += "<div class=\"card\"><h2>Snapshot Upload</h2>";
  html += "<div class=\"kv\"><span class=\"k\">Upload enabled</span><span class=\"v " +
          statusClass(config.uploadEnabled) + "\">" + yesNo(config.uploadEnabled) + "</span></div>";
  html += "<div class=\"kv\"><span class=\"k\">Last result</span><span class=\"v " +
          statusClass(state.lastUploadOk) + "\">" + htmlEscape(state.lastUploadMessage) + "</span></div>";
  html += "<div class=\"kv\"><span class=\"k\">Last upload</span><span class=\"v\">" +
          agoText(state.lastUploadAtMs) + "</span></div>";
  html += "<div class=\"kv\"><span class=\"k\">Bytes</span><span class=\"v\">" +
          String(state.lastUploadBytes) + "</span></div>";
  html += "</div>";

  html += "<div class=\"card\"><h2>Heartbeat</h2>";
  html += "<div class=\"kv\"><span class=\"k\">Last result</span><span class=\"v " +
          statusClass(state.lastHeartbeatOk) + "\">" + htmlEscape(state.lastHeartbeatMessage) + "</span></div>";
  html += "<div class=\"kv\"><span class=\"k\">Last heartbeat</span><span class=\"v\">" +
          agoText(state.lastHeartbeatAtMs) + "</span></div>";
  html += "<div class=\"kv\"><span class=\"k\">Backend URL</span><span class=\"v\">" +
          htmlEscape(config.backendBaseUrl) + "</span></div>";
  html += "</div>";

  html += "<div class=\"card\"><h2>Camera & OTA</h2>";
  html += "<div class=\"kv\"><span class=\"k\">Camera</span><span class=\"v " +
          statusClass(state.cameraReady) + "\">" + htmlEscape(state.cameraMessage) + "</span></div>";
  html += "<div class=\"kv\"><span class=\"k\">Frame size</span><span class=\"v\">" +
          htmlEscape(cameraFrameSizeLabel()) + "</span></div>";
  html += "<div class=\"kv\"><span class=\"k\">Arduino OTA</span><span class=\"v " +
          statusClass(otaIsReady()) + "\">" + htmlEscape(otaMessage()) + "</span></div>";
  html += "</div>";

  html += "</section>";

  html += "<div class=\"actions\">";
  html += "<form method=\"POST\" action=\"/capture-now\"><button type=\"submit\">Capture and Upload Now</button></form>";
  html += "<form method=\"POST\" action=\"/restart\"><button class=\"ghost\" type=\"submit\">Restart Device</button></form>";
  html += "<form method=\"POST\" action=\"/reset-defaults\"><button class=\"danger\" type=\"submit\">Reset to Defaults</button></form>";
  html += "<a class=\"linkbtn accent\" href=\"/ota\">Open OTA Update</a>";
  html += "</div>";

  html += "<section class=\"layout\">";

  html += "<div class=\"panel\">";
  html += "<h2>Live Preview</h2>";
  html += "<img class=\"preview\" src=\"/camera/latest.jpg\" alt=\"Live preview\">";
  html += "<div class=\"note\">This preview grabs a fresh frame from the camera each time the page loads.</div>";
  html += "</div>";

  html += "<div class=\"panel\">";
  html += "<h2>Device Settings</h2>";
  html += "<form class=\"gridform\" method=\"POST\" action=\"/save\">";
  html += "<div class=\"field\"><label>WiFi SSID</label><input name=\"wifi_ssid\" value=\"" +
          htmlEscape(config.wifiSsid) + "\"></div>";
  html += "<div class=\"field\"><label>WiFi Password</label><input name=\"wifi_password\" type=\"password\" value=\"" +
          htmlEscape(config.wifiPassword) + "\"></div>";
  html += "<div class=\"field\"><label>Backend API Base URL</label><input name=\"backend_base_url\" value=\"" +
          htmlEscape(config.backendBaseUrl) + "\"></div>";
  html += "<div class=\"field\"><label>Ingest API Key</label><input name=\"ingest_api_key\" value=\"" +
          htmlEscape(config.ingestApiKey) + "\"></div>";
  html += "<div class=\"field\"><label>Device ID</label><input name=\"device_id\" value=\"" +
          htmlEscape(config.deviceId) + "\"></div>";
  html += "<div class=\"field\"><label>Line ID</label><input name=\"line_id\" value=\"" +
          htmlEscape(config.lineId) + "\"></div>";
  html += "<div class=\"field\"><label>Capture Interval (sec)</label><input name=\"capture_interval_sec\" type=\"number\" min=\"5\" max=\"86400\" value=\"" +
          String(config.captureIntervalSec) + "\"></div>";
  html += "<div class=\"field\"><label>Heartbeat Interval (sec)</label><input name=\"heartbeat_interval_sec\" type=\"number\" min=\"5\" max=\"3600\" value=\"" +
          String(config.heartbeatIntervalSec) + "\"></div>";
  html += "<div class=\"field\"><label>JPEG Quality</label><input name=\"jpeg_quality\" type=\"number\" min=\"6\" max=\"30\" value=\"" +
          String(config.jpegQuality) + "\"></div>";
  html += "<div class=\"field\"><label>Frame Size</label><select name=\"frame_size_code\">" +
          frameSizeOptions(config.frameSizeCode) + "</select></div>";
  html += "<div class=\"field\"><label>OTA Hostname</label><input name=\"ota_hostname\" value=\"" +
          htmlEscape(config.otaHostname) + "\"></div>";
  html += "<div class=\"field\"><label>OTA Password</label><input name=\"ota_password\" type=\"password\" value=\"" +
          htmlEscape(config.otaPassword) + "\"></div>";
  html += "<div class=\"checkbox\"><input id=\"upload_enabled\" name=\"upload_enabled\" type=\"checkbox\"";
  if (config.uploadEnabled) {
    html += " checked";
  }
  html += "><label for=\"upload_enabled\">Enable periodic snapshot upload</label></div>";
  html += "<div class=\"field\"><button type=\"submit\">Save Settings</button></div>";
  html += "</form>";
  html += "<div class=\"note\">If WiFi cannot connect, join <strong>" + htmlEscape(configDefaultApSsid()) +
          "</strong> with password <strong>" + htmlEscape(configApPassword()) +
          "</strong>, then open <strong>http://192.168.4.1/</strong>.</div>";
  html += "</div>";

  html += "</section>";
  html += "</div></body></html>";
  return html;
}

String resultPageHtml(const String &title, const String &message, bool success) {
  String html;
  html.reserve(2200);
  html += F(
      "<!DOCTYPE html><html><head><meta charset=\"UTF-8\">"
      "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">"
      "<title>ESP32-CAM Solar Monitor</title>"
      "<style>body{margin:0;background:#09111d;color:#e8f0fb;font-family:Segoe UI,Arial,sans-serif;}"
      ".wrap{min-height:100vh;display:grid;place-items:center;padding:24px;}"
      ".card{max-width:640px;background:#0f1a2c;border:1px solid rgba(130,165,255,.18);border-radius:24px;padding:28px;}"
      "h1{margin:0 0 10px;}p{line-height:1.6;color:#c5d4ef;}a{color:#7cb8ff;text-decoration:none;font-weight:700;}"
      ".ok{color:#65d88f;}.bad{color:#ff6b6b;}</style></head><body><div class=\"wrap\"><div class=\"card\">");
  html += "<h1 class=\"" + String(success ? "ok" : "bad") + "\">" + htmlEscape(title) + "</h1>";
  html += "<p>" + htmlEscape(message) + "</p>";
  html += "<p><a href=\"/\">Return to home</a></p>";
  html += "</div></div></body></html>";
  return html;
}

String statusJson() {
  const AppConfig &config = configGet();
  const RuntimeState &state = runtimeState();
  String json;
  json.reserve(1200);

  json += "{";
  json += "\"device_id\":\"" + jsonEscape(config.deviceId) + "\",";
  json += "\"wifi_mode\":\"" + jsonEscape(wifiModeLabel()) + "\",";
  json += "\"station_connected\":";
  json += state.stationConnected ? "true" : "false";
  json += ",";
  json += "\"station_ip\":\"" + jsonEscape(state.stationIp) + "\",";
  json += "\"ap_active\":";
  json += state.accessPointActive ? "true" : "false";
  json += ",";
  json += "\"ap_ip\":\"" + jsonEscape(state.accessPointIp) + "\",";
  json += "\"camera_ready\":";
  json += state.cameraReady ? "true" : "false";
  json += ",";
  json += "\"last_upload_ok\":";
  json += state.lastUploadOk ? "true" : "false";
  json += ",";
  json += "\"last_upload_message\":\"" + jsonEscape(state.lastUploadMessage) + "\",";
  json += "\"last_heartbeat_ok\":";
  json += state.lastHeartbeatOk ? "true" : "false";
  json += ",";
  json += "\"last_heartbeat_message\":\"" + jsonEscape(state.lastHeartbeatMessage) + "\"";
  json += "}";

  return json;
}

void handleRoot() {
  sendNoCacheHeaders();
  server.send(200, "text/html; charset=utf-8", rootPageHtml());
}

void handleStatusJson() {
  sendNoCacheHeaders();
  server.send(200, "application/json; charset=utf-8", statusJson());
}

void handleCameraPreview() {
  camera_fb_t *frame = nullptr;
  String message;
  if (!cameraCaptureFrame(frame, message)) {
    sendNoCacheHeaders();
    server.send(503, "text/plain; charset=utf-8", message);
    return;
  }

  sendNoCacheHeaders();
  server.setContentLength(frame->len);
  server.send(200, "image/jpeg", "");
  server.client().write(frame->buf, frame->len);
  cameraReleaseFrame(frame);
}

void handleCaptureNow() {
  String message;
  const bool ok = serverSyncCaptureNow(message);
  sendNoCacheHeaders();
  server.send(200, "text/html; charset=utf-8", resultPageHtml(
      ok ? "Snapshot upload complete" : "Snapshot upload failed", message, ok));
}

void handleSave() {
  AppConfig updated = configGet();
  updated.wifiSsid = server.arg("wifi_ssid");
  updated.wifiPassword = server.arg("wifi_password");
  updated.backendBaseUrl = server.arg("backend_base_url");
  updated.ingestApiKey = server.arg("ingest_api_key");
  updated.deviceId = server.arg("device_id");
  updated.lineId = server.arg("line_id");
  updated.captureIntervalSec = static_cast<uint16_t>(server.arg("capture_interval_sec").toInt());
  updated.heartbeatIntervalSec = static_cast<uint16_t>(server.arg("heartbeat_interval_sec").toInt());
  updated.jpegQuality = static_cast<uint8_t>(server.arg("jpeg_quality").toInt());
  updated.frameSizeCode = static_cast<uint8_t>(server.arg("frame_size_code").toInt());
  updated.otaHostname = server.arg("ota_hostname");
  updated.otaPassword = server.arg("ota_password");
  updated.uploadEnabled = server.hasArg("upload_enabled");

  String message;
  if (!configSave(updated, message)) {
    sendNoCacheHeaders();
    server.send(400, "text/html; charset=utf-8", resultPageHtml("Save failed", message, false));
    return;
  }

  sendNoCacheHeaders();
  server.send(200, "text/html; charset=utf-8",
              resultPageHtml("Settings saved", message + " The device will restart now.", true));
  delay(1200);
  ESP.restart();
}

void handleRestart() {
  sendNoCacheHeaders();
  server.send(200, "text/html; charset=utf-8",
              resultPageHtml("Restarting", "The device will restart now.", true));
  delay(1200);
  ESP.restart();
}

void handleResetDefaults() {
  String message;
  configResetToDefaults(message);
  sendNoCacheHeaders();
  server.send(200, "text/html; charset=utf-8",
              resultPageHtml("Defaults restored", message + " The device will restart now.", true));
  delay(1200);
  ESP.restart();
}

void handleOtaPage() {
  String html;
  html.reserve(4200);
  html += F(
      "<!DOCTYPE html><html><head><meta charset=\"UTF-8\">"
      "<meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">"
      "<title>ESP32-CAM OTA</title>"
      "<style>body{margin:0;background:#09111d;color:#e8f0fb;font-family:Segoe UI,Arial,sans-serif;}"
      ".wrap{max-width:760px;margin:0 auto;padding:24px;}.card{background:#0f1a2c;border:1px solid rgba(130,165,255,.18);"
      "border-radius:24px;padding:24px;}.btn{background:#3d7eff;color:#fff;border:0;border-radius:14px;padding:12px 16px;font-weight:700;cursor:pointer;}"
      "input[type=file]{width:100%;padding:12px;color:#e8f0fb;}a{color:#7cb8ff;text-decoration:none;font-weight:700;}"
      ".note{margin-top:14px;padding:12px 14px;border-radius:14px;background:rgba(61,126,255,.1);color:#c5d4ef;line-height:1.5;}</style>"
      "</head><body><div class=\"wrap\"><div class=\"card\">");
  html += "<h1>OTA Update</h1>";
  html += "<p>Upload a compiled ESP32 firmware <strong>.bin</strong> file. The board will reboot automatically after a successful update.</p>";
  html += "<p><strong>Arduino OTA:</strong> " + htmlEscape(otaMessage()) + "</p>";
  html += "<p><strong>Web OTA status:</strong> " + htmlEscape(otaUploadMessage) + "</p>";
  html += "<form method=\"POST\" action=\"/ota/upload\" enctype=\"multipart/form-data\">";
  html += "<input type=\"file\" name=\"firmware\" accept=\".bin\" required>";
  html += "<p><button class=\"btn\" type=\"submit\">Upload Firmware</button></p>";
  html += "</form>";
  html += "<div class=\"note\">Tip: browser OTA is easiest when you are on the same WiFi. Arduino OTA also works when the hostname is reachable on the LAN.</div>";
  html += "<p><a href=\"/\">Return to home</a></p>";
  html += "</div></div></body></html>";
  server.send(200, "text/html; charset=utf-8", html);
}

void handleOtaUploadDone() {
  sendNoCacheHeaders();
  server.send(200, "text/html; charset=utf-8",
              resultPageHtml(otaUploadOk ? "OTA upload complete" : "OTA upload failed",
                             otaUploadMessage, otaUploadOk));
  if (otaUploadOk) {
    delay(1500);
    ESP.restart();
  }
}

void handleOtaUploadData() {
  HTTPUpload &upload = server.upload();

  if (upload.status == UPLOAD_FILE_START) {
    otaUploadOk = Update.begin(UPDATE_SIZE_UNKNOWN);
    otaUploadMessage = otaUploadOk ? "Upload started." : "Unable to start OTA update.";
    return;
  }

  if (upload.status == UPLOAD_FILE_WRITE) {
    if (!otaUploadOk) {
      return;
    }
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
      otaUploadOk = false;
      otaUploadMessage = String("Write failed: ") + Update.errorString();
    }
    return;
  }

  if (upload.status == UPLOAD_FILE_END) {
    if (!otaUploadOk) {
      return;
    }
    otaUploadOk = Update.end(true);
    otaUploadMessage =
        otaUploadOk ? "Firmware uploaded successfully. Rebooting now."
                    : String("Finalize failed: ") + Update.errorString();
    return;
  }

  if (upload.status == UPLOAD_FILE_ABORTED) {
    otaUploadOk = false;
    otaUploadMessage = "Upload aborted.";
  }
}

void handleNotFound() {
  sendNoCacheHeaders();
  if (wifiIsAccessPointActive()) {
    server.sendHeader("Location", "http://192.168.4.1/", true);
    server.send(302, "text/plain", "");
    return;
  }
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

}  // namespace

void webPortalBegin() {
  if (webServerStarted) {
    return;
  }

  server.on("/", HTTP_GET, handleRoot);
  server.on("/status.json", HTTP_GET, handleStatusJson);
  server.on("/camera/latest.jpg", HTTP_GET, handleCameraPreview);
  server.on("/capture-now", HTTP_POST, handleCaptureNow);
  server.on("/save", HTTP_POST, handleSave);
  server.on("/restart", HTTP_POST, handleRestart);
  server.on("/reset-defaults", HTTP_POST, handleResetDefaults);
  server.on("/ota", HTTP_GET, handleOtaPage);
  server.on("/ota/upload", HTTP_POST, handleOtaUploadDone, handleOtaUploadData);
  server.onNotFound(handleNotFound);
  server.begin();
  webServerStarted = true;
}

void webPortalLoop() {
  if (webServerStarted) {
    server.handleClient();
  }
}
