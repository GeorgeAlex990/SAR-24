#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define M1_A 5
#define M1_B 4
#define M2_A 2
#define M2_B 14

#define HONK_PIN 12

const char* ssid = "SAR-24"; // SSID of the Access Point
const char* password = "-SAR-24-"; // Password of the Access Point

IPAddress apIP(192, 168, 152, 1);
IPAddress netMsk(255, 255, 255, 0);

AsyncWebServer server(80);

String outputState(int output) {
  if (digitalRead(output)) {
    return "HIGH";
  } else {
    return "LOW";
  }
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>SAR-24</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html { font-family: Arial; display: inline-block; text-align: center; }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    body { max-width: 600px; margin:0 auto; padding-bottom: 25px; }
    .button { background-color: #ccc; border: none; color: white; padding: 16px 40px;
      text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer; }
    .button:hover { background-color: #555; }
    .button:active { background-color: #aaa; } /* Highlight when button is pressed */
  </style>
</head>
<body>
  <h2>SAR-24 Master Control</h2>
  <div id="buttonContainer">
    <button id="2" class="button">FORWARD</button>
    <br>
    <button id="14" class="button">LEFT</button>
    <button id="5" class="button">BACKWARD</button>
    <button id="4" class="button">RIGHT</button>
    <br>
    <button id="12" class="button">HONK</button>
  </div>

<script>
document.querySelectorAll('.button').forEach(button => {
  button.addEventListener('touchstart', function() {
    pushButton(this, 1);
  });
  button.addEventListener('touchend', function() {
    pushButton(this, 0);
  });
});

function pushButton(button, state) {
  var pin = button.id;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/update?output="+pin+"&state="+state, true);
  xhr.send();
}
</script>
</body> 
</html>
)rawliteral";

String processor(const String& var){
  if(var == "BUTTONPLACEHOLDER"){
    String buttons = "";
    buttons += "<div id=\"buttonContainer\">";
    buttons += "<button id=\"2\" class=\"button\">FORWARD</button>";
    buttons += "<br>";
    buttons += "<button id=\"14\" class=\"button\">LEFT</button>";
    buttons += "<button id=\"5\" class=\"button\">BACKWARD</button>";
    buttons += "<button id=\"4\" class=\"button\">RIGHT</button>";
    buttons += "</div>";
    return buttons;
  }
  return String();
}

void BRAKE() {
  digitalWrite(M1_A, LOW);
  digitalWrite(M1_B, LOW);
  digitalWrite(M2_A, LOW);
  digitalWrite(M2_B, LOW);
}

void BACKWARD() {
  digitalWrite(M1_A, HIGH);
  digitalWrite(M1_B, LOW);
  digitalWrite(M2_A, HIGH);
  digitalWrite(M2_B, LOW);
}

void FORWARD() {
  digitalWrite(M1_A, LOW);
  digitalWrite(M1_B, HIGH);
  digitalWrite(M2_A, LOW);
  digitalWrite(M2_B, HIGH);
}

void RIGHT() {
  digitalWrite(M1_A, LOW);
  digitalWrite(M1_B, HIGH);
  digitalWrite(M2_A, HIGH);
  digitalWrite(M2_B, LOW);
}

void LEFT() {
  digitalWrite(M1_A, HIGH);
  digitalWrite(M1_B, LOW);
  digitalWrite(M2_A, LOW);
  digitalWrite(M2_B, HIGH);
}

void HONK_START() {
  digitalWrite(HONK_PIN, HIGH);
}

void HONK_END() {
  digitalWrite(HONK_PIN, LOW);
}

void setup(){
  Serial.begin(115200);

  // Configure ESP8266 as an Access Point
  Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");
  WiFi.softAPConfig(apIP, apIP, netMsk);

  //Configure ESP8266 Pins
  pinMode(M1_A, OUTPUT);
  digitalWrite(M1_A, LOW);
  pinMode(M1_B, OUTPUT);
  digitalWrite(M1_B, LOW);
  pinMode(M2_A, OUTPUT);
  digitalWrite(M2_A, LOW);
  pinMode(M2_B, OUTPUT);
  digitalWrite(M2_B, LOW);
  pinMode(HONK_PIN, OUTPUT);
  digitalWrite(HONK_PIN, LOW);
  
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String outputMessage;
    if (request->hasParam("output") && request->hasParam("state")) {
      int output = request->getParam("output")->value().toInt();
      int state = request->getParam("state")->value().toInt();
      if (output == 2 && state == 1) {
        FORWARD();
      }
      if (output == 4 && state == 1) {
        RIGHT();
      }
      if (output == 5 && state == 1) {
        BACKWARD();
      }
      if (output == 14 && state == 1) {
        LEFT();
      }
      if (output == 12 && state == 1) {
        HONK_START();
      }
      if (output == 12 && state == 0) {
        HONK_END();
      }
      if (state == 0) {
        BRAKE();
      }
      outputMessage = "GPIO " + String(output) + " set to " + String(state);
    } else {
      outputMessage = "Invalid request";
    }
    request->send(200, "text/plain", "OK");
  });

  server.begin();
}

void loop() {}
