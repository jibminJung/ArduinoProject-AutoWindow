# ArduinoProject-AutoWindow
2021 arduino project

Window Open/Close controller with Fine dust sensor and water sensor

Project is developed in Arduino IDE, c++

Used-hardware : Arduino Mega 2560, Node32s, HC-06, ESP-01,Sharp GP2Y1014AU0F (Dust Sensor), Water Level Sensor, 28BYJ-48 (Step Motor)

*********************************
## Project is devided into 2 parts.

Sensor Device and Window Control Device. 

Sensor Device collect information of dust density of the air and value from water sensor. 
Then it will send the information to Window Control Device through bluetooth connection.

Window Control Device recieve the data and it opens/closes window according to the data. 
If dust density level is high, close window. If dust level is low, open window. if rain is detected by water sensor, close window.
Window Control Device have touch button for auto/manual mode transition. It also have esp module to run a webserver, so user can check current status from browser.



