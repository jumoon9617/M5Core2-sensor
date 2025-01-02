#include <M5Core2.h>
#include "M5_DLight.h"
#include "Adafruit_SGP30.h"

M5_DLight sensor;
uint16_t lux;

void setup()
{
    M5.begin();
    M5.Lcd.setTextSize(2);
    M5.Lcd.println("Unit_DLight_M5Core2 test");
    sensor.begin();
    sensor.setMode(CONTINUOUSLY_H_RESOLUTION_MODE);
}

void loop()
{
    lux = sensor.getLUX();

    // 数値部分を更新するためのクリア
    M5.Lcd.fillRect(0, 20, 240, 40, BLACK); // 画面の特定の領域を黒でクリア

    // 更新後の数値を表示
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.printf("lux: %d", lux);

    delay(100);
}
