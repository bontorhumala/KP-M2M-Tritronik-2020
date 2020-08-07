#include "touch_variables.h"
#include "display_variables.h"
#include "connection_variables.h"
void setup() {
  Serial.begin(115200);
  TouchSetup();
  DisplaySetup();
  ConnectionSetup();
  DisplayIntro();
}

void loop() {
  // put your main code here, to run repeatedly:
  button_t which_button_touched = WhichButtonTouched();
  client->loop();
  if (which_button_touched != SETTING){
    DisplayMainMenu(which_button_touched); 
  } else {
    DisplaySetting();
  }
}
