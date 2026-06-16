# Planning Implementasi R02

## Status

Dokumen ini menggantikan planning bootstrap awal sebagai acuan implementasi aktif untuk portal ESP32-CAM dan server upload compatibility check.

Repo saat ini bukan lagi scaffold kosong. Baseline aktif yang sudah ada:

- modular firmware sudah hidup
- setup portal lokal sudah hidup
- fallback AP sudah hidup
- snapshot upload raw JPEG sudah hidup
- heartbeat sudah hidup
- Arduino OTA dan browser OTA sudah hidup

## Tujuan R02

Menyiapkan paket arahan yang membuat AI berikutnya melanjutkan repo ini dengan benar:

- melanjutkan branch aktif, bukan restart
- memoles local web portal agar lebih kuat dan lebih rapi
- menjaga kestabilan alur kirim ke backend
- memastikan kontrak kamera tetap kompatibel dengan backend solar monitor
- menjaga dokumentasi tetap sinkron dengan state implementasi nyata

## Scope Yang Dikunci

Area kerja utama:

1. local ESP32-CAM portal UI polish
2. upload flow hardening
3. backend compatibility check
4. documentation alignment

Area yang sengaja tidak dibuka pada R02:

- redesign ingest contract
- migrasi ke MQTT/WebSocket/cloud architecture lain
- framework frontend berat di portal
- arsip gambar jangka panjang di device
- backend feature track baru di luar compatibility fix konservatif

## Kondisi Implementasi Yang Harus Dipertahankan

### Firmware

- firmware aktif tetap di `02_Firmware/ESP32CamServerBridge`
- struktur `src/` tetap dipakai
- `.ino` tetap tipis
- loop tetap non-blocking
- `delay()` tidak dipakai di runtime loop utama

### Local Portal

Portal harus tetap punya kemampuan berikut:

- menampilkan current network state
- menampilkan current upload state
- menampilkan current heartbeat state
- menampilkan camera readiness
- live preview
- save settings
- capture/upload now
- restart
- reset defaults
- open OTA page

### Server Sync

Kontrak yang dijaga:

- `POST /api/v1/devices/{device_id}/heartbeat`
- `POST /api/v1/devices/{device_id}/camera-snapshot`

Aturan penting:

- upload body tetap raw `image/jpeg`
- `X-API-Key` opsional tetap didukung
- snapshot archive tetap dimiliki backend
- device hanya kirim dan melaporkan status, bukan menyimpan archive panjang

## Arah Desain UI

UI portal harus mengikuti bahasa desain yang terinspirasi dari solar monitor viewer.

### Style language

- latar gelap
- surface navy dalam
- border glow tipis
- tipografi jelas
- komposisi panel terasa seperti control panel

### Warna fungsi

- amber-gold untuk solar/source
- green untuk healthy/connected/OK
- blue untuk camera/load/active transport
- red untuk warning/failure
- slate untuk info netral

### Layout target

- hero atas dengan identity dan status singkat
- live preview sebagai fokus utama
- kartu status ringkas di sekitar preview
- action row jelas dan cepat dipakai
- settings area tetap padat tapi nyaman dibaca
- OTA page dan result page ikut tema yang sama

### Constraint

- tanpa external JS/CSS/CDN
- inline HTML/CSS tetap ringan
- responsive untuk browser HP dan laptop
- readable untuk user lapangan non-teknis

## Backend Compatibility Check

AI berikutnya wajib membaca repo backend solar monitor dan memastikan endpoint berikut tersedia dan tetap selaras:

- `POST /api/v1/devices/{device_id}/camera-snapshot`
- `POST /api/v1/devices/{device_id}/heartbeat`
- `GET /api/v1/research/devices/{device_id}/camera-snapshot/latest`
- `GET /api/v1/research/devices/{device_id}/camera-snapshot/latest.jpg`
- `GET /api/v1/research/camera-snapshots/latest`
- `GET /api/v1/research/camera-snapshots/latest.jpg`

Repo backend referensi yang diharapkan:

- `..\2026-esp32-solar-power-monitor-backend-server`
- fallback GitHub:
  - `https://github.com/TaufikAS0/2026-esp32-solar-power-monitor-backend-server`

Jika mismatch ditemukan:

- dokumentasikan mismatch
- pilih fix konservatif
- jangan mengubah kontrak publik tanpa instruksi user

## Verification Minimum

Hasil kerja dianggap benar jika:

- compile target `esp32:esp32:esp32cam` masih lolos
- fallback AP masih dapat dipakai
- portal mode station masih dapat diakses
- live preview masih render
- capture/upload now masih bekerja
- periodic upload masih bekerja
- heartbeat masih bekerja
- OTA page masih bekerja
- snapshot contract tidak regress
- desain portal terlihat konsisten dengan referensi solar monitor
- docs dan worklog tidak lagi menyebut repo sebagai bootstrap-only

## Deliverable Yang Diharapkan Dari Executor Berikutnya

Saat selesai, executor harus melaporkan:

1. branch aktif
2. perubahan utama
3. file penting yang disentuh
4. verifikasi yang benar-benar dijalankan
5. mismatch backend yang ditemukan
6. risiko tersisa dan rekomendasi next step
