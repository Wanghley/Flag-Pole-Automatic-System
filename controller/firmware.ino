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
double upStep,downStep, current=0;
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

void setPos(int pos){ // 1 up, 0 middle, -1 down
  Serial.println(current);
  if(pos==1){
    if(current<up){
      myStepper.step((up-current));
      Serial.println(up-current);
      current = up;
    }
  }else if(pos==-1){ // TODO: Problem on lowering from up when passes by middle
    if(current>down){
      myStepper.step((down-current));
      Serial.println(down-current);
      current = down;
    }
  }else if(pos==0){
    Serial.println(current);
    if(current<middle){
      myStepper.step((middle-current));
      current = middle;
    }
    else{
      myStepper.step(-(current-middle));
      current = middle;
    }
    Serial.println(current);
  }

  //for debug only
  Serial.println(current);


}

void loop() {
  if(digitalRead(btnMaintanance)==0){
    digitalWrite(ledMaintance, HIGH);
    if(digitalRead(btnUP)==0){
      myStepper.step(STEPS_PER_REV/4.0);
      upStep += STEPS_PER_REV/4.0;
      current+=upStep;
      delay(40);
      // FOR DEBUG ONLY
      // Serial.println(current);
    }
    if(digitalRead(btnDOWN)==0){
      myStepper.step((-STEPS_PER_REV/4.0));
      downStep += -STEPS_PER_REV/4.0;
      current+=downStep;
      delay(40);
      // FOR DEBUG ONLY
      // Serial.println(current);
    }
    if(digitalRead(btnMIDDLE)==0){
      up = upStep;
      down = abs(upStep+downStep);
      middle = (up-down)/2;

      // For debug only
      Serial.println(up);
      Serial.println(middle);
      Serial.println(down);

      blink(ledMaintance,3,300);
    }
  }else{
    digitalWrite(ledMaintance, LOW);
    if(Serial.available()){
      char c = Serial.read();
      if(c=='u'){
        setPos(1);      
      }else if(c=='d'){
        setPos(-1);
      }else if(c=='m'){
        setPos(0);
      }
    }
  }
}

void blink(int port, int repetitions, int duration){
  for(int i=0;i<repetitions;i++){
    digitalWrite(port, HIGH);
    delay(200);
    digitalWrite(port, LOW);
    delay(200);
  } 
}