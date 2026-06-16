#include "CameraDriver.h"

#include "../../config/Pins.h"
#include "../../config/Settings.h"
#include "../../state/SystemState.h"

namespace {

bool ready = false;
String statusText = "Camera has not been initialized yet.";

framesize_t frameSizeFromCode(uint8_t code) {
  switch (code) {
    case 0:
      return FRAMESIZE_QVGA;
    case 1:
      return FRAMESIZE_VGA;
    case 2:
      return FRAMESIZE_SVGA;
    case 3:
      return FRAMESIZE_XGA;
    case 4:
      return FRAMESIZE_SXGA;
    case 5:
      return FRAMESIZE_UXGA;
    default:
      return FRAMESIZE_SVGA;
  }
}

bool initCameraInternal() {
  if (ready) {
    esp_camera_deinit();
    ready = false;
  }

  const AppConfig &config = configGet();

  camera_config_t cameraConfig = {};
  cameraConfig.ledc_channel = LEDC_CHANNEL_0;
  cameraConfig.ledc_timer = LEDC_TIMER_0;
  cameraConfig.pin_d0 = Y2_GPIO_NUM;
  cameraConfig.pin_d1 = Y3_GPIO_NUM;
  cameraConfig.pin_d2 = Y4_GPIO_NUM;
  cameraConfig.pin_d3 = Y5_GPIO_NUM;
  cameraConfig.pin_d4 = Y6_GPIO_NUM;
  cameraConfig.pin_d5 = Y7_GPIO_NUM;
  cameraConfig.pin_d6 = Y8_GPIO_NUM;
  cameraConfig.pin_d7 = Y9_GPIO_NUM;
  cameraConfig.pin_xclk = XCLK_GPIO_NUM;
  cameraConfig.pin_pclk = PCLK_GPIO_NUM;
  cameraConfig.pin_vsync = VSYNC_GPIO_NUM;
  cameraConfig.pin_href = HREF_GPIO_NUM;
  cameraConfig.pin_sccb_sda = SIOD_GPIO_NUM;
  cameraConfig.pin_sccb_scl = SIOC_GPIO_NUM;
  cameraConfig.pin_pwdn = PWDN_GPIO_NUM;
  cameraConfig.pin_reset = RESET_GPIO_NUM;
  cameraConfig.xclk_freq_hz = 20000000;
  cameraConfig.pixel_format = PIXFORMAT_JPEG;
  cameraConfig.frame_size = frameSizeFromCode(config.frameSizeCode);
  cameraConfig.grab_mode = CAMERA_GRAB_LATEST;
  cameraConfig.fb_location = CAMERA_FB_IN_PSRAM;
  cameraConfig.jpeg_quality = config.jpegQuality;
  cameraConfig.fb_count = 2;

  if (!psramFound()) {
    cameraConfig.frame_size = FRAMESIZE_VGA;
    cameraConfig.fb_location = CAMERA_FB_IN_DRAM;
    cameraConfig.fb_count = 1;
    if (cameraConfig.jpeg_quality < 12) {
      cameraConfig.jpeg_quality = 12;
    }
  }

  const esp_err_t result = esp_camera_init(&cameraConfig);
  if (result != ESP_OK) {
    char errorBuffer[20];
    snprintf(errorBuffer, sizeof(errorBuffer), "0x%X", result);
    statusText = String("Camera init failed: ") + errorBuffer;
    runtimeState().cameraReady = false;
    runtimeState().cameraMessage = statusText;
    return false;
  }

  sensor_t *sensor = esp_camera_sensor_get();
  if (sensor != nullptr && sensor->id.PID == OV3660_PID) {
    sensor->set_vflip(sensor, 1);
    sensor->set_brightness(sensor, 1);
    sensor->set_saturation(sensor, -2);
  }

  ready = true;
  statusText = String("Camera ready: ") + configFrameSizeLabel(config.frameSizeCode) +
               ", JPEG quality " + String(config.jpegQuality);
  runtimeState().cameraReady = true;
  runtimeState().cameraMessage = statusText;
  return true;
}

}  // namespace

bool cameraBegin() {
  return initCameraInternal();
}

bool cameraReconfigure() {
  return initCameraInternal();
}

bool cameraIsReady() {
  return ready;
}

String cameraStatusMessage() {
  return statusText;
}

String cameraFrameSizeLabel() {
  return configFrameSizeLabel(configGet().frameSizeCode);
}

bool cameraCaptureFrame(camera_fb_t *&frame, String &message) {
  frame = nullptr;

  if (!ready && !initCameraInternal()) {
    message = statusText;
    return false;
  }

  frame = esp_camera_fb_get();
  if (frame == nullptr) {
    statusText = "Camera capture failed.";
    runtimeState().cameraReady = false;
    runtimeState().cameraMessage = statusText;
    message = statusText;
    ready = false;
    return false;
  }

  message = String("Captured ") + frame->len + " bytes.";
  runtimeState().cameraReady = true;
  runtimeState().cameraMessage = statusText;
  return true;
}

void cameraReleaseFrame(camera_fb_t *frame) {
  if (frame != nullptr) {
    esp_camera_fb_return(frame);
  }
}
