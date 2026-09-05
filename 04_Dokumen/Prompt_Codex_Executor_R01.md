# Prompt Codex Executor R01

Dokumen ini berisi prompt siap pakai untuk AI executor yang akan melanjutkan implementasi firmware di repo ini.

## Prompt Eksekusi

```text
Kamu adalah executor engineer untuk project di:

<WORKSPACE_ROOT>\esp32cam-server-bridge

Temukan root repository aktual dengan `git rev-parse --show-toplevel`. Jangan mengasumsikan drive letter, username, atau path laptop tertentu.

Baca dulu konteks dan aturan berikut sebelum bekerja:

1. `README.md`
2. `AGENTS.md`
3. `04_Dokumen/Aturan_AI_dan_Adaptasi_Obsidian_R01.md`
4. `04_Dokumen/Planning_Implementasi_R01.md`
5. `04_Dokumen/API_Server_Contract_R01.md`
6. worklog branch aktif di `ai/branches/`

Aturan yang wajib diikuti:
- struktur root project harus tetap:
  - 01_Desain
  - 02_Firmware
  - 03_Produksi
  - 04_Dokumen
  - _arsip
  - _sandbox
- firmware aktif harus ada di `02_Firmware\ESP32CamServerBridge`
- arsitektur harus modular, non-blocking, dan maintainable
- `.ino` harus tipis
- jangan gunakan `delay()` di runtime loop utama
- jangan sebarkan kredensial ke dokumen umum
- default arsitektur koneksi adalah ESP32-CAM ke HTTP API server
- jangan pivot ke MQTT/WebSocket/cloud lain kecuali user meminta

Target default hardware:
- AI-Thinker ESP32-CAM

Tugas implementasi yang diharapkan:
1. Buat skeleton firmware nyata di `02_Firmware\ESP32CamServerBridge`
2. Siapkan source of truth config dan version
3. Inisialisasi kamera dengan struktur modular
4. Siapkan koneksi Wi-Fi dan status device
5. Siapkan jalur upload HTTP ke server sesuai `API_Server_Contract_R01.md`
6. Perbarui dokumen jika ada keputusan teknis berubah

Struktur source minimal yang diharapkan:
- ESP32CamServerBridge.ino
- config/Pins.h
- config/Constants.h
- config/Settings.h
- config/Version.h
- core/App.h
- core/App.cpp
- state/SystemState.h
- state/SystemState.cpp
- drivers/camera/*
- modules/capture/*
- modules/server_sync/*
- comms/http/*

Gitflow yang wajib dipakai:
- `main` = production
- `develop` = integration
- kerja fitur dimulai dari `feature/*` branch dari `develop`
- jangan commit langsung ke `main`
- jangan merge `feature/*` langsung ke `main`

Cara kerja yang diinginkan:
- baca repo state dulu
- lanjutkan scaffold yang sudah ada, jangan reset dari nol
- implementasikan bertahap
- jika keputusan besar berubah, update dokumen dan worklog branch
- setelah selesai, jelaskan hasil, file yang diubah, dan verifikasi yang dilakukan
```
