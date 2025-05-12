#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// Access Point credentials
const char* ssid = "ESP32-FingerHand";
const char* password = "12345678";

// Create 5 servo objects
Servo thumbServo, indexServo, middleServo, ringServo, littleServo;

// GPIO pins for servos
const int thumbPin = 13;
const int indexPin = 14;
const int middlePin = 27;
const int ringPin = 26;
const int littlePin = 25;

WebServer server(80);

// Responsive HTML UI for 5 servos
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 Finger Control</title>
  <style>
    body { font-family: sans-serif; text-align: center; background: #f2f2f2; padding: 20px; }
    .slider-container { background: white; padding: 15px; margin: 10px auto; border-radius: 10px; width: 90%; max-width: 400px; box-shadow: 0 0 10px rgba(0,0,0,0.1); }
    input[type=range] { width: 100%; }
    .label { font-weight: bold; margin-bottom: 5px; }
  </style>
</head>
<body>
  <h2>ESP32 Finger Servo Control</h2>

  <div class="slider-container">
    <div class="label">Thumb: <span id="thumbVal">90</span>°</div>
    <input type="range" min="0" max="180" value="90" oninput="update('thumb', this.value)">
  </div>

  <div class="slider-container">
    <div class="label">Index: <span id="indexVal">90</span>°</div>
    <input type="range" min="0" max="180" value="90" oninput="update('index', this.value)">
  </div>

  <div class="slider-container">
    <div class="label">Middle: <span id="middleVal">90</span>°</div>
    <input type="range" min="0" max="180" value="90" oninput="update('middle', this.value)">
  </div>

  <div class="slider-container">
    <div class="label">Ring: <span id="ringVal">90</span>°</div>
    <input type="range" min="0" max="180" value="90" oninput="update('ring', this.value)">
  </div>

  <div class="slider-container">
    <div class="label">Little: <span id="littleVal">90</span>°</div>
    <input type="range" min="0" max="180" value="90" oninput="update('little', this.value)">
  </div>

  <script>
    function update(finger, val) {
      document.getElementById(finger + 'Val').innerText = val;
      fetch(`/set?finger=${finger}&angle=${val}`);
    }
  </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);

  // Start AP mode
  WiFi.softAP(ssid, password);
  Serial.print("ESP32 AP IP: ");
  Serial.println(WiFi.softAPIP());

  // Attach all servos
  thumbServo.setPeriodHertz(50);
  indexServo.setPeriodHertz(50);
  middleServo.setPeriodHertz(50);
  ringServo.setPeriodHertz(50);
  littleServo.setPeriodHertz(50);

  thumbServo.attach(thumbPin);
  indexServo.attach(indexPin);
  middleServo.attach(middlePin);
  ringServo.attach(ringPin);
  littleServo.attach(littlePin);

  // Center all fingers
  thumbServo.write(90);
  indexServo.write(90);
  middleServo.write(90);
  ringServo.write(90);
  littleServo.write(90);

  // Web routes
  server.on("/", []() {
    server.send(200, "text/html", htmlPage);
  });

  server.on("/set", []() {
    String finger = server.arg("finger");
    int angle = server.arg("angle").toInt();
    angle = constrain(angle, 0, 180);

    if (finger == "thumb") thumbServo.write(angle);
    else if (finger == "index") indexServo.write(angle);
    else if (finger == "middle") middleServo.write(angle);
    else if (finger == "ring") ringServo.write(angle);
    else if (finger == "little") littleServo.write(angle);

    server.send(200, "text/plain", "OK");
  });

  server.begin();
  Serial.println("Web server started");
}

void loop() {
  server.handleClient();
}