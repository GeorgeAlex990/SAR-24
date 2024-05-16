#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Wire.h>

#define HONK_PIN D8

#define Arduino_motors 8

int output,state;

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
    <button id="1" class="button">FORWARD</button>
    <br>
    <button id="3" class="button">LEFT</button>
    <button id="2" class="button">BACKWARD</button>
    <button id="4" class="button">RIGHT</button>
    <br>
    <br>
    <button id="5" class="button">HONK</button>
    <br>
    <br>
    <button id="6" class="button">CAM UP</button>
    <br>
    <button id="8" class="button">CAM LEFT</button>
    <button id="7" class="button">CAM DOWN</button>
    <button id="9" class="button">CAM RIGHT</button>
    <br>
    <button id="10" class="button">CAM CENTER</button>
    <br>
    <br>
    <button id="11" class="button">SIREN</button>
    <button id="12" class="button">HIGH</button>
    <button id="13" class="button">LOW</button>
    <br>
    <button id="14" class="button">??</button>
    <button id="15" class="button">PAUSE</button>
  
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
    buttons += "<button id=\"1\" class=\"button\">FORWARD</button>";
    buttons += "<br>";
    buttons += "<button id=\"3\" class=\"button\">LEFT</button>";
    buttons += "<button id=\"2\" class=\"button\">BACKWARD</button>";
    buttons += "<button id=\"4\" class=\"button\">RIGHT</button>";
    buttons += "</div>";
    return buttons;
  }
  return String();
}

void HONK_START() {
  digitalWrite(HONK_PIN, HIGH);
}

void HONK_END() {
  digitalWrite(HONK_PIN, LOW);
}

void setup(){
  Serial.begin(115200);

  //Configure the I2C Bus
  Wire.begin(D1,D2);

  Wire.beginTransmission(Arduino_motors); /* begin with device address 8 */
  Wire.write("                   ");  /* sends command */
  Wire.endTransmission();    /* stop transmitting */

  // Configure ESP8266 as an Access Point
  Serial.println(WiFi.softAP(ssid, password) ? "Ready" : "Failed!");
  WiFi.softAPConfig(apIP, apIP, netMsk);

  //Configure ESP8266 Pins
  pinMode(HONK_PIN, OUTPUT);
  digitalWrite(HONK_PIN, LOW);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String outputMessage;
    if (request->hasParam("output") && request->hasParam("state")) {
      output = request->getParam("output")->value().toInt();
      state = request->getParam("state")->value().toInt();
      if (output == 1 && state == 1) {
        Wire.beginTransmission(Arduino_motors); /* begin with device address 8 */
        Wire.write("F");  /* sends command */
        Wire.endTransmission();    /* stop transmitting */
        Serial.println("FORWARD");
      }
      if (output == 4 && state == 1) {
        Wire.beginTransmission(Arduino_motors); /* begin with device address 8 */
        Wire.write("R");  /* sends command */
        Wire.endTransmission();    /* stop transmitting */
        Serial.println("RIGHT");
      }
      if (output == 2 && state == 1) {
        Wire.beginTransmission(Arduino_motors); /* begin with device address 8 */
        Wire.write("B");  /* sends command */
        Wire.endTransmission();    /* stop transmitting */
        Serial.println("BACKWARD");
      }
      if (output == 3 && state == 1) {
        Wire.beginTransmission(Arduino_motors); /* begin with device address 8 */
        Wire.write("L");  /* sends command */
        Wire.endTransmission();    /* stop transmitting */
        Serial.println("LEFT");
      }
      if (output == 6 && state == 1) {
        Wire.beginTransmission(Arduino_motors); /* begin with device address 8 */
        Wire.write("W");  /* sends command */
        Wire.endTransmission();    /* stop transmitting */
        Serial.println("CAM UP");
      }
      if (output == 8 && state == 1) {
        Wire.beginTransmission(Arduino_motors); /* begin with device address 8 */
        Wire.write("D");  /* sends command */
        Wire.endTransmission();    /* stop transmitting */
        Serial.println("CAM RIGHT");
      }
      if (output == 7 && state == 1) {
        Wire.beginTransmission(Arduino_motors); /* begin with device address 8 */
        Wire.write("S");  /* sends command */
        Wire.endTransmission();    /* stop transmitting */
        Serial.println("CAM DOWN");
      }
      if (output == 9 && state == 1) {
        Wire.beginTransmission(Arduino_motors); /* begin with device address 8 */
        Wire.write("A");  /* sends command */
        Wire.endTransmission();    /* stop transmitting */
        Serial.println("CAM LEFT");
      }
      if (output == 10 && state == 1) {
        Wire.beginTransmission(Arduino_motors); /* begin with device address 8 */
        Wire.write("C");  /* sends command */
        Wire.endTransmission();    /* stop transmitting */
        Serial.println("CAM CENTER");
      }
      if (output == 5 && state == 1) {
        HONK_START();
      }
      if (output == 5 && state == 0) {
        HONK_END();
      }
      if (output == 11 && state == 1) {
        Wire.beginTransmission(Arduino_motors); /* begin with device address 8 */
        Wire.write("Q");  /* sends command */
        Wire.endTransmission();    /* stop transmitting */
        Serial.println("SIREN");
      }
      if (output == 12 && state == 1) {
        Wire.beginTransmission(Arduino_motors); /* begin with device address 8 */
        Wire.write("H");  /* sends command */
        Wire.endTransmission();    /* stop transmitting */
        Serial.println("HIGH");
      }
      if (output == 13 && state == 1) {
        Wire.beginTransmission(Arduino_motors); /* begin with device address 8 */
        Wire.write("L");  /* sends command */
        Wire.endTransmission();    /* stop transmitting */
        Serial.println("LOW");
      }
      if (output == 14 && state == 1) {
        Wire.beginTransmission(Arduino_motors); /* begin with device address 8 */
        Wire.write("?");  /* sends command */
        Wire.endTransmission();    /* stop transmitting */
        Serial.println("??");
      }
      
      if (output == 1 && state == 0 || output == 2 && state == 0 || output == 3 && state == 0 || output == 4 && state == 0 ) {
        Wire.beginTransmission(Arduino_motors); /* begin with device address 8 */
        Wire.write("P");  /* sends command */
        Wire.endTransmission();    /* stop transmitting */
        Serial.println("BRAKE");
      }
      outputMessage = "GPIO " + String(output) + " set to " + String(state);
    } else {
      outputMessage = "Invalid request";
    }
    request->send(200, "text/plain", "OK");
  });

  server.begin();
}

void loop() {
  Wire.requestFrom(Arduino_motors, 13); /* request & read data of size 13 from slave */
  while(Wire.available()){
    char c = Wire.read();
    Serial.print(c);
  }
  Serial.println();
  delay(1000);
}
