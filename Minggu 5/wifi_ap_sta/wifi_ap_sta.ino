#include <WiFi.h>
#include <WiFiAP.h>
#include <WebSocketServer.h>
#include <ArduinoJson.h>
 
WiFiServer server(80);
WebSocketServer webSocketServer;

const char* assid = "ESP32";
const char* apassword = "AAAAABBBBB";

// Connect to WiFi Access Point
void handleReceivedMessage(String message){
 
  StaticJsonDocument<150> doc;   //Memory pool
  deserializeJson(doc, message); // parse message

  auto error = deserializeJson(doc, message); // check for deserialization error
  if (error) {
      Serial.print(F("deserializeJson() failed with code "));
      Serial.println(error.c_str());
      return;
  }
 
  const char * ssid = doc["ssid"];         //Get sensor type value
  const char * password = doc["password"]; //Get sensor type value

  WiFi.begin(ssid,password);
  Serial.println("Connecting to WiFi___");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.print("Connected to the WiFi network");
  Serial.println(" SSID: DaDeDa");
  Serial.println(WiFi.localIP());
  
  webSocketServer.sendData("Device Connected to" + ssid);
  Serial.println();
  Serial.println("----- NEW DATA FROM CLIENT ----");
 
  Serial.print("SSID: ");
  Serial.println(ssid);
 
  Serial.print("Password: ");
  Serial.println(password);
 
  Serial.println("------------------------------");
}
 
void setup() {
  Serial.begin(115200);
 
  delay(2000);
  WiFi.mode(WIFI_AP_STA);
//  WiFi.begin(ssid, password); 
  WiFi.softAP(assid, apassword);
 
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(1000);
//    Serial.println("Connecting to WiFi..");
//  }
 
  Serial.println("Connected to the WiFi network");
//  Serial.println(WiFi.localIP());
  Serial.println(WiFi.softAPIP());
 
  server.begin();
  delay(100);
}
 
void loop() {
 
  WiFiClient client = server.available();
 
  if (client.connected() && webSocketServer.handshake(client)) {
 
    String data;      
 
    while (client.connected()) {
 
      data = webSocketServer.getData();
 
      if (data.length() > 0) {
         const char * ssid = handleReceivedMessage(data);
         webSocketServer.sendData(data);
         
      }
 
      delay(10); // Delay needed for receiving the data correctly
   }
 
   Serial.println("The client disconnected");
   delay(100);
  }
 
  delay(100);
}
