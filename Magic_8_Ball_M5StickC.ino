#define FS_NO_GLOBALS
#include <FS.h>
#include <TFT_eSPI.h>
#include <M5StickC.h>
#include "Free_Fonts.h"

#ifdef ESP32
#include "SPIFFS.h"
#endif

char *magicTop[] = {"As I see it,", "Ask again", "Better not", "Cannot predict", "Concentrate", "Dont count", "It is", "Decidedly", "Most", "My reply", "My sources", "Outlook not", "Outlook", "Reply hazy,", "Signs point", "Very", "Without", "Yes", "Yes", "You may rely"};
char *magicBottom[] = {"yes", "later", "tell you now", "now", "and ask again", "on it", "certain", "so", "likely", "is no", "say no", "so good", "good", "try again", "to yes", "doubtful", "a doubt", " ", "definitely", "on it"};
long randomPick;
float accX = 0;
float accY = 0;
float accZ = 0;

TFT_eSPI tft = TFT_eSPI();

void setup() {

  Serial.begin(115200);

  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield();
  }

  Serial.println("\r\nSPIFFS initialised.");

  M5.begin();
  M5.IMU.Init();
  randomSeed(analogRead(0));
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  for (int vertPos = -80; vertPos <= 0; vertPos += 3) {
    drawBmp("/magic8.bmp", 0, vertPos);
  }
  delay(1000);
  for (int vertPos = 0; vertPos >= -10; vertPos--) {
    drawBmp("/magic8.bmp", 0, vertPos);
  }
  Serial.println("Magic 8 Ball");
  Serial.println("Shake to Continue");
  tft.setTextSize(1);
  tft.setTextColor(TFT_WHITE);
  tft.drawCentreString("Shake to Continue", 79, 65, 2);
}

void loop() {

  while (1) {
    M5.IMU.getAccelData(&accX, &accY, &accZ);
    if (accX > 1.5 ||  accY > 1.5 ) {
      break;
    }
  }
  randomPick = random(sizeof(magicTop) / sizeof(char*));
  tft.fillScreen(0x0003);
  delay(1000);
  for (int txtColor = 1; txtColor <= 9; txtColor++) {
    switch (txtColor) {
      case 1: tft.setTextColor(0x18E3); break;
      case 2: tft.setTextColor(0x39E7); break;
      case 3: tft.setTextColor(0x5AEB); break;
      case 4: tft.setTextColor(0x7BEF); break;
      case 5: tft.setTextColor(0x9CF3); break;
      case 6: tft.setTextColor(0xBDF7); break;
      case 7: tft.setTextColor(0xDEFB); break;
      case 8: tft.setTextColor(TFT_WHITE); break;
    }
    tft.setFreeFont(FF18);
    tft.drawCentreString(magicTop[randomPick], 79, 17, GFXFF);
    tft.drawCentreString(magicBottom[randomPick], 79, 42, GFXFF);
    delay(100);
  }
}
