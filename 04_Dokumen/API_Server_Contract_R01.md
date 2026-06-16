# API Server Contract R01

Dokumen ini adalah draft kontrak server untuk baseline awal. Belum ada implementasi firmware aktif yang mengirim request ini.

## Tujuan

Menjaga agar executor firmware dan executor server punya asumsi awal yang sama saat project mulai diisi.

## Arah Data

`ESP32-CAM -> HTTP API Server -> storage / downstream service`

## Base URL

Default format:

```text
http://<mini-pc-ip>:8000/api/v1
```

Nilai final tetap configurable di firmware.

## Endpoint Draft

### 1. Heartbeat

`POST /api/v1/devices/{device_id}/heartbeat`

Body JSON draft:

```json
{
  "device_id": "esp32cam-01",
  "fw_version": "v0.1.0",
  "uptime_ms": 123456,
  "wifi_rssi": -58,
  "free_heap": 180000,
  "camera_ready": true,
  "server_enabled": true
}
```

### 2. Upload Frame

`POST /api/v1/devices/{device_id}/frames`

Content type draft:

```text
multipart/form-data
```

Field draft:

- `frame` = file JPEG
- `frame_id` = unique ID dari device
- `captured_at_ms` = timestamp device
- `camera_profile` = nama profile kamera
- `content_type` = `image/jpeg`

Header draft:

- `X-API-Key: <api_key>`

### 3. Optional Device Config Fetch

`GET /api/v1/devices/{device_id}/config`

Endpoint ini optional. Jangan diandalkan sebelum server side benar-benar menyediakannya.

## Prinsip Kontrak

- `device_id` harus stabil.
- nama field jangan berubah-ubah tanpa update dokumen.
- jika server menolak upload, firmware harus punya error path yang jelas.
- kontrak ini boleh berkembang, tetapi perubahan harus dicatat sebelum executor besar mulai coding.

## Hal Yang Belum Diputuskan

- perlu HTTPS atau tidak
- retry payload disimpan di RAM saja atau storage lokal
- apakah frame dikirim periodik atau event-based
- batas ukuran JPEG target
- apakah server memberi ACK detail untuk retry logic
