#pragma once

#include <Arduino.h>

void wifiBegin();
void wifiLoop();
bool wifiIsConnected();
bool wifiIsAccessPointActive();
String wifiStationIp();
String wifiAccessPointIp();
String wifiModeLabel();
