#define FS_NO_GLOBALS
#define IWIDTH  160
#define IHEIGHT 15
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
long lastUpdate = 0;
bool isShake = false;
bool scrollOnce = false;

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);

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
  tft.fillScreen(0x0003);

  for (int vertPos = -80; vertPos <= -5; vertPos += 3) {
    drawBmp("/magic8.bmp", 0, vertPos);
  }
}

void scroller(String msg, int xpos)
{
  M5.IMU.getAccelData(&accX, &accY, &accZ);
  if (accX > 1.5 ||  accY > 1.5 ) {
    isShake = true;
  }
  spr.fillSprite(0x0003);
  spr.setTextSize(1);           // Font size scaling is x1
  spr.setTextFont(2);           // Font 2 selected
  spr.setTextColor(TFT_WHITE, 0x0003);  // White text, no background colour
  spr.setTextWrap(false);       // Turn of wrap so we can print past end of sprite

  // Need to print twice so text appears to wrap around at left and right edges
  spr.setCursor(xpos, 2);  // Print text at xpos
  spr.print(msg);
  if (xpos == 0) {
    scrollOnce = true;
  }
  if (scrollOnce) {
    spr.setCursor(xpos - IWIDTH, 2); // Print text at xpos - sprite width
    spr.print(msg);
  }
}

void loop() {

  while (1) {
    spr.createSprite(IWIDTH, IHEIGHT);
    for (float pos = IWIDTH; pos > 0; pos -= 0.05) {
      scroller("Shake to Continue", pos);
      spr.pushSprite(0, 65);
      if (isShake) {
        break;
      }
    }
    spr.deleteSprite();
    if (isShake) {
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
    delay(90);
  }
  isShake = false;
  scrollOnce = false;
  delay(1000);
}
