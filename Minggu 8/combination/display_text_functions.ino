// TFT Display Activity
// dipanggil di void DisplayMainMenu(button_t button);
void textCursorChoose(String activity){
  tft.setFreeFont(&OpenSans_SemiBold18pt7b);
  if (activity == WASHER) {
    tft.setCursor(95, 208); 
  } else {
    tft.setCursor(105, 208); 
  }
  tft.setTextColor(TFT_GREEN, TFT_WHITE);
  tft.println(activity);
}

// TFT Display Activity, Time Left
// dipanggil di void DisplayMainMenu(button_t button);
void textCursorStatus(String activity, int timeLeft){
  tft.setFreeFont(&OpenSans_SemiBold18pt7b);
  tft.setCursor(65, 208);
  tft.setTextColor(TFT_RED, TFT_WHITE);
  tft.println(activity + " (" + timeLeft + "m)");
}

// Menuliskan jenis landry (washer/dryer) yang sudah selesai di screen
// dipanggil di void DisplayMainMenu(button_t button);
void textLaundryDone(String activity){
  tft.setFreeFont(&OpenSans_SemiBold18pt7b);
  tft.setCursor(50,120);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  tft.println(activity + " Selesai");
}

// Display text input Pin oleh user
// dipanggil di void DisplayManual(String activity);
void textNumberPin(int digitPlace, int number){
  tft.setFreeFont(&OpenSans_SemiBold18pt7b);
  tft.setCursor(70+50*(digitPlace),120);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  if (number == -2){
    tft.println("*");
  } else if (number == -1){
    tft.println("_");
  } else {
    tft.println(number); 
  }
}

void textNetworkStatus(){
  String ssid = "ssid";
  tft.setFreeFont(&OpenSans_SemiBold12pt7b);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(40, 80);
  tft.println("SSID   : " + WiFi.SSID());
  tft.setCursor(40, 120);
  char temp[15];
  sprintf(temp, "%d.%d.%d.%d", bytes[0],bytes[1],bytes[2],bytes[3]);
  tft.println("IP        : " + String(temp));
  tft.setCursor(40, 160);
  if (client->isConnected()){
    tft.println("Status: Connected"); 
  } else {
    tft.println("Status: Disconnected"); 
  }
}

void textAbout(){
  String application = "Laundry Controller";
  float firmware = 1.0;
  
  tft.setFreeFont(&OpenSans_SemiBold9pt7b);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(20, 100);
  tft.println("Application : " + application);
  tft.setCursor(20, 120);
  tft.println("Device ID    : " + String(deviceID));
  tft.setCursor(20, 140);
  tft.println("Firmware    : " + String(firmware));
}

void textWifiSetup(IPAddress IP){
  bytes[0] = IP & 0xFF;
  bytes[1] = (IP >> 8) & 0xFF;
  bytes[2] = (IP >> 16) & 0xFF;
  bytes[3] = (IP >> 24) & 0xFF;
  char temp[15];
  sprintf(temp, "%d.%d.%d.%d", bytes[0],bytes[1],bytes[2],bytes[3]);
  tft.setFreeFont(&OpenSans_SemiBold12pt7b);
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(45, 160);
  tft.println("SSID   : " + String(assid));
  tft.setCursor(45, 190);
  tft.println("IP        : " + String(temp));
}
