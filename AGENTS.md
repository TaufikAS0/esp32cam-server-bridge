@C:\Users\ASUS\.codex\RTK.md

# AGENTS.md - 2026 ESP32Cam Server Bridge

## Mission

Repo ini adalah bootstrap project ESP32-CAM yang akan terkoneksi ke server. Saat membaca repo ini, anggap implementasi firmware belum final dan tugas utama adalah melanjutkan scaffold ini dengan disiplin arsitektur, dokumentasi, dan Gitflow.

## Read First

1. `README.md`
2. `04_Dokumen/Aturan_AI_dan_Adaptasi_Obsidian_R01.md`
3. `04_Dokumen/Planning_Implementasi_R01.md`
4. `04_Dokumen/API_Server_Contract_R01.md`
5. `ai/branches/<branch-slug>.md` bila file branch aktif sudah ada

## Working Path

Semua kerja untuk project ini harus berada di:
`D:\JIN Work\Document\0. JIN Project Github Programming\2026 ESP32Cam Server Bridge`

## Hard Rules

- Pertahankan struktur root:
  - `01_Desain`
  - `02_Firmware`
  - `03_Produksi`
  - `04_Dokumen`
  - `_arsip`
  - `_sandbox`
- Implementasi firmware aktif harus berada di `02_Firmware/ESP32CamServerBridge`.
- Default board adalah AI-Thinker ESP32-CAM kecuali user mengganti target hardware.
- Gunakan arsitektur modular. Entry point firmware harus tipis dan hanya berisi bootstrap serta panggilan update tingkat tinggi.
- Pisahkan tanggung jawab ke area seperti `config`, `core`, `state`, `drivers`, `modules`, `comms`, dan `utils`.
- Hindari `delay()` di runtime loop utama. Gunakan flow non-blocking.
- Jangan sebar SSID, password, API key, token, atau kredensial mentah ke README dan dokumen umum.
- Jika butuh file lokal sensitif, gunakan file lokal yang di-ignore Git.
- Jangan pivot ke MQTT, WebSocket, atau cloud stack lain kecuali user meminta. Arah default repo ini adalah ESP32-CAM `->` HTTP API server.
- Pisahkan dokumen manual dari output generated. Jangan hapus dokumen bootstrap ini saat implementasi mulai berjalan.

## Source Of Truth

Untuk repo ini, source of truth lokal adalah:

1. `AGENTS.md`
2. `README.md`
3. semua dokumen di `04_Dokumen/`

Jika ada keputusan arsitektur yang masih ambigu, pakai vault firmware berikut sebagai referensi induk:

- `D:\JIN Work\Document\0. JIN Project Github Programming\2026 Vault Firmware Rules\00_Start_Here\Firmware_AI_Vault_Home.md`
- `D:\JIN Work\Document\0. JIN Project Github Programming\2026 Vault Firmware Rules\01_Rules\Rules_Firmware_Standar.md`
- `D:\JIN Work\Document\0. JIN Project Github Programming\2026 Vault Firmware Rules\01_Rules\Rules_GitHub_Gitflow.md`

## Default Firmware Direction

- Framework default: Arduino-ESP32.
- Library default untuk konektivitas: `WiFi.h`.
- Web local/debug boleh memakai `WebServer.h` built-in jika dibutuhkan.
- Simpan config runtime ke `Preferences` bila memang harus survive reboot.
- Desain upload server harus memisahkan:
  - inisialisasi kamera
  - capture frame
  - queue/buffer
  - HTTP upload
  - retry/error state

## Gitflow Guardian

Gunakan Gitflow ketat:

- `main` = production
- `develop` = integration
- `feature/*` = branch kerja dari `develop`
- `release/*` = branch stabilisasi dari `develop`
- `hotfix/*` = branch darurat dari `main`

Larangan:

- jangan commit langsung ke `main`
- jangan push langsung ke `main`
- jangan merge `feature/*` ke `main`

Saat diminta melakukan operasi Git, gunakan struktur jawaban ini:

- Goal
- Is it allowed by policy? (Yes/No)
- Correct branch flow
- Safe commands to run
- Risks / notes

## Branch Worklog

- Simpan status branch di `ai/branches/<branch-slug>.md`.
- Baca line `CURRENT | ...` dan dua update terbaru sebelum mengubah worklog.
- Tambahkan update terbaru di paling atas bagian `## Latest Updates`.

## Expected First Implementation

Saat implementasi dimulai, target file minimal yang diharapkan adalah:

- `ESP32CamServerBridge.ino`
- `config/Pins.h`
- `config/Constants.h`
- `config/Settings.h`
- `core/App.h`
- `core/App.cpp`
- `state/SystemState.h`
- `state/SystemState.cpp`
- `drivers/camera/*`
- `modules/capture/*`
- `modules/server_sync/*`
- `comms/http/*`

## Mini PC Handoff

Jika executor berikutnya bekerja di mini PC, arahkan dia membaca `04_Dokumen/HANDOFF_MINI_PC_R01.md` dan `04_Dokumen/Prompt_Codex_Executor_R01.md` sebelum mengubah kode.
