/*
  Functions List:
    1. bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap);
    2. void DisplaySetup();
    3. void DisplayMainMenu();
*/

/////////////////////////////////////////////////////////////////////////
// This next function will be called during decoding of the jpeg file to
// render each block to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
/////////////////////////////////////////////////////////////////////////
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


/////////////////////////////////////////////////////////////////////////
// Function to be called in void setup()
/////////////////////////////////////////////////////////////////////////
void DisplaySetup() {
  // Initialise the TFT
  tft.begin();
  tft.setRotation(1);  //landscape
  tft.fillScreen(TFT_WHITE);
  tft.setSwapBytes(true); // We need to swap the colour bytes (endianess)
  tft.setTextWrap(true, false);
  
  spiSD.begin(14, 27, 13, 15);
  if (!SD.begin(SD_CS, spiSD)) {
    Serial.println("Card Mount failed! Insert MicroSD Card!\nRestarting in 5 Seconds");
    tft.setFreeFont(&OpenSans_SemiBold9pt7b);
    tft.setTextColor(TFT_BLACK);
    tft.setCursor(20, 100);
    tft.println("Insert MicroSD Card");
    tft.setCursor(20, 120);
    tft.println("Restarting in 5 seconds");
    for (int i = 5; i > 0; i--){
      Serial.print(i);
      Serial.print("..");
      tft.setCursor(120-i*20, 140);
      tft.println(i);
      delay(1000);
    }
    tft.setCursor(20, 160);
    tft.println("Restarting Now");
    Serial.println("Restarting Now");
    ESP.restart();
  }
  
  // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);

  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);

  Serial.println("\r\nInitialisation done.");
}


/////////////////////////////////////////////////////////////////////////
// Startup Intro, LOGO
// dipanggil di void setup();
/////////////////////////////////////////////////////////////////////////
void DisplayIntro() {
  TJpgDec.drawSdJpg(0, 0, LOGO_IOTERA);
  delay(2000);

  TJpgDec.drawSdJpg(0, 0, LOGO_MAIN);
  delay(2000);
  tft.fillScreen(TFT_WHITE);
}


/////////////////////////////////////////////////////////////////////////
// Main Menu
// dipanggil di void loop();
/////////////////////////////////////////////////////////////////////////
void DisplayMainMenu(button_t button) {
  if (isMainMenuActive) { // laundry is active
    const size_t capacity = JSON_OBJECT_SIZE(4);
    tft.fillScreen(TFT_WHITE);
    if(client->isConnected()){
      DynamicJsonDocument doc(capacity);
      doc["sensor"] = (cursor_main_menu == WASHER) ? "washer" : "dryer" ;
      doc["param"] = "value";
      doc["value"] = true;
      docString.remove(0);
      serializeJson(doc, docString);
      client->publish(TOPIC, docString);
      TJpgDec.drawSdJpg(5, 210, MENU_WIFI_CONNECTED);
    } else {
      TJpgDec.drawSdJpg(5, 210, MENU_WIFI_DISCONNECTED);
    }
    unsigned long time_left = nMilliseconds - time_begin;
    unsigned long time_total;
    if (cursor_main_menu == WASHER) {
      time_total = washing_time * 1000;
      tft.fillRoundRect(20, 30, 120 + 12, 120 + 10, 8, TFT_BLACK);
      TJpgDec.drawSdJpg(26, 35, MENU_WASHER_ACTIVE);
      TJpgDec.drawSdJpg(26 + 26 + 120, 35, MENU_DRYER_INACTIVE);
    } else {
      time_total = drying_time * 1000;
      tft.fillRoundRect(20 + 26 + 120, 30, 120 + 12, 120 + 10, 8, TFT_BLACK);
      TJpgDec.drawSdJpg(26, 35, MENU_WASHER_INACTIVE);
      TJpgDec.drawSdJpg(26 + 26 + 120, 35, MENU_DRYER_ACTIVE);
    }
    while (time_left < time_total) {
      tft.fillRect(60, 170, 250, 65, TFT_WHITE);
      time_left = nMilliseconds - time_begin;
      textCursorStatus(cursor_main_menu, time_left / 60000);
      delay(1000);
    }
    isMainMenuActive = false;
    tft.fillScreen(TFT_WHITE);
    textLaundryDone(cursor_main_menu);
//    DisableTimer();
//    client->loop();
//    EnableTimer();
    if(client->isConnected()){
      DynamicJsonDocument doc(capacity);
      doc["sensor"] = (cursor_main_menu == WASHER) ? "washer" : "dryer" ;
      doc["param"] = "value";
      doc["value"] = false;
      docString.remove(0);
      serializeJson(doc, docString);
      client->publish(TOPIC, docString);
    }
    delay(3000);
  } else { // laundry is inactive
    tft.fillScreen(TFT_WHITE);
    if(client->isConnected()){
      TJpgDec.drawSdJpg(5, 210, MENU_WIFI_CONNECTED);
    } else {
      TJpgDec.drawSdJpg(5, 210, MENU_WIFI_DISCONNECTED);
    }
    if (cursor_main_menu == WASHER) {
      tft.fillRoundRect(20, 30, 120 + 12, 120 + 10, 8, TFT_BLACK);
    } else {
      tft.fillRoundRect(20 + 26 + 120, 30, 120 + 12, 120 + 10, 8, TFT_BLACK);
    }
    TJpgDec.drawSdJpg(26, 35, MENU_WASHER_INACTIVE);
    TJpgDec.drawSdJpg(26 + 26 + 120, 35, MENU_DRYER_INACTIVE);
    textCursorChoose(cursor_main_menu);
    // button touched = check
    while (button == NOTHING) {
      button = WhichButtonTouched();
    }
    /* aluminium CHECK BERMASALAH*/
    if (button == CHECK) {
      isMainMenuActive = DisplayPayment();
    } else if (button == MANUAL) {
      isMainMenuActive = DisplayManual();
    } else {
      // button touched = left or right
      if (button == LEFT || button == RIGHT) {
        if (cursor_main_menu == WASHER) {
          cursor_main_menu = DRYER;
        } else {
          cursor_main_menu = WASHER;
        }
      }
    }
  }
}


