#include <Arduino.h>
#include <M5Core2.h>

void setup()
{
    // M5Stackの初期化
    M5.begin();
    M5.Lcd.fillScreen(BLACK); // 背景を黒に設定
    M5.Lcd.setTextColor(WHITE);

    // 顔の輪郭（円）
    M5.Lcd.drawCircle(160, 120, 100, WHITE); // 中心(160, 120)、半径100px

    // 眉毛
    M5.Lcd.drawLine(120, 80, 140, 80, WHITE); // 左眉毛
    M5.Lcd.drawLine(180, 80, 200, 80, WHITE); // 右眉毛

    // 目
    M5.Lcd.drawCircle(130, 100, 5, WHITE); // 左目
    M5.Lcd.drawCircle(190, 100, 5, WHITE); // 右目

    // 鼻（の字）
    M5.Lcd.drawLine(160, 110, 150, 140, WHITE); // 鼻の左下
    M5.Lcd.drawLine(150, 140, 170, 140, WHITE); // 鼻の横線
    M5.Lcd.drawLine(170, 140, 160, 110, WHITE); // 鼻の右下

    // 口
    M5.Lcd.drawLine(130, 180, 190, 180, WHITE);
    M5.Lcd.drawLine(190, 180, 190, 200, WHITE);
    M5.Lcd.drawLine(190, 200, 130, 200, WHITE);
    M5.Lcd.drawLine(130, 200, 130, 180, WHITE);
}

void loop()
{
    // 特に動作は不要
}