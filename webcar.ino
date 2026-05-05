/*
Authors: Talha and Abdullah
Date: 2025-01-20
Description: Code for car which is controlled via website
*/

#include <WiFi.h>
#include <WebServer.h>

// Function Protoypes
void handle_OnConnect();
void buttonPressed(const String& button);
void handle_NotFound();
String SendHTML();

// WiFi Information
const char* ssid = "Allah";
const char* password = "teoolz1596";

WebServer server(80); // Port 80 uses HTTP

void setup() {
  Serial.begin(115200);

  // Set motor pins as output
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);

  // Everything from this point has to do with WiFi and setting up the Web Server
  Serial.print("SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) { // Code to do while it is waiting for a connection
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\n\nWiFi Connected");

  Serial.print("Web Address: ");
  Serial.println(WiFi.localIP());

  // Web Server Setup
  server.on("/", handle_OnConnect);
  server.on("/up", []() { buttonPressed("up"); });
  server.on("/down", []() { buttonPressed("down"); });
  server.on("/left", []() { buttonPressed("left"); });
  server.on("/right", []() { buttonPressed("right"); });
  server.on("/stop", []() { buttonPressed("stop"); });
  server.onNotFound(handle_NotFound);
  server.begin();
}

void loop() {
  server.handleClient();  // Handle client request
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML());  // Send HTML code for web page
}

void buttonPressed(const String& button) {
  Serial.println("Button pressed: " + button);  // Print the button status to Serial Monitor

  // Control the motors using analogWrite
  if (button == "up") {
    analogWrite(4, 200);  // Motor control for forward direction
    analogWrite(5, 0); 
    analogWrite(18, 200);  
    analogWrite(19, 0);
  } else if (button == "down") {
    analogWrite(4, 0);
    analogWrite(5, 200); 
    analogWrite(18, 0);  
    analogWrite(19, 200);
  } else if (button == "left") {
    analogWrite(4, 200);
    analogWrite(5, 0); 
    analogWrite(18, 0);  
    analogWrite(19, 200);
  } else if (button == "right") {
    analogWrite(4, 0);
    analogWrite(5, 200); 
    analogWrite(18, 200);  
    analogWrite(19, 0);
  } else if (button == "stop") {
    analogWrite(4, 0);
    analogWrite(5, 0); 
    analogWrite(18, 0);  
    analogWrite(19, 0);
  }

  server.send(200, "text/html", SendHTML()); // Return to the same page
}

void handle_NotFound() { // Handle Exceptions
  server.send(404, "text/plain", "Not found");
}

String SendHTML() {
  String ptr = "<!DOCTYPE html><html lang=\"en\">\n";
  ptr += "<head>\n";
  ptr += "<meta charset=\"UTF-8\">\n";
  ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
  ptr += "<title>Buttons</title>\n";
  ptr += "<script>\n";
  ptr += "function logMessage(direction) {\n";
  ptr += "  fetch('/' + direction)\n";
  ptr += "    .then(response => response.text())\n";
  ptr += "    .then(text => console.log(text));\n";
  ptr += "}\n";
  ptr += "</script>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "  <button onclick=\"logMessage('up')\">Up</button>\n";
  ptr += "  <button onclick=\"logMessage('down')\">Down</button>\n";
  ptr += "  <button onclick=\"logMessage('left')\">Left</button>\n";
  ptr += "  <button onclick=\"logMessage('right')\">Right</button>\n";
  ptr += "  <button onclick=\"logMessage('stop')\">Stop</button>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