/////////////////////////////////////////////////////////////////////////
// Display Payment Page
// dipanggil di void DisplayMainMenu(button_t button);
/////////////////////////////////////////////////////////////////////////
// belum termasuk bayar beneran ke ovo/qris
bool DisplayPayment() {
  button_t button = NOTHING;
  bool hasPaid = false;
  bool hasCancelled = false;
  bool wantToPay = false;
  TJpgDec.drawSdJpg(0, 0, POPUP_PAYMENT_OPTION);
  delay(2000);
  while (!wantToPay && !hasCancelled) {
    button = NOTHING;
    tft.fillScreen(TFT_WHITE);
    TJpgDec.drawSdJpg(55, 85, PAYMENT_LOGO_OVO);
    TJpgDec.drawSdJpg(50, 85 + 50, PAYMENT_LOGO_QRIS);
    tft.fillRect(250, 85, 25, 70, TFT_WHITE); // bersihkan checklist
    if (cursor_payment == 'O') {
      TJpgDec.drawSdJpg(250, 85, MENU_CHECKLIST);
    } else {
      TJpgDec.drawSdJpg(250, 85 + 50, MENU_CHECKLIST);
    }
    while (button == NOTHING) {
      button = WhichButtonTouched();
    }
    if (button == CROSS) {
      hasCancelled = true;
    } else if (button == LEFT || button == RIGHT) {
      if (cursor_payment == 'O') {
        cursor_payment = 'Q';
      } else {
        cursor_payment = 'O';
      }
    } else if (button == CHECK) {
      wantToPay = true;
    }
  }
  if (wantToPay) {
    if (cursor_payment == 'O') {
      TJpgDec.drawSdJpg(0, 0, OTHERS_OVO_QR); // size ovo qr harus dikecilin
    } else {
      TJpgDec.drawSdJpg(0, 0, OTHERS_INVOICE);
    }
    delay(3000); // nanti diganti dengan indikator sudah bayar dari ovo/qris
    hasPaid = true;
  }
  if (hasPaid) { // nanti diganti dengan indikator sudah bayar dari ovo/qris
    TJpgDec.drawSdJpg(0, 0, POPUP_PAYMENT_SUCCESS);
    time_begin = nMilliseconds; // begin time downcounter
    delay(3000);
    return true;
  }
  if (hasCancelled) {
    TJpgDec.drawSdJpg(0, 0, POPUP_PAYMENT_CANCEL);
    delay(3000);
    return false;
  }
}

bool DisplayInputPin(char whichMode){
  button_t button = NOTHING;
  int count_digit_pin = 0;
  int number[4] = {0, -1, -1, -1};
  bool isPinCorrect = true;
  tft.fillScreen(TFT_WHITE);
  for (int i = 0; i < 4; i++) {
    textNumberPin(i, number[i]);
  }
  while (count_digit_pin < 4) {
    while (button == NOTHING) {
      button = WhichButtonTouched();
    }
    tft.fillScreen(TFT_WHITE);
    if (button == CHECK) {
      count_digit_pin++;
      number[count_digit_pin] = (number[count_digit_pin] == -1) ? 0 : number[count_digit_pin];
    } else if (button == CROSS) {
      count_digit_pin--;
      count_digit_pin = (count_digit_pin < 0) ? 1 : count_digit_pin;
    } else if (button == RIGHT) {
      number[count_digit_pin]++;
      if (number[count_digit_pin] > 9) {
        number[count_digit_pin] = 0;
      }
    } else if (button == LEFT) {
      number[count_digit_pin]--;
      if (number[count_digit_pin] < 0) {
        number[count_digit_pin] = 9;
      }
    }
    for (int i = 0; i < 4; i++) {
      if (i == count_digit_pin) {
        textNumberPin(i, number[i]);
      } else {
        if (number[i] == -1) {
          textNumberPin(i, -1);
        } else {
          textNumberPin(i, -2);
        }
      }
    }
    button = NOTHING;
  }
  tft.fillScreen(TFT_WHITE);
  for (int i = 0; i < 4; i++) {
    if (number[i] != PIN[i]) {
      isPinCorrect = false;
    }
  }
  if (isPinCorrect) {
    TJpgDec.drawSdJpg(0, 0, POPUP_CORRECT_PIN);
    if (whichMode == 'm'){
      time_begin = nMilliseconds; // begin time downcounter 
    }
    delay(3000);
    return true;
  } else {
    TJpgDec.drawSdJpg(0, 0, POPUP_WRONG_PIN);
    delay(3000);
    return false;
  }
}

