# Handoff Mini PC R01

Dokumen ini dipakai saat repo ini akan dilanjutkan dari mini PC atau mesin lain.

## Tujuan

Memberi jalur baca singkat agar executor berikutnya tidak perlu menebak repo ini dibuat untuk apa.

## Repo Saat Ini

- status: implemented baseline exists
- branch kerja aktif: `feature/esp32cam-web-ota-uploader`
- firmware: sudah diimplementasikan dalam baseline kerja pertama
- yang sudah siap:
  - struktur repo dan Gitflow dasar
  - firmware modular aktif
  - local setup portal
  - fallback setup AP
  - live preview
  - snapshot upload raw JPEG
  - heartbeat
  - Arduino OTA
  - browser OTA

## Urutan Baca Di Mesin Baru

1. clone repo
2. baca `README.md`
3. baca `AGENTS.md`
4. baca `04_Dokumen/Aturan_AI_dan_Adaptasi_Obsidian_R01.md`
5. baca `02_Firmware/ESP32CamServerBridge/README.md`
6. baca `04_Dokumen/API_Server_Contract_R01.md`
7. baca `04_Dokumen/Planning_Implementasi_R02.md`
8. baca `04_Dokumen/Prompt_Codex_Executor_R02.md`
9. baca `prompt.md`
10. baca `ai/branches/feature-esp32cam-web-ota-uploader.md`

## Ekspektasi

- jangan reset implementasi yang sudah ada
- lanjutkan baseline aktif di `02_Firmware/ESP32CamServerBridge`
- pertahankan arsitektur `ESP32-CAM -> HTTP API server`
- fokus utama lanjutan:
  - UI polish portal lokal
  - hardening upload dan heartbeat
  - backend compatibility check
  - doc alignment
- sinkronkan perubahan keputusan dengan dokumen dan worklog branch

## Prompt Singkat

Gunakan `prompt.md` sebagai quick entrypoint dan `Prompt_Codex_Executor_R02.md` sebagai prompt detail utama. Jika ingin versi ringkas, pakai ini:

```text
Lanjutkan repo ESP32-CAM ini dari branch `feature/esp32cam-web-ota-uploader`, bukan dari nol.
Baca README.md, AGENTS.md, 02_Firmware/ESP32CamServerBridge/README.md, 04_Dokumen/API_Server_Contract_R01.md, 04_Dokumen/Planning_Implementasi_R02.md, prompt.md, dan ai/branches/feature-esp32cam-web-ota-uploader.md.
Fokus kerja ada di UI polish portal lokal, hardening upload/heartbeat, compatibility check ke backend solar monitor, dan sinkronisasi dokumen.
Implementasi harus modular, non-blocking, dan tetap mengikuti arah ESP32-CAM -> HTTP API server.
```
