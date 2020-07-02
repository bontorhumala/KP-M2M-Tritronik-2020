void intro(){
  TJpgDec.drawSdJpg(0, 0, logo + "iotera.jpg");
  delay(2000);
  
  TJpgDec.drawSdJpg(0, 0, logo + "mainlogo.jpg");
  delay(2000);
  tft.fillScreen(TFT_WHITE);
}
