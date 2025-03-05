#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include "ui.h"
#include "ui_helpers.h"
#include "freeRTOS/task.h"
#include "freeRTOS/FreeRTOS.h"

#include <SD.h>
#include <Update.h>
#include <FS.h>
#include <SPI.h>
#define SD_CS    5     // Pin CS
#define SD_MOSI  6     // Pin MOSI
#define SD_MISO  7     // Pin MISO
#define SD_SCK   8     // Pin SCK
#define MAX_FILES 10  // Maksimal file yang ditampilkan di dropdown

const char* updateBin = "/firmware.bin";
String fileList[MAX_FILES];  // Menyimpan daftar file .bin
lv_obj_t *dropdown;          // Pointer ke dropdown UI
//switch_states = lv_obj_has_state(target, LV_STATE_CHECKED);

#include <Wire.h>
#include <HardwareSerial.h>

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240

const int led_pins = 40;
const int led_pins1 = 41;


#define BUTTON_NEXT_PIN   1  // Tombol untuk navigasi (pindah fokus objek), aktif LOW
#define BUTTON_SELECT_PIN 2  // Tombol untuk seleksi (eksekusi aksi pada objek fokus), aktif LOW
#define LVGL_TICK_PERIOD  20
lv_group_t *groups[6];
int current_screen = 0; // Layar default adalah layar 1
TaskHandle_t Task1;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[SCREEN_WIDTH * SCREEN_HEIGHT / 10];

TFT_eSPI tft = TFT_eSPI();
HardwareSerial SerialCentral(1);

// Definisi pin touchscreen
#define XPT2046_IRQ 3     // GPIO3 (TOUCH3)
#define XPT2046_MOSI 35   // GPIO35 (SPI1_06)
#define XPT2046_MISO 37   // GPIO37 (SPIDQS)
#define XPT2046_CLK 36    // GPIO36 (SPI1_07)
#define XPT2046_CS 47     // GPIO47 (SPICLK_P)

// Inisialisasi touchscreen
XPT2046_Touchscreen touchscreen(XPT2046_CS);

// Variabel data sensor
String currentSO2 = "0.000 ppm";
String currentVOC = "0.000 ppm";
String currentNO2 = "0.000 ppm";
String currentCO = "0.000 ppm";
String currentPM1 = "0.000 ug/m3";
String currentPM25 = "0.000 ug/m3";
String currentPM10 = "0.000 ug/m3";

String receivedData = "";
bool dataReceived = false;

/* Fungsi Display */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

