#include <Smartcar.h>
#include <Wire.h>


Odometer encoderLeft, encoderRight;
SR04 frontSensor;
SR04 backSensor;
Gyroscope gyro;
Car car;


const int TRIGGER1_PIN = 6; //D6
const int ECHO1_PIN = 5; //D5
const int TRIGGER_PIN = 4;
const int ECHO_PIN = 7;
const int encoderLeftPin = 2;
const int encoderRightPin = 3;

 int fwSpeed = 70; //set forward speed
 int bwSpeed = -70; //set backward speed
 int rDegree =  75; //set degrees to turn right
 int lDegree = -75; //set degrees to turn left
 char Direction = 'n';

int frDistance;
int baDistance;
boolean controls = true;

void setup() {
  Serial3.begin(9600);
  Serial.begin(9600);

  gyro.attach();
  encoderLeft.attach(encoderLeftPin);
  encoderRight.attach(encoderRightPin);
  frontSensor.attach(TRIGGER_PIN, ECHO_PIN);
  backSensor.attach(TRIGGER1_PIN, ECHO1_PIN);

  // start components
  gyro.begin();
  encoderLeft.begin();
  encoderRight.begin();
  car.begin(encoderLeft, encoderRight, gyro); //initialize the car using the encoders

  //ini Ultrasonic distance meansurement as 0
  frDistance = 0;
  baDistance = 0;

}

void loop(){
  dancing();
  
  frDistance = frontSensor.getDistance();
  baDistance = backSensor.getDistance();
  
    int curSpeed = car.getSpeed(); // read the current speed of car
    if (curSpeed == 0) {
        Direction = 'n';
    }

    if (NoObstacle(frDistance) == false && Direction != 'b' && Direction != 'n') {
        car.stop();
    } 
    else if (NoObstacle(baDistance) == false && Direction == 'b') {
        car.stop();
    }
    else{
        handleInput();
    }
}

  void dancing(){
   if (Serial.available()) {
    char input;
    frDistance = frontSensor.getDistance();
    baDistance = backSensor.getDistance();
    
    while (Serial.available()) input = Serial.read(); //read everything that has been received so far and log down the last entry
    switch (input) {
      case 'k': //forward
        Direction = 'k';
//        car.setMotorSpeed(10,10);
          car.go(10);
          car.rotate(-180);
          car.go(-15);
          car.rotate(80);
        break;
      case 's': //backward
        Direction = 's';
//        car.setMotorSpeed(20,20);
        car.go(-10);
        car.rotate(60);
        car.go(20);
        car.rotate(-60);
        break;
      case 'h': //turn left
        Direction = 'h';
//        car.setMotorSpeed(0,70);
        car.go(20);
       car.rotate(90);
       car.go(5);
       car.rotate(-90);
        break;
      default: //if there isn't any command
        car.setSpeed(0);
        car.setAngle(0);
    }
  }
  }

void handleInput() {
  if (Serial3.available()) {
     char type;
     char input;
     int power;
     unsigned char arr[3];
     frDistance = frontSensor.getDistance();
     baDistance = backSensor.getDistance();
     
    while (Serial3.available() > 0)
    Serial3.readBytes(arr,3);
    type = (char)arr[0];
    power =(int) arr[1];
    input =(char)arr[2];

    if(type == 'j'){
      switch (input) {
        case 'f': //forward
          if(NoObstacle(frDistance) == false) {
            car.setSpeed(0);
          }
          else{
            car.setSpeed(power);
            car.setAngle(0);
          }
          break;
        case 'b': //backward
          if(NoObstacle(baDistance) == false) {
            car.setSpeed(0);
          }
          else{
            car.setSpeed(power * -1);
            car.setAngle(0);
          }
          break;
        case 'l': //turn left
          if(NoObstacle(frDistance) == false) {
            car.setSpeed(0);
          }
          else{
            car.setSpeed(power);
            car.setAngle(lDegree);
          }
          break;
        case 'r': //turn right
          if(NoObstacle(frDistance) == false) {
            car.setSpeed(0);
          }
          else{
            car.setSpeed(power);
            car.setAngle(rDegree);
          }
          break;
        case 'q': //turn left front
         if(NoObstacle(frDistance) == false) {
            car.setSpeed(0);
          }
         else{
          Serial.println(power);
          car.setMotorSpeed(power/2 ,power);
         }
          break;
        case 'e': //turn right front
         if(NoObstacle(frDistance) == false) {
            car.setSpeed(0);
          }
         else{
          car.setMotorSpeed(power, power/2);
         }
          break;
        case 'z': //turn left bottom
          if(NoObstacle(baDistance) == false) {
            car.setSpeed(0);
          }
         else{
            car.setMotorSpeed(-1 * power/2, -1* power);
         }
          break;
        case 'c': //turn right bottom
          if(NoObstacle(baDistance) == false) {
            car.setSpeed(0);
          }
         else{
            car.setMotorSpeed(-1 * power, -1 * power/2);
         }
          break;
        case 's': //stop car
          car.setSpeed(0);
          car.setAngle(0);
          break;
        default: //if there isn't any command
          car.setSpeed(0);
          car.setAngle(0);
      }
    }
    else if(type == 'd'){
      if(power == 1){
        fwSpeed =  35; //set forward speed
        bwSpeed = -35; //set backward speed
        rDegree =  35; //set degrees to turn right
        lDegree = -35;
     } else if(power == 2){
        fwSpeed =  50; //set forward speed
        bwSpeed = -50; //set backward speed
        rDegree =  50; //set degrees to turn right
        lDegree = -50;
     } else if(power == 3){
        fwSpeed =  75; //set forward speed
        bwSpeed = -75; //set backward speed
        rDegree =  75; //set degrees to turn right
        lDegree = -75;
     }

     // Serial.println(type);
     //Serial.println(power);
     //Serial.println(input);
     switch (input) {
       case 'f': //forward
         Direction = 'f';
          if(NoObstacle(frDistance) == true){
              car.setSpeed(fwSpeed);
              //Serial.println(fwSpeed);
              car.setAngle(0);
          }
         break;
       case 'b': //backward
         Direction = 'b';
          if(NoObstacle(baDistance) == true){
              car.setSpeed(bwSpeed);
              car.setAngle(0);
          } 
         break;
       case 'l': //turn left
         Direction = 'l';
          if(NoObstacle(frDistance) == true){
              car.setSpeed(fwSpeed);
              car.setAngle(lDegree);
          }
         break;
       case 'r': //turn right
         Direction = 'r';
          if(NoObstacle(frDistance) == true){
              car.setSpeed(fwSpeed);
              car.setAngle(rDegree);
          }
         break;
       case 's': //stop car
         car.setSpeed(0);
         car.setAngle(0);
         break;
       default: //if there isn't any command
         car.setSpeed(0);
         car.setAngle(0);
    }
    }
  }
}




boolean NoObstacle(int distance) {
  if (distance > 20 || distance == 0) {
    return true;
  }
  else {
    return false;
  }
}

