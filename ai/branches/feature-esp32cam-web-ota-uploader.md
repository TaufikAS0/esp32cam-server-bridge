CURRENT | V0.1 | 2026-06-16 23:59 ICT | Codex | First working ESP32-CAM firmware implemented with web setup, snapshot upload, heartbeat, and OTA | Next: push branch and optionally open PR into develop
READING RULE | Read the CURRENT line and the first two entries under Latest Updates before editing.

# Branch Worklog

- Branch: `feature/esp32cam-web-ota-uploader`
- Project: `esp32cam-server-bridge`
- Purpose: `implement the first working ESP32-CAM to backend bridge on top of the existing scaffold`

## Latest Updates

### V0.1 | 2026-06-16 23:59 ICT | Codex
- Did: implemented modular ESP32-CAM firmware with setup portal, setup AP fallback, snapshot upload, heartbeat, Arduino OTA, browser OTA, and compile-verified Arduino sketch structure using `src/`
- Files: `02_Firmware/ESP32CamServerBridge/*`, `04_Dokumen/API_Server_Contract_R01.md`
- Next: push this branch to origin and decide whether to merge into `develop`
