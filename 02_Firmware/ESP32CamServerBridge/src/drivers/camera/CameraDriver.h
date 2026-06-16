#pragma once

#include <Arduino.h>
#include <esp_camera.h>

bool cameraBegin();
bool cameraReconfigure();
bool cameraIsReady();
String cameraStatusMessage();
String cameraFrameSizeLabel();
bool cameraCaptureFrame(camera_fb_t *&frame, String &message);
void cameraReleaseFrame(camera_fb_t *frame);
