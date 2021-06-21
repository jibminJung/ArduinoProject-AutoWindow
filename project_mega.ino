/*
 *2021.06.03 
 *먼지센서 회로 구성 및 장착
 *먼지 센싱값 블루투스,와이파이 송수신 양호
 *no_dust값 수정 (이후 계속 수정 필요)
 *먼지 센싱값 변화가 급격하므로, 평균값을 내도록 추가
 *
 */


#define dustPin A1
#define v_led 2


//미세먼지 센서값
float vo_value=0.0;

// 센서값을 전압값으로 전환하여 담을 변수
float sensor_voltage=0.0;

// 실제 미세 먼지 밀도 변수
float dust_density=0.0;

//깨끗한 공기일 때, 전압 값
float no_dust = 0.1;

/*
 * 먼지 값의 평균 도출을 위한 변수
 * 표본 수 10개
 * 값을 담을 배열
 * 인덱스 저장 변수
 * 총합 값 변수
 * 평균 값 변수
 */
const int sample = 10;
float dust_values[sample];
int index = 0;                
float total = 0;
float avg = 0; 

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(v_led,OUTPUT);
  for (int i = 0; i < sample; i++){ //배열 초기화
    dust_values[i] = 0.0;
  }   
}

char x[8];
void loop() {
  if(Serial1.available()) {
    int recieved = Serial1.read();
    if(recieved==0){//nodeMCU로부터 0을 받으면 수위센서 값을 보내줍니다.
      Serial.println("recieved 0, sending rain data");
      Serial1.print("<");
      Serial1.print(map(analogRead(A0),0,1023,0,255));
      Serial1.print(",");
    }else if(recieved ==1){//nodeMCU로부터 1을 받으면 미세먼지 센서 값을 보내줍니다.
      Serial.println("recieved 1, sending dust data");
      dustSensing();
      //미세먼지 값을 보내주는 코드 작성
      avg = getAvg(dust_density);
      Serial1.print(dtostrf(avg,5,2,x));
      Serial1.print(">");
    }
  }
  
  
  
 
}
void dustSensing(){
  // 미세 먼지 센서 동작
  digitalWrite(v_led,LOW); // 적외선 LED ON
  delayMicroseconds(280); // 280us동안 딜레이
  vo_value=analogRead(dustPin); // 데이터를 읽음
  delayMicroseconds(40); // 320us - 280us
  digitalWrite(v_led,HIGH); // 적외선 LED OFF
  delayMicroseconds(9680); // 10ms(주기) -320us(펄스 폭) 한 값
  
  sensor_voltage=vo_value * 5.0 / 1024;
  dust_density=(sensor_voltage-no_dust) / 0.005;
}

float getAvg(float dust_density){
  //먼지 센싱 값 평균 도출하기
  total = total - dust_values[index]; //총합에서 이전 센싱값 제거
  dust_values[index] = dust_density; //새로운 센싱 값 배열에 저장
  total = total + dust_values[index]; //총합에 새로운 센싱값 추가
  index = (index + 1)%sample;  //모듈러 연산으로 인덱스를 다시 원점으로
  avg = total/sample; //평균=총합/표본수

  return avg;
}
