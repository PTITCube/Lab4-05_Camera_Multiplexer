/**
 * VÍ DỤ 4-06: CHUYỂN MẠCH CAMERA & GPS QUA MULTIPLEXER 74HC4066
 */
#include <Arduino.h>
#include <PTITCube.h>

PTIT_CDH cdh;
PTIT_Camera camera;
PTIT_GPS gps;
int photoCount = 0;

void setup() {
    cdh.begin(115200);
    camera.init(12, 14);
    gps.init(12, 14);
    gps.selectGPS();
}

void loop() {
    if (cdh.available()) {
        char cmd = cdh.read();
        if (cmd == 'p' || cmd == 'P') {
            camera.selectCamera();
            delay(200);

            String path = "/photos/mux_" + String(photoCount++) + ".jpg";
            if (camera.captureToFile(path.c_str())) {
                cdh.println("Camera: Da luu " + path);
            } else {
                cdh.println("Camera: Chup that bai.");
            }

            gps.selectGPS();
            delay(100);
        }
    }

    gps.update();
    static unsigned long lastGps = 0;
    if (millis() - lastGps >= 2000) {
        lastGps = millis();
        if (gps.hasFix()) {
            cdh.printf("GPS: Lat: %.6f, Lng: %.6f\n", gps.getLatitude(), gps.getLongitude());
        }
    }
}
