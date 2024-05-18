#include <Wire.h>
#include <Servo.h>

// Setting motor pins on arduino
#define M1A 2
//#define PWM1 3 Not needed at this moment
#define M1B 4
#define M2A 5
//#define PWM2 6 Not needed at this moment
#define M2B 7

// Setting the I2C address of the arduino
#define ADDRESS 8

// Setting pins for the Servo motors
#define SERVO_Y_PIN 9
#define SERVO_Z_PIN 10

Servo servo_y; /*Creating the servo_y object representing the tilt servo*/
Servo servo_z; /*Creating the servo_z object representing the rotation servo*/

int poz_y = 90;
int poz_z = 90;

// Setting positions for the camera
void CAM_Center() {
  poz_y = 90;
  poz_z = 90;

  servo_y.write(poz_y);
  servo_z.write(poz_z);
}

void CAM_Up() {
  poz_y = 130;

  for (int i = 90; i <= poz_y; i = i + 10) {
    servo_y.write(i);
  }
}

void CAM_Down() {
  poz_y = 70;

  for (int i = 90; i >= poz_y; i = i - 10) {
    servo_y.write(i);
  }
}

void CAM_Left() {
  poz_z = 45;

  for (int i = 90; i >= poz_z; i = i - 15) {
    servo_z.write(i);
  }
}

void CAM_Right() {
  poz_z = 135;

  for (int i = 90; i <= poz_z; i = i + 15) {
    servo_z.write(i);
  }
}

// Setting the movement of the robot
void Backward() {
  digitalWrite(M1A, HIGH);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, HIGH);
  digitalWrite(M2B, LOW);
}

void Forward() {
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, HIGH);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, HIGH);
}

void Right() {
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, HIGH);
  digitalWrite(M2A, HIGH);
  digitalWrite(M2B, LOW);
}

void Left() {
  digitalWrite(M1A, HIGH);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, HIGH);
}

void Brake() {
  digitalWrite(M1A, LOW);
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);
  digitalWrite(M2B, LOW);
}

// function that executes whenever data is received from master
void receiveEvent(int howMany) {
  while (Wire.available()) {
    char command[2]; // Assuming commands won't exceed 2 characters
    int i = 0;
    while (Wire.available()) {
      char c = Wire.read();
      if (c == '\n') { // End of command reached
        command[i] = '\0'; // Null-terminate the command string
        break;
      }
      command[i++] = c;
    }
    Serial.println(command);
    // Perform actions based on the received command
    //ENGINE CONTROL
    if (strncmp(command, "F",1) == 0) {
      // Perform action for FORWARD
      Forward();
      Serial.println("Received FORWARD command");
    } else if (strncmp(command, "B",1) == 0) {
      // Perform action for BACKWARD
      Backward();
      Serial.println("Received BACKWARD command");
    } else if (strncmp(command, "L",1) == 0) {
      // Perform action for LEFT
      Left();
      Serial.println("Received LEFT command");
    } else if (strncmp(command, "R",1) == 0) {
      // Perform action for RIGHT
      Right();
      Serial.println("Received RIGHT command");
    } else if (strncmp(command, "P",1) == 0) {
      // Perform action for BRAKE
      Brake();
      Serial.println("Received BRAKE command");
    } //CAMERA CONTROL
    else if (strncmp(command, "W",1) == 0) {
      // Perform action for CAM UP
      CAM_Up();
      Serial.println("Received CAM UP command");
    } else if (strncmp(command, "S",1) == 0) {
      // Perform action for CAM DOWN
      CAM_Down();
      Serial.println("Received CAM DOWN command");
    } else if (strncmp(command, "D",1) == 0) {
      // Perform action for CAM RIGHT
      CAM_Right();
      Serial.println("Received CAM RIGHT command");
    } else if (strncmp(command, "A",1) == 0) {
      // Perform action for CAM LEFT
      CAM_Left();
      Serial.println("Received CAM LEFT command");
    } else if (strncmp(command, "C",1) == 0) {
      // Perform action for CAM CENTER
      CAM_Center();
      Serial.println("Received CAM CENTER command");
    } else {
      // Unknown command
      Serial.println("Received unknown command");
    }
  }
}

// function that executes whenever data is requested from master
void requestEvent() {
 Wire.write("Hello Master");  /*send string on request */
}

void setup() {
  // put your setup code here, to run once:
  // Attach the servos to Arduino
  servo_y.attach(SERVO_Y_PIN);
  servo_z.attach(SERVO_Z_PIN);

  //Reset Camera position
  CAM_Center();

  //Pin Manager
  pinMode(M1A, OUTPUT);
  pinMode(M1B, OUTPUT);
  pinMode(M2A, OUTPUT);
  pinMode(M2B, OUTPUT);
  //pinMode(PWM1, OUTPUT); Not needed at this moment
  //pinMode(PWM2, OUTPUT); Not needed at this moment
  
  Wire.begin(ADDRESS);                /* join i2c bus with address 8 */
  Wire.onReceive(receiveEvent); /* register receive event */
  Wire.onRequest(requestEvent); /* register request event */
  Serial.begin(9600);           /* start serial for debug */
}

void loop() {
  // put your main code here, to run repeatedly:

}
