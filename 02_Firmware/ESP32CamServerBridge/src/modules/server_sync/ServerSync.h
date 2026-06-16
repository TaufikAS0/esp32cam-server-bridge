#pragma once

#include <Arduino.h>

void serverSyncBegin();
void serverSyncLoop();
bool serverSyncCaptureNow(String &message);
bool serverSyncSendHeartbeatNow(String &message);
