# Pin Map R01

Dokumen ini sengaja masih berupa template konfirmasi.

## Default Hardware Assumption

- Board: AI-Thinker ESP32-CAM
- Camera stack: `esp_camera` dari Arduino-ESP32 core

## Catatan Penting

- Mapping pin kamera mengikuti profil board yang dipilih di contoh resmi `esp_camera`.
- Jangan hardcode pin map dari internet tanpa memastikan board variant yang benar-benar dipakai user.
- Jika board ternyata bukan AI-Thinker ESP32-CAM, perbarui dokumen ini sebelum coding kamera besar.

## Yang Harus Dikonfirmasi

- board variant final
- apakah flash LED akan dipakai
- apakah microSD dipakai atau tidak
- apakah ada trigger eksternal seperti PIR, tombol, atau sensor lain
- power supply final dan skenario brownout

## Saran Executor

Saat mulai implementasi, buat `config/Pins.h` sebagai source of truth firmware dan sinkronkan nilainya dengan dokumen ini.
