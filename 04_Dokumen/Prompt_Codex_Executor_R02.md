# Prompt Codex Executor R02

Dokumen ini adalah prompt detail untuk AI executor yang akan melanjutkan repo ESP32-CAM pada state implementasi aktif, bukan state bootstrap awal.

## Posisi Saat Ini

Repo ini sudah memiliki first working firmware pada branch:

- `feature/esp32cam-web-ota-uploader`

Baseline yang harus dianggap benar saat mulai bekerja:

- firmware modular aktif sudah ada
- web portal lokal sudah ada
- Wi-Fi station + fallback AP sudah ada
- live preview sudah ada
- snapshot upload raw `image/jpeg` ke backend sudah ada
- heartbeat ke backend sudah ada
- Arduino OTA dan browser OTA sudah ada

Jadi tugasmu adalah melanjutkan, memoles, mengeraskan, dan menyelaraskan implementasi yang sudah hidup. Jangan memulai ulang dari nol.

## Prompt Eksekusi

```text
Kamu adalah executor engineer untuk repo `esp32cam-server-bridge`.

Kamu WAJIB melanjutkan implementasi yang sudah ada di branch:
- `feature/esp32cam-web-ota-uploader`

Jangan reset arsitektur, jangan rewrite dari nol, dan jangan pivot dari HTTP API flow yang sudah ada.

Sebelum bekerja, baca urutan ini:

1. `README.md`
2. `AGENTS.md`
3. `04_Dokumen/Aturan_AI_dan_Adaptasi_Obsidian_R01.md`
4. `02_Firmware/ESP32CamServerBridge/README.md`
5. `04_Dokumen/API_Server_Contract_R01.md`
6. `ai/branches/feature-esp32cam-web-ota-uploader.md`
7. `04_Dokumen/Planning_Implementasi_R02.md`
8. `prompt.md`

Lalu baca visual reference dari repo backend solar monitor:

- repo yang diharapkan: `..\2026-esp32-solar-power-monitor-backend-server`
- file referensi:
  - `research_viewer/index.html`
  - `research_viewer/app.css`

Jika repo backend tidak ada di sibling path itu, cari clone lokalnya atau fetch:
- `https://github.com/TaufikAS0/2026-esp32-solar-power-monitor-backend-server`

Scope kerja yang DIIZINKAN:
1. refine local ESP32-CAM web portal UI
2. preserve dan harden upload flow:
   - `POST /api/v1/devices/{device_id}/heartbeat`
   - `POST /api/v1/devices/{device_id}/camera-snapshot`
3. verify compatibility dengan backend snapshot latest/archive behavior yang sudah ada
4. update docs agar sesuai real current state

Out of scope kecuali user meminta eksplisit:
- ubah ingest contract
- ganti arsitektur ke MQTT/WebSocket/cloud lain
- menjadikan backend sebagai project track baru
- menambah frontend framework berat, CDN, atau external asset dependency ke portal ESP32

Aturan arsitektur yang wajib dipertahankan:
- struktur root project tetap:
  - `01_Desain`
  - `02_Firmware`
  - `03_Produksi`
  - `04_Dokumen`
  - `_arsip`
  - `_sandbox`
- firmware aktif tetap di `02_Firmware/ESP32CamServerBridge`
- `.ino` harus tipis
- runtime loop tetap non-blocking
- jangan gunakan `delay()` di loop utama
- jangan bocorkan password, token, atau API key ke dokumen umum
- backend tetap owner untuk latest snapshot dan archive interval
- device tidak menyimpan long-term archive

Brief desain portal yang harus diikuti:
- feel: solar control panel
- dark background
- deep navy surfaces
- subtle glow borders
- visual hierarchy yang jelas
- tetap ringan untuk embedded inline HTML/CSS

Makna warna:
- solar/source = amber-gold
- healthy/connected = green
- camera/load/transport active = blue
- warning/failure = red
- neutral info = slate

Layout yang ditargetkan:
- top hero:
  - title
  - device ID
  - firmware version
  - uptime
  - Wi-Fi mode/status
- main visual focus:
  - large live preview card
- compact status cards:
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
- OTA page dan result page harus konsisten dengan bahasa visual yang sama

Constraint UI:
- tanpa external JS/CSS/CDN
- HTML/CSS compact dan maintainable
- responsive untuk phone dan laptop
- mudah dibaca user lapangan non-teknis
- jangan tampil seperti admin CRUD generik

Contract upload yang harus dipertahankan:
- heartbeat:
  - `POST /api/v1/devices/{device_id}/heartbeat`
- snapshot upload:
  - `POST /api/v1/devices/{device_id}/camera-snapshot`
- snapshot upload body harus tetap raw `image/jpeg`
- optional `X-API-Key` support harus tetap ada

Portal wajib tetap menampilkan:
- last upload result
- last heartbeat result
- last upload age
- bytes sent
- backend base URL

Compatibility check backend yang wajib kamu lakukan:
- `POST /api/v1/devices/{device_id}/camera-snapshot`
- `POST /api/v1/devices/{device_id}/heartbeat`
- `GET /api/v1/research/devices/{device_id}/camera-snapshot/latest`
- `GET /api/v1/research/devices/{device_id}/camera-snapshot/latest.jpg`
- `GET /api/v1/research/camera-snapshots/latest`
- `GET /api/v1/research/camera-snapshots/latest.jpg`

Jika ada mismatch:
- dokumentasikan gap dulu
- lakukan fix konservatif
- jangan redesign backend tanpa instruksi user

Acceptance criteria minimum:
- firmware masih compile untuk `esp32:esp32:esp32cam`
- setup AP fallback masih jalan
- station mode portal masih jalan
- live preview masih render
- `capture and upload now` masih jalan
- periodic upload masih jalan
- heartbeat masih jalan
- OTA page masih jalan
- tidak ada regression pada snapshot upload contract
- desain portal terlihat mengikuti solar-monitor dark control-panel language
- docs dan branch worklog mencerminkan kondisi repo sekarang, bukan bootstrap-only state

Cara kerja yang diinginkan:
- baca repo state dulu
- lanjutkan yang sudah ada
- ubah seperlunya, jangan sweeping rewrite
- kalau keputusan teknis berubah, update dokumen yang relevan
- setelah selesai, laporkan:
  1. branch aktif
  2. commit/working tree state
  3. ringkasan perubahan
  4. file penting yang diubah
  5. verifikasi yang dijalankan
  6. risiko atau next step
```

## Catatan Tambahan

- `prompt.md` adalah quick entrypoint
- dokumen ini adalah prompt detail source of truth
- `Planning_Implementasi_R02.md` memegang implementation plan yang lebih operasional
