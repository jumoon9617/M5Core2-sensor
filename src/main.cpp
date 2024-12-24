#include <M5Core2.h>
#include "Adafruit_SGP30.h"
#include "M5_DLight.h"

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

char info[40];

void loop()
{
  lux = sensor.getLUX();
  sprintf(info, "lux: %d", lux);
  M5.Lcd.println(info);
}