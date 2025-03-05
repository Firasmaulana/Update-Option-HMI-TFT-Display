# Update-Option-HMI-TFT-Display

# ESP32-S3 Sensor Monitor with UI

## Deskripsi Proyek
Proyek ini merupakan sistem monitoring sensor berbasis ESP32-S3 yang menggunakan tampilan UI berbasis LVGL pada layar TFT 3.2 inci. Sistem ini membaca data sensor kualitas udara, menampilkannya dalam bentuk teks dan grafik, serta mendukung pembaruan firmware melalui SD Card.

## Fitur Utama
- **Tampilan UI dengan LVGL**: Data sensor ditampilkan secara real-time.
- **Dukungan Touchscreen**: Menggunakan touchscreen XPT2046.
- **Navigasi UI dengan Tombol**: Dua tombol digunakan untuk berpindah antar menu dan memilih opsi.
- **Pembaruan Firmware via SD Card**: ESP32-S3 dapat memperbarui firmware secara langsung dari file `.bin` yang ada di SD Card.
- **Data Sensor Kualitas Udara**: Membaca data dari berbagai sensor seperti SO2, VOC, NO2, CO, PM1, PM2.5, dan PM10.
- **Kontrol LED dan Buzzer**: Indikasi menggunakan LED dan buzzer berdasarkan kondisi sistem.

## Perangkat Keras yang Digunakan
1. **ESP32-S3** (Board utama)
2. **Layar TFT 3.2 inci**
3. **XPT2046 Touchscreen Controller**
4. **SD Card Module**
5. **Sensor Kualitas Udara**
6. **Push Button (2 buah)**
7. **LED dan Buzzer**

## Koneksi Pin
| Komponen       | Pin ESP32-S3 |
|---------------|-------------|
| SD CS        | GPIO 5       |
| SD MOSI      | GPIO 6       |
| SD MISO      | GPIO 7       |
| SD SCK       | GPIO 8       |
| Touch CS     | GPIO 47      |
| Touch MOSI   | GPIO 35      |
| Touch MISO   | GPIO 37      |
| Touch CLK    | GPIO 36      |
| Tombol Next  | GPIO 1       |
| Tombol Select| GPIO 2       |
| LED 1        | GPIO 40      |
| LED 2        | GPIO 41      |
| UART RX      | GPIO 15      |
| UART TX      | GPIO 16      |

## Instalasi dan Konfigurasi
### 1. Instalasi Library yang Dibutuhkan
Pastikan Anda telah menginstal library berikut di Arduino IDE:
- [LVGL](https://github.com/lvgl/lvgl)
- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)
- [XPT2046_Touchscreen](https://github.com/PaulStoffregen/XPT2046_Touchscreen)
- [FreeRTOS](https://github.com/espressif/esp-idf/tree/master/components/freertos)
- [Update](https://github.com/espressif/arduino-esp32/tree/master/libraries/Update)
- [SD](https://github.com/espressif/arduino-esp32/tree/master/libraries/SD)
- [SPI](https://github.com/espressif/arduino-esp32/tree/master/libraries/SPI)
- [Wire](https://www.arduino.cc/en/Reference/Wire)

### 2. Konfigurasi TFT_eSPI
Edit file `User_Setup.h` di dalam folder **TFT_eSPI** sesuai dengan pin ESP32-S3 yang digunakan.

### 3. Upload Firmware ke ESP32-S3
1. Sambungkan ESP32-S3 ke PC melalui USB.
2. Pilih board **ESP32-S3 Dev Module** di Arduino IDE.
3. Upload kode ke ESP32-S3.

## Cara Menggunakan
### 1. Navigasi UI
- **Tombol Next** untuk berpindah antar objek di UI.
- **Tombol Select** untuk memilih opsi atau membuka layar lain.

### 2. Pembaruan Firmware melalui SD Card
1. Letakkan file firmware baru (`firmware.bin`) ke dalam SD Card di direktori root.
2. Masukkan SD Card ke ESP32-S3.
3. Jika firmware terdeteksi, akan muncul opsi "Update?" di UI.
4. Pilih opsi untuk memulai pembaruan firmware.

### 3. Membaca Data Sensor
- Data sensor dikirimkan melalui **UART Serial (GPIO 15 & 16)**
- Format data: `SO2:xxx,VOC:xxx,NO2:xxx,CO:xxx,PM1:xxx,PM25:xxx,PM10:xxx`
- UI akan otomatis diperbarui setelah menerima data baru.

## Struktur Kode
- **setup()**: Inisialisasi hardware, UI, dan SD Card.
- **loop()**: Memproses data sensor dan menangani pembaruan firmware.
- **processReceivedData()**: Parsing data sensor dari UART.
- **updateSensorUI()**: Menampilkan data sensor di layar.
- **updateFromSD()**: Memulai proses update firmware.
- **checkUpdateSwitch()**: Mengecek apakah tombol update ditekan.
- **my_disp_flush()**: Fungsi untuk menggambar tampilan pada layar.
- **my_touchpad_read()**: Fungsi untuk membaca input touchscreen.

## Catatan Tambahan
- Pastikan kabel touchscreen dan layar TFT terhubung dengan benar.
- Jika SD Card tidak terbaca, periksa koneksi pin SPI dan format kartu SD.
- Jangan cabut daya saat pembaruan firmware berlangsung untuk menghindari kerusakan sistem.

---
**Penulis:** [Firas Maulana]  
