#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// ==========================
// PIN DEFINITIONS
// ==========================
// Left Motor
#define IN1 5   // D1
#define IN2 4   // D2
#define ENA 13  // D7 (Speed/PWM)

// Right Motor
#define IN3 14  // D5
#define IN4 12  // D6
#define ENB 15  // D8 (Speed/PWM)

// ==========================
// CONFIGURATION
// ==========================
const char *ssid = "Robot_Car";
const char *password = "password123";

// Speed (0-1023 for ESP8266 10-bit PWM)
// Adjust these values if motors are too fast/slow
int motorSpeed = 800; 

ESP8266WebServer server(80);

// ==========================
// WEB FRAMEWORK (HTML/CSS/JS)
// ==========================
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP8266 Robot Controller</title>
  <style>
    body {
      background-color: #1a1a1a;
      color: #00ffcc;
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      text-align: center;
      margin: 0;
      padding: 0;
      display: flex;
      flex-direction: column;
      height: 100vh;
      justify-content: center;
      align-items: center;
      user-select: none;
    }
    h1 { margin-bottom: 20px; font-weight: 300; letter-spacing: 2px; }
    .control-pad {
      display: grid;
      grid-template-columns: 100px 100px 100px;
      grid-gap: 15px;
      background: #2a2a2a;
      padding: 30px;
      border-radius: 20px;
      box-shadow: 0 10px 30px rgba(0,0,0,0.5);
    }
    .btn {
      width: 100px;
      height: 100px;
      background: #333;
      border: 2px solid #00ffcc;
      border-radius: 15px;
      color: #00ffcc;
      font-size: 24px;
      display: flex;
      justify-content: center;
      align-items: center;
      cursor: pointer;
      transition: all 0.2s ease;
      box-shadow: 0 4px 6px rgba(0,0,0,0.3);
    }
    .btn:active {
      background: #00ffcc;
      color: #1a1a1a;
      transform: translateY(2px);
      box-shadow: 0 2px 4px rgba(0,0,0,0.2);
    }
    .btn.stop {
      background: #ff4444;
      border-color: #ff4444;
      color: white;
    }
    .btn.stop:active {
      background: #cc0000;
      color: white;
    }
    /* Grid placement */
    .btn-up { grid-column: 2; }
    .btn-left { grid-column: 1; grid-row: 2; }
    .btn-stop { grid-column: 2; grid-row: 2; }
    .btn-right { grid-column: 3; grid-row: 2; }
    .btn-down { grid-column: 2; grid-row: 3; }
    
    .status { margin-top: 20px; font-size: 14px; opacity: 0.7; }
  </style>
</head>
<body>
  <h1>ROBOT CONTROL</h1>
  <div class="control-pad">
    <div class="btn btn-up" ontouchstart="move('F')" onmousedown="move('F')" ontouchend="stop()" onmouseup="stop()">&#9650;</div>
    
    <div class="btn btn-left" ontouchstart="move('L')" onmousedown="move('L')" ontouchend="stop()" onmouseup="stop()">&#9664;</div>
    <div class="btn btn-stop stop" onclick="stop()">STOP</div>
    <div class="btn btn-right" ontouchstart="move('R')" onmousedown="move('R')" ontouchend="stop()" onmouseup="stop()">&#9654;</div>
    
    <div class="btn btn-down" ontouchstart="move('B')" onmousedown="move('B')" ontouchend="stop()" onmouseup="stop()">&#9660;</div>
  </div>
  <div class="status" id="status-text">Ready</div>

  <script>
    function move(dir) {
      document.getElementById('status-text').innerText = 'Moving: ' + dir;
      fetch('/move?dir=' + dir).catch(console.error);
    }
    function stop() {
      document.getElementById('status-text').innerText = 'Stopped';
      fetch('/move?dir=S').catch(console.error);
    }
    
    // Prevent context menu on long press
    document.oncontextmenu = function(event) { event.preventDefault(); event.stopPropagation(); return false; };
  </script>
</body>
</html>
)rawliteral";

// ==========================
// MOTOR LOGIC
// ==========================
void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void turnLeft() {
  // Left motor Back, Right motor Forward (Spin turn)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

void turnRight() {
  // Left motor Forward, Right motor Back (Spin turn)
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, motorSpeed);
  analogWrite(ENB, motorSpeed);
}

// ==========================
// HTTP HANDLERS
// ==========================
void handleRoot() {
  server.send(200, "text/html", index_html);
}

void handleMove() {
  if (server.hasArg("dir")) {
    String dir = server.arg("dir");
    if (dir == "F") moveForward();
    else if (dir == "B") moveBackward();
    else if (dir == "L") turnLeft();
    else if (dir == "R") turnRight();
    else if (dir == "S") stopMotors();
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Bad Request");
  }
}

// ==========================
// SETUP & LOOP
// ==========================
void setup() {
  Serial.begin(115200);
  
  // Motor Pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  stopMotors();

  // WiFi AP
  Serial.print("Setting up Access Point...");
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // Web Server
  server.on("/", handleRoot);
  server.on("/move", handleMove);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
