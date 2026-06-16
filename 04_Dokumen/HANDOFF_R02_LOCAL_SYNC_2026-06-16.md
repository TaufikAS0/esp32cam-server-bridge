# Handoff R02 Local Sync 2026-06-16

Dokumen ini menjembatani state branch lokal `feature/esp32cam-com10-upload` setelah sinkronisasi dengan baseline remote R02.

## Current Truth

- branch lokal aktif: `feature/esp32cam-com10-upload`
- baseline implementasi R02 dari remote `feature/esp32cam-web-ota-uploader` sudah di-cherry-pick ke branch lokal ini
- firmware aktif sekarang memakai layout `src/` di `02_Firmware/ESP32CamServerBridge`
- compile untuk `esp32:esp32:esp32cam` sudah lolos
- firmware hasil integrasi sudah di-upload ke `COM10`

## What Was Verified Here

- `arduino-cli compile --fqbn esp32:esp32:esp32cam 02_Firmware/ESP32CamServerBridge`
- upload ke board di `COM10`
- source default config saat ini mengarah ke:
  - SSID: `HardwareControl`
  - password Wi-Fi: `jayaabadi100`
  - backend base URL: `http://192.168.1.100/api/v1`

## What Was Not Fully Verified Here

- repo backend referensi yang disebut dokumen tidak ada di sibling path:
  - `..\\2026-esp32-solar-power-monitor-backend-server`
- karena itu, compatibility check penuh terhadap endpoint backend belum bisa dibuktikan di mesin ini
- UI portal baru belum saya inspeksi lewat browser visual setelah sync R02

## What The Next AI Should Do

1. baca `prompt.md`
2. baca `04_Dokumen/Prompt_Codex_Executor_R02.md`
3. baca `04_Dokumen/Planning_Implementasi_R02.md`
4. baca `ai/branches/feature-esp32cam-com10-upload.md`
5. jika repo backend tersedia, verifikasi endpoint:
   - `POST /api/v1/devices/{device_id}/heartbeat`
   - `POST /api/v1/devices/{device_id}/camera-snapshot`
   - `GET /api/v1/research/devices/{device_id}/camera-snapshot/latest`
   - `GET /api/v1/research/devices/{device_id}/camera-snapshot/latest.jpg`
   - `GET /api/v1/research/camera-snapshots/latest`
   - `GET /api/v1/research/camera-snapshots/latest.jpg`
6. jika ada mismatch, dokumentasikan gap dulu sebelum mengubah contract

## Important Note

Dokumen R02 asli masih menyebut branch `feature/esp32cam-web-ota-uploader` sebagai baseline. Pada mesin ini, baseline itu sudah diserap ke branch lokal `feature/esp32cam-com10-upload`, jadi AI berikutnya bisa melanjutkan dari branch lokal ini tanpa mengulang sinkronisasi ulang.
