// インクルードファイル
#include <M5Core2.h>
#include "Ambient.h"

// 各センサに必要なインクルードファイル
// #include <M5Core2.h>
#include "Adafruit_SGP30.h"

// Wi-Fi
WiFiClient client;
// Wi-Fiを入れる
const char *ssid = "sensor-net";         // sensor-net      教室のWi-Fi ssid
const char *password = "sensor-net0101"; // sensor-net0101  教室のWi-Fi pass

// Ambient
Ambient ambient;
unsigned int channelId = 85296;            // AmbientのチャネルID
const char *writeKey = "11f450fefbb573df"; // ライトキー (not read key) 先にAmbientでチャンネルを作ること
Adafruit_SGP30 sgp;
long last_millis = 0;

void setup()
{
    M5.begin(true, false, true, true);
    WiFi.begin(ssid, password); //  Wi-Fi APに接続
    while (WiFi.status() != WL_CONNECTED)
    { //  Wi-Fi AP接続待ち
        delay(100);
    }
    Serial.print("WiFi connected\r\nIP address: ");
    Serial.println(WiFi.localIP());
    ambient.begin(channelId, writeKey, &client); // チャネルIDとライトキーを指定してAmbientの初期化

    // センサの初期化
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(80, 0);
    M5.Lcd.println("TVOC TEST");
    if (!sgp.begin())
    {
        M5.Lcd.println("Sensor not found");
        while (1)
            ;
    }
    M5.Lcd.setCursor(0, 80);
    M5.Lcd.println("\nInitialization...");
}

void loop()
{
    // センサの動き
    static int i = 15;
    while (i > 0)
    {
        if (millis() - last_millis > 1000)
        {
            last_millis = millis();
            i--;
            M5.Lcd.fillRect(20, 120, 60, 30, BLACK);
            M5.Lcd.drawNumber(i, 20, 120, 2);
        }
    }
    M5.Lcd.fillRect(0, 80, 90, 100, BLACK);

    if (!sgp.IAQmeasure())
    {
        Serial.println("Measurement failed");
        return;
    }
    M5.Lcd.fillRect(100, 40, 220, 90, TFT_BLACK);
    M5.Lcd.setCursor(0, 50);
    M5.Lcd.printf("TVOC:%d ppb\n", sgp.TVOC);
    M5.Lcd.printf("eCO2:%d ppm\n", sgp.eCO2);
    delay(500);

    // Ambientに送る　例：気圧 　更に文字列変換のヒントは上↑にあります。
    ambient.set(1, sgp.TVOC);
    ambient.set(2, sgp.eCO2);
    // ambient.set(3, pressure);
    ambient.send();
    M5.Lcd.print("Sending to Ambient...\r\n");
    delay(5000);
}