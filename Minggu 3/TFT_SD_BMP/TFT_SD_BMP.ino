
//====================================================================================
//                                  Libraries
//====================================================================================

#include "FS.h"
#include "SD.h"
#include "SPI.h"

// Call up the TFT library
#include <TFT_eSPI.h> // Hardware-specific library for ESP8266

// Invoke TFT library this will set the TFT chip select high
TFT_eSPI tft = TFT_eSPI();

#define SD_CS 22 // SD chip select

//====================================================================================
//                                    Setup
//====================================================================================
void setup()
{
  Serial.begin(115200); // Used for messages and the C array generator

  // Initialise the SD library before the TFT so the chip select gets set
  if (!SD.begin(SD_CS)) {
    Serial.println("Initialisation failed!");
    while (1) yield(); // Stay here twiddling thumbs waiting
  }
  Serial.println("\r\nInitialisation done.");

  // Now initialise the TFT
  tft.begin();
  tft.setRotation(0);  // 0 & 2 Portrait. 1 & 3 landscape
  tft.fillScreen(TFT_BLACK);
}

//====================================================================================
//                                    Loop
//====================================================================================
void loop()
{
  tft.fillScreen(random(0xFFFF));
  drawBmp("/parrot.bmp", 0, 0);//, 16);
  delay(4000);
}
//====================================================================================

