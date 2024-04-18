# ESP8266-MASTER

This is the brain of the robot.
From here the signals are delivered to the motors and to the horn.

Also, it runs as AccessPoint and connects via WiFi to the Android Device with the SAR-24 App installed.

### Connections

| ESP8266 | L298N | BUZZER |
|:-------:|:-----:|:------:|
|   GND   |  GND  |   GND  |
|  GPIO2  |  IN3  |    -   |
|  GPIO4  |  IN2  |    -   |
|  GPIO5  |  IN1  |    -   |
|  GPIO12 |   -   |   3V   |
|  GPIO14 |  IN4  |    -   |
