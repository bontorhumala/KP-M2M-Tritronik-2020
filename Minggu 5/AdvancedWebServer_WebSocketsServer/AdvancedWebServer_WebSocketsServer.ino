#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>

const char *assid = "ESP32-AP";
const char *apassword = "AAAAABBBBB";
const char* www_username = "admin";
const char* www_password = "esp32";
const int http_port = 80;
const int ws_port = 1337;
enum setting {NETWORK, SETUP_WIFI, ABOUT, REBOOT, DFLT};
enum setting choice = DFLT; // default value
String ptr = "<!doctype html><html><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><head><title>ESP32 Network Setting</title><style>.header {padding: 20px;text-align: center;background: #1abc9c;color: white;}.header h1 {font-size: 40px;}label.a {padding: 12px 0;font: 20px Arial, sans-serif;display: inline-block;}input[type=text] {margin: 8px 0;width: 100%;padding: 12px;border: 3px solid #ccc;-webkit-transition: 0.5s;transition: 0.5s;outline: none;resize: vertical;}input[type=text]:focus {border: 3px solid #555;}input[type=text]:hover {background-color: #ebebeb;}input[type=submit] {background-color: #4CAF50;border: none;color: white;padding: 14px 20px;text-decoration: none;margin: 8px 0;border-radius: 4px;cursor: pointer;width: 100%;float: right;-webkit-transition: 0.3s;transition: 0.3s;font: 20px Arial, sans-serif;}input[type=submit]:hover {background-color: #357e39;}.button {background-color: #a10707;border: none;color: white;padding: 14px 20px;text-decoration: none;margin: 8px 0;border-radius: 4px;cursor: pointer;width: 100%;float: left;-webkit-transition: 0.3s;transition: 0.3s;font: 20px Arial, sans-serif;}.button:hover {background-color: #5c0404;}.log {font: 15px Arial, sans-serif;border-radius: 5px;background-color: #f2f2f2;padding: 20px;}.col-10 {float: left;width: 10%;margin-top: 6px;}.col-90 {float: left;width: 90%;margin-top: 6px;}.col-50 {float: left;width: 50%;margin-top: 6px;}.row:after {content: \"\";display: table;clear: both;}@media screen and (max-width: 1000px) {.col-50,.col-10,.col-90,input[type=submit] {width: 100%;margin-top: 0;}}</style></head><body><div class=\"header\"><h1>Laundry Controller ESP32</h1><p>To Connect ESP32 To Access Point or Router</p></div><form name=\"publish\"><div class=\"row\"><div class=\"col-10\"><label class=a>SSID:</label></div><div class=\"col-90\"><input type=\"text\" name=\"ssid\" placeholder=\"ssid\" /><br></div></div><div class=\"row\"><div class=\"col-10\"><label class=a>Password:</label></div><div class=\"col-90\"><input type=\"text\" name=\"pwd\" placeholder=\"password\" /></div></div><div class=\"row\"><div class=\"col-50\"><button class=button id=\"ButtonStop\">Close Connection</button></div><div class=\"col-50\"><input type=\"submit\" value=\"Send\" /></div></div></form><div class=\"col-10\"><label class=a>Log:</label></div><div class=log id=\"messages\"></div><script>let url = \"ws://192.168.4.1:1337/\";let socket = new WebSocket(url);document.forms.publish.onsubmit = function () {let message = { ssid: this.ssid.value, password: this.pwd.value };let outgoingMessage = JSON.stringify(message);socket.send(outgoingMessage);return false;};socket.onmessage = function (event) {let incomingMessage = event.data;showMessage(incomingMessage);};socket.onclose = function (event) {console.log(`Closed ${event.code}`);alert(\"Websocket disconnected\\nRefresh page to begin new connection\");};function showMessage(message) {let messageElem = document.createElement('div');messageElem.textContent = message;document.getElementById('messages').prepend(messageElem);}ButtonStop.onclick = function () {if (socket.readyState === WebSocket.OPEN) {var r = confirm(\"Are you sure to close connection?\");if (r == true) {socket.close();} else {alert(\"Input SSID and Password to connect ESP32 to Access Point with Internet\");}} else {alert(\"WebSocket already closed\");}}</script></body></html>";
WebServer server(http_port);
WebSocketsServer webSocket = WebSocketsServer(ws_port);

