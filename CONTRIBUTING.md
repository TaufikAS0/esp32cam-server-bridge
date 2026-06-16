# Contributing

Repo ini disiapkan sebagai baseline kerja untuk firmware ESP32-CAM yang akan terkoneksi ke server. Perlakukan repo ini sebagai project engineering, bukan folder eksperimen bebas.

## Start Here

1. baca `README.md`
2. baca `AGENTS.md`
3. baca `04_Dokumen/Aturan_AI_dan_Adaptasi_Obsidian_R01.md`
4. baca `02_Firmware/ESP32CamServerBridge/README.md`
5. baca `04_Dokumen/Planning_Implementasi_R02.md`
6. baca `prompt.md`
7. cek `ai/branches/<branch-slug>.md` untuk status branch aktif

## Branch Policy

- `main` = production snapshot
- `develop` = integration branch
- `feature/*` = kerja fitur
- `release/*` = persiapan rilis
- `hotfix/*` = perbaikan darurat

Jangan push kerja harian langsung ke `main`.

## Repo Rules

- source code aktif ada di `02_Firmware/ESP32CamServerBridge`
- dokumen engineering dan handoff ada di `04_Dokumen`
- file sensitif tidak boleh masuk dokumen umum
- struktur modular harus dipertahankan saat firmware mulai diisi
- jika menambah artifact generated, jelaskan mana file manual dan mana file generated

## Pull Request Checklist

- perubahan sesuai scope branch
- update worklog branch bila pekerjaannya bermakna
- tidak ada kredensial mentah ikut ter-commit
- dokumen di `04_Dokumen` ikut diperbarui jika keputusan arsitektur berubah
- jika menyentuh target server, sinkronkan `API_Server_Contract_R01.md`
