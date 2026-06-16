#include "ServerSync.h"

#include "../../comms/http/BackendHttpClient.h"

void serverSyncBegin() {
  backendClientBegin();
}

void serverSyncLoop() {
  backendClientLoop();
}

bool serverSyncCaptureNow(String &message) {
  return backendClientCaptureNow(message);
}

bool serverSyncSendHeartbeatNow(String &message) {
  return backendClientSendHeartbeatNow(message);
}