/* Fungsi untuk membaca touchscreen */
void my_touchpad_read(lv_indev_drv_t * indev_driver, lv_indev_data_t * data) {
    if (touchscreen.touched()) {
        TS_Point p = touchscreen.getPoint();
        Serial.print("Touch detected at: X=");
        Serial.print(p.x);
        Serial.print(", Y=");
        Serial.println(p.y);

        // Konversi koordinat agar sesuai dengan layar
        data->point.x = map(p.x, 200, 3700, 0, SCREEN_WIDTH);
        data->point.y = map(p.y, 240, 3800, 0, SCREEN_HEIGHT);
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
}

void updateSensorUI() {
    lv_label_set_text(ui_LabelSO2, currentSO2.c_str());
    lv_label_set_text(ui_LabelVOC, currentVOC.c_str());
    lv_label_set_text(ui_LabelNO2, currentNO2.c_str());
    lv_label_set_text(ui_LabelCO, currentCO.c_str());
    lv_label_set_text(ui_LabelPM1, currentPM1.c_str());
    lv_label_set_text(ui_LabelPM25, currentPM25.c_str());
    lv_label_set_text(ui_LabelPM10, currentPM10.c_str());
    lv_coord_t pm1_value = currentPM1.substring(0, currentPM1.indexOf(" ")).toInt();
    lv_coord_t pm25_value = currentPM25.substring(0, currentPM25.indexOf(" ")).toInt();
    lv_coord_t pm10_value = currentPM10.substring(0, currentPM10.indexOf(" ")).toInt();

    lv_chart_set_next_value(ui_Chart2, ui_Chart2_series_1, pm1_value);
    lv_chart_set_next_value(ui_Chart2, ui_Chart2_series_2, pm25_value);
    lv_chart_set_next_value(ui_Chart2, ui_Chart2_series_3, pm10_value);
    
    lv_chart_refresh(ui_Chart2);
}

void processReceivedData(String data) {
    Serial.println("Received data: " + data);
    
    currentSO2 = data.substring(data.indexOf("SO2:") + 4, data.indexOf(",", data.indexOf("SO2:"))) + " ppm";
    currentVOC = data.substring(data.indexOf("VOC:") + 4, data.indexOf(",", data.indexOf("VOC:"))) + " ppm";
    currentNO2 = data.substring(data.indexOf("NO2:") + 4, data.indexOf(",", data.indexOf("NO2:"))) + " ppm";
    currentCO = data.substring(data.indexOf("CO:") + 3, data.indexOf(",", data.indexOf("CO:"))) + " ppm";
    currentPM1 = data.substring(data.indexOf("PM1:") + 4, data.indexOf(",", data.indexOf("PM1:"))) + " ug/m3";
    currentPM25 = data.substring(data.indexOf("PM25:") + 5, data.indexOf(",", data.indexOf("PM25:"))) + " ug/m3";
    currentPM10 = data.substring(data.indexOf("PM10:") + 5, data.length()) + " ug/m3";
    
    updateSensorUI();
}

/*Set tick routine needed for LVGL internal timings*/
static uint32_t my_tick_get_cb (void) { return millis(); }

//Create timer for LED control
void led_update_cb(lv_timer_t * timer) {
  // Update the LED status according to the corresponding switch.
    if(switch_states == true) {
        tone(led_pins1, 440);  // Nyalakan LED
        delay(100);
        tone(led_pins1, 659);
        delay(100);
        noTone(led_pins1);
    } else {
        //digitalWrite(led_pins1, LOW);   // Matikan LED
        noTone(led_pins1);
    }

    if(switch_states1 == true) {
        tone(led_pins1, 494);  // Nyalakan LED
        delay(100);
        tone(led_pins1, 587);
        delay(100);
        noTone(led_pins1);
    } else {
        //digitalWrite(led_pins1, LOW);   // Matikan LED
        noTone(led_pins1);
    }
    if (digitalRead(BUTTON_SELECT_PIN) == LOW) {  // Button ditekan (active low karena pull-up)
    // Bunyikan buzzer
    digitalWrite(led_pins, HIGH);
    delay(100);  // Buzzer berbunyi selama 100ms
    digitalWrite(led_pins, LOW);
    
    // Tunggu sampai button dilepas untuk menghindari bounce
    while(digitalRead(BUTTON_SELECT_PIN) == LOW) {
      delay(10);
    }
    }
    if (digitalRead(BUTTON_NEXT_PIN) == LOW) {  // Button ditekan (active low karena pull-up)
    // Bunyikan buzzer
    tone(led_pins1, 587);
    delay(100);  // Buzzer berbunyi selama 100ms
    noTone(led_pins1);
    
    // Tunggu sampai button dilepas untuk menghindari bounce
    while(digitalRead(BUTTON_NEXT_PIN) == LOW) {
      delay(10);
    }
    }
}
   // add more lED or Relay
   //digitalWrite(led_pins[n], switch_states[n] ? HIGH : LOW);

void set_group(){
    // Membuat grup untuk 3 layar
    groups[0] = lv_group_create();  // Grup untuk Layar 1
    groups[1] = lv_group_create();  // Grup untuk Layar 2
    groups[2] = lv_group_create();  // Grup untuk Layar 3
    groups[3] = lv_group_create();
    groups[4] = lv_group_create();
    groups[5] = lv_group_create();
    groups[6] = lv_group_create();
    // Menambahkan objek UI (misalnya tombol) ke grup Layar 1
    lv_group_add_obj(groups[0], ui_Button1);
    
    // Menambahkan objek UI ke grup Layar 2
    lv_group_add_obj(groups[1], ui_Button2);
    lv_group_add_obj(groups[1], ui_Button3);
    lv_group_add_obj(groups[1], ui_Button6);
    lv_group_add_obj(groups[1], ui_Button11);
    lv_group_add_obj(groups[1], ui_Button10);
    lv_group_add_obj(groups[1], ui_Button15);

    // Menambahkan objek UI ke grup Layar 3
    lv_group_add_obj(groups[2], ui_Button4);

    lv_group_add_obj(groups[3], ui_Button5);

    lv_group_add_obj(groups[4], ui_Button8);
    lv_group_add_obj(groups[4], ui_Button9);
    lv_group_add_obj(groups[4], ui_Button7);

    lv_group_add_obj(groups[5], ui_Button12);
    lv_group_add_obj(groups[5], ui_Button13);

    lv_group_add_obj(groups[6], ui_Button14);

    // Mengatur grup default yang digunakan oleh LVGL adalah grup layar 1
    lv_group_set_default(groups[0]);
}

// Fungsi untuk mengirim event "pressed" ke objek tombol, kemudian mengatur ulang status objek
void button_state(lv_obj_t *button, bool Pressed) {
    if (Pressed) {
        lv_event_send(button, LV_EVENT_PRESSED, NULL);  // Kirim event "pressed" ke objek tombol
        lv_obj_clear_state(button, LV_STATE_PRESSED);     // Hapus status "pressed" dari objek
        lv_obj_add_state(button, LV_STATE_DEFAULT);       // Atur kembali status ke default
    }
}

void group_action(void *pvParameters) {
    bool prevNextState   = false;   // Menyimpan status tombol NEXT dari pembacaan sebelumnya
    bool prevSelectState = false;   // Menyimpan status tombol SELECT dari pembacaan sebelumnya

    while(1) {
        // Baca status tombol (asumsi: tombol terhubung dengan konfigurasi internal pull-up, jadi LOW berarti ditekan)
        bool nextPressed   = (digitalRead(BUTTON_NEXT_PIN) == LOW);
        bool selectPressed = (digitalRead(BUTTON_SELECT_PIN) == LOW);

        // Jika tombol NEXT baru saja ditekan, pindahkan fokus ke objek berikutnya dalam grup
        if (nextPressed && !prevNextState) {
            lv_group_t *current_group = groups[current_screen];  // Ambil grup sesuai layar aktif
            lv_group_focus_next(current_group);  // Pindah fokus ke objek berikutnya
            delay(50); // Delay sederhana untuk debounce (menghindari pembacaan berulang karena bouncing)
        }

        // Jika tombol SELECT baru saja ditekan, lakukan aksi seleksi berdasarkan objek yang sedang fokus
        if (selectPressed && !prevSelectState) {
            lv_group_t *current_group = groups[current_screen];  // Ambil grup layar aktif
            lv_obj_t *focused_obj   = lv_group_get_focused(current_group);  // Dapatkan objek yang sedang mendapatkan fokus

            // Eksekusi aksi sesuai dengan layar aktif dan objek yang dipilih
            if (current_screen == 0) { // Jika layar 1 aktif
                if (focused_obj == ui_Button1) {
                    lv_scr_load(ui_Screen3); // Ganti tampilan ke layar 2
                    current_screen = 1;      // Perbarui variabel layar aktif
                    button_state(focused_obj, true); // Kirim event dan update status objek
                }
            }
            else if (current_screen == 1) { // Jika layar 2 aktif
                if (focused_obj == ui_Button2) {
                    lv_scr_load(ui_Screen2); // Kembali ke layar 1
                    current_screen = 2;
                    button_state(focused_obj, true);
                }
                else if (focused_obj == ui_Button3) {
                    lv_scr_load(ui_Screen4); // Kembali ke layar 1
                    current_screen = 3;
                    button_state(focused_obj, true);
                }
                else if (focused_obj == ui_Button6) {
                    lv_scr_load(ui_Screen5); // Kembali ke layar 1
                    current_screen = 4;
                    // Aksi tambahan untuk tombol 5 pada layar 2
                    button_state(focused_obj, true);
                }
                else if (focused_obj == ui_Button11) {
                    lv_scr_load(ui_Screen6); // Kembali ke layar 1
                    current_screen = 5;
                    // Aksi tambahan untuk tombol 5 pada layar 2
                    button_state(focused_obj, true);
                }
                else if (focused_obj == ui_Button10) {
                    lv_scr_load(ui_Screen1); // Kembali ke layar 1
                    current_screen = 0;
                    // Aksi tambahan untuk tombol 5 pada layar 2
                    button_state(focused_obj, true);
                }
                else if (focused_obj == ui_Button15) {
                    lv_scr_load(ui_Screen7); // Kembali ke layar 1
                    current_screen = 6;
                    // Aksi tambahan untuk tombol 5 pada layar 2
                    button_state(focused_obj, true);
                }
            }
            else if (current_screen == 2) { // Jika layar 3 aktif
                if (focused_obj == ui_Button4) {
                    lv_scr_load(ui_Screen3); // Kembali ke layar 1
                    current_screen = 1;
                    button_state(focused_obj, true);
                }
            }
            else if (current_screen == 3) { // Jika layar 3 aktif
                if (focused_obj == ui_Button5) {
                    lv_scr_load(ui_Screen3); // Kembali ke layar 1
                    current_screen = 1;
                    button_state(focused_obj, true);
                }
            }
            else if (current_screen == 4) { // Jika layar 2 aktif
                if (focused_obj == ui_Button9) {
                    button_state(focused_obj, true);
                    delay(100);
                    switch_states1 = false;
                }
                else if (focused_obj == ui_Button8) {
                    button_state(focused_obj, true);
                    delay(100);
                    switch_states = false;
                }
                else if (focused_obj == ui_Button7) {
                    lv_scr_load(ui_Screen3);
                    current_screen = 1;
                    button_state(focused_obj, true);
                }

            }
            else if (current_screen == 5) { // Jika layar 2 aktif
                if (focused_obj == ui_Button13) {
                    lv_scr_load(ui_Screen3);
                    current_screen = 1;
                    button_state(focused_obj, true);
                }
                else if (focused_obj == ui_Button12) {
                    lv_scr_load(ui_Screen3);
                    current_screen = 1;
                    button_state(focused_obj, true);
                }
            }
            else if (current_screen == 6) { // Jika layar 2 aktif
                if (focused_obj == ui_Button14) {
                    lv_scr_load(ui_Screen3);
                    current_screen = 1;
                    button_state(focused_obj, true);
                }
            }
            delay(50); // Delay debounce untuk tombol SELECT
        }

        // Simpan status tombol saat ini untuk perbandingan pada iterasi berikutnya
        prevNextState   = nextPressed;
        prevSelectState = selectPressed;

        // Beri waktu kecil pada task untuk memungkinkan task lain berjalan (FreeRTOS)
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void updateFromSD() {
    Serial.println("Memulai update dari SD Card...");
    File updateBinFile = SD.open(updateBin);
    if (updateBinFile) {
        if(updateBinFile.isDirectory()){
            Serial.println("Error, firmware.bin is not a file");
            updateBinFile.close();
            return;
        }

        size_t updateSize = updateBinFile.size();

        if (updateSize > 0) {
            Serial.println("Try to start update");
            performUpdate(updateBinFile, updateSize);
        } else {
            Serial.println("Error, file is empty");
        }

        updateBinFile.close();
    } else {
        Serial.println("Could not load firmware.bin from sd root");
    }
}

void initSDCard() {
    // Inisialisasi SD Card dengan SPI yang terpisah
    SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
    
    // Tambahan debugging untuk SD Card
    Serial.println("Mencoba mount SD Card...");
    if (!SD.begin(SD_CS)) {
        Serial.println("SD Card Mount Failed");
        Serial.println("Periksa koneksi pin:");
        Serial.println("SD CS   -> GPIO" + String(SD_CS));
        Serial.println("SD MOSI -> GPIO" + String(SD_MOSI));
        Serial.println("SD MISO -> GPIO" + String(SD_MISO));
        Serial.println("SD SCK  -> GPIO" + String(SD_SCK));
        return;
    }
    
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE) {
        Serial.println("No SD Card attached");
        return;
    }
    
    Serial.print("SD Card Type: ");
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }
    
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
    Serial.println("SD Card Mounted Successfully");
    
    
    // Cek file firmware.bin di SD Card
    if (SD.exists(updateBin)) {
        Serial.println("firmware.bin ditemukan di SD Card");
        lv_label_set_text(ui_LabelOFF4, "Update?");
        //
    } else {
        Serial.println("firmware.bin tidak ditemukan di SD Card");
    }
}

// Fungsi ini diasumsikan dipanggil dari HMI Squareline saat switch diaktifkan
void setUpdateFlag(bool value) {
    switch_states2 = value;
}

void listBinFiles() {
    File root = SD.open("/");
    if (!root) {
        Serial.println("Failed to open directory!");
        return;
    }
    
    int fileCount = 0;
    String options = ""; // String untuk daftar opsi dropdown

    while (true) {
        File entry = root.openNextFile();
        if (!entry) break;

        String fileName = entry.name();
        if (fileName.endsWith(".bin")) {
            if (fileCount < MAX_FILES) {
                fileList[fileCount] = fileName;
                Serial.println("File ditemukan: " + fileName);
                options += fileCount > 0 ? "\n" : ""; 
                options += fileName;
                fileCount++;
            }
        }
        entry.close();
    }
    root.close();

    if (fileCount > 0) {
        lv_dropdown_set_options(ui_Dropdown2, options.c_str());
    } else {
        lv_dropdown_set_options(ui_Dropdown2, "No .bin files found");
    }
}

void updateSelectedFirmware() {
    SPI.end();  
    delay(100);
    SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
    char selectedFile[64];
    memset(selectedFile, 0, sizeof(selectedFile));

    if (ui_Dropdown2 == NULL) {
        Serial.println("Error: Dropdown belum diinisialisasi!");
        return;
    }

    lv_dropdown_get_selected_str(ui_Dropdown2, selectedFile, sizeof(selectedFile));

    if (strlen(selectedFile) == 0) {
        Serial.println("Error: Tidak ada firmware yang dipilih!");
        return;
    }

    Serial.print("Firmware yang dipilih: ");
    Serial.println(selectedFile);

    String firmwarePath = "/" + String(selectedFile);

    if (!SD.exists(firmwarePath)) {
        Serial.println("Error: File tidak ditemukan di SD Card!");
        return;
    }

    Serial.println("Memulai update dari " + firmwarePath);

    File updateBinFile = SD.open(firmwarePath);
    if (!updateBinFile) {
        Serial.println("Error: Gagal membuka file!");
        return;
    }

    size_t updateSize = updateBinFile.size();
    if (updateSize == 0) {
        Serial.println("Error: File kosong!");
        updateBinFile.close();
        return;
    }

    Serial.println("Memulai update...");
    performUpdate(updateBinFile, updateSize);
}

// Fungsi untuk memproses update firmware
void performUpdate(Stream &updateSource, size_t updateSize) {
    Serial.println("Menyiapkan update firmware...");

    if (!Update.begin(updateSize)) {
        Serial.println("Error: Tidak dapat memulai update!");
        return;
    }

    size_t written = Update.writeStream(updateSource);
    Serial.println("Jumlah data yang ditulis: " + String(written) + " byte");

    if (written != updateSize) {
        Serial.println("Error: Data tidak sesuai! " + String(written) + "/" + String(updateSize));
        return;
    }

    if (!Update.end()) {
        Serial.println("Error: Update gagal dengan kode " + String(Update.getError()));
        return;
    }

    Serial.println("Update berhasil! Rebooting...");
    delay(1000);
    ESP.restart();
}

// Fungsi untuk mengecek apakah tombol update ditekan
void checkUpdateSwitch() {
    if (switch_states2) {
        Serial.println("Switch update aktif, memulai pembaruan firmware...");
        switch_states2 = false;  
        updateSelectedFirmware();
    }
}



void setup() {
    Serial.begin(115200);
    SerialCentral.begin(9600, SERIAL_8N1, 15, 16);
    Serial.println("ESP32-S3 Sensor Monitor with UI Started");

    pinMode(led_pins, OUTPUT);
    pinMode(led_pins1, OUTPUT);

    pinMode(BUTTON_NEXT_PIN, INPUT_PULLUP);
    pinMode(BUTTON_SELECT_PIN, INPUT_PULLUP);



    digitalWrite(led_pins, HIGH);
    delay(100);  // Buzzer berbunyi selama 100ms
    digitalWrite(led_pins, LOW);

    lv_init();
    tft.begin();
    tft.setRotation(1);
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, SCREEN_WIDTH * SCREEN_HEIGHT / 10);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = SCREEN_WIDTH;
    disp_drv.ver_res = SCREEN_HEIGHT;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    ui_init();
    // Membuat dan mengatur grup-grup untuk tiap layar
    set_group();
    // Membuat task FreeRTOS untuk menangani input tombol, task ini dijalankan pada core 1
    xTaskCreatePinnedToCore(group_action, "GroupTask", 8192, NULL, 4, &Task1, 1);
    lv_timer_create(led_update_cb, 100, NULL);
    Serial.println("Setup selesai, LVGL siap!");
    initSDCard();
    listBinFiles();
    SPI.end();
    SPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
    SPI.setFrequency(1000000);
    Serial.println("SPI Initialized for Touchscreen");

    if (!touchscreen.begin()) {
        Serial.println("Touchscreen tidak terdeteksi!");
    } else {
        Serial.println("Touchscreen terdeteksi!");
    }
    touchscreen.setRotation(1);
}

