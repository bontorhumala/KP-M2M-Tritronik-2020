// Callback: send homepage
void handle_OnConnect() {
    if (!server.authenticate(www_username, www_password)) {
      return server.requestAuthentication();
    }
  server.send(200, "text/html", ptr);
}

void hande_OnNotFound() {
  server.send(404, "text/plain", "Not found");
}

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
      strncpy(ssid,doc["ssid"],50);
      strncpy(password,doc["password"],50);

      client = new EspMQTTClient(
        ssid,
        password,
        "mqtt.iotera.io",  // MQTT Broker server ip
        "mqtt_1000000113_1e0963e0-70b0-4eab-81c8-1c795c0faf48",   // Can be omitted if not needed
        "wq1dzxxvjazlkpsk",   // Can be omitted if not needed
        deviceID,     // Client name that uniquely identify your device
        8883              // The MQTT port, default to 1883. this line can be omitted
      );
      client->enableDebuggingMessages(); // Enable debugging messages sent to serial output
      client->enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overrited with enableHTTPWebUpdater("user", "password").
      break;
    }
    // For everything else: do nothing
    default: {
        break;
      }
  }
}

void ConnectionSetup(){
  pinMode(PIN_DRYER,INPUT);
  pinMode(PIN_WASHER,INPUT);
  
  client = new EspMQTTClient(
    ssid,
    password,
    "mqtt.iotera.io",  // MQTT Broker server ip
    "mqtt_1000000113_1e0963e0-70b0-4eab-81c8-1c795c0faf48",   // Can be omitted if not needed
    "wq1dzxxvjazlkpsk",   // Can be omitted if not needed
    deviceID,     // Client name that uniquely identify your device
    8883              // The MQTT port, default to 1883. this line can be omitted
  );
  
  // Optionnal functionnalities of EspMQTTClient : 
  client->enableDebuggingMessages(); // Enable debugging messages sent to serial output
//  client->enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overrited with enableHTTPWebUpdater("user", "password").
  //client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true
  
  // On HTTP request for root, provide index.html file
  server.on("/", handle_OnConnect);
  // Handle requests for pages that do not exist
  server.onNotFound(hande_OnNotFound);
  // Start WebSocket server and assign callback
  webSocket.onEvent(onWebSocketEvent);
}
void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
  client->subscribe(TOPIC, [](const String & payload) {
    Serial.println(payload);
  });

  // Publish a message to "mytopic/test"
  client->publish(TOPIC, "ESP32 Connected"); // You can activate the retain flag by setting the third parameter to true
}

bool ConnectionWifiSetup(){
  client->loop();
  // put your main code here, to run repeatedly:
  if (!client->isConnected()) {
    //WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(assid, apassword);
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
    
    textWifiSetup(WiFi.softAPIP());
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
    unsigned int ip = WiFi.localIP();
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
    return true;
  } else {
    return false;
  }
}
