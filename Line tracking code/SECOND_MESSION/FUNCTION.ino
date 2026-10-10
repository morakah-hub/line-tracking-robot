void Line_Right(int spd,char crossdetect){
  fline(spd, 20, 150, 'r', crossdetect);
}
void Line_Left(int spd,char crossdetect){
  fline(spd, 20, 150, 'l', crossdetect);
}
void Line_Forward(int spd,char crossdetect){
  fline(spd, 20, 150, 'p', crossdetect);
}
void Line_Stop(int spd,char crossdetect){
  fline(spd, 20, 150, 's', crossdetect);
}

void Line_Object(int spd,int range,char select){
  flineOb(spd, 20, 150, range, select);
}
void Line_Timer(int spd,int tim){
  TimerPID(spd, 20, 150, tim);
}

void LineSen_Right(int spd,int NumSensor){
  flineSen(spd, 35, 150, 'r', NumSensor);
}
void LineSen_Left(int spd,int NumSensor){
  flineSen(spd, 35, 150, 'l', NumSensor);
}

void Forward(int spd){
  Motor(spd,spd);
}
void Backward(int spd){
  Motor(-spd,-spd);
}
void Left(int spd){
  Motor(-spd,spd);
}
void Right(int spd){
  Motor(spd,-spd);
}
void Stop(int spd){
  Motor(0,0);
}
