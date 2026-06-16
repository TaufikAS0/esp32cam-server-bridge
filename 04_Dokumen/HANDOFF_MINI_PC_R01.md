# Handoff Mini PC R01

Dokumen ini dipakai saat repo ini akan dilanjutkan dari mini PC atau mesin lain.

## Tujuan

Memberi jalur baca singkat agar executor berikutnya tidak perlu menebak repo ini dibuat untuk apa.

## Repo Saat Ini

- status: bootstrap only
- firmware: belum diimplementasikan
- yang sudah siap: struktur folder, aturan AI, draft kontrak server, prompt executor, dan Gitflow dasar

## Urutan Baca Di Mesin Baru

1. clone repo
2. baca `README.md`
3. baca `AGENTS.md`
4. baca `04_Dokumen/Aturan_AI_dan_Adaptasi_Obsidian_R01.md`
5. baca `04_Dokumen/Planning_Implementasi_R01.md`
6. baca `04_Dokumen/API_Server_Contract_R01.md`
7. baca `04_Dokumen/Prompt_Codex_Executor_R01.md`
8. baca `ai/branches/develop.md`

## Ekspektasi

- jangan hapus struktur bootstrap
- lanjutkan implementasi di `02_Firmware/ESP32CamServerBridge`
- ikuti Gitflow dari `develop` ke `feature/*`
- sinkronkan perubahan keputusan dengan dokumen

## Prompt Singkat

Gunakan `Prompt_Codex_Executor_R01.md` sebagai prompt utama. Jika ingin versi ringkas, pakai ini:

```text
Lanjutkan repo bootstrap ESP32-CAM ini tanpa mengacak struktur yang sudah ada.
Baca README.md, AGENTS.md, 04_Dokumen/Aturan_AI_dan_Adaptasi_Obsidian_R01.md, Planning_Implementasi_R01.md, API_Server_Contract_R01.md, dan ai/branches/develop.md.
Kerjakan dari branch feature yang dibuat dari develop.
Implementasi harus modular, non-blocking, dan tetap mengikuti arah ESP32-CAM -> HTTP API server.
```
