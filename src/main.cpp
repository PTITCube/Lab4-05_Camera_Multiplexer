/**
 * VÍ DỤ 4-06: CHUYỂN MẠCH CAMERA & GPS QUA MULTIPLEXER 74HC4066
 */
#include <Arduino.h>
#include <PTITCube.h>

PTIT_CDH cdh;

PTIT_Camera camera;
PTIT_GPS gps;

void setup() {
    cdh.begin(115200);

    camera.init(12, 14); // MUX: gpsSelect=12, camSelect=14
    gps.init(12, 14);
}

void loop() {
    // 1. Chế độ Camera
    camera.selectCamera();
    delay(100);

    if (camera.isReady()) {
        cdh.println("[MUX] Kênh Camera: Chụp ảnh...");
        camera.captureToFile("/photos/multiplexer_test.jpg");
    }
    delay(3000);

    // 2. Chế độ GPS
    gps.selectGPS();
    delay(500);

    cdh.println("[MUX] Kênh GPS: Đang cập nhật...");
    unsigned long startTime = millis();
    while (millis() - startTime < 3000) {
        gps.update();
        if (gps.hasFix()) {
            cdh.printf("[GPS] Lat: %.6f, Lng: %.6f\n", gps.getLatitude(), gps.getLongitude());
            break;
        }
        delay(10);
    }

    delay(2000);
}
