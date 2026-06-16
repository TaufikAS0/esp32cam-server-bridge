# ESP32CamServerBridge Firmware Scaffold

Folder ini sengaja masih kosong secara implementasi, tetapi struktur internalnya sudah disiapkan untuk firmware modular yang akan:

- inisialisasi kamera ESP32-CAM
- mengelola Wi-Fi dan config device
- menangkap frame JPEG
- mengirim frame atau metadata ke server HTTP
- menangani retry dan status perangkat

## Struktur Yang Disiapkan

- `config/` untuk pin, konstanta, settings, dan version source of truth
- `core/` untuk orchestrator aplikasi
- `state/` untuk shared runtime state
- `drivers/` untuk pembungkus kamera, storage, dan hardware low-level
- `modules/` untuk logika capture dan sync
- `comms/` untuk HTTP client atau protocol lain yang memang disetujui
- `utils/` untuk helper kecil yang generik
- `docs/` untuk catatan firmware-level
- `test/` untuk smoke test dan bench notes
- `tools/` untuk helper script lokal

## File Minimum Yang Diharapkan Saat Implementasi Dimulai

- `ESP32CamServerBridge.ino`
- `config/Pins.h`
- `config/Constants.h`
- `config/Settings.h`
- `config/Version.h`
- `core/App.h`
- `core/App.cpp`
- `state/SystemState.h`
- `state/SystemState.cpp`

## Aturan Singkat

- Jangan taruh seluruh logika di `.ino`.
- Jangan campur akses kamera, retry upload, dan state bisnis dalam satu file.
- Jika server contract berubah, sinkronkan juga `../../04_Dokumen/API_Server_Contract_R01.md`.
