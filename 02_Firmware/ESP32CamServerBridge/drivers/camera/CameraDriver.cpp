#include "drivers/camera/CameraDriver.h"

#include "config/Pins.h"
#include "config/Settings.h"

bool CameraDriver::begin(String* errorMessage) {
  camera_config_t config = {};
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Pins::kY2;
  config.pin_d1 = Pins::kY3;
  config.pin_d2 = Pins::kY4;
  config.pin_d3 = Pins::kY5;
  config.pin_d4 = Pins::kY6;
  config.pin_d5 = Pins::kY7;
  config.pin_d6 = Pins::kY8;
  config.pin_d7 = Pins::kY9;
  config.pin_xclk = Pins::kXclk;
  config.pin_pclk = Pins::kPclk;
  config.pin_vsync = Pins::kVsync;
  config.pin_href = Pins::kHref;
  config.pin_sccb_sda = Pins::kSiod;
  config.pin_sccb_scl = Pins::kSioc;
  config.pin_pwdn = Pins::kPwdn;
  config.pin_reset = Pins::kReset;
  config.xclk_freq_hz = 20000000;
  config.frame_size = Settings::kFrameSize;
  config.pixel_format = Settings::kPixelFormat;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = Settings::kJpegQuality;
  config.fb_count = Settings::kFrameBufferCount;

  const esp_err_t result = esp_camera_init(&config);
  if (result != ESP_OK) {
    if (errorMessage != nullptr) {
      *errorMessage = "esp_camera_init failed with code " + String(static_cast<int>(result));
    }
    return false;
  }

  sensor_t* sensor = esp_camera_sensor_get();
  if (sensor != nullptr) {
    sensor->set_framesize(sensor, Settings::kFrameSize);
  }

  return true;
}
