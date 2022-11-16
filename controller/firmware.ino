#include <Stepper.h>
#include <EEPROM.h>

#define A 8
#define B 9
#define C 10
#define D 11
#define STEPS_PER_REV 200 // 11.25° per step
#define btnUP 6
#define btnDOWN 5
#define btnMIDDLE 7
#define btnMaintanance A3
#define ledON A4
#define ledMaintance A5

#define PI 3.1415926535897932384626433832795

Stepper myStepper = Stepper(STEPS_PER_REV, A, C, B, D);

double bottonStep = 0;
double upStep,downStep, current=0;
double up,down,middle;

void setup() {
  pinMode(A,OUTPUT);
  pinMode(B,OUTPUT);
  pinMode(C,OUTPUT);
  pinMode(D,OUTPUT);
  myStepper.setSpeed(50);
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

void setPos(int pos){ // 1 up, 0 middle, -1 down
  if(pos==1){
    if(current<up){
      myStepper.step((up-current));
      current = up;
    }
  }else if(pos==-1){ // TODO: Problem on lowering from up when passes by middle
    if(current>down){
      myStepper.step((down-current));
      current = down;
    }
  }else if(pos==0){
    if(current<middle){
      myStepper.step((middle-current));
      current = middle;
    }
    else{
      myStepper.step(-(current-middle));
      current = middle;
    }
  }

  // FOR DEBUG ONLY
  // Serial.println(current);
}

// Write int data into Arduino EEPROM
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

void blink(int port, int repetitions, int duration){
  for(int i=0;i<repetitions;i++){
    digitalWrite(port, HIGH);
    delay(200);
    digitalWrite(port, LOW);
    delay(200);
  } 
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