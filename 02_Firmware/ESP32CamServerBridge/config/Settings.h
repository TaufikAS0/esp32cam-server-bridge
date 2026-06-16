#ifndef CONFIG_SETTINGS_H
#define CONFIG_SETTINGS_H

#include "esp_camera.h"

namespace Settings {
constexpr framesize_t kFrameSize = FRAMESIZE_QVGA;
constexpr pixformat_t kPixelFormat = PIXFORMAT_JPEG;
constexpr int kJpegQuality = 12;
constexpr int kFrameBufferCount = 1;
}  // namespace Settings

#endif
