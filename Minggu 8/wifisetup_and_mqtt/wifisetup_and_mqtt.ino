#include <WiFi.h>
#include <ArduinoJson.h>
#include <EspMQTTClient.h>
#include <WebSocketsServer.h>

#define TOPIC "device/data/1000000113/1e0963e0-70b0-4eab-81c8-1c795c0faf48"
#define PIN_DRYER 36
#define PIN_WASHER 39

const char *assid = "ESP32-AP";
const char *apassword = "AAAAABBBBB";
const char* www_username = "admin";
const char* www_password = "esp32";
char ssid[50];
char password[50];
const int http_port = 80;
const int ws_port = 1337;
enum setting {NETWORK, SETUP_WIFI, ABOUT, REBOOT, DFLT};
enum setting choice = DFLT; // default value
String ptr = "<!doctype html><html><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><head><title>ESP32 Network Setting</title><style>.header {padding: 20px;text-align: center;background: #1abc9c;color: white;}.header h1 {font-size: 40px;}label.a {padding: 12px 0;font: 20px Arial, sans-serif;display: inline-block;}input[type=text] {margin: 8px 0;width: 100%;padding: 12px;border: 3px solid #ccc;-webkit-transition: 0.5s;transition: 0.5s;outline: none;resize: vertical;}input[type=text]:focus {border: 3px solid #555;}input[type=text]:hover {background-color: #ebebeb;}input[type=submit] {background-color: #4CAF50;border: none;color: white;padding: 14px 20px;text-decoration: none;margin: 8px 0;border-radius: 4px;cursor: pointer;width: 100%;float: right;-webkit-transition: 0.3s;transition: 0.3s;font: 20px Arial, sans-serif;}input[type=submit]:hover {background-color: #357e39;}.button {background-color: #a10707;border: none;color: white;padding: 14px 20px;text-decoration: none;margin: 8px 0;border-radius: 4px;cursor: pointer;width: 100%;float: left;-webkit-transition: 0.3s;transition: 0.3s;font: 20px Arial, sans-serif;}.button:hover {background-color: #5c0404;}.log {font: 15px Arial, sans-serif;border-radius: 5px;background-color: #f2f2f2;padding: 20px;}.col-10 {float: left;width: 10%;margin-top: 6px;}.col-90 {float: left;width: 90%;margin-top: 6px;}.col-50 {float: left;width: 50%;margin-top: 6px;}.row:after {content: \"\";display: table;clear: both;}@media screen and (max-width: 1000px) {.col-50,.col-10,.col-90,input[type=submit] {width: 100%;margin-top: 0;}}</style></head><body><div class=\"header\"><h1>Laundry Controller ESP32</h1><p>To Connect ESP32 To Access Point or Router</p></div><form name=\"publish\"><div class=\"row\"><div class=\"col-10\"><label class=a>SSID:</label></div><div class=\"col-90\"><input type=\"text\" name=\"ssid\" placeholder=\"ssid\" /><br></div></div><div class=\"row\"><div class=\"col-10\"><label class=a>Password:</label></div><div class=\"col-90\"><input type=\"text\" name=\"pwd\" placeholder=\"password\" /></div></div><div class=\"row\"><div class=\"col-50\"><button class=button id=\"ButtonStop\">Close Connection</button></div><div class=\"col-50\"><input type=\"submit\" value=\"Send\" /></div></div></form><div class=\"col-10\"><label class=a>Log:</label></div><div class=log id=\"messages\"></div><script>let url = \"ws://192.168.4.1:1337/\";let socket = new WebSocket(url);document.forms.publish.onsubmit = function () {let message = { ssid: this.ssid.value, password: this.pwd.value };let outgoingMessage = JSON.stringify(message);socket.send(outgoingMessage);return false;};socket.onmessage = function (event) {let incomingMessage = event.data;showMessage(incomingMessage);};socket.onclose = function (event) {console.log(`Closed ${event.code}`);alert(\"Websocket disconnected\\nRefresh page to begin new connection\");};function showMessage(message) {let messageElem = document.createElement('div');messageElem.textContent = message;document.getElementById('messages').prepend(messageElem);}ButtonStop.onclick = function () {if (socket.readyState === WebSocket.OPEN) {var r = confirm(\"Are you sure to close connection?\");if (r == true) {socket.close();} else {alert(\"Input SSID and Password to connect ESP32 to Access Point with Internet\");}} else {alert(\"WebSocket already closed\");}}</script></body></html>";
WebServer server(http_port);
WebSocketsServer webSocket = WebSocketsServer(ws_port);
EspMQTTClient *client;
uint8_t c_num;

void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
  client->subscribe(TOPIC, [](const String & payload) {
    Serial.println(payload);
  });

  // Publish a message to "mytopic/test"
  client->publish(TOPIC, "ESP32 Connected"); // You can activate the retain flag by setting the third parameter to true

}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_DRYER,INPUT);
  pinMode(PIN_WASHER,INPUT);
  client = new EspMQTTClient(
    ssid,
    password,
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
  // On HTTP request for root, provide index.html file
  server.on("/", handle_OnConnect);
  // Handle requests for pages that do not exist
  server.onNotFound(hande_OnNotFound);
  // Start WebSocket server and assign callback
  webSocket.onEvent(onWebSocketEvent);
}
bool x = false;
bool isWebSocketConnected = true;
void loop() {
  client->loop();
  // put your main code here, to run repeatedly:
  if (millis()>10000 && x == false){
    if (!client->isConnected()) {
//      WiFi.mode(WIFI_AP_STA);
      WiFi.softAP(assid, apassword);
      Serial.print("IP address: ");
      Serial.println(WiFi.softAPIP());
      server.begin();
      webSocket.begin();
      Serial.println("Access web to connect to station");
      isWebSocketConnected = true;
    }
    while (!client->isConnected()) {
      server.handleClient();
      webSocket.loop();
      client->loop();
    }
    if (client->isConnected()) {
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
    x = true;
  }
}
