// https://github.com/STEMpedia/eviveProjects/tree/master/imageToFlashMemoryIconsForTFT
// https://www.instructables.com/id/Converting-Images-to-Flash-Memory-Iconsimages-for-/
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ILI9341.h> // Hardware-specific library

#include "bitmaps.h"
#include "bitmapsLarge.h"

#define TFT_DC 4
#define TFT_CS 15
#define TFT_RST 2
#define TFT_MISO 19         
#define TFT_MOSI 23           
#define TFT_CLK 18 

// If using the breakout, change pins as desired
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void setup() {
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);

//Case 1: Monochrome Bitmaps 
  tft.drawBitmap(0+130,0,bell,16,16,ILI9341_WHITE);
  delay(2000);
  tft.drawBitmap(20+130,0,semiCircle, 50, 25, ILI9341_WHITE);
  delay(2000);
  tft.drawBitmap(80+130,0,wifi_icon, 20, 15, ILI9341_WHITE);
  delay(2000);
  tft.drawBitmap(110+130,0,battery_icon, 20, 15, ILI9341_WHITE);
  delay(2000);
  tft.drawBitmap(140+130,0,user_icon, 15, 15, ILI9341_WHITE);
  delay(2000);
  tft.drawBitmap(0+130,30,evive_logo, 71, 71, ILI9341_CYAN);
  delay(2000);

//Case 2: Multi Colored Images/Icons
  int h = 128,w = 97, row, col, buffidx=0;
  for (row=0; row<h; row++) { // For each scanline...
    for (col=0; col<w; col++) { // For each pixel...
      //To read from Flash Memory, pgm_read_XXX is required.
      //Since image is stored as uint16_t, pgm_read_word is used as it uses 16bit address
      tft.drawPixel(col, row, pgm_read_word(evive_in_hand + buffidx));
      buffidx++;
    } // end pixel
  }
  delay(2000);
  
  h = 128+112;w = 97+223;buffidx=0;
  for (row=112; row<h; row++) { // For each scanline...
    for (col=223; col<w; col++) { // For each pixel...
      //To read from Flash Memory, pgm_read_XXX is required.
      //Since image is stored as uint16_t, pgm_read_word is used as it uses 16bit address
      tft.drawPixel(col, row, pgm_read_word(evive_in_hand + buffidx));
      buffidx++;
    } // end pixel
  }
}

void loop() {
}
