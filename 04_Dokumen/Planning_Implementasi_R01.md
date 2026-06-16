# Planning Implementasi R01

## Status

Dokumen ini masih perencanaan. Repo belum berisi firmware final.

## Tujuan Project

Membangun firmware ESP32-CAM yang:

- terkoneksi ke Wi-Fi
- mampu mengambil frame atau snapshot dari kamera
- mengirim data ke server pada mini PC
- tetap bisa di-debug dan dikembangkan tanpa merombak struktur saat scope membesar

## Asumsi Default

- hardware default: AI-Thinker ESP32-CAM
- framework default: Arduino-ESP32
- komunikasi utama: HTTP request ke server
- payload utama: JPEG snapshot plus metadata
- penyimpanan lokal: minimal dan hanya bila memang dibutuhkan untuk retry

## Batas Scope Scaffold Ini

- belum ada implementasi source code aktif
- belum ada endpoint device yang benar-benar jalan
- belum ada upload kamera yang diuji
- baru ada struktur project, aturan AI, dan draft kontrak server

## Arsitektur Yang Direkomendasikan

### Root Project

- `01_Desain` untuk diagram, skema wiring, dan keputusan desain
- `02_Firmware` untuk source firmware
- `03_Produksi` untuk artifact rilis atau paket deploy
- `04_Dokumen` untuk planning, handoff, API, dan aturan repo
- `_arsip` untuk materi lama
- `_sandbox` untuk percobaan yang belum resmi

### Firmware Internal

- `config`
  - pin map
  - konstanta
  - settings
  - version source of truth
- `core`
  - orchestrator aplikasi
- `state`
  - shared runtime state
- `drivers`
  - kamera
  - storage lokal bila kelak dipakai
- `modules`
  - capture workflow
  - server sync workflow
- `comms`
  - HTTP client/request builder
- `utils`
  - timer, logger, helper kecil

## Fase Implementasi Yang Disarankan

### Fase 1 - Bring-up Lokal

- inisialisasi kamera
- Wi-Fi STA
- fallback atau mode setup bila perlu
- health/status log serial
- endpoint debug lokal bila dibutuhkan

### Fase 2 - Server Upload Dasar

- konfigurasi `device_id`, `api_base`, dan `api_key`
- kirim heartbeat
- kirim JPEG snapshot ke server
- tangani timeout, retry, dan error state

### Fase 3 - Hardening

- queue atau buffer retry
- backoff logic
- telemetry status upload
- dokumentasi bench dan acceptance test

## Pertanyaan Yang Harus Dikonfirmasi Sebelum Coding Besar

1. Upload mode utama: periodik, event-based, atau on-demand?
2. Server menerima `multipart/form-data` atau format lain?
3. Perlu local web UI atau cukup serial log plus upload server?
4. Perlu retry buffer di RAM saja atau juga penyimpanan lokal?
5. Perlu HTTPS atau cukup HTTP LAN dulu?

## Definition Of Done Untuk Bootstrap Ini

- nama project sudah jelas
- foldering sudah siap
- repo GitHub sudah ada
- aturan AI untuk executor berikutnya sudah tertulis
- dokumen handoff mini PC sudah tersedia
