#include <QTRSensors.h>
#define initialSpeed 100
#define Kp 
#define Kd   

QTRSensors qtr;

int rightMotor;
int leftMotor;
bool flag=1;
char* path;
int pathLen=0;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];
int leftValue=0;
int rightValue=0;
int midVal=0;
int action;                                                 
uint16_t position;

void setup()
{
  // configure the sensors
  qtr.setTypeRC();
  qtr.setSensorPins((const uint8_t[]){}, SensorCount);      //Pins to be added.
  qtr.setEmitterPin(2);

  delay(500);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); // turn on Arduino's LED to indicate we are in calibration mode

  // 2.5 ms RC read timeout (default) * 10 reads per calibrate() call
  // = ~25 ms per calibrate() call.
  // Call calibrate() 400 times to make calibration take about 10 seconds.
  for (uint16_t i = 0; i < 400; i++)
  {
    qtr.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW); // turn off Arduino's LED to indicate we are through with calibration
  
  
  delay(1000);
}

void loop()
{
  // read calibrated sensor values and obtain a measure of the line position
  // from 0 to 5000 (for a white line, use readLineWhite() instead)
  dryRun(); 
  actualRun(); 
}
void dryRun(){
  while(flag){  
    switch(readSensor()){
      case 0:
        turnBack();
        pathStore('B');
        break;
      case 1:
        turnLeft();
        //pathStore('L');
        break;
      case 2
        turnRight();
        break;
      case 3:
        turnLeft();
        pathStore('L');
        break;
      case 4:
        turnLeft();
        pathStore('L');
        break;
      case 5:
        moveInchFor();
        break;
      case 6:
        moveInchFor();
        Stop();
        flag=0;
        break;
      case 7:
        moveInchFor();
        pathStore('S');
        break;
      case 8:
        turnLeft();
        pathStore('L');
        break;
    }
  }
}
int readSensor(){
  moveInchFor();
  leftVal=0;
  rightVal=0;
  for(int i=0;i<SensorCount;++i){
    sensorValues[i]=map(sensorValues[i], 0, 1023, 0, 1);
  }
  midVal=sensorValues[SensorCount/2] || sensorValues[SensorCount/2+1];
  for(int i=0;i<SensorCount/2-1;++i){
    leftVal=leftVal || sessorValues[i];
    rightVal= rightVal || sensorValues[SensorCount-i];
  }
  if(leftVal==0 && rightVal==0 && midVal==0){                                  //Dead-End
    action=0;
  }
  if(leftVal==1 && rightVal==0 && readSensor()==0){                            //Left Only
    action=1;
  }
  if(leftVal==1 && rightVal==0 && readSensor()==5){                            //Left and Straight
    action=8;
  }
  if(leftVal==0 && rightVal==1 && readSensor()==0){                           //Right Only
    action=2;
  }
  if(leftVal==0 && rightVal==1 && readSensor()==5){                           //Right and Straight 
    action=7;
  }
  if(leftVal==0 && rightVal==0 && midVal==1){                                //Straight
    action=5;
  }
  if(leftVal==1 && rightVal==1 && midVal==1 && readSensor()==0){            //T
    action=3;
  }else
  if(leftVal==1 && rightVal==1 && midVal==1 && readSensor()==5){            //cross
    action=4;
  }
  else
    if(leftVal==1 && rightVal==1 && (readSensor()==5))                      //END
      acion=6;                                                              
  return action;
}

void PIDcontrol(){
  int motorSpeed= Kp*error + Kd*(Error-lastError);
  lastError = error;
  leftMotor=initialSpeed+motorSpeed;
  rightMotor=initalSpeed-motorSpeed;
  constrain(leftMotor,0,255);
  constrain(rightMotor,0,255);
}

void pathStore(char x){
  path[pathLen++]= x;
  if(pathLen < 3 || path[pathLen-2] != 'B')
    return;

  int totalAngle = 0;
  int i;
  for(i=0;i<3;i++)
  {
    switch(path[pathLen-i])
    {
      case 'R':
        totalAngle += 90;
        break;
      case 'L':
        totalAngle += 270;
        break;
      case 'B':
        totalAngle += 180;
        break;
    }
  }
  totalAngle = totalAngle % 360;
  switch(totalAngle)
  {
    case 0:
      path[pathLen - 3] = 'S';
      break;
    case 90:
      path[pathLen - 3] = 'R';
      break;
    case 180:
      path[pathLen - 3] = 'B';
      break;
    case 270:
      path[pathLen - 3] = 'L';
      break;
  }
  pathLen -= 2;
}
void actualRun(){
  int var=0;
  flag=1;
  while(flag){  
    switch(readSensor()){
      case 1:
        turnLeft();
        //pathStore('L');
        break;
      case 2:
        turnRight();
        break;
      case 3:
      case 4:
      case 7:
      case 8:
         switch(path[var++]){
          case 'R':
            turnRight();
            break;
          case 'L':
            turnleft();
            break;
          case 'B':
            turnBack();
            break;
          case 'S':
            moveInchfor();
            break;
        }
        break;
      case 5:
        goStraight();
        break;
      case 6:
        moveInchFor();
        Stop();
        flag=0;
        break;
    }
  }
}
void moveInchFor(){
  position = qtr.readLineWhite(sensorValues);
  error= position- 2500;
  PIDcontrol(); 
}
