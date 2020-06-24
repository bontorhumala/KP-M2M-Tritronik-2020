// Example for library:
// https://github.com/Bodmer/TJpg_Decoder

// This example if for an ESP8266 or ESP32, it renders a Jpeg file
// that is stored in a SD card file. The test image is in the sketch
// "data" folder (press Ctrl+K to see it). You must save the image
// to the SD card using you PC.

// Include the jpeg decoder library
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
  Serial.println("\n\n Testing TJpg_Decoder library");

  // Initialise SD before TFT
  if (!SD.begin(SD_CS, spiSD)) {
    Serial.println(F("SD.begin failed!"));
    while (1) delay(0);
  }
  Serial.println("\r\nInitialisation done.");

  // Initialise the TFT
  tft.begin();
  tft.setTextColor(0xFFFF, 0x0000);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true); // We need to swap the colour bytes (endianess)

  // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);

  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);
}

void loop()
{
  tft.setRotation(2);  // portrait
  tft.fillScreen(random(0xFFFF));
  // The image is 300 x 300 pixels so we do some sums to position image in the middle of the screen!
  // Doing this by reading the image width and height from the jpeg info is left as an exercise!
  int x = (tft.width()  - 300) / 2 - 1;
  int y = (tft.height() - 300) / 2 - 1;

  // Time recorded for test purposes
  uint32_t t = millis();

  // Get the width and height in pixels of the jpeg if you wish
  // TIDAK BEKERJA
//  uint16_t w = 0, h = 0;
//  TJpgDec.getSdJpgSize(&w, &h, "/EagleEye.jpg");
//  Serial.print("Width = "); Serial.print(w); Serial.print(", height = "); Serial.println(h);

  // Draw the image, top left at 0,0
  TJpgDec.drawSdJpg(x, y, "/EagleEye.jpg");
  // How much time did rendering take
  t = millis() - t;
  Serial.print("EagleEye.jpg => ");Serial.print(t); Serial.println(" ms");

  // Wait before drawing again
  delay(2000);
  
  tft.setRotation(2);  // portrait
  tft.fillScreen(random(0xFFFF));
  t = millis();
  // Draw the image, top left at 0,0
  TJpgDec.drawSdJpg(0, 0, "/Baboon40.jpg");
  // How much time did rendering take
  t = millis() - t;
  Serial.print("Baboon40.jpg => ");Serial.print(t); Serial.println(" ms");
  
  // Wait before drawing again
  delay(2000);

  tft.setRotation(2);  // portrait
  tft.fillScreen(random(0xFFFF));
  t = millis();
  // Draw the image, top left at 0,0
  TJpgDec.drawSdJpg(0, 0, "/lena20k.jpg");
  // How much time did rendering take
  t = millis() - t;
  Serial.print("lena20k.jpg => ");Serial.print(t); Serial.println(" ms");
  
  // Wait before drawing again
  delay(2000);
  
  tft.setRotation(1);  // landscape
  tft.fillScreen(random(0xFFFF));  
  t = millis();
  // Draw the image, top left at 0,0
  TJpgDec.drawSdJpg(0, 0, "/Mouse480.jpg");
  // How much time did rendering take
  t = millis() - t;
  Serial.print("Mouse480.jpg => ");Serial.print(t); Serial.println(" ms");
  
  // Wait before drawing again
  delay(2000);

  tft.setRotation(1);  // landscape
  tft.fillScreen(random(0xFFFF));
  t = millis();
  // Draw the image, top left at 0,0
  TJpgDec.drawSdJpg(0, 0, "/evive_in_hand.jpg");
  // How much time did rendering take
  t = millis() - t;
  Serial.print("evive_in_hand.jpg => ");Serial.print(t); Serial.println(" ms");
  
  // Wait before drawing again
  delay(2000);

  tft.setRotation(1);  // landscape
  tft.fillScreen(random(0xFFFF));
  t = millis();
  // Draw the image, top left at 0,0
  TJpgDec.drawSdJpg(0, 0, "/wifi.bmp");
  // How much time did rendering take
  t = millis() - t;
  Serial.print("wifi.bmp => ");Serial.print(t); Serial.println(" ms");
  
  // Wait before drawing again
  delay(2000);
}