volatile int interruptCounter = 0;
int totalInterruptCounter = 0;

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;
  portEXIT_CRITICAL_ISR(&timerMux);

}
uint8_t c_num;
// Callback: receiving any WebSocket message
void onWebSocketEvent(uint8_t client_num, WStype_t type, uint8_t * payload, size_t length) {
  c_num = client_num;
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
    case WStype_TEXT: {

        // Print out raw message
        Serial.printf("[%u] Received text: %s\n", client_num, payload);
        StaticJsonDocument<150> doc;   //Memory pool
        const char * msg = (char*)payload;

        auto error = deserializeJson(doc, msg); // check for deserialization error
        if (error) {
          Serial.print(F("deserializeJson() failed with code "));
          Serial.println(error.c_str());
          return;
        }

        const char * ssid = doc["ssid"];         //Get sensor type value
        const char * password = doc["password"]; //Get sensor type value
        Serial.println("Connecting to WiFi___");
        int status = WiFi.begin(ssid, password);
        if (status != WL_CONNECTED) {
          Serial.print("Unable to connect to ");
          Serial.println(ssid);
        }
      }
      break;
    // For everything else: do nothing
    default: {
        break;
      }
  }
}

// Callback: send homepage
void handle_OnConnect() {
  //  if (!server.authenticate(www_username, www_password)) {
  //    return server.requestAuthentication();
  //  }
  server.send(200, "text/html", ptr);
}

void hande_OnNotFound() {
  server.send(404, "text/plain", "Not found");
}

void setup(void) {
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000000, true);
  timerAlarmEnable(timer);
  Serial.begin(115200);
  delay(2000);
  // On HTTP request for root, provide index.html file
  server.on("/", handle_OnConnect);
  // Handle requests for pages that do not exist
  server.onNotFound(hande_OnNotFound);
  // Start WebSocket server and assign callback
  webSocket.onEvent(onWebSocketEvent);
}
bool x = false;
bool isWebSocketConnected = true;
void loop(void) {
  if (interruptCounter > 0) {
    portENTER_CRITICAL(&timerMux);
    interruptCounter--;
    portEXIT_CRITICAL(&timerMux);
    totalInterruptCounter++;
    Serial.print("An interrupt has occurred. Total number: ");
    Serial.println(totalInterruptCounter);
  }
  if (totalInterruptCounter == 15) {
    choice = SETUP_WIFI;
  }
  if (totalInterruptCounter == 50) {
    choice = SETUP_WIFI;
  }
  switch (choice) {
    case NETWORK: choice = DFLT; break;
    case SETUP_WIFI: {
        choice = DFLT;
        if (WiFi.status() != WL_CONNECTED) {
          WiFi.mode(WIFI_AP_STA);
          WiFi.softAP(assid, apassword);
          Serial.print("IP address: ");
          Serial.println(WiFi.softAPIP());
          server.begin();
          webSocket.begin();
          Serial.println("Access web to connect to station");
          isWebSocketConnected = true;
        }
        while (WiFi.status() != WL_CONNECTED) {
          server.handleClient();
          webSocket.loop();
        }
        if (WiFi.status() == WL_CONNECTED) {
          char sendtext[100];
          unsigned int ip = WiFi.localIP();
          unsigned char bytes[4];
          bytes[0] = ip & 0xFF;
          bytes[1] = (ip >> 8) & 0xFF;
          bytes[2] = (ip >> 16) & 0xFF;
          bytes[3] = (ip >> 24) & 0xFF;
          sprintf(sendtext, "Connected to %s Network with Local IP: %d.%d.%d.%d\n", WiFi.SSID(), bytes[0], bytes[1], bytes[2], bytes[3]);
          Serial.print(sendtext);
          if (isWebSocketConnected == true) {
            webSocket.sendTXT(c_num, sendtext);
            webSocket.sendTXT(c_num, "Server Disconnected");
            webSocket.close();
            server.stop();
            WiFi.softAPdisconnect(true);
            isWebSocketConnected = false;
          }
        }
        break;
      }
    case ABOUT: choice = DFLT; break;
    case REBOOT: choice = DFLT; break;
    default: break;
  }
}
