int maks[4] = {0, 0, 0, 0}; // max value from setup function
int mins[4] = {100, 100, 100, 100}; // min value from loop function

int tes[4] = {0, 0, 0, 0}; // max value from loop function
int tes1[4] = {60, 60, 60, 60}; // min value from loop function

void setup() {
  Serial.begin(9600);
  delay(1000); // give me time to bring up serial monitor
  Serial.println("ESP32 Touch Test");
  Serial.println("Calibrating... Do Not Touch The Plate!");
  Serial.println("Searching for max and min value of touch sensor");
  long w = millis();
  while (millis() - w < 5000) {
    int a = touchRead(13);
    int b = touchRead(12);
    int c = touchRead(14);
    int d = touchRead(27);
    maks[0] = (maks[0] < a) ? a : maks[0];
    maks[1] = (maks[1] < b) ? b : maks[1];
    maks[2] = (maks[2] < c) ? c : maks[2];
    maks[3] = (maks[3] < d) ? d : maks[3];
    mins[0] = (mins[0] > a) ? a : mins[0];
    mins[1] = (mins[1] > b) ? b : mins[1];  
    mins[2] = (mins[2] > c) ? c : mins[2];
    mins[3] = (mins[3] > d) ? d : mins[3];
  }
  Serial.println("Touch the plate!");
  Serial.println("Begin in... 3");
  delay(1000);
  Serial.println("2");
  delay(1000);
  Serial.println("1");
  delay(1000);
  Serial.println("NOW");
}

void loop() {
  // read touch pin
  int a = touchRead(13);
  int b = touchRead(12);
  int c = touchRead(14);
  int d = touchRead(27);
  
  // max and min value check
  tes[0] = (tes[0] < a) ? a : tes[0];
  tes[1] = (tes[1] < b) ? b : tes[1];  
  tes[2] = (tes[2] < c) ? c : tes[2];
  tes[3] = (tes[3] < d) ? d : tes[3];
  tes1[0] = (tes1[0] > a) ? a : tes1[0];
  tes1[1] = (tes1[1] > b) ? b : tes1[1];  
  tes1[2] = (tes1[2] > c) ? c : tes1[2];
  tes1[3] = (tes1[3] > d) ? d : tes1[3];
  
  // Serial print
  for (int i = 0; i < 4; i++){
    Serial.print(" ");
    Serial.print(maks[i]);
  }
  Serial.print(" - ");
  for (int i = 0; i < 4; i++){
    Serial.print(" ");
    Serial.print(mins[i]);
  }
  Serial.print(" - ");
  for (int i = 0; i < 4; i++){
    Serial.print(" ");
    Serial.print(tes[i]);
  }
  Serial.print(" - ");
  for (int i = 0; i < 4; i++){
    Serial.print(" ");
    Serial.print(tes1[i]);
  }
  Serial.print(" - ");
  Serial.print(a); Serial.print("\t"); 
  Serial.print(b); Serial.print("\t"); 
  Serial.print(c); Serial.print("\t"); 
  Serial.print(d); Serial.print("\t"); 
  Serial.println();
//  delay(100);
}
