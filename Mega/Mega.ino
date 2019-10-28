// Include Arduino Wire library for I2C
#include <Wire.h>
#include <Keypad.h>
#include <Stepper.h>


// Define Slave I2C Address
const int floorAddress[5] = {9, 10, 11, 12, 13};
// Define Slave answer size
#define ANSWERSIZE 3

/* Pins for 7-Segment Display */
const int latchPin = 14; // Pin connected to ST_CP of 74HC595
const int clockPin = 15; // Pin connected to SH_CP of 74HC595
const int dataPin = 16; // Pin connected to DS of 74HC595
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

const int stepsPerRevolution = 32;
Stepper myStepper = Stepper(stepsPerRevolution, 2, 4, 3, 5); //motor



// Array for receiving inputs
bool inputButtonDown[5] = {0, 0, 0, 0, 0};
bool inputButtonUp[5] = {0, 0, 0, 0, 0};
bool inputDestinationFloor[5] = {0, 0, 0, 0, 0};

bool elevatorDirection;
int destinationFloor;
int currentFloor;



void setup() {
  // Setup for pins
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  // Initialize I2C communications as Master
  //Wire.begin();
  myStepper.setSpeed(700);
  // Setup serial monitor
  Serial.begin(9600);
  Serial.println("Lift Master");
  currentFloor = 3; // Test
  elevatorDirection = 1; // Test
  destinationFloor = 4; // Test
}


void loop() {
  //keypad
  char key = keypad.getKey();
  // choose floor
  if (key == '1') {inputDestinationFloor[0] = 1; Serial.println("Going to floor 1!");}
  else if (key == '2') {inputDestinationFloor[0] = 0; Serial.println("Going to floor 2!");} // !!!!!!!!!!!!
  else if (key == '3') {inputDestinationFloor[2] = 1; Serial.println("Going to floor 3!");}
  else if (key == '4') {inputDestinationFloor[3] = 1; Serial.println("Going to floor 4!");}
  else if (key == '5') {inputDestinationFloor[4] = 1; Serial.println("Going to floor 5!");}
  else {Serial.println("Key not recognized");}


  // Function to use the motor
  useMotor(inputDestinationFloor[0]);
  

  //sendData();
  //receiveData();
  setDisplay(currentFloor);
  delay(50);
  
}



void sendData() {
  for (int i = 0; i < 5; i++) {
    Serial.println((String)"Transmission verieping " + floorAddress[i] + " started!");
    Wire.beginTransmission(floorAddress[i]); //begin transmission
    Wire.write(currentFloor);
    Wire.write(elevatorDirection);
    Wire.endTransmission(); //end transmission
    Serial.println((String)"Transmission verieping " + floorAddress[i] + " done!");
  }
}

void receiveData() {
  Serial.println("Receive data:");
  for (int i = 0; i < 5; i++) {
    Wire.requestFrom(floorAddress[i], ANSWERSIZE);
    while (Wire.available()) {
      inputDestinationFloor[i] = Wire.read();
      inputButtonDown[i] = Wire.read();
      inputButtonUp[i] = Wire.read();
    }
  }
  Serial.println("Receiving done");
}

void printData() {
  Serial.println("Send data:");
  for (int i = 0; i < 5; i++) {
    Serial.println("///////////////////////////////////////");
    Serial.println((String)"verdieping 5 currentFloor: " + inputDestinationFloor[i]);
    Serial.println((String)"verdieping 5 up: " + inputButtonUp[i]);
    Serial.println((String)"verdieping 5 down: " + inputButtonDown[i]);
  }
  Serial.println("Sending done");
}

void setDisplay (int number) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, segData[number]);
  digitalWrite(latchPin, HIGH);
}

void useMotor(bool goUp){ // 0 = Down, 1 = Up
  // Step one revolution in one direction:
  myStepper.step(stepsPerRevolution);
  if (goUp){ // Up
    myStepper.step(-stepsPerRevolution);
  }
  else { // Down
    myStepper.step(stepsPerRevolution);
  }
}
