// Include Arduino Wire library for I2C
#include <Wire.h>
#include <Keypad.h>
#include <Stepper.h>


// Define Slave I2C Address
const int floorAddress[5] = {9,10,11,12,13};
// Define Slave answer size
#define ANSWERSIZE 6

//keypad
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] {13, 12, 11, 10};
byte colPins[COLS] {9, 8, 7, 6};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

Stepper myStepper = Stepper(stepsPerRevolution, 2, 4, 3, 5); //motor
const int stepsPerRevolution = 32;


// Array for receiving inputs
bool inputButtonDown[5] = {0,0,0,0,0};
bool inputButtonUp[5] = {0,0,0,0,0};
bool inputDestinationFloor[5] = {0,0,0,0,0};

int direction;
int destinationFloor;
int currentFloor;



void setup() {
  // Initialize I2C communications as Master
  Wire.begin();
  myStepper.setSpeed(700);
  // Setup serial monitor
  Serial.begin(9600);
  Serial.println("Lift Master");
  currentFloor = 3;
  direction = 1;
  destinationFloor = 4;
}


void loop() {
  //keypad
  char key = keypad.getKey();
  Serial.print(key);

  // Step one revolution in one direction:
  myStepper.step(stepsPerRevolution);

  sendData();
  receiveData();
  
  delay(50);
}



void sendData() {
  for (int i = 0; i < 5; i++) {
    Serial.println((String)"Transmission verieping " + floorAddress[i] + " started!");
    Wire.beginTransmission(floorAddress[i]); //begin transmission
    Wire.write(currentFloor);
    Wire.write(direction);
    Wire.write(destinationFloor);
    Wire.endTransmission(); //end transmission
    Serial.println((String)"Transmission verieping " + floorAddress[i] + " done!");
  }
}

void receiveData() {
  Serial.println("Receive data:");
  for (int i = 0; i < 5; i++) {
    Wire.requestFrom(floorAddress[i], ANSWERSIZE);
    while (Wire.available()) {
      inputButtonDown[i] = Wire.read();
      inputButtonUp[i] = Wire.read();
      inputDestinationFloor[i] = Wire.read();
    }
  }
  Serial.println("Receiving done");
}

void printData(){
  Serial.println("Send data:");
  for (int i = 0; i < 5; i++) {
    Serial.println("///////////////////////////////////////");
    Serial.println((String)"verdieping 5 currentFloor: " + inputDestinationFloor[i]);
    Serial.println((String)"verdieping 5 up: " + inputButtonUp[i]);
    Serial.println((String)"verdieping 5 down: " + inputButtonDown[i]);
  }
  Serial.println("Sending done");
}
