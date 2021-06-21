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

***
## Arduino circuit - sensors
<img src="https://user-images.githubusercontent.com/71082601/122698363-2566af00-d282-11eb-9c1a-8e000ca2fa6c.png" width="80%">

## Node32s circuit - window control
<img src="https://user-images.githubusercontent.com/71082601/122698399-39121580-d282-11eb-9b33-724dc78f2abb.png" width="80%">

***
## Sample Images

<img src="https://user-images.githubusercontent.com/71082601/122698649-ade54f80-d282-11eb-8e87-9c0b2137480c.jpg" width="60%">
<img src="https://user-images.githubusercontent.com/71082601/122698671-b76eb780-d282-11eb-8075-f15f265f95ba.jpg" width="60%">
<img src="https://user-images.githubusercontent.com/71082601/122698676-be95c580-d282-11eb-84aa-356de9a880c6.jpg" width="60%">

***
## Sample Video

[Video](https://dongguk0-my.sharepoint.com/personal/wlqals1_dongguk_edu/_layouts/15/onedrive.aspx?id=%2Fpersonal%2Fwlqals1%5Fdongguk%5Fedu%2FDocuments%2F%EA%B7%B8%EB%A6%BC%2F%EC%B9%B4%EB%A9%94%EB%9D%BC%20%EC%95%A8%EB%B2%94%2F2021%2FHwVideoEditor%5F2021%5F06%5F11%5F180427567%2Emp4&parent=%2Fpersonal%2Fwlqals1%5Fdongguk%5Fedu%2FDocuments%2F%EA%B7%B8%EB%A6%BC%2F%EC%B9%B4%EB%A9%94%EB%9D%BC%20%EC%95%A8%EB%B2%94%2F2021&originalPath=aHR0cHM6Ly9kb25nZ3VrMC1teS5zaGFyZXBvaW50LmNvbS86djovZy9wZXJzb25hbC93bHFhbHMxX2RvbmdndWtfZWR1L0ViTmtVR0k5bmpwQ3ZkTklrUTBCeUpjQmhLZFFBTnpBT21zVUd4WlZ2RjlTRlE%5FcnRpbWU9dTRnVzIxdzAyVWc, "vid link")

