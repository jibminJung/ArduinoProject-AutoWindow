#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

#include <HardwareSerial.h>
HardwareSerial esp(2);

#include <LCD_I2C.h>
LCD_I2C lcd(0x27);

#include <Stepper.h>
const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
Stepper myStepper(stepsPerRevolution, 4, 2, 0, 15); //in4, in2, in3, in1

String name = "HC-06";
char *pin = "1234";
bool connected;
unsigned long time_previous, time_current;

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars]; 

boolean newData = false;

void setup() {
  Serial.begin(115200);
  esp.begin(115200);
  
  myStepper.setSpeed(14);
  
  lcd.begin();
  lcd.backlight();

  SerialBT.begin("ESP32test", true); 
  Serial.println(F("The device started in master mode, make sure remote BT device is on!"));
  lcd.print("starting");
  connected = SerialBT.connect(name);
  
  if(connected) {
    Serial.println(F("Connected Succesfully!"));
  } else {
    while(!SerialBT.connected(10000)) {
      Serial.println(F("Failed to connect. Make sure remote device is available and in range, then restart app.")); 
    }
  }
  updateLCD();
  time_previous = millis();
}

//센서 값을 저장할 변수
int rain = 0;
float dust = 0.0;

//현재 상태를 저장할 변수. 각각 자/수동모드와 창문 개/폐상태
boolean autoMode = false;
boolean windowOpen = false;

const int SHORT_PRESS_TIME = 300; //터치버튼을 위한 짧은 시간 기준
const int LONG_PRESS_TIME  = 3000; //터치버튼을 위한 긴 시간 기준

//터치 버튼을 위한 변수들
boolean btnPrevState = false;
boolean btnNowState;
unsigned long btnPressedTime, btnReleasedTime;
boolean isPressing = false;
boolean isLongDetected = false;

char x[8];
void loop() {
  recvWithStartEndMarkers();
  if (newData == true) {
      strcpy(tempChars, receivedChars);
          // this temporary copy is necessary to protect the original data
          //   because strtok() used in parseData() replaces the commas with \0
      parseData();
      newData = false;
      updateLCD();
  }
  if(millis()-time_previous>1000){//1초에 한번씩 정보를 요청합니다.
    SerialBT.write(0);
    SerialBT.write(1);

    time_previous = millis();
  }
  
  if(esp.available()){//esp로부터 신호가 오면, 현재 상태 정보들을 보내줍니다.
    Serial.println("recieved data from esp");
    esp.read();
    esp.print("<");
    esp.print(rain);
    esp.print(",");
    esp.print(dtostrf(dust,5,2,x));
    esp.print(",");
    esp.print(windowOpen?1:0);
    esp.print(">");
  }
  

  if(autoMode){//오토모드인 경우
    if(windowOpen){//창문이 열려있는 상태의 경우
      if(rain>50 || dust>70){//비가 오면 창문을 닫습니다.
        closeWindow();
        updateLCD();
      }
    }else{//창문이 닫힌 상태의 경우
      if(rain<50 && dust<70){//비가 오지 않으면서 먼지도 없는 경우 창문을 엽니다.
        openWindow();
        updateLCD();
      }
    }
  }

  //버튼 조작부
  btnNowState = (touchRead(T9)<20)?true:false; //터치버튼 핀의 값을 읽어옵니다. 20이하면 터치중인 것으로 간주하고 true, 이상이면 false
  if(btnNowState && !btnPrevState){//버튼이 이전에 눌리지 않았고, 지금 눌림
    btnPressedTime = millis();
    isPressing = true;
    isLongDetected = false;
  }else if(btnPrevState && !btnNowState) { // 버튼이 눌려있다가, 떨어짐
    isPressing = false;
    btnReleasedTime = millis();
    long pressDuration = btnReleasedTime - btnPressedTime;
    if( pressDuration > SHORT_PRESS_TIME && pressDuration<LONG_PRESS_TIME){//눌렀던 시간이 short시간보다 길고, long시간 보다 짧으면 짧은 버튼 터치로 인식
      if(autoMode){//auto모드인 경우
        lcd.clear();
        lcd.print("it is an auto mode");
        delay(1000);
      }else{//수동모드인 경우
        if(windowOpen){//창문이 열려있는 경우, 닫는다.
          closeWindow();
          updateLCD();
        }else{//창문이 닫혀있는 경우, 연다.
          openWindow();
          updateLCD();
        }
      }
      
      Serial.println("A short press is detected");
    }
      
  }
  if(isPressing == true && isLongDetected == false) {
    long pressDuration = millis() - btnPressedTime;

    if( pressDuration > LONG_PRESS_TIME ) {//버튼 길게 누른 것이 확인된 경우, auto/manual모드 전환
      isLongDetected = true;
      autoMode = !autoMode;
      updateLCD();
      Serial.println("A long press is detected");
    }
  }
  btnPrevState = btnNowState;
  
  
}
void openWindow(){//창문열기
  lcd.clear();
  lcd.print("opening window...");
  myStepper.step(-stepsPerRevolution*6.8);
  windowOpen = true;
}
void closeWindow(){//창문닫기
  lcd.clear();
  lcd.print("closing window...");
  myStepper.step(stepsPerRevolution*6.8);
  windowOpen = false;
}
void updateLCD(){//화면 갱신
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Rain ");
  lcd.print(rain);
  lcd.setCursor(9,0);
  lcd.print(autoMode?"   AUTO":" MANUAL");
  lcd.setCursor(0, 1);
  lcd.print("Dust");
  lcd.setCursor(4,1);
  lcd.print(dust);
  lcd.setCursor(11, 1);
  lcd.print(windowOpen?" Open":"Close");
}
void recvWithStartEndMarkers() {//<데이터1,데이터2..> 형태의 정보를 받기 위한 메소드
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (SerialBT.available() > 0 && newData == false) {
        rc = SerialBT.read();

        if (recvInProgress == true) {//시작문자를 받고, 데이터 수신이 진행 중이면
            if (rc != endMarker) {//수신한 데이터가 종료문자가 아니면, 배열에 추가
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {//수신한 데이터가 종료문자라면, 종료 표시 추가하고, 수신 중 상태 해제, 인덱스 초기화, 새로운 데이터의 생성을 표시
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {//수신한 데이터가 시작문자인 경우, 수신 중 상태 적용
            recvInProgress = true;
        }
    }
}
void parseData() {      //수신한 데이터를 파싱합니다.

    char * strtokIndx; // 인덱싱을 위한 변수

    strtokIndx = strtok(tempChars,",");      //배열을 ,로 나누는 인덱스
    rain = atoi(strtokIndx);
 
    strtokIndx = strtok(NULL, ","); //이전 종료지점부터
    dust = atof(strtokIndx);

}
