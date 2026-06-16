# 2026 ESP32Cam Server Bridge

Repository untuk project ESP32-CAM yang mengambil gambar lalu mengirimkannya ke server, dengan handoff yang disiapkan agar executor AI lain bisa melanjutkan baseline yang sudah hidup.

## Status

- Repo ini sudah memiliki first working firmware baseline.
- Branch implementasi aktif saat ini adalah `feature/esp32cam-web-ota-uploader`.
- Baseline yang sudah ada:
  - firmware modular
  - local setup portal
  - setup AP fallback
  - live preview
  - snapshot upload raw JPEG
  - heartbeat
  - Arduino OTA
  - browser OTA
- Fokus lanjutan saat ini: UI polish portal lokal, upload hardening, backend compatibility check, dan sinkronisasi dokumen.

## Asumsi Default

- Board default: AI-Thinker ESP32-CAM.
- Firmware style: Arduino-ESP32 modular dan Arduino IDE friendly.
- Arah konektivitas utama: device `->` HTTP API server.
- Server target diasumsikan berjalan di mini PC.
- Jika keputusan teknis belum pasti, pakai dokumen di `04_Dokumen/` dan `AGENTS.md` sebagai source of truth repo ini.
- Backend tetap dianggap owner untuk latest snapshot dan archive interval.

## Struktur Root

```text
2026 ESP32Cam Server Bridge/
|-- 01_Desain/
|-- 02_Firmware/
|   `-- ESP32CamServerBridge/
|       |-- src/
|       |   |-- config/
|       |   |-- core/
|       |   |-- state/
|       |   |-- drivers/
|       |   |-- modules/
|       |   `-- comms/
|       |-- docs/
|       |-- test/
|       `-- tools/
|-- 03_Produksi/
|-- 04_Dokumen/
|-- _arsip/
|-- _sandbox/
|-- ai/
|   `-- branches/
|-- .github/
|-- AGENTS.md
|-- CONTRIBUTING.md
`-- CODEOWNERS
```

## Dokumen Penting

- `AGENTS.md`
- `04_Dokumen/Aturan_AI_dan_Adaptasi_Obsidian_R01.md`
- `02_Firmware/ESP32CamServerBridge/README.md`
- `04_Dokumen/API_Server_Contract_R01.md`
- `04_Dokumen/Planning_Implementasi_R02.md`
- `04_Dokumen/Prompt_Codex_Executor_R02.md`
- `04_Dokumen/HANDOFF_MINI_PC_R01.md`
- `prompt.md`

## Aturan Kerja Singkat

- Jangan reset implementasi yang sudah ada dari nol bila perubahan bisa dilakukan inkremental.
- Jangan jadikan repo ini single-file firmware.
- Jaga `.ino` tetap tipis; logika utama harus dipisah ke `config`, `core`, `state`, `drivers`, `modules`, `comms`, dan `utils`.
- Hindari `delay()` di loop runtime utama.
- Jangan menyebar kredensial ke README atau dokumen umum.
- Gunakan Gitflow: `main` untuk production snapshot, `develop` untuk integration, `feature/*` untuk kerja fitur.
- Pertahankan jalur default `ESP32-CAM -> HTTP API server`; jangan pivot ke MQTT/WebSocket/cloud lain tanpa instruksi user.

## Langkah Berikutnya Yang Disarankan

1. Gunakan `prompt.md` atau `04_Dokumen/Prompt_Codex_Executor_R02.md` saat melakukan handoff ke AI lain.
2. Poles desain local web portal agar lebih kuat dan konsisten dengan visual language solar monitor.
3. Harden alur snapshot upload dan heartbeat tanpa mengubah kontrak publik.
4. Verifikasi kompatibilitas dengan repo backend solar monitor sebelum mengubah integrasi kamera.
