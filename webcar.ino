/*
Authors: Talha and Abdullah
Date: 2025-01-20
Description: Code for car which is controlled via website
Documentation used: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/wifi.html
*/

#include <WiFi.h>
#include <WebServer.h>

// Function Protoypes
void handle_OnConnect();
void buttonPressed(const String& button);
void handle_NotFound();
String SendHTML();

// WiFi Information
const char* ssid = "";
const char* password = "";

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
String ptr = R"DELIMITER_FOR_WEBSITE( 

<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>WebCar</title>
</head>

<body>
    <div class="menu">
        <h1>WebCar</h1>
    </div>

    <div class="grid-container">
        <div class="TopArrow">
            <button onclick="logMessage('up')" type="button">
                <img src="https://t4.ftcdn.net/jpg/05/30/79/15/360_F_530791557_T8hNharBQQJqmw0R2FRjCrC9CgNwaeui.jpg">
            </button>
        </div>

        <div class="LeftArrow">
            <button onclick="logMessage('left')" type="button">
                <img src="https://t4.ftcdn.net/jpg/05/30/79/15/360_F_530791557_T8hNharBQQJqmw0R2FRjCrC9CgNwaeui.jpg">
            </button>
        </div>

        <div class="StopButton">
            <button onclick="logMessage('stop')" type="button">STOP</button>
        </div>

        <div class="RightArrow">
            <button onclick="logMessage('right')" type="button">
                <img src="https://t4.ftcdn.net/jpg/05/30/79/15/360_F_530791557_T8hNharBQQJqmw0R2FRjCrC9CgNwaeui.jpg">
            </button>
        </div>

        <div class="DownArrow">
            <button onclick="logMessage('down')" type="button">
                <img src="https://t4.ftcdn.net/jpg/05/30/79/15/360_F_530791557_T8hNharBQQJqmw0R2FRjCrC9CgNwaeui.jpg">
            </button>
        </div>

        <div class="Direction" id="Direction"></div>
    </div>

    <style>
        body {
            display: flex;
            flex-direction: column;
            align-items: center;
        }

        .menu h1 {
            color: blue;
            font-size: 3rem;
            margin-bottom: 30px;
        }

        .grid-container {
            display: grid;
            /* Creates 3 equal columns and 3 equal rows */
            grid-template-columns: 1fr 1fr 1fr;
            grid-template-rows: 1fr 1fr 1fr;
            gap: 15px;
            max-width: 400px;
            /* Limits size on desktop */
            aspect-ratio: 1 / 1;
            /* Keeps it a perfect square */
        }

        .TopArrow {
            grid-column: 2;
            grid-row: 1;
            transform: rotate(-90deg);
        }

        .LeftArrow {
            grid-column: 1;
            grid-row: 2;
            transform: rotate(180deg);
        }

        .RightArrow {
            grid-column: 3;
            grid-row: 2;
            transform: rotate(0deg);
        }

        .DownArrow {
            grid-column: 2;
            grid-row: 3;
            transform: rotate(90deg);
        }

        .StopButton {
            grid-column: 2;
            grid-row: 2;
        }


        button {
            width: 100%;
            height: 100%;
            background-color: rgb(255, 255, 255);
            border: 2px solid #000000;
            border-radius: 15px;
            cursor: pointer;
            display: flex;
            align-items: center;
            justify-content: center;

            padding: 10px;
        }

        button:active {
            background-color: #ffffff;
            transform: scale(0.85);
        }

        img {
            max-width: 80%;
            max-height: 80%;
            pointer-events: none;
        }

        .StopButton button {
            color: rgb(0, 0, 0);
            border-color: black;
            font-size: 1.5rem;
            font-weight: bold;
            border-radius: 15px;
        }

        .Direction {
            color: black;
            font-size: 1.5rem;
            grid-row: 1;
            grid-column: 1;
        }
    </style>

    <script>
        function logMessage(direction) {
            if (direction === 'stop')
                document.getElementById("Direction").textContent = "Stationary";
            else
                document.getElementById("Direction").textContent = "Direction: " + direction;

            console.log("Sending command: " + direction);
            fetch('/' + direction).then(response => response.text()).then(text => console.log(text));
        }
    </script>

</body>
</html>

)DELIMITER_FOR_WEBSITE"; 

  return ptr;
}
