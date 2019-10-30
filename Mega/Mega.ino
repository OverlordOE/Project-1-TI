// Include Arduino Wire library for I2C
#include <Wire.h>
#include <Keypad.h>
#include <Stepper.h>


// Define Slave I2C Address
const int floorAddress[5] = {9, 10, 11, 12, 13};
// Define Slave answer size
#define ANSWERSIZE 3

/* Pins for 7-Segment Display */
const int latchPin = 12; // Pin connected to ST_CP of 74HC595
const int clockPin = 8; // Pin connected to SH_CP of 74HC595
const int dataPin = 11; // Pin connected to DS of 74HC595
byte segData[10] = {125, 48, 110, 122, 51, 91, 95, 112, 127, 123}; // Array without the decimal


//keypad
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] {27, 29, 31, 33};
byte colPins[COLS] {35, 37, 39, 41};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


//motor
const int stepsPerRevolution = 32;
Stepper myStepper = Stepper(stepsPerRevolution, 2, 4, 3, 5); //motor


// Array for receiving inputs
bool inputButtonDown[5] = {0, 0, 0, 0, 0};
bool inputButtonUp[5] = {0, 0, 0, 0, 0};
int inputDestinationFloor[5] = {2, 0, 0, 0, 0};

bool elevatorDirection;
int destinationFloor;
int currentFloor;



void setup() {
  // Setup for pins
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  // Setup serial monitor
  Serial.begin(9600);
  Serial.println("Lift Master");

  currentFloor = 0;
  Wire.begin();
  myStepper.setSpeed(100);
}



void loop() {
  char key = keypad.getKey();
  
  // choose floor
  if (key == '1') {inputDestinationFloor[0] = 1;}
  else if (key == '2') {inputDestinationFloor[0] = 0;}
  else if (key == '3') {inputDestinationFloor[0] = 2;}
  else if (key == '4') {inputDestinationFloor[3] = 1;}
  else if (key == '5') {inputDestinationFloor[4] = 1;}
  
  //choose speed
  if (key == 'A') {myStepper.setSpeed(100);}
  else if (key == 'B') {myStepper.setSpeed(300);}
  else if (key == 'C') {myStepper.setSpeed(500);}

  useMotor(inputDestinationFloor[0]);
  
  //7Seg test  
  currentFloor++;
  if (currentFloor > 4) {currentFloor=0;}

  sendData();
  receiveData();
  setDisplay(currentFloor);
  delay(50);
}



void sendData() {
  for (int i = 0; i < 5; i++) {
    Wire.beginTransmission(floorAddress[i]); //begin transmission
    Wire.write(currentFloor);
    Wire.write(elevatorDirection);
    Wire.endTransmission(); //end transmission
  }
}



void receiveData() {
  for (int i = 0; i < 5; i++) {
    Wire.requestFrom(floorAddress[i], ANSWERSIZE);
    while (Wire.available()) {
      //inputDestinationFloor[i] = Wire.read();
      inputButtonDown[i] = Wire.read();
      inputButtonUp[i] = Wire.read();
    }
  }
}



void setDisplay (int number) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, segData[number]);
  digitalWrite(latchPin, HIGH);
}



void useMotor(int goUp){ // 0 = Down, 1 = Up
  // Step one revolution in one direction:
  if (goUp == 1){ // Up
    myStepper.step(-stepsPerRevolution);
  }
  else if (goUp == 0) { // Down
    myStepper.step(stepsPerRevolution);
  }else {}
}
