/**
 * VÍ DỤ 4-06: CHUYỂN MẠCH CAMERA & GPS QUA MULTIPLEXER 74HC4066
 */
#include <Arduino.h>
#include <PTITCube.h>

PTIT_CDH cdh;

PTIT_Camera camera;
PTIT_GPS gps;
int photoCounter = 0;

bool capturePhoto() {
    camera.selectCamera();
    delay(200);

    char path[32];
    snprintf(path, sizeof(path), "/photos/mux_img_%d.jpg", photoCounter);

    bool success = camera.captureToFile(path);
    if (success) {
        cdh.printf("Camera: Da luu %s\n", path);
        photoCounter++;
    } else {
        cdh.println("Camera: Chup that bai.");
    }

    gps.selectGPS();
    delay(100);
    return success;
}

void setup() {
    cdh.begin(115200);

    camera.init(12, 14); // MUX: gpsSelect=12, camSelect=14
    gps.init(12, 14);

    gps.selectGPS();
}

void loop() {
    // 1. Cho phép chuyển sang Camera chụp ảnh theo lệnh ('cam', 'capture', 'photo')
    if (cdh.available()) {
        String cmd = cdh.readStringUntil('\n');
        cmd.trim();
        if (cmd.equalsIgnoreCase("cam") || cmd.equalsIgnoreCase("capture") || cmd.equalsIgnoreCase("photo")) {
            capturePhoto();
            return;
        }
    }

    // 2. Chế độ luân phiên tự động: Định kỳ chuyển sang Camera chụp 1 ảnh
    static unsigned long lastCapture = 0;
    if (millis() - lastCapture >= 10000) {
        lastCapture = millis();
        capturePhoto();
    }

    // 3. Đọc dữ liệu GPS khi đang ở kênh GPS
    gps.update();
    static unsigned long lastGpsPrint = 0;
    if (millis() - lastGpsPrint >= 2000) {
        lastGpsPrint = millis();
        if (gps.hasFix()) {
            cdh.printf("GPS: Lat: %.6f, Lng: %.6f | Alt: %.1fm\n",
                       gps.getLatitude(), gps.getLongitude(), gps.getAltitude());
        }
    }
}