/////////////////////////////////////////////////////////////////////////
// Display Manual Mode Page
// dipanggil di void DisplayMainMenu(button_t button);
/////////////////////////////////////////////////////////////////////////
// parameter activity untuk menentukan antara washer atau dryer
bool DisplayManual() {
  tft.fillScreen(TFT_WHITE);
  TJpgDec.drawSdJpg(0, 0, POPUP_ENTER_MANUAL_MODE);
  delay(3000);
  return DisplayInputPin('m');
}

setting_t SettingOption(char opr, setting_t cursor_setting){
  if (cursor_setting == NETWORK){
    if (opr == '+'){
      return SETUP_WIFI;  
    } else if (opr == '-') {
      return REBOOT;
    }
  } else if (cursor_setting == SETUP_WIFI){
    if (opr == '+'){
      return ABOUT;  
    } else if (opr == '-') {
      return NETWORK;
    }
  } else if (cursor_setting == ABOUT){
    if (opr == '+'){
      return REBOOT;  
    } else if (opr == '-') {
      return SETUP_WIFI;
    }
  } else if (cursor_setting == REBOOT){
    if (opr == '+'){
      return NETWORK;  
    } else if (opr == '-') {
      return ABOUT;
    }
  } else {
    return cursor_setting;
  }
}
/////////////////////////////////////////////////////////////////////////
// Display Setting Menu Page
// dipanggil di void loop();
/////////////////////////////////////////////////////////////////////////
void DisplaySetting() {
  button_t button = NOTHING;
  tft.fillScreen(TFT_WHITE);
  TJpgDec.drawSdJpg(50, 50, SETTING_NETWORK);
  TJpgDec.drawSdJpg(50, 50 + 35, SETTING_SETUP);
  TJpgDec.drawSdJpg(50, 50 + 70, SETTING_ABOUT);
  TJpgDec.drawSdJpg(50, 50 + 105, SETTING_REBOOT);
  while (button != CHECK) {
    button = NOTHING;
    tft.fillRect(250, 55, 30, 135, TFT_WHITE); // bersihkan checklist
    TJpgDec.drawSdJpg(250, 55 + (35 * cursor_setting), MENU_CHECKLIST); // buat checklist
    while (button == NOTHING) {
      button = WhichButtonTouched();
    }
    if (button == LEFT) {
      cursor_setting = SettingOption('-', cursor_setting);
    } else if (button == RIGHT) {
      cursor_setting = SettingOption('+', cursor_setting);
    } else if (button == CROSS) {
      TJpgDec.drawSdJpg(0, 0, POPUP_FINISH_PROMPT);
      button = NOTHING;
      while (button != CROSS && button != CHECK) { // cek lagi
        button = WhichButtonTouched();
      }
      if (button == CROSS){
        tft.fillScreen(TFT_WHITE);
        TJpgDec.drawSdJpg(50, 50, SETTING_NETWORK);
        TJpgDec.drawSdJpg(50, 50 + 35, SETTING_SETUP);
        TJpgDec.drawSdJpg(50, 50 + 70, SETTING_ABOUT);
        TJpgDec.drawSdJpg(50, 50 + 105, SETTING_REBOOT);
      }
    }
  }
  if (cursor_setting == NETWORK){
    TJpgDec.drawSdJpg(0, 0, LOGO_MAINBLUR);
    textNetworkStatus();
  } else if (cursor_setting == ABOUT){
    TJpgDec.drawSdJpg(0, 0, LOGO_MAINBLUR);
    textAbout();
  } else if (cursor_setting == REBOOT){
    TJpgDec.drawSdJpg(0, 0, POPUP_REBOOTING);
    delay(2000);
    ESP.restart();
  } else if (cursor_setting == SETUP_WIFI){
    if(DisplayInputPin('s')){
      DisableTimer();
      
      // begin smart config
      TJpgDec.drawSdJpg(0, 0, POPUP_SMART_CONFIG_MODE);
      tft.setFreeFont(&OpenSans_SemiBold12pt7b);
      tft.setTextColor(TFT_BLACK);
      if (ConnectionWifiSetup()){
        tft.setCursor(20, 220);
        char sendtext[50]; // text dsiplay berhasil connect dan local ip
        sprintf(sendtext, "Connected to %s\n", WiFi.SSID());
        tft.println(sendtext);
      } else {
        tft.println("Failed to Connect. Please Retry");
      }

      EnableTimer();
    }
  }
//  TJpgDec.drawSdJpg(0, 0, POPUP_CONFIG_RECEIVED);
  delay(5000);
}
