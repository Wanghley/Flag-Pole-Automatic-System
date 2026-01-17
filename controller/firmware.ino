#include <AccelStepper.h>
#include <EEPROM.h>

// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1
#define STEPS_PER_REV 1600 // steps needed for Nema 23
#define btnUP 6
#define btnDOWN 5
#define btnMIDDLE 7
#define btnMaintanance A3
#define ledON A4
#define ledMaintance A5
#define speed 3000

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

double bottonStep = 0;
double upStep,downStep, current=0;
double up,down,middle;

void setup() {
  // Set the maximum speed and acceleration:
  stepper.setMaxSpeed(3000);
  stepper.setAcceleration(100000);

  Serial.begin(115200);

  pinMode(ledMaintance, OUTPUT);
  digitalWrite(ledMaintance, LOW);
  pinMode(ledON, OUTPUT);
  digitalWrite(ledON, LOW);

  pinMode(btnMaintanance, INPUT_PULLUP);
  pinMode(btnUP, INPUT_PULLUP);
  pinMode(btnDOWN, INPUT_PULLUP);

  pinMode(btnMIDDLE, INPUT_PULLUP);
  
  // Get storaged data from EEPROM
  up = readIntFromEEPROM(0);
  middle = readIntFromEEPROM(2);
  down = readIntFromEEPROM(4);

  // FOR DEBUG ONLY
  // Serial.println(up);
  // Serial.println(middle);
  // Serial.println(down);
}

void writeIntIntoEEPROM(int address, int number){ 
  byte byte1 = number >> 8;
  byte byte2 = number & 0xFF;
  EEPROM.write(address, byte1);
  EEPROM.write(address + 1, byte2);
}

// Read data from Arduino EEPROM
int readIntFromEEPROM(int address){
  byte byte1 = EEPROM.read(address);
  byte byte2 = EEPROM.read(address + 1);
  return (byte1 << 8) + byte2;
}

void setPos(int pos){ // 1 up, 0 middle, -1 down
  if(pos==1){
    if(current<up){
      while(stepper.isRunning()){}
      stepper.move((up-current));
      stepper.runToPosition();
      current = up;
    }
  }else if(pos==-1){ // TODO: Problem on lowering from up when passes by middle
    if(current>down){
      while(stepper.isRunning()){}
      stepper.move((down-current));
      stepper.runToPosition();
      current = down;
    }
  }else if(pos==0){
    if(current<middle){
      while(stepper.isRunning()){}
      stepper.move((middle-current));
      stepper.runToPosition();
      current = middle;
    }
    else{
      while(stepper.isRunning()){}
      stepper.move(-(current-middle));
      stepper.runToPosition();
      current = middle;
    }
  }

  // FOR DEBUG ONLY
  // Serial.println(current);
}

void blink(int port, int repetitions, int duration){
  for(int i=0;i<repetitions;i++){
    digitalWrite(port, HIGH);
    delay(200);
    digitalWrite(port, LOW);
    delay(200);
  } 
}

void loop() {
  // Set the target position:
  // stepper.moveTo(8000);
  // // Run to target position with set speed and acceleration/deceleration:
  // stepper.runToPosition();

  // delay(1000);

  // // Move back to zero:
  // stepper.moveTo(0);
  // stepper.runToPosition();

  // delay(1000);

  if(digitalRead(btnMaintanance)==0){
    digitalWrite(ledMaintance, HIGH);
    if(digitalRead(btnUP)==0 && !stepper.isRunning()){
      stepper.setSpeed(speed);
      stepper.move(STEPS_PER_REV);
      upStep += STEPS_PER_REV;
      current+=upStep;
      // FOR DEBUG ONLY
      // Serial.println(current);
      stepper.runToPosition();
      delay(40);
    }
    if(digitalRead(btnDOWN)==0  && !stepper.isRunning()){
      stepper.setSpeed(-speed);
      stepper.move(-STEPS_PER_REV);
      downStep += -STEPS_PER_REV;
      current+=downStep;
      // FOR DEBUG ONLY
      // Serial.println(current);
      stepper.runToPosition();
      delay(40);
    }
    if(digitalRead(btnMIDDLE)==0){
      up = upStep;
      down = abs(upStep+downStep);
      middle = (up-down)/2;

      // FOR DEBUG ONLY
      // Serial.println(up);
      // Serial.println(middle);
      // Serial.println(down);

      writeIntIntoEEPROM(0,up);
      writeIntIntoEEPROM(2,middle);
      writeIntIntoEEPROM(4,down);

      blink(ledMaintance,3,300);
    }
  }else{
    digitalWrite(ledMaintance, LOW);
    if(Serial.available() && up!=-1 && middle!=-1 && down!=-1){
      char c = Serial.read();
      if(c=='u'){
        setPos(1);
        Serial.println(1);    
      }else if(c=='d'){
        setPos(-1);
        Serial.println(-1);
      }else if(c=='m'){
        setPos(0);
        Serial.println(0);
      }
    }
  }
}