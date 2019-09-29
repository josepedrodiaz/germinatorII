void handleRoot() {
  server.send(200, "text/html", "<a href=\"/data.csv\">Descargar data.csv</a>");   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}
