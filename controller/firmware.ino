#include <Stepper.h>

#define A 8
#define B 9
#define C 10
#define D 11
#define STEPS_PER_REV 512 // 11.25° per step
// Real steps per revolution is 512. Needed to multiply by 4 for some unknown reason
#define PI 3.1415926535897932384626433832795

const unsigned int MAX_MESSAGE_LENGTH = 1;

const double H = 20.0; //height of the pole in CM
const double radius = 1.0;
float upOrDown = 0.0;

Stepper myStepper = Stepper(STEPS_PER_REV, A, C, B, D);

double stepsNeeded = H/(2*PI*radius);
int roundedStepsNeeded = (int) stepsNeeded;

void setup() {
  pinMode(A,OUTPUT);
  pinMode(B,OUTPUT);
  pinMode(C,OUTPUT);
  pinMode(D,OUTPUT);
  myStepper.setSpeed(50);
  Serial.begin(9600);
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
  while (Serial.available() > 0)
 {
   //Create a place to hold the incoming message
   static char message[MAX_MESSAGE_LENGTH];
   static unsigned int message_pos = 0;

   //Read the next available byte in the serial receive buffer
   char inByte = Serial.read();

   if(inByte=='u'){
     if(upOrDown==0.0){
       goUp();
     }else if(upOrDown==1/2){
       goMiddle(1); // Go back up
     }
     upOrDown = 1.0;
   }else if(inByte=='d'){
     if(upOrDown==1.0){
       goDown();
     }else if(upOrDown==1/2){
       goMiddle(0); // Go back up
     }
     upOrDown = 0.0;
   }else if(inByte=='m'){
     if(upOrDown==0.0){
       goMiddle(1);
     }else if(upOrDown==1.0){
       goMiddle(0); // Go back up
     }
     upOrDown = .5;
   }
 }
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
