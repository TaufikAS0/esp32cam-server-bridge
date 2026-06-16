# Prompt Pack R02

Gunakan file ini sebagai prompt singkat siap-pakai untuk AI executor berikutnya.

## Project Context

Kamu melanjutkan repo `esp32cam-server-bridge`, bukan membuat ulang dari nol.

- branch kerja yang harus dilanjutkan: `feature/esp32cam-web-ota-uploader`
- fokus repo utama: firmware ESP32-CAM + local web portal + server upload compatibility check
- backend diperlakukan sebagai target contract yang sudah ada, bukan product track baru
- arah arsitektur tetap: `ESP32-CAM -> HTTP API server -> latest image + archive + viewer`

## Required Read Order

Baca urutan ini dulu sebelum mengubah kode:

1. `README.md`
2. `AGENTS.md`
3. `04_Dokumen/Aturan_AI_dan_Adaptasi_Obsidian_R01.md`
4. `02_Firmware/ESP32CamServerBridge/README.md`
5. `04_Dokumen/API_Server_Contract_R01.md`
6. `ai/branches/feature-esp32cam-web-ota-uploader.md`
7. `04_Dokumen/Planning_Implementasi_R02.md`
8. `04_Dokumen/Prompt_Codex_Executor_R02.md`

Lalu baca style reference dari backend solar monitor:

- sibling repo yang diharapkan: `..\2026-esp32-solar-power-monitor-backend-server`
- file referensi visual:
  - `research_viewer/index.html`
  - `research_viewer/app.css`

Jika repo backend tidak ada di sibling path itu, cari clone lokalnya atau fetch dari:

- `https://github.com/TaufikAS0/2026-esp32-solar-power-monitor-backend-server`

## Current Branch And Current Truth

Anggap kondisi repo saat ini sebagai baseline aktif:

- firmware modular sudah ada di `02_Firmware/ESP32CamServerBridge`
- `.ino` entrypoint sudah tipis
- portal lokal sudah ada
- Wi-Fi station + fallback setup AP sudah ada
- live preview sudah ada
- snapshot upload raw JPEG ke backend sudah ada
- heartbeat ke backend sudah ada
- Arduino OTA dan browser OTA sudah ada

Jangan reset arsitektur, jangan pindah framework, dan jangan mengulang implementasi dari awal.

## Locked Scope

Kerjakan area ini saja:

1. poles desain local web portal ESP32-CAM agar lebih rapi dan konsisten
2. pertahankan dan harden upload flow berikut:
   - `POST /api/v1/devices/{device_id}/heartbeat`
   - `POST /api/v1/devices/{device_id}/camera-snapshot`
3. cek kompatibilitas dengan backend snapshot latest/archive yang sudah ada di repo solar backend
4. perbarui dokumen bila current truth atau keputusan teknis berubah

Out of scope kecuali user meminta:

- mengubah ingest contract
- pivot ke MQTT, WebSocket, atau cloud architecture lain
- menjadikan backend sebagai product track baru
- menambah framework frontend berat, CDN, atau asset external besar ke portal ESP32

## UI Design Brief

Portal harus memakai bahasa desain yang terinspirasi dari solar monitor viewer, tetapi tetap ringan untuk embedded device.

Kunci visual:

- dark background
- deep navy surface
- subtle border glow
- hierarki jelas antara hero, preview, status cards, dan settings

Makna warna:

- solar/source: amber-gold
- healthy/connected/battery OK: green
- camera/load/active transport: blue
- warning/upload fail: red
- neutral/offline/info sekunder: slate

Target layout portal:

- top hero:
  - title
  - device ID
  - firmware version
  - uptime
  - Wi-Fi mode/status
- main visual area:
  - large live preview card sebagai focal point
- compact operational cards:
  - upload status
  - heartbeat status
  - backend target
  - camera readiness
- action row:
  - capture/upload now
  - restart
  - reset defaults
  - open OTA
- settings area:
  - Wi-Fi
  - backend URL
  - API key
  - device ID / line ID
  - capture interval
  - heartbeat interval
  - JPEG quality
  - frame size
  - OTA hostname/password

OTA page dan result page harus ikut bahasa visual yang sama.

UI constraints:

- no external JS/CSS/CDN dependency
- HTML/CSS tetap compact dan maintainable
- responsive untuk phone dan laptop
- status mudah dibaca user lapangan non-teknis
- feel harus seperti `solar control panel`, bukan admin CRUD generik

## Server Upload Contract

Jangan ubah kontrak ini tanpa instruksi baru:

- heartbeat:
  - `POST /api/v1/devices/{device_id}/heartbeat`
- snapshot upload:
  - `POST /api/v1/devices/{device_id}/camera-snapshot`
- content body upload harus tetap raw `image/jpeg`, bukan multipart
- header `X-API-Key` opsional harus tetap didukung

Perilaku yang harus dipertahankan:

- periodic snapshot upload configurable dari portal
- heartbeat punya interval terpisah
- manual capture/upload action tetap ada
- portal menampilkan:
  - last upload result
  - last heartbeat result
  - last upload age
  - bytes sent
  - backend base URL

Backend compatibility check wajib memverifikasi endpoint berikut:

- `POST /api/v1/devices/{device_id}/camera-snapshot`
- `POST /api/v1/devices/{device_id}/heartbeat`
- `GET /api/v1/research/devices/{device_id}/camera-snapshot/latest`
- `GET /api/v1/research/devices/{device_id}/camera-snapshot/latest.jpg`
- `GET /api/v1/research/camera-snapshots/latest`
- `GET /api/v1/research/camera-snapshots/latest.jpg`

Jika menemukan mismatch, dokumentasikan gap dulu dan lakukan fix yang konservatif.

## Implementation Rules

- pertahankan struktur root project
- pertahankan firmware aktif di `02_Firmware/ESP32CamServerBridge`
- pertahankan struktur modular `src/`
- `.ino` harus tetap tipis
- jangan gunakan `delay()` di main runtime loop
- jangan sebarkan kredensial ke dokumen umum
- jangan tambahkan arsip image jangka panjang di device
- backend tetap owner untuk latest snapshot dan interval archive

## Verification Checklist

Acceptance minimum:

- firmware masih compile untuk `esp32:esp32:esp32cam`
- setup AP fallback masih jalan
- station mode portal masih jalan
- live preview masih tampil
- `capture and upload now` masih jalan
- periodic upload masih jalan
- heartbeat masih jalan
- OTA page masih jalan
- tidak ada regression pada snapshot upload contract
- desain portal terlihat mengikuti solar-monitor-inspired dark control-panel style
- dokumen dan branch worklog mencerminkan state repo sekarang, bukan bootstrap-only state

## Expected Final Report Format

Saat selesai, laporkan dengan format berikut:

1. branch aktif dan commit yang dikerjakan
2. ringkasan perubahan utama
3. file penting yang diubah
4. hasil verifikasi yang benar-benar dijalankan
5. mismatch backend yang ditemukan, jika ada
6. risiko tersisa atau pekerjaan lanjutan yang direkomendasikan
