void textCursorStatus(String activity, int timeLeft){
  tft.setFreeFont(&OpenSans_SemiBold18pt7b);
  tft.setCursor(65, 208);
  tft.setTextColor(TFT_RED, TFT_WHITE);
  tft.println(activity + " (" + timeLeft + "m)");
}

void textCursorChoose(String activity){
  tft.setFreeFont(&OpenSans_SemiBold18pt7b);
  tft.setCursor(95, 208);
  tft.setTextColor(TFT_GREEN, TFT_WHITE);
  tft.println(activity);
}

void textNetworkStatus(){
  tft.setFreeFont(&OpenSans_SemiBold12pt7b);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(40, 80);
  tft.println("SSID   : " + ssid);
  tft.setCursor(40, 120);
  tft.println("IP        : " + ip_addr);
  tft.setCursor(40, 160);
  if (isNetworkConnected){
    tft.println("Status: Connected"); 
  } else {
    tft.println("Status: Disconnected"); 
  }
}

void about(){
  tft.setFreeFont(&OpenSans_SemiBold9pt7b);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(20, 100);
  tft.println("Application : " + application);
  tft.setCursor(20, 120);
  tft.println("Device ID    : " + deviceID);
  tft.setCursor(20, 140);
  tft.println("Firmware    : " + String(firmware));
}
