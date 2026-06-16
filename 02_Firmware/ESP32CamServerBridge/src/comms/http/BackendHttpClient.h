#pragma once

#include <Arduino.h>

void backendClientBegin();
void backendClientLoop();
bool backendClientCaptureNow(String &message);
bool backendClientSendHeartbeatNow(String &message);