void loop() {
checkUpdateSwitch();
    while (SerialCentral.available()) {
        char c = SerialCentral.read();
        if (c == '\n') {
            dataReceived = true;
            processReceivedData(receivedData);
            receivedData = "";
        } else {
            receivedData += c;
        }
    }
    // if (switch_states2 == true) {
    //     SPI.end();
    //     initSDCard();

    //     updateStarted = true; // Set flag agar update hanya terjadi sekali
    //     Serial.println("Switch diaktifkan, memulai update firmware...");
        
    //     // Cek ulang SD Card sebelum memulai update
    //     if (!SD.begin(SD_CS)) {
    //         Serial.println("SD Card tidak terdeteksi. Update dibatalkan.");
    //         updateStarted = false; // Reset flag jika update gagal
    //         switch_states2 = false; // Reset switch state
    //     } else if (!SD.exists(updateBin)) {
    //         Serial.println("File firmware.bin tidak ditemukan. Update dibatalkan.");
    //         updateStarted = false; // Reset flag jika update gagal
    //         switch_states2 = false; // Reset switch state
    //     } else {
    //         // Jalankan update
    //         updateFromSD();
    //         // Catatan: tidak perlu reset flag karena perangkat akan reboot jika berhasil
    //     }
    // }
    lv_timer_handler();
    delay(5);
}

