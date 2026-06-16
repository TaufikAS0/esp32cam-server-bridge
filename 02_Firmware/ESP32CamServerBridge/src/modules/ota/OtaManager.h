#pragma once

#include <Arduino.h>

void otaBegin();
void otaLoop();
bool otaIsReady();
String otaHostname();
String otaMessage();
