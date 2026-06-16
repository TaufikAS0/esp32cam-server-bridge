#ifndef CONFIG_PINS_H
#define CONFIG_PINS_H

namespace Pins {
constexpr int kPwdn = 32;
constexpr int kReset = -1;
constexpr int kXclk = 0;
constexpr int kSiod = 26;
constexpr int kSioc = 27;

constexpr int kY9 = 35;
constexpr int kY8 = 34;
constexpr int kY7 = 39;
constexpr int kY6 = 36;
constexpr int kY5 = 21;
constexpr int kY4 = 19;
constexpr int kY3 = 18;
constexpr int kY2 = 5;
constexpr int kVsync = 25;
constexpr int kHref = 23;
constexpr int kPclk = 22;

constexpr int kLedFlash = 4;
}  // namespace Pins

#endif
