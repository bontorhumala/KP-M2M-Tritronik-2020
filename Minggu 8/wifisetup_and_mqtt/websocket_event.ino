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
        "TestClient",     // Client name that uniquely identify your device
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
