# 2026 ESP32Cam Server Bridge

Bootstrap repository untuk project ESP32-CAM yang akan mengambil gambar lalu mengirimkannya ke server, dengan target penerus kerja di mini PC atau executor AI lain.

## Status

- Repo ini masih tahap scaffold.
- Belum ada implementasi firmware final.
- Fokus saat ini: judul project, foldering, aturan AI, handoff, dan kesiapan GitHub.

## Asumsi Default

- Board default: AI-Thinker ESP32-CAM.
- Firmware style: Arduino-ESP32 modular dan Arduino IDE friendly.
- Arah konektivitas utama: device `->` HTTP API server.
- Server target diasumsikan berjalan di mini PC.
- Jika keputusan teknis belum pasti, pakai dokumen di `04_Dokumen/` dan `AGENTS.md` sebagai source of truth repo ini.

## Struktur Root

```text
2026 ESP32Cam Server Bridge/
|-- 01_Desain/
|-- 02_Firmware/
|   `-- ESP32CamServerBridge/
|       |-- config/
|       |-- core/
|       |-- state/
|       |-- drivers/
|       |-- modules/
|       |-- comms/
|       |-- utils/
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
- `04_Dokumen/Planning_Implementasi_R01.md`
- `04_Dokumen/API_Server_Contract_R01.md`
- `04_Dokumen/Prompt_Codex_Executor_R01.md`
- `04_Dokumen/HANDOFF_MINI_PC_R01.md`

## Aturan Kerja Singkat

- Jangan jadikan repo ini single-file firmware jika project mulai tumbuh.
- Jaga `.ino` tetap tipis; logika utama harus dipisah ke `config`, `core`, `state`, `drivers`, `modules`, `comms`, dan `utils`.
- Hindari `delay()` di loop runtime utama.
- Jangan menyebar kredensial ke README atau dokumen umum.
- Gunakan Gitflow: `main` untuk production snapshot, `develop` untuk integration, `feature/*` untuk kerja fitur.

## Langkah Berikutnya Yang Disarankan

1. Konfirmasi kontrak server final.
2. Implementasi skeleton firmware di `02_Firmware/ESP32CamServerBridge`.
3. Tambahkan bring-up kamera, Wi-Fi, dan uploader HTTP.
4. Validasi flow dari mini PC menggunakan prompt handoff yang sudah disiapkan.
