#include "App.h"

#include "../config/Settings.h"
#include "../config/Version.h"
#include "../drivers/camera/CameraDriver.h"
#include "../modules/network/WifiManager.h"
#include "../modules/ota/OtaManager.h"
#include "../modules/portal/PortalServer.h"
#include "../modules/server_sync/ServerSync.h"
#include "../state/SystemState.h"

#include <Arduino.h>

void appSetup() {
  Serial.begin(115200);
  delay(250);

  Serial.println();
  Serial.print(FW_NAME);
  Serial.print(" ");
  Serial.print(FW_VERSION);
  Serial.print(" build ");
  Serial.print(FW_BUILD_DATE);
  Serial.print(" ");
  Serial.println(FW_BUILD_TIME);

  configBegin();
  runtimeStateBegin();
  cameraBegin();
  wifiBegin();
  otaBegin();
  serverSyncBegin();
  webPortalBegin();
}

void appLoop() {
  wifiLoop();
  otaLoop();
  serverSyncLoop();
  webPortalLoop();
  yield();
}
