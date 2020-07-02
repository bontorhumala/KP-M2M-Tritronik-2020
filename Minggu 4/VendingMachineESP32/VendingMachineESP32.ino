// https://onlinejpgtools.com/change-jpg-quality
// Karena belum mengetahui jalannya program, jadi untuk sementara belum dibuat modular
//https://github.com/adafruit/Adafruit-GFX-Library/blob/master/fontconvert/fontconvert_win.md
//https://learn.adafruit.com/adafruit-gfx-graphics-library/using-fonts
#include <TJpg_Decoder.h>

// Include SD
#define FS_NO_GLOBALS
#include <FS.h>
#ifdef ESP32
  #include "SPIFFS.h" // ESP32 only
#endif

#define SD_CS   15

// Include the TFT library https://github.com/Bodmer/TFT_eSPI
#include "SPI.h"
SPIClass spiSD(HSPI);
#include <TFT_eSPI.h>              // Hardware-specific library
TFT_eSPI tft = TFT_eSPI();         // Invoke custom library

// GLOBAL VARIABLES
String washer = "Washer";
String dryer = "Dryer";

String logo = "/img_jpg_rotated/logo/";
String menu = "/img_jpg_rotated/menu/";
String payment = "/img_jpg_rotated/payment/";
String popup = "/img_jpg_rotated/popup/";
String setting = "/img_jpg_rotated/setting/";
String others = "/img_jpg_rotated/";

// isi dari inisialisasi variabel hanya contoh
String ssid = "PICKLON";
String ip_addr = "192.xxx.xxx.xxx";
boolean isNetworkConnected = true;
String application = "XXXXXXXX";
String deviceID = "XXXXXXXXXXXXXXX";
float firmware = 1.0;

// This next function will be called during decoding of the jpeg file to
// render each block to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
   // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);

  // This might work instead if you adapt the sketch to use the Adafruit_GFX library
  // tft.drawRGBBitmap(x, y, bitmap, w, h);

  // Return 1 to decode next block
  return 1;
}


void setup()
{
  Serial.begin(115200);
  spiSD.begin(14,27,13,15);

  // Initialise SD before TFT
  if (!SD.begin(SD_CS, spiSD)) {
    Serial.println(F("SD.begin failed!"));
    while (1) delay(0);
  }
  
  Serial.println("\r\nInitialisation done.");

  // Initialise the TFT
  tft.begin();
  tft.setRotation(1);  //landscape
  tft.fillScreen(TFT_WHITE);
  tft.setSwapBytes(true); // We need to swap the colour bytes (endianess)
  tft.setTextWrap(true,false);
  // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);

  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);
  
//  intro();
}

void loop()
{  
  // washer
  tft.fillScreen(TFT_WHITE);
  textCursorChoose(washer);
  tft.fillRoundRect(20,30,120+12,120+10,8,TFT_BLACK);
  TJpgDec.drawSdJpg(26, 35, menu + "icon_washing_inactive.jpg");
  TJpgDec.drawSdJpg(26+26+120, 35, menu + "icon_laundry_inactive.jpg");
  TJpgDec.drawSdJpg(5, 210, menu + "wifi_connected.jpg");
  delay(5000);
  
  // dryer
  tft.fillScreen(TFT_WHITE);
  textCursorStatus(dryer, millis()/1000);
  tft.fillRoundRect(20+26+120,30,120+12,120+10,8,TFT_BLACK);
  TJpgDec.drawSdJpg(26, 35, menu + "icon_washing_inactive.jpg");
  TJpgDec.drawSdJpg(26+26+120, 35, menu + "icon_laundry_active.jpg");
  TJpgDec.drawSdJpg(5, 210, menu + "wifi_disconnected.jpg");
  delay(5000);

//  // payment qris
//  tft.fillScreen(TFT_WHITE);
//  TJpgDec.drawSdJpg(70, 100, payment + "logoqris.jpg");
//  TJpgDec.drawSdJpg(150+80, 105, logo + "checklist.jpg");
//  delay(5000);
//
//  //payment success
//  tft.fillScreen(TFT_WHITE);
//  TJpgDec.drawSdJpg(0, 0, popup + "payment_success.jpg");
//  delay(5000);
//
//  tft.fillScreen(TFT_WHITE);
//  TJpgDec.drawSdJpg(50, 50, setting + "network.jpg");
//  TJpgDec.drawSdJpg(50, 50+35, setting + "smart_config.jpg");
//  TJpgDec.drawSdJpg(50, 50+70, setting + "about.jpg");
//  TJpgDec.drawSdJpg(50, 50+105, setting + "reset.jpg");
//  for (int i = 1; i < 5; i++){
//    checkListPos(i);
//    delay(2000);
//  }

  tft.fillScreen(TFT_WHITE);
  TJpgDec.drawSdJpg(0, 0, logo + "mainlogo-blur.jpg");
  textNetworkStatus();
  delay(5000);

  tft.fillScreen(TFT_WHITE);
  TJpgDec.drawSdJpg(0, 0, logo + "mainlogo-blur.jpg");
  about();
  delay(5000);
}
