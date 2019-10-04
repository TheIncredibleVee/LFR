


#define leftFarSensor A0
#define leftNearSensor A1
#define rightFarSensor A4
#define rightNearSensor A3
#define midSensor A2
#define leftMotor1 11
#define leftmotor2  12
#define rightmotor1 3
#define rightMotor2 4
#define rightMotorPWM 5
#define leftMotorPWM 10
#define pushButton                          //Add PIN

int rightFarVal;
int rightNearVal;
int leftFarVal;
int leftNearVal;
int midVal;
int val;
boolean push=0;
char* path;
int pathLen=0;

void setup(){
    pinMode(leftFarSensor, INPUT);
    pinMode(leftNearSensor, INPUT);
    pinMode(rightFarSensor, INPUT);
    pinMode(rightNearSensor, INPUT);
    pinMode(midSensor, INPUT);
    pinMode(pushButton, INPUT);

    Serial.begin(9600);

    pinMode(leftMotor1, OUTPUT);
    pinMode(leftMotorPWM, OUTPUT)
    pinMode(rightmotor1, OUTPUT);
    pinMode(leftmotor2, OUTPUT);
    pinMode(rightMotor2, OUTPUT);
}

void loop(){
    dryRun(); 
    if(digitalRead(pushButton))
        actualRun();
    else   
        Serial.println("Wating for Response");
}

int readSensors(){
    //Serial.println("Reading values");
    leftFarVal=digitalRead(leftFarSensor);
    leftNearVal=digitalRead(leftNearSensor);
    rightNearVal=digitalRead(rightNearSensor);
    rightFarVal=digitalRead(rightFarSensor);
    midVal=digitalRead(midSensor);
    Serial.print(leftFarVal);
    Serial.print('\t');
    Serial.print(leftNearVal);
    Serial.print('\t');
    Serial.print(midVal);
    Serial.print('\t');
    Serial.print(rightNearVal);
    Serial.print('\t');
    Serial.print(rightFarVal);
    Serial.print('\t');
    Serial.println();
    val=rightFarVal+(2*rightNearVal)+(4*midVal)+(8*leftNearVal)+(16*leftFarVal);

    switch(val){
            case 0:
            case 1:
            case 2:
            case 8:
            case 9:
            case 10:
            case 16:
            case 17:
            case 18:
                action=4;
                break;

            case 3:
            case 11:
            case 19:
                action=8;
                break;

            case 4:
            case 5:
            case 6:
            case 12:
            case 13:
            case 14:
            case 20:
            case 21:
            case 22:
                action=3;
                break;

            case 7:
            case 15:
            case 23:
                action=1;
                break;
            
            case 24:
            case 25:
            case 26:
                action=7;
                break;
            
            case 27:
                action=5;
                break;

            case 28:
            case 29:
            case 30:
                action=2;
                break;
            case 31:
                action=0;
                break;
        }
}

void leftTurn(){
    Serial.println("Turn Left");
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(rightmotor1, LOW);
    digitalWrite(leftmotor2, LOW);
    digitalWrite(rightMotor2, LOW);
    digitalWrite(leftMotorPWM, HIGH);
    digitalWrite(rightMotorPWM, HIGH);

}

void rightTurn(){
    Serial.println("Turn Right");
    digitalWrite(leftMotor1, LOW);
    digitalWrite(rightmotor1, HIGH);
    digitalWrite(leftmotor2, LOW);
    digitalWrite(rightMotor2, LOW);
    digitalWrite(leftMotorPWM, HIGH);
    digitalWrite(rightMotorPWM, HIGH);
}

void moveInchFor(){
    Serial.println("Go Straight");
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(rightmotor1, HIGH);
    digitalWrite(leftmotor2, LOW);
    digitalWrite(rightMotor2, LOW);
    digitalWrite(leftMotorPWM, HIGH);
    digitalWrite(rightMotorPWM, HIGH);
}

void Stop(){
    Serial.println("STOP");
    digitalWrite(leftMotorPWM, LOW);
    digitalWrite(rightMotorPWM, LOW);
}

void turnBack() {
    digitalWrite(leftMotor1, HIGH);
    digitalWrite(rightmotor1, LOW);
    digitalWrite(leftmotor2, LOW);
    digitalWrite(rightMotor2, HIGH);
    digitalWrite(leftMotorPWM, HIGH);
    digitalWrite(rightMotorPWM, HIGH);
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
        break;
      case 2
        turnRight();
        break;
      case 3:
        turnLeft();
        pathStore('L');
        break;
      case 4:
        moveInchFor();
        if(readSensors()==4){
            flag=0;
            break;
        }
        turnLeft();
        pathStore('L');
        break;
      case 5:
        moveInchFor();
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
    delay(100);                     //Change Time Experimentally
    Stop();
  }
}

void pathStore(char x){
  path[pathLen++]= x;
  if(pathLen < 3 || path[pathLen-2] != 'B')
    return;

  int totalAngle = 0;
  int i;
  for(i=1;i=<3;i++)
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
  while(flag){  
    switch(readSensor()){
      case 1:
        turnLeft();
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
    }
  }
}
