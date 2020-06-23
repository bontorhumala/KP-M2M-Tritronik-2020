bool isGotTouchLeftButton = false;
bool isGotTouchRightButton = false;
bool isGotTouchCrossButton = false;
bool isGotTouchCheckButton = false;
 
 // buat batas bawah (10) dan batas atas(kalibrasi)
 // kalibrasi batas atas dulu pas pertama kali setup
 
void setup() {
  Serial.begin(115200);
  delay(1000);
  touchAttachInterrupt(13, gotTouchLeftButton, 51);
  touchAttachInterrupt(12, gotTouchRightButton, 55);
  touchAttachInterrupt(14, gotTouchCrossButton, 56);
  touchAttachInterrupt(27, gotTouchCheckButton, 58);
}

void loop() {
  if (isGotTouchLeftButton == true){
    Serial.println("LEFT");
    isGotTouchLeftButton = false;
  }
  if (isGotTouchRightButton == true){
    Serial.println("RIGHT");
    isGotTouchRightButton = false;
  } 
  if (isGotTouchCrossButton == true){
    Serial.println("CROSS");
    isGotTouchCrossButton = false;
  } 
  if (isGotTouchCheckButton == true){
    Serial.println("CHECK");
    isGotTouchCheckButton = false;
  }
  delay(300);
}

void gotTouchLeftButton(){
  isGotTouchLeftButton == true;  
}

void gotTouchRightButton(){
  isGotTouchRightButton = true;
}

void gotTouchCrossButton(){
  isGotTouchCrossButton = true;
}

void gotTouchCheckButton(){
  isGotTouchCheckButton = true;
}
