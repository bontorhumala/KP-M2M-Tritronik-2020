//#include <WiFi.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
//
//#include <WiFiClient.h>
//#include <WebServer.h>
//#include <ESPmDNS.h>

const char *assid = "ESP32-AP";
const char *apassword = "AAAAABBBBB";
const int dns_port = 53;
const int http_port = 80;
const int ws_port = 1337;
const int led_pin = 15;

AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(1337);

const int led = 2;
// Callback: receiving any WebSocket message
void onWebSocketEvent(uint8_t client_num, WStype_t type, uint8_t * payload, size_t length) {
  // Figure out the type of WebSocket event
  switch (type) {
    // Client has disconnected
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", client_num);
      break;

    // New client has connected
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(client_num);
        Serial.printf("[%u] Connection from ", client_num);
        Serial.println(ip.toString());
      }
      break;

    // Handle text messages from client
    case WStype_TEXT:{

      // Print out raw message
      Serial.printf("[%u] Received text: %s\n", client_num, payload);
      StaticJsonDocument<150> doc;   //Memory pool
      const char * msg=(char*)payload;
//      deserializeJson(doc, msg); // parse message

      auto error = deserializeJson(doc, msg); // check for deserialization error
      if (error) {
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(error.c_str());
        return;
      }

      const char * ssid = doc["ssid"];         //Get sensor type value
      const char * password = doc["password"]; //Get sensor type value
      WiFi.begin(ssid, password);
      Serial.println("Connecting to WiFi___");
      int timeout = 0;
      while (WiFi.status() != WL_CONNECTED && timeout <= 10) {
        delay(1000);
        timeout++;
        Serial.print(".");
      }
      Serial.print("Connected to the WiFi network");
      Serial.println(" SSID: DaDeDa");
      Serial.println(WiFi.localIP());
      }
      break;
    // For everything else: do nothing
    default:{break;}
  }
}

// Callback: send homepage
void onIndexRequest(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                 "] HTTP GET request of " + request->url());
  request->send(SPIFFS, "/form-websocket.html", "text/html");
}

//// Callback: send style sheet
//void onCSSRequest(AsyncWebServerRequest *request) {
//  IPAddress remote_ip = request->client()->remoteIP();
//  Serial.println("[" + remote_ip.toString() +
//                  "] HTTP GET request of " + request->url());
//  request->send(SPIFFS, "/style.css", "text/css");
//}

// Callback: send 404 if requested file does not exist
void onPageNotFound(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                 "] HTTP GET request of " + request->url());
  request->send(404, "text/plain", "Not found");
}

void setup(void) {
  Serial.begin(115200);
   
  delay(2000);
  WiFi.mode(WIFI_AP_STA);
  
  WiFi.softAP(assid, apassword);

  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  if( !SPIFFS.begin()){
    Serial.println("Error mounting SPIFFS");
    while(1);
  }
  // On HTTP request for root, provide index.html file
  server.on("/", HTTP_GET, onIndexRequest);

  // On HTTP request for style sheet, provide style.css
//  server.on("/style.css", HTTP_GET, onCSSRequest);

  // Handle requests for pages that do not exist
  server.onNotFound(onPageNotFound);

  // Start web server
  server.begin();

  // Start WebSocket server and assign callback
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
}

void loop(void) {
  if (WiFi.status() != WL_CONNECTED){
    webSocket.loop();
  }
}
