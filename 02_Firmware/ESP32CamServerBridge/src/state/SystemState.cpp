#include "SystemState.h"

namespace {

RuntimeState state;

}  // namespace

void runtimeStateBegin() {
  state.bootAtMs = millis();
  state.cameraReady = false;
  state.cameraMessage = "Camera has not been initialized yet.";

  state.stationConnected = false;
  state.stationSsid = "-";
  state.stationIp = "-";
  state.stationRssi = 0;

  state.accessPointActive = false;
  state.accessPointSsid = "-";
  state.accessPointIp = "-";

  state.otaReady = false;
  state.otaMessage = "Waiting for WiFi.";

  state.lastUploadOk = false;
  state.lastUploadMessage = "No snapshot has been uploaded yet.";
  state.lastUploadHttpCode = 0;
  state.lastUploadBytes = 0;
  state.lastUploadAtMs = 0;

  state.lastHeartbeatOk = false;
  state.lastHeartbeatMessage = "No heartbeat has been sent yet.";
  state.lastHeartbeatHttpCode = 0;
  state.lastHeartbeatAtMs = 0;
}

RuntimeState &runtimeState() {
  return state;
}
