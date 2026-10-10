#include <Adafruit_MCP3008.h>
Adafruit_MCP3008 adc;
#define NUM_SENSORS 8
uint8_t sck = 13;
uint8_t miso = 12;
uint8_t mosi = 11;
uint8_t ssf = 10;
uint8_t ssb = 9;
int minValueF[NUM_SENSORS] = {158, 62, 51, 73, 41, 52, 44, 119};
int maxValueF[NUM_SENSORS] = {931, 913, 904, 918, 886, 897, 893, 918};

uint8_t F_PIN[NUM_SENSORS] = {0, 1, 2, 3, 4, 5, 6, 7};
int F[NUM_SENSORS];

int Error_F = 0, LastError_F;

int trigpin = 10;
int echopin = 13;

long duration, distance;
int Dist;

////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////Turn Function//////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
void TurnL() { //Turn Left
  MotorStop();
  Motor(-120, 180); delay(50);
  while (1) {
    readCalibrateF();
    Motor(-120, 180);
    if (F[2] >= 501) {
      Motor(180,-120);delay(50);
      MotorStop();
      break;
    }
  }
}

void TurnR() { //Turn Right
  MotorStop();
  Motor(180, -120); delay(50);
  while (1) {
    readCalibrateF();
    Motor(180, -120);
    if (F[6] >= 501) {
      Motor(-120,180);delay(50);
      MotorStop();
      break;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

void RobotSetup() { //Robot Setup
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
  for (int i = 2; i <= 8; i++) {
    pinMode(i, OUTPUT);
  }
  pinMode(13, INPUT);
}


////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////Sensor Function/////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
void readPrivateF() { //Raw sensor value
  for (int _sensor = 0; _sensor < NUM_SENSORS; _sensor++) {
    digitalWrite(ssf, HIGH);
    adc.begin(sck, mosi, miso, ssb);
    F[_sensor] = adc.readADC(F_PIN[_sensor]);
  }
}

void caribrateSensorF(int pauseTime, int samples) { //Calibrate
  for (int i = 0; i < NUM_SENSORS; i++) {
    minValueF[i] = 1023;
    maxValueF[i] = 0;
  }
  for (int startSamp = 0;  startSamp <= samples; startSamp++) {
    readPrivateF();
    for (int i = 0; i < NUM_SENSORS; i++) {
      minValueF[i] = (F[i] <= minValueF[i] ? F[i] : minValueF[i]);
      maxValueF[i] = (F[i] >= maxValueF[i] ? F[i] : maxValueF[i]);
    }
    delay(pauseTime);
  }
}

void displaySensor() { //Use to calibrate Sensor
  Serial.println("Press OK to start caribrate Front Sensor");
  OK();
  Serial.println("Caribrating");
  Beep(700);
  caribrateSensorF(20, 250);
  Beep(700);
  Serial.println("Finish");
  Serial.println("  ");
  Serial.print("int minValueF[NUM_SENSORS] = {");
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(minValueF[i]);
    if ( i < NUM_SENSORS - 1) Serial.print(",");
    else Serial.println("};");
  }

  Serial.print("int maxValueF[NUM_SENSORS] = {");
  for (int i = 0; i < NUM_SENSORS; i++) {
    Serial.print(maxValueF[i]);
    if ( i < NUM_SENSORS - 1) Serial.print(",");
    else Serial.println("};");
  }
  while (1);
}

void readCalibrateF() { //Use to Read the Calibrated sensor value >>  F[0] -> F[7]
  readPrivateF();
  for (int i = 0; i < NUM_SENSORS; i++) {
    unsigned int calmin, calmax, rangeValue;
    long x = 0;
    calmin = minValueF[i];
    calmax = maxValueF[i];
    x = map(F[i], calmin, calmax, 0, 1000);
    if (x < 0) x = 0;
    if (x > 1000) x = 1000;
    F[i] = x;
  }
}

////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////Distance Sensor///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

long data() {
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  digitalWrite(trigpin, LOW);
  delayMicroseconds(8);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(8);
  digitalWrite(trigpin, LOW);
  duration = pulseIn (echopin, HIGH);
  distance = duration / 29 / 2;
  return distance;
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////

void OK() {
  while (digitalRead(13) == 0) {

  }
}

int OK_PUSH() {
  if (digitalRead(13) == 0)return 0;
  else {
    return 1;
  }
}
void BEEP(int beepC, int beeptime, int beepsleep) {
  for (int beepcounter = 1; beepcounter <= beepC; beepcounter++) {
    digitalWrite(8, 1);
    delay(beeptime);
    digitalWrite(8, 0);
    delay(beepsleep);
  }
}

void Beep(int delayb) {
  digitalWrite(8, 1);
  delay(delayb);
  digitalWrite(8, 0);
  delay(10);
}

////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////PID FUNCTION////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


void stdPID(int baseSpeed, int Kp, int Kd) { //Pure Standard PID
  CalErrorF();
  int PIDValue = (Kp * Error_F) + (Kd * (Error_F - LastError_F));
  LastError_F  = Error_F;
  int leftPower = baseSpeed - PIDValue;
  int rightPower = baseSpeed + PIDValue;
  if ( leftPower > 255 ) leftPower = 255;
  if ( leftPower < -100 ) leftPower = -100;
  if (rightPower > 255) rightPower = 255;
  if (rightPower < -100 ) rightPower =  -100;
  Motor(leftPower, rightPower);
}

void TimerPID(int baseSpeed, int Kp, int Kd, int totalTime) { //Pid Timer
  unsigned long startTime  = millis();
  unsigned long endTime = startTime + totalTime;
  while (millis() <= endTime) {
    stdPID(baseSpeed, Kp , Kd);
  }
}

void fline(int baseSpeed, int Kp, int Kd, char select, char tag) { //Pid Line Follower
  if (tag == 'l') {
    while (1) {
      stdPID(baseSpeed, Kp, Kd);
      readCalibrateF();
      if (
        (F[0] >= 501 && F[2] >= 501)
      ) {
        break;
      }
    }
  } else if (tag == 'r') {
    while (1) {
      stdPID(baseSpeed, Kp, Kd);
      readCalibrateF();
      if (
        (F[5] >= 501 && F[7] >= 501)
      ) {
        break;
      }
    }
  } else {
    while (1) {
      stdPID(baseSpeed, Kp, Kd);
      readCalibrateF();
      if (
        (F[2] >= 501 && F[5] >= 501) || (F[0] >= 501 && F[2] >= 501) || (F[5] >= 501 && F[7] >= 501)
      ) {
        break;
      }
    }
  }
  if (select == 's') {
    MotorStop();
  } else if (select == 'l') {
    TurnL();
  } else if (select == 'r') {
    TurnR();
  } else if (select == 'p') {
    Motor(baseSpeed, baseSpeed); delay(40);
  }
}

void flineSen(int baseSpeed, int Kp, int Kd, char select, int SensorNumber) { //Pid Line Follower
  if (SensorNumber == 0) {
    while (1) {
      stdPID(baseSpeed, Kp, Kd);
      readCalibrateF();
      if (F[0] >= 501) {
        break;
      }
    }
  } else if (SensorNumber == 1) {
    while (1) {
      stdPID(baseSpeed, Kp, Kd);
      readCalibrateF();
      if (F[1] >= 501) {
        break;
      }
    }
  }else if (SensorNumber == 2) {
    while (1) {
      stdPID(baseSpeed, Kp, Kd);
      readCalibrateF();
      if (F[2] >= 501) {
        break;
      }
    }
  }else if (SensorNumber == 3) {
    while (1) {
      stdPID(baseSpeed, Kp, Kd);
      readCalibrateF();
      if (F[3] >= 501) {
        break;
      }
    }
  }else if (SensorNumber == 4) {
    while (1) {
      stdPID(baseSpeed, Kp, Kd);
      readCalibrateF();
      if (F[4] >= 501) {
        break;
      }
    }
  }else if (SensorNumber == 5) {
    while (1) {
      stdPID(baseSpeed, Kp, Kd);
      readCalibrateF();
      if (F[5] >= 501) {
        break;
      }
    }
  }else if (SensorNumber == 6) {
    while (1) {
      stdPID(baseSpeed, Kp, Kd);
      readCalibrateF();
      if (F[6] >= 501) {
        break;
      }
    }
  }else if (SensorNumber == 7) {
    while (1) {
      stdPID(baseSpeed, Kp, Kd);
      readCalibrateF();
      if (F[7] >= 501) {
        break;
      }
    }
  }else {
    while (1) {
      stdPID(baseSpeed, Kp, Kd);
      readCalibrateF();
      if (
        (F[2] >= 501 && F[5] >= 501) || (F[0] >= 501 && F[2] >= 501) || (F[5] >= 501 && F[7] >= 501)
      ) {
        break;
      }
    }
  }
  if (select == 's') {
    MotorStop();
  } else if (select == 'l') {
    TurnL();
  } else if (select == 'r') {
    TurnR();
  } else if (select == 'p') {
    Motor(baseSpeed, baseSpeed); delay(40);
  }
}

void flineOb(int baseSpeed, int Kp, int Kd, int dis, char select) { //Pid Object
  while (1) {
    stdPID(baseSpeed, Kp, Kd);
    readCalibrateF();
    Dist = data();
    if (
      Dist >= 3 && Dist <= dis
    ) {
      MotorStop();
      break;
    }
  }
  if (select == 'r') { // Avoid to right
    MotorStop(); delay(10);
    Motor(120, -80);
    delay(140);
    MotorStop(); delay(10);
    Motor(50, 200);
    delay(220);
    while (1) {
      Motor(60, 200);
      readCalibrateF();
      if (F[2] >= 501 || F[5] >= 501) {
        Motor(80, 80); delay(25);
        MotorStop();
        break;
      }
    }
    TurnR();
    //Motor(150, -150); delay(150);
   /*while (1) {
      readCalibrateF();
      Motor(150, -150);
      if (F[5] >= 501) {
        Motor(-150, 150); delay(20);
        MotorStop();
        break;
      }
    }*/
     MotorStop(); delay(10);
  }

  else if (select == 'l') { // Avoid to Left
    MotorStop(); delay(10);
    Motor(-80, 120);
    delay(150);
    MotorStop(); delay(10);
    Motor(250, 60);
    delay(220);
    //  while (1) {
    //    Motor(250, 20);
    //    readCalibrateF();
    //    if (F[2] >= 501 || F[5] >= 501) {
    //        Motor(80, 80); delay(30);
    //        MotorStop();
    //        break;
    //    }
    //    }
    //    TurnL();
    //    MotorStop(); delay(10);
      }
}
void AvoidRight(){
MotorStop(); delay(10);
    Motor(120, -80);
    delay(140);
    MotorStop(); delay(10);
    Motor(50, 200);
    delay(250);
    while (1) {
      Motor(60, 200);
      readCalibrateF();
      if (F[2] >= 501 || F[5] >= 501) {
        Motor(80, 80); delay(25);
        MotorStop();
        break;
      }
    }
 TurnR();
    
     MotorStop(); delay(10);





}
void AvoidLeft(){
    MotorStop(); delay(10);
    Motor(-80, 120);
    delay(130);
    MotorStop(); delay(10);
    Motor(250, 60);
    delay(250);
    while (1) {
       Motor(250, 20);
       readCalibrateF();
       if (F[2] >= 501 || F[5] >= 501) {
           Motor(80, 80); delay(30);
           MotorStop();
           break;
       }
       }
       TurnL();
       MotorStop(); delay(10);

}
void CalErrorF() { //Calculate PD Error value
  readCalibrateF();
  if (F[0] >= 501 && F[1] <= 501 && F[2] <= 501 && F[3] <= 501 && F[4] <= 501 && F[5] <= 501 && F[6] <= 501 && F[7] <= 501)Error_F = 7;
  else if (F[0] >= 501 && F[1] >= 501 && F[2] <= 501 && F[3] <= 501 && F[4] <= 501 && F[5] <= 501 && F[6] <= 501 && F[7] <= 501)Error_F = 6;
  else if (F[0] <= 501 && F[1] >= 501 && F[2] <= 501 && F[3] <= 501 && F[4] <= 501 && F[5] <= 501 && F[6] <= 501 && F[7] <= 501)Error_F = 5;
  else if (F[0] <= 501 && F[1] >= 501 && F[2] >= 501 && F[3] <= 501 && F[4] <= 501 && F[5] <= 501 && F[6] <= 501 && F[7] <= 501)Error_F = 4;
  else if (F[0] <= 501 && F[1] <= 501 && F[2] >= 501 && F[3] <= 501 && F[4] <= 501 && F[5] <= 501 && F[6] <= 501 && F[7] <= 501)Error_F = 3;
  else if (F[0] <= 501 && F[1] <= 501 && F[2] >= 501 && F[3] >= 501 && F[4] <= 501 && F[5] <= 501 && F[6] <= 501 && F[7] <= 501)Error_F = 2;
  else if (F[0] <= 501 && F[1] <= 501 && F[2] <= 501 && F[3] >= 501 && F[4] <= 501 && F[5] <= 501 && F[6] <= 501 && F[7] <= 501)Error_F = 1;
  else if (F[0] <= 501 && F[1] <= 501 && F[2] <= 501 && F[3] >= 501 && F[4] >= 501 && F[5] <= 501 && F[6] <= 501 && F[7] <= 501)Error_F = 0;
  else if (F[0] <= 501 && F[1] <= 501 && F[2] <= 501 && F[3] <= 501 && F[4] >= 501 && F[5] <= 501 && F[6] <= 501 && F[7] <= 501)Error_F = -1;
  else if (F[0] <= 501 && F[1] <= 501 && F[2] <= 501 && F[3] <= 501 && F[4] >= 501 && F[5] >= 501 && F[6] <= 501 && F[7] <= 501)Error_F = -2;
  else if (F[0] <= 501 && F[1] <= 501 && F[2] <= 501 && F[3] <= 501 && F[4] <= 501 && F[5] >= 501 && F[6] <= 501 && F[7] <= 501)Error_F = -3;
  else if (F[0] <= 501 && F[1] <= 501 && F[2] <= 501 && F[3] <= 501 && F[4] <= 501 && F[5] >= 501 && F[6] >= 501 && F[7] <= 501)Error_F = -4;
  else if (F[0] <= 501 && F[1] <= 501 && F[2] <= 501 && F[3] <= 501 && F[4] <= 501 && F[5] <= 501 && F[6] >= 501 && F[7] <= 501)Error_F = -5;
  else if (F[0] <= 501 && F[1] <= 501 && F[2] <= 501 && F[3] <= 501 && F[4] <= 501 && F[5] <= 501 && F[6] >= 501 && F[7] >= 501)Error_F = -6;
  else if (F[0] <= 501 && F[1] <= 501 && F[2] <= 501 && F[3] <= 501 && F[4] <= 501 && F[5] <= 501 && F[6] <= 501 && F[7] >= 501)Error_F = -7;
  else if (F[0] <= 501 && F[1] <= 501 && F[2] <= 501 && F[3] <= 501 && F[4] <= 501 && F[5] <= 501 && F[6] <= 501 && F[7] <= 501)Error_F = 0;//LastError_F;
}
