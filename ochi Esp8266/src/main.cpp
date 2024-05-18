#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Setting the addreses for the displays
#define OLED_ADDR_1 0x3C
#define OLED_ADDR_2 0x3D

// Creating two different objects representing the two oleds
Adafruit_SSD1306 display1(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_ADDR_1);
Adafruit_SSD1306 display2(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_ADDR_2);

int blink_y_s = 30, blink_y_d = 0, eyecover = 30;

void image() {
  display1.clearDisplay();

  // Form the IRIS for one eye
  display1.fillCircle(64, 32, 30, 1);
  display1.fillCircle(64, 32, 10, 0);

  // Form the eyelid for one eye
  display1.fillRect(0,0,128,eyecover,0);
  display1.drawLine(10, 7+blink_y_s, 118, 7+blink_y_d, 0);
  display1.drawLine(10, 8+blink_y_s, 118, 8+blink_y_d, 0);
  display1.drawLine(10, 9+blink_y_s, 118, 9+blink_y_d, 0);
  display1.drawLine(10, 10+blink_y_s, 118, 10+blink_y_d, 1);
  display1.drawLine(10, 11+blink_y_s, 118, 11+blink_y_d, 1);
  display1.drawLine(10, 12+blink_y_s, 118, 12+blink_y_d, 1);
  display1.drawLine(11, 13+blink_y_s, 118, 13+blink_y_d, 1);
  display1.drawLine(12, 14+blink_y_s, 117, 14+blink_y_d, 1);
  display1.drawLine(12, 15+blink_y_s, 117, 15+blink_y_d, 1);
  display1.drawLine(11, 16+blink_y_s, 118, 16+blink_y_d, 0);
  display1.drawLine(12, 17+blink_y_s, 117, 17+blink_y_d, 0);
  display1.drawLine(12, 18+blink_y_s, 117, 18+blink_y_d, 0);
  display1.drawLine(12, 19+blink_y_s, 117, 19+blink_y_d, 0);
  display1.drawLine(12, 20+blink_y_s, 117, 20+blink_y_d, 0);
  display1.display();
  
  display2.clearDisplay();

  // Form the IRIS for the other eye
  display2.fillCircle(64, 32, 30, 1);
  display2.fillCircle(64, 32, 10, 0);

  // Form the eyelid for the other eye
  display2.fillRect(0,0,128,eyecover,0);
  display2.drawLine(10, 7+blink_y_d, 118, 7+blink_y_s, 0);
  display2.drawLine(10, 8+blink_y_d, 118, 8+blink_y_s, 0);
  display2.drawLine(10, 9+blink_y_d, 118, 9+blink_y_s, 0);
  display2.drawLine(10, 10+blink_y_d, 118, 10+blink_y_s, 1);
  display2.drawLine(10, 11+blink_y_d, 118, 11+blink_y_s, 1);
  display2.drawLine(10, 12+blink_y_d, 118, 12+blink_y_s, 1);
  display2.drawLine(11, 13+blink_y_d, 118, 13+blink_y_s, 1);
  display2.drawLine(12, 14+blink_y_d, 117, 14+blink_y_s, 1);
  display2.drawLine(12, 15+blink_y_d, 117, 15+blink_y_s, 1);
  display2.drawLine(11, 16+blink_y_d, 118, 16+blink_y_s, 0);
  display2.drawLine(12, 17+blink_y_d, 117, 17+blink_y_s, 0);
  display2.drawLine(12, 18+blink_y_d, 117, 18+blink_y_s, 0);
  display2.drawLine(12, 19+blink_y_d, 117, 19+blink_y_s, 0);
  display2.drawLine(12, 20+blink_y_d, 117, 20+blink_y_s, 0);
  display2.display();
}

void setup() {
  Serial.begin(9600);

  // Initialize both displays
  if(!display1.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR_1)) {
    Serial.println(F("SSD1306 allocation failed x3c"));
    for(;;); // Don't proceed, loop forever
  }
  if(!display2.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR_2)) {
    Serial.println(F("SSD1306 allocation failed x3d"));
    for(;;); // Don't proceed, loop forever
  }
  
  // Other setup code here
  blink_y_s = 15; blink_y_d = 15; eyecover = 10;
  image();
}

void loop() {
  // Your loop code here
  // The eyes will blink
  for (int i = 0; i <= 20; ++i) {
    blink_y_s = 5+i; blink_y_d = 5+i; eyecover = 13+i;
  image();
  }
  for (int i = 20; i >= 0; --i) {
    blink_y_s = 5+i; blink_y_d = 5+i; eyecover = 13+i;
  image();
  }
  delay(1000);
}