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
