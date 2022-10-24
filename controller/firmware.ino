#include <Stepper.h>
#include <EEPROM.h>

#define A 8
#define B 9
#define C 10
#define D 11
#define STEPS_PER_REV 512 // 11.25° per step
#define btnUP 6
#define btnDOWN 5
#define btnMIDDLE 7
#define btnMaintanance A3
#define ledON A4
#define ledMaintance A5
// Real steps per revolution is 512. Needed to multiply by 4 for some unknown reason
#define PI 3.1415926535897932384626433832795

const unsigned int MAX_MESSAGE_LENGTH = 1;

const double H = 20.0; //height of the pole in CM
const double radius = 1.0;
float upOrDown = 0.0;

Stepper myStepper = Stepper(STEPS_PER_REV, A, C, B, D);

double stepsNeeded = H/(2*PI*radius);
int roundedStepsNeeded = (int) stepsNeeded;

double bottonStep = 0;
double upStep,downStep;
double initial=0, up, down,middle;

void setup() {
  pinMode(A,OUTPUT);
  pinMode(B,OUTPUT);
  pinMode(C,OUTPUT);
  pinMode(D,OUTPUT);
  myStepper.setSpeed(50);
  Serial.begin(9600);

  pinMode(ledMaintance, OUTPUT);
  digitalWrite(ledMaintance, LOW);
  pinMode(ledON, OUTPUT);
  digitalWrite(ledON, LOW);

  pinMode(btnMaintanance, INPUT_PULLUP);
  pinMode(btnUP, INPUT_PULLUP);
  pinMode(btnDOWN, INPUT_PULLUP);

  pinMode(btnMIDDLE, INPUT_PULLUP);
}

void goUp(){
  myStepper.step(STEPS_PER_REV*4*roundedStepsNeeded);
  myStepper.step(STEPS_PER_REV*(stepsNeeded-roundedStepsNeeded));
}

void goDown(){
  myStepper.step(-STEPS_PER_REV*4*roundedStepsNeeded);
  myStepper.step(-STEPS_PER_REV*(stepsNeeded-roundedStepsNeeded));
}

void goMiddle(int upOrDown){
  if(upOrDown==1){
    myStepper.step(((STEPS_PER_REV*4*roundedStepsNeeded)/2));
    myStepper.step( (STEPS_PER_REV*(stepsNeeded-roundedStepsNeeded)/2));
  }else{
    myStepper.step(-((STEPS_PER_REV*4*roundedStepsNeeded)/2));
    myStepper.step(-(STEPS_PER_REV*(stepsNeeded-roundedStepsNeeded)/2));
  }
}

void loop() {
  if(digitalRead(btnMaintanance)==0){
    digitalWrite(ledMaintance, HIGH);
    if(digitalRead(btnUP)==0){
      myStepper.step(STEPS_PER_REV/16.0);
      upStep += STEPS_PER_REV/16.0;
      delay(100);
      Serial.println(upStep);
    }
    if(digitalRead(btnDOWN)==0){
      myStepper.step((-STEPS_PER_REV/16.0));
      downStep -= STEPS_PER_REV/16.0;
      delay(100);
      Serial.println(upStep);
    }
    if(digitalRead(btnMIDDLE)==0){
      Serial.println("Entered here");
      up = upStep;
      down = downStep;
      middle = (up-down)/2;
      digitalWrite(ledMaintance, HIGH);
      delay(200);
      digitalWrite(ledMaintance, LOW);
      delay(200);
      digitalWrite(ledMaintance, HIGH);
      delay(200);
      digitalWrite(ledMaintance, LOW);
      delay(200);
      digitalWrite(ledMaintance, HIGH);
      delay(200);
      digitalWrite(ledMaintance, LOW);
      delay(200);
      digitalWrite(ledMaintance, HIGH);
      delay(200);
    }
  }else{
    digitalWrite(ledMaintance, LOW);
  }

  if(Serial.available()){
    char c = Serial.read();
    if(c=='u'){
      goUp();
    }
  }


//   while (Serial.available() > 0)
//  {
//    //Create a place to hold the incoming message
//    static char message[MAX_MESSAGE_LENGTH];
//    static unsigned int message_pos = 0;

//    //Read the next available byte in the serial receive buffer
//    char inByte = Serial.read();

//    if(inByte=='u'){
//      if(upOrDown==0.0){
//        goUp();
//      }else if(upOrDown==1/2){
//        goMiddle(1); // Go back up
//      }
//      upOrDown = 1.0;
//    }else if(inByte=='d'){
//      if(upOrDown==1.0){
//        goDown();
//      }else if(upOrDown==1/2){
//        goMiddle(0); // Go back up
//      }
//      upOrDown = 0.0;
//    }else if(inByte=='m'){
//      if(upOrDown==0.0){
//        goMiddle(1);
//      }else if(upOrDown==1.0){
//        goMiddle(0); // Go back up
//      }
//      upOrDown = .5;
  //  }
//  }
}

// void onestep(){
// write(1,0,0,0);
// delay(1);
// write(1,1,0,0);
// delay(1);
// write(0,1,0,0);
// delay(1);
// write(0,1,1,0);
// delay(1);
// write(0,0,1,0);
// delay(1);
// write(0,0,1,1);
// delay(1);
// write(0,0,0,1);
// delay(1);
// write(1,0,0,1);
// delay(1);
// }
