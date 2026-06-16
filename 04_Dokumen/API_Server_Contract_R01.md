# API Server Contract R01

This document now reflects the first working firmware-to-backend contract used by the ESP32-CAM bridge implementation.

## Goal

Keep firmware and backend assumptions aligned for the current image snapshot workflow.

## Data Direction

`ESP32-CAM -> HTTP API Server -> latest image storage + archive + dashboard viewer`

## Base URL

Default device-side format:

```text
http://<mini-pc-ip>/api/v1
```

Current deployment default on the mini PC:

```text
http://192.168.1.100/api/v1
```

The firmware still keeps this value configurable from the local web portal.

## Active Endpoints

### 1. Heartbeat

`POST /api/v1/devices/{device_id}/heartbeat`

Content type:

```text
application/json
```

Current heartbeat body:

```json
{
  "schema_version": "solar-backend-r01",
  "source_mode": "esp32",
  "device_id": "CP_Panel_Surya",
  "line_id": "CP_Panel_Surya",
  "running": true,
  "active_scenario": "camera_snapshot_uploader",
  "backend_base_url": "http://192.168.1.100/api/v1",
  "last_send_ok": true,
  "last_send_at_ms": 123456
}
```

### 2. Snapshot Upload

`POST /api/v1/devices/{device_id}/camera-snapshot`

Content type:

```text
image/jpeg
```

Request body:

- raw JPEG bytes only

Optional header when the backend ingest API key is enabled:

```text
X-API-Key: <api_key>
```

Expected result:

- HTTP `201` on success

## Current Design Decisions

- `device_id` must stay stable because the backend stores latest snapshot per device.
- snapshot upload uses raw `image/jpeg` instead of multipart form data.
- the camera firmware sends a lightweight heartbeat but does not send full solar telemetry samples.
- timestamp basis for heartbeat is device uptime via `millis()`.

## Still Open

- whether offline caching to local storage should be added
- whether HTTPS support is required for future remote deployments
- whether optional metadata upload should be added for camera profile, frame tag, or field label
