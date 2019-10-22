// Include Arduino Wire library for I2C
#include <Wire.h>
#include <Keypad.h>
#include <Stepper.h>

// Define Slave I2C Address
#define VERD_1 9
#define VERD_2 10
#define VERD_3 11
#define VERD_4 12
#define VERD_5 13

// Define Slave answer size
#define ANSWERSIZE 6

// Array for receiving inputs
char input1[10] = {};
char input2[10] = {};
char input3[10] = {};
char input4[10] = {};
char input5[10] = {};

int index;
int newState;
int direction;
int destination;
int state;
const int stepsPerRevolution = 32;

//keypad
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS]={
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS]{13,12,11,10};
byte colPins[COLS]{9,8,7,6};

Keypad keypad = Keypad(makeKeymap(keys),rowPins,colPins,ROWS,COLS);
Stepper myStepper = Stepper(stepsPerRevolution, 2,4,3,5); //motor

void setup() {

  // Initialize I2C communications as Master
  Wire.begin();
  myStepper.setSpeed(700);
  // Setup serial monitor
  Serial.begin(9600);
  Serial.println("Lift Master");
  newState = 3;
  direction = 1;
  destination = 4;
}

void loop() {
  delay(50);
  //keypad
  char key = keypad.getKey();
  Serial.print(key); 
  
  // Step one revolution in one direction:
  myStepper.step(stepsPerRevolution);
  
  sendData(VERD_1, newState, direction, destination);
  sendData(VERD_2, newState, direction, destination);
  sendData(VERD_3, newState, direction, destination);
  sendData(VERD_4, newState, direction, destination);
  sendData(VERD_5, newState, direction, destination);

  Serial.println("Receive data:");
  ///////////////////////////////////////////////////////////////////////////////////////////////
  // Read response from Slave1
  Wire.requestFrom(VERD_1, ANSWERSIZE);
  index = 0;
  while (Wire.available()) {
    input1[index] = Wire.read();
    index++;
  }
  state = input1[0];
  Serial.println((String)"verdieping 1: " + state);
  
  ///////////////////////////////////////////////////////////////////////////////////////////////
  // Read response from Slave2
  Wire.requestFrom(VERD_2, ANSWERSIZE);
  index = 0;
  while (Wire.available()) {
    input2[index] = Wire.read();
    index++;
  }
  state = input2[0];
  Serial.println((String)"verdieping 2: " + state);
    
  ///////////////////////////////////////////////////////////////////////////////////////////////
  // Read response from Slave3
  Wire.requestFrom(VERD_3, ANSWERSIZE);
  index = 0;
  while (Wire.available()) {
    input3[index] = Wire.read();
    index++;
  }
  state = input3[0];
  Serial.println((String)"verdieping 3: " + state);
  
  ///////////////////////////////////////////////////////////////////////////////////////////////
  // Read response from Slave4
  Wire.requestFrom(VERD_4, ANSWERSIZE);
  index = 0;
  while (Wire.available()) {
    input4[index] = Wire.read();
    index++;
  }
  state = input4[0];
  Serial.println((String)"verdieping 4: " + state);

  ///////////////////////////////////////////////////////////////////////////////////////////////
  // Read response from Slave5
  Wire.requestFrom(VERD_5, ANSWERSIZE);
  index = 0;
  while (Wire.available()) {
    input5[index] = Wire.read();
    index++;
  }
  state = input5[0];
  Serial.println((String)"verdieping 5: " + state);
  ///////////////////////////////////////////////////////////////////////////////////////////////
  Serial.println("Receiving done");
  }

  void sendData(int verdieping, int newState, int direction, int destination) {
    Serial.println((String)"Transmission verieping " + verdieping + " started!");
    Wire.beginTransmission(verdieping); //begin transmission
    Wire.write(newState);
    Wire.write(direction);
    Wire.write(destination);
    Wire.endTransmission(); //end transmission
    Serial.println((String)"Transmission verieping " + verdieping + " done!");
    }