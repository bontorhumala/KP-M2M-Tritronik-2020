/*
  SimpleMQTTClient.ino
  The purpose of this exemple is to illustrate a simple handling of MQTT and Wifi connection.
  Once it connects successfully to a Wifi network and a MQTT broker, it subscribe to a topic and send a message to it.
  It will also send a message delayed 5 seconds later.
*/

#include "EspMQTTClient.h"
#include <ArduinoJson.h>
#define PIN_DRYER 36
#define PIN_WASHER 39
#define TOPIC "device/data/1000000113/1e0963e0-70b0-4eab-81c8-1c795c0faf48"

EspMQTTClient *client;

void setup()
{
  Serial.begin(115200);
  pinMode(PIN_DRYER,INPUT);
  pinMode(PIN_WASHER,INPUT);
  client = new EspMQTTClient(
    "a",
    "a",
    "mqtt.iotera.io",  // MQTT Broker server ip
    "mqtt_1000000113_1e0963e0-70b0-4eab-81c8-1c795c0faf48",   // Can be omitted if not needed
    "wq1dzxxvjazlkpsk",   // Can be omitted if not needed
    "TestClient",     // Client name that uniquely identify your device
    8883              // The MQTT port, default to 1883. this line can be omitted
  );
  // Optionnal functionnalities of EspMQTTClient : 
  client->enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client->enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overrited with enableHTTPWebUpdater("user", "password").
//  client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
  client->subscribe(TOPIC, [](const String & payload) {
    Serial.println(payload);
  });

  // Publish a message to "mytopic/test"
  client->publish(TOPIC, "ESP32 Connected"); // You can activate the retain flag by setting the third parameter to true

}

bool LastDryerStatus = false;
bool LastWasherStatus = false;
const size_t capacity = JSON_OBJECT_SIZE(3);
DynamicJsonDocument doc(capacity);

bool DryerStatus = false;
bool WasherStatus = false;
bool x = false;
void loop() {
  client->loop();
  doc["param"] = "value";
  if (millis()>10000 && x == false){
    client = new EspMQTTClient(
      "RedmiD",
      "AAAAABBBBB",
      "mqtt.iotera.io",  // MQTT Broker server ip
      "mqtt_1000000113_1e0963e0-70b0-4eab-81c8-1c795c0faf48",   // Can be omitted if not needed
      "wq1dzxxvjazlkpsk",   // Can be omitted if not needed
      "TestClient",     // Client name that uniquely identify your device
      8883              // The MQTT port, default to 1883. this line can be omitted
    );
    client->enableDebuggingMessages(); 
    client->enableHTTPWebUpdater();
    x = true;
  }
  if (millis()%6000 == 0){
    DryerStatus = !DryerStatus;
    WasherStatus = !WasherStatus;
  }
  
  // Buat supaya berubah kaya interrupt
  if (DryerStatus != LastDryerStatus && client->isConnected()) {
    doc["sensor"] = "dryer";  
    if (DryerStatus == HIGH){
      doc["value"] = true;      
    } else {
      doc["value"] = false;
    }
    String docString;
    serializeJson(doc, docString);
    client->publish(TOPIC, docString); // You can activate the retain flag by setting the third parameter to true
    LastDryerStatus = DryerStatus;
//    client->subscribe(TOPIC, [](const String & payload) {
//      Serial.println(payload);
//    });  
  }

  if (WasherStatus != LastWasherStatus && client->isConnected()) {
    doc["sensor"] = "washer";  
    if (WasherStatus == HIGH){
      doc["value"] = true;      
    } else {
      doc["value"] = false;
    }
    String docString;
    serializeJson(doc, docString);
    client->publish(TOPIC, docString); // You can activate the retain flag by setting the third parameter to true|
    LastWasherStatus = WasherStatus;
//    client->subscribe(TOPIC, [](const String & payload) {
//      Serial.println(payload);
//    });  
  }  
}
