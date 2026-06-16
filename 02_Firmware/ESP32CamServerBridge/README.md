# ESP32CamServerBridge Firmware

This firmware is now implemented as a modular Arduino-ESP32 project for AI Thinker ESP32-CAM.

## What it does

- connects to the field WiFi as a station
- falls back to a setup access point if WiFi fails
- serves a browser setup page on port `80`
- captures JPEG frames from the camera
- uploads snapshots to the backend with raw `image/jpeg`
- sends a lightweight heartbeat to the backend
- supports Arduino OTA and browser-based OTA upload

## Active source layout

The sketch entry file stays at the firmware root:

- `ESP32CamServerBridge.ino`

The compiled modular source lives under:

- `src/config/`
- `src/core/`
- `src/state/`
- `src/drivers/`
- `src/modules/`
- `src/comms/`

This `src/` layout is intentional so Arduino CLI and Arduino IDE both compile the nested modules correctly.

## Main modules

- `src/core/App.*`
  - top-level boot and loop orchestration
- `src/config/Settings.*`
  - persistent device settings using `Preferences`
- `src/state/SystemState.*`
  - shared runtime status for portal and backend sync
- `src/drivers/camera/CameraDriver.*`
  - camera bring-up and capture
- `src/modules/network/WifiManager.*`
  - WiFi station flow and setup AP fallback
- `src/modules/portal/PortalServer.*`
  - local setup UI, live preview, restart, and web OTA
- `src/modules/ota/OtaManager.*`
  - Arduino OTA lifecycle
- `src/comms/http/BackendHttpClient.*`
  - snapshot upload and heartbeat HTTP calls
- `src/modules/server_sync/ServerSync.*`
  - scheduling wrapper for backend sync actions

## Setup experience

### Normal path

If the device joins WiFi successfully, open the device IP in a browser.

### Fallback path

If WiFi cannot connect, the firmware starts a setup access point and local portal automatically.

Factory defaults are defined in:

- `src/config/Settings.cpp`

## Backend integration

Current backend target is aligned to the Solar Monitor mini-PC backend:

- heartbeat: `POST /api/v1/devices/{device_id}/heartbeat`
- snapshot upload: `POST /api/v1/devices/{device_id}/camera-snapshot`

Snapshot upload uses raw JPEG body, not multipart form data.

## Compile

Example:

```powershell
arduino-cli compile --fqbn esp32:esp32:esp32cam .\02_Firmware\ESP32CamServerBridge
```

## OTA

Two OTA paths are available:

- Arduino OTA over WiFi
- browser upload at `/ota`

## Notes

- The runtime loop is non-blocking.
- This firmware is for image snapshots plus heartbeat, not for full solar telemetry sampling.
