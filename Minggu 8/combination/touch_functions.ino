/*
  Functions List:
    1. void touch_setup();
    2. void IRAM_ATTR InterruptService();
    3. int WhichButtonTouched();
*/

/////////////////////////////////////////////////////////////////////////
// Function to be called in void setup()
/////////////////////////////////////////////////////////////////////////
void TouchSetup(){
  // Start interrupt service.
  // Create timerInterruptService to main loop semaphore.
  timerLoopSemaphore = xSemaphoreCreateBinary();
  EnableTimer();
//
//  // Use timer 0 (4 available from 0 through 3), with a prescale of 80.
//  timerInterruptService = timerBegin(0, 80, true);
//
//  // Attach timerInterruptService() to timer.
//  timerAttachInterrupt(timerInterruptService, & InterruptService, true);
//
//  // Set alarm to call onTimer function every millisecond (value in microseconds)
//  // with repeat (third parameter).
//  timerAlarmWrite(timerInterruptService, 1000, true);
//
//  // Start the alarm.
//  timerAlarmEnable(timerInterruptService);
}


/////////////////////////////////////////////////////////////////////////
// INTERRUPT SERVICE FOR FILTERING TOUCH, 1000Hz Timer
/////////////////////////////////////////////////////////////////////////
void IRAM_ATTR InterruptService() {
  static int nButton1Count = 0;              // button 1 count
  static int nButton2Count = 0;              // button 2 count
  static int nButton3Count = 0;              // button 3 count
  static int nButton4Count = 0;              // button 4 count
  static int nButtonDelay = BUTTON_DELAY - 1;// button update delay counter
  static int nLoopDelay  = LOOP_DELAY - 1;   // loop update delay counter
  
  nMilliseconds += 1; // Update milliseconds counter.
  
  // Update loop().
  if (nLoopDelay) {
    // Not yet time to update.
    nLoopDelay --;
  } else {
    // Time to update, reset delay.
    nLoopDelay = LOOP_DELAY - 1;
    // Execute a main loop pass.
    xSemaphoreGiveFromISR(timerLoopSemaphore, NULL);
  }

  // Update buttons.
  if (nButtonDelay){
    // Not yet time to update.
    nButtonDelay --;
  } else{
    // Time to update, reset delay.
    nButtonDelay = BUTTON_DELAY - 1;
    
    // Update button 1.
    if (touchRead(4) < 51) { //threshold
      // Below threshold, filter.
      if (nButton1Count < BUTTON_FILTER) {
        // Keep filtering.
        nButton1Count += 1;
        nButton1 = 0;
      } else {
        // Filtered enough, button 1 is pressed.
        nButton1 = 1;
      }
    } else {
      // Above threshold, reset button and filter.
      nButton1 = nButton1Count = 0;
    }

    // Update button 2.
    if (touchRead(32) < 49) { //threshold
      // Below threshold, filter.
      if (nButton2Count < BUTTON_FILTER) {
        // Keep filtering.
        nButton2Count += 1;
        nButton2 = 0;
      } else {
        // Filtered enough, button 2 is pressed.
        nButton2 = 1;
      }
    } else {
      // Above threshold, reset button and filter.
      nButton2 = nButton2Count = 0;
    }

    // Update button 3.
    if (touchRead(33) < 48) { //threshold
      // Below threshold, filter.
      if (nButton3Count < BUTTON_FILTER){
        // Keep filtering.
        nButton3Count += 1;
        nButton3 = 0;
      } else {
        // Filtered enough, button 3 is pressed.
        nButton3 = 1;
      }
    } else {
      // Above threshold, reset button and filter.
      nButton3 = nButton3Count = 0;
    }
    
    // Update button 4.
    if (touchRead(12) < 65) { // thresholdnya suka berubah"
      // Below threshold, filter.
      if (nButton4Count < BUTTON_FILTER){
        // Keep filtering.
        nButton4Count += 1;
        nButton4 = 0;
      } else {
        // Filtered enough, button 4 is pressed.
        nButton4 = 1;
      }
    } else {
      // Above threshold, reset button and filter.
      nButton4 = nButton4Count = 0;
    }
  }
}

/////////////////////////////////////////////////////////////////////////
// Which button is touched right now
/////////////////////////////////////////////////////////////////////////
button_t WhichButtonTouched(){
  static bool isButton1 = false;
  static bool isButton2 = false;
  static bool isButton3 = false;
  static bool isButton4 = false;
  static bool isButtonSetting = false;
  static bool isButtonManual = false;
  if (xSemaphoreTake(timerLoopSemaphore, portMAX_DELAY) == pdTRUE) {
    if (nButton1 == 1 && nButton3 == 1){
      if (isButtonManual == false){
        Serial.println("LEFT+CROSS");
        isButtonManual = true; 
        isButton1 = true;
        isButton3 = true;
        return MANUAL;
      }
    } else {
      isButtonManual = false; 
    }
    
    if (nButton1 == 1 && nButton2 == 1){
      if (isButtonSetting == false){
        Serial.println("LEFT+RIGHT");
        isButtonSetting = true; 
        isButton1 = true;
        isButton2 = true;
        return SETTING;
      }
    } else {
      isButtonSetting = false; 
    }
    
    if (nButton1 == 1){
      if (isButton1 == false){
        Serial.println("LEFT");
        isButton1 = true;  
        return LEFT;
      }
    } else {
      isButton1 = false;
    }

    if (nButton2 == 1){
      if (isButton2 == false){
        Serial.println("RIGHT");
        isButton2 = true;  
        return RIGHT;
      }
    } else {
      isButton2 = false;
    }

    if (nButton3 == 1){
      if (isButton3 == false){
        Serial.println("CROSS");
        isButton3 = true;  
        return CROSS;
      }
    } else {
      isButton3 = false;
    }
    
    if (nButton4 == 1){
      if (isButton4 == false){
        Serial.println("CHECK");
        isButton4 = true;  
        return CHECK;
      }
    } else {
      isButton4 = false;
    }
  }
  else {
    Serial.println("loop(): using portMAX_DELAY as the xSemaphoreTake timout value, this should never print.\n");
  }
  return NOTHING;
}

void EnableTimer(){
  //re-enable timer
  timerInterruptService = timerBegin(0, 80, true);
  timerAttachInterrupt(timerInterruptService, & InterruptService, true);
  timerAlarmWrite(timerInterruptService, 1000, true);
  timerAlarmEnable(timerInterruptService);
}

void DisableTimer(){
  //disable timer
  timerAlarmDisable(timerInterruptService);    // stop alarm
  timerDetachInterrupt(timerInterruptService);  // detach interrupt
  timerEnd(timerInterruptService);      // end timer
}
