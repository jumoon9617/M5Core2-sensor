#include <M5Core2.h>
#include <ClosedCube_TCA9548A.h>
#include "Adafruit_SGP30.h"
#include "Ambient.h"

// センサのライブラリをinclude
#include "M5_ENV.h"

#define PaHub_I2C_ADDRESS 0x71
ClosedCube::Wired::TCA9548A tca9548a;

// Wi-Fi
WiFiClient client;
// Wi-Fiを入れる
const char *ssid = "SRC-WiFi-2G";              // sensor-net      教室のWi-Fi ssid
const char *password = "SoftwareResearchClub"; // sensor-net0101  教室のWi-Fi pass

// Ambient
Ambient ambient;
unsigned int channelId = 86262;            // AmbientのチャネルID
const char *writeKey = "c25541f2b6c1916b"; // ライトキー (not read key) 先にAmbientでチャンネルを作ること

// センサの変数宣言
// TVOC/eCO2
Adafruit_SGP30 sgp;
long last_millis = 0;
float tvoc = 0;
float eco2 = 0;

// ENV.3
SHT3X sht30;
QMP6988 qmp6988;

float tmp = 0.0;
float hum = 0.0;
float pressure = 0.0;

// 各センサ
const uint8_t ENV_CHANNEL = 1;
const uint8_t GAS_CHANNEL = 0;

void setup()
{
    M5.Lcd.setTextSize(2);
    M5.begin();
    Wire.begin();
    WiFi.begin(ssid, password);
    Serial.begin(115200);
    while (WiFi.status() != WL_CONNECTED)
    { //  Wi-Fi AP接続待ち
        delay(100);
    }
    M5.Lcd.println("WiFi connected\r\nIP address: ");
    M5.Lcd.println(WiFi.localIP());
    ambient.begin(channelId, writeKey, &client);

    tca9548a.address(PaHub_I2C_ADDRESS);

    tca9548a.selectChannel(ENV_CHANNEL);
    // tca9548a.selectChannel(0);
    qmp6988.init();

    tca9548a.selectChannel(GAS_CHANNEL);
    // tca9548a.selectChannel(1);

    if (!sgp.begin())
    {
        M5.Lcd.println("TVOC/eCO2 Sensor not found");
        while (1)
            ;
    }

    // if (!tca9548a.selectChannel(ENV_CHANNEL))
    // {
    //   M5.Lcd.println("Failed to select ENV channel");
    //   while (1)
    //     ;
    // }
    // if (!tca9548a.selectChannel(GAS_CHANNEL))
    // {
    //   M5.Lcd.println("Failed to select GAS channel");
    //   while (1)
    //     ;
    // }
}

void loop()
{
    // ENV3
    tca9548a.selectChannel(ENV_CHANNEL);
    pressure = qmp6988.calcPressure();
    if (sht30.get() == 0)
    {
        tmp = sht30.cTemp;
        hum = sht30.humidity;
    }
    else
    {
        M5.Lcd.println("Failed to get SHT30 data");
    }

    // TVOC/eCO2
    static int i = 5;
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

    tca9548a.selectChannel(GAS_CHANNEL);
    if (!sgp.IAQmeasure())
    { // Commands the sensor to take a single eCO2/VOC
        // measurement.  命令传感器进行一次eCO2/VOC测量
        M5.Lcd.println("Failed to measure TVOC/eCO2");
        return;
    }
    tvoc = sgp.TVOC;
    eco2 = sgp.eCO2;

    // Lcd表示
    M5.Lcd.fillRect(100, 40, 220, 90, BLACK);
    M5.Lcd.setCursor(0, 50);
    M5.Lcd.printf("Temp: %2.1f  \r\nHumi: %2.0f%%  \r\nPressure:%2.0fPa\r\n",
                  tmp, hum, pressure);
    M5.Lcd.printf("TVOC:%d ppb\n", sgp.TVOC);
    M5.Lcd.printf("eCO2:%d ppm\n", sgp.eCO2);
    delay(2000);

    ambient.set(1, tmp);
    ambient.set(2, hum);
    ambient.set(3, pressure);
    ambient.set(4, sgp.TVOC);
    ambient.set(5, sgp.eCO2);
    ambient.send();
    M5.Lcd.print("Sending to Ambient...\r\n");
    delay(5000);
}