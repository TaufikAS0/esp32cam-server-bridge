# Aturan AI dan Adaptasi Obsidian R01

Dokumen ini menyalin inti aturan dari vault Obsidian firmware ke bentuk yang lebih langsung dipatuhi oleh AI executor di repo ini.

## Sumber Yang Dipakai

Aturan repo ini disusun dari note berikut:

1. `2026 Vault Firmware Rules/00_Start_Here/Firmware_AI_Vault_Home.md`
2. `2026 Vault Firmware Rules/01_Rules/Rules_Firmware_Standar.md`
3. `2026 Vault Firmware Rules/01_Rules/Rules_GitHub_Gitflow.md`
4. `2026 Vault Firmware Rules/CONTRIBUTING.md`
5. `2026 Vault Firmware Rules/01_Rules/Rules_Warna_Node_Obsidian.md`

## Inti Aturan Yang Dibawa Ke Repo Ini

### 1. Vault Mindset

- Perlakukan aturan ini sebagai source of truth kerja repo.
- Jangan anggap repo sebagai scratchpad bebas.
- Bedakan dokumen manual dari artifact generated.

### 2. Read Order Untuk AI

Saat executor baru masuk ke repo ini, baca dalam urutan:

1. `README.md`
2. `AGENTS.md`
3. dokumen ini
4. `02_Firmware/ESP32CamServerBridge/README.md`
5. `Planning_Implementasi_R02.md`
6. `API_Server_Contract_R01.md`
7. `prompt.md`
8. worklog branch aktif di `ai/branches/`

### 3. Aturan Foldering

Gunakan struktur root modern:

- `01_Desain`
- `02_Firmware`
- `03_Produksi`
- `04_Dokumen`
- `_arsip`
- `_sandbox`

Jangan menambah hirarki baru di root tanpa alasan kuat.

### 4. Aturan Firmware

- `.ino` atau entry file harus tipis.
- Pisahkan `config`, `core`, `state`, `drivers`, `modules`, `comms`, dan `utils`.
- Hindari `delay()` di runtime loop utama.
- Jangan campur akses hardware, logic bisnis, dan transport server di satu file.
- Jika config runtime harus survive reboot, simpan di `Preferences` atau mekanisme setara yang memang disetujui.

### 5. Aturan Konektivitas

- Default repo ini adalah device ke HTTP API server.
- Jangan mengganti arah arsitektur ke MQTT, WebSocket, atau cloud lain tanpa instruksi user.
- Flow upload dan retry harus dirancang jelas sejak awal walau implementasinya belum lengkap.

### 6. Aturan Dokumentasi Minimum

Minimal dokumen yang harus tetap hidup:

- `README.md`
- `AGENTS.md`
- `02_Firmware/ESP32CamServerBridge/README.md`
- `Planning_Implementasi_R02.md`
- `API_Server_Contract_R01.md`
- handoff prompt untuk executor berikutnya
- `prompt.md`

Jika keputusan desain berubah, perbarui dokumen sebelum atau bersamaan dengan perubahan kode.

### 7. Aturan Rahasia dan Profil Lokal

- Jangan taruh password, API key, atau token mentah di dokumen umum.
- Jika harus ada data lokal, letakkan di file lokal yang di-ignore Git.
- Jangan salin pola vault merah atau profil sensitif ke dokumen umum repo ini.

### 8. Aturan Gitflow

- `main` = production snapshot
- `develop` = integration
- `feature/*` = branch fitur dari `develop`
- `release/*` = branch stabilisasi dari `develop`
- `hotfix/*` = branch darurat dari `main`

Larangan:

- jangan commit atau push kerja harian langsung ke `main`
- jangan merge `feature/*` ke `main`
- jangan lompat merge tanpa jalur Gitflow yang benar

### 9. Adaptasi Aturan Warna Obsidian

Aturan warna Obsidian dibaca sebagai klasifikasi informasi, bukan kewajiban visual repo Git.

Interpretasi yang relevan untuk repo ini:

- entry point penting = `README.md`, `AGENTS.md`, handoff docs
- aturan inti = dokumen rule dan planning
- baseline aman = implementasi yang sudah tervalidasi nanti
- profil sensitif = tidak boleh masuk dokumen umum
- arsip = pindahkan ke `_arsip`, bukan bercampur dengan file aktif

### 10. Behavior Yang Wajib Diikuti AI

- jangan hapus scaffold ini lalu mulai ulang dari nol tanpa alasan kuat
- jangan menebak kontrak server jika dokumen kontrak belum diperbarui
- jangan membuat struktur source acak di luar `02_Firmware/ESP32CamServerBridge`
- jangan menganggap repo kosong berarti bebas melanggar aturan arsitektur

## Checklist Executor

- [ ] membaca urutan dokumen yang benar
- [ ] bekerja di branch yang sesuai
- [ ] menjaga struktur root
- [ ] menjaga modularitas firmware
- [ ] memperbarui dokumen bila keputusan berubah
- [ ] menghindari kebocoran kredensial
