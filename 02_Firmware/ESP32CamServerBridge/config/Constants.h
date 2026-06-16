#ifndef CONFIG_CONSTANTS_H
#define CONFIG_CONSTANTS_H

namespace Constants {
constexpr unsigned long kSerialBaudRate = 115200UL;
constexpr unsigned long kStatusLogIntervalMs = 5000UL;
constexpr unsigned long kWifiConnectAttemptIntervalMs = 10000UL;
constexpr unsigned long kWifiConnectTimeoutMs = 15000UL;
constexpr char kFirmwareVersion[] = "v0.1.0-bringup";
constexpr char kDeviceName[] = "esp32cam-bridge";
}  // namespace Constants

#endif
