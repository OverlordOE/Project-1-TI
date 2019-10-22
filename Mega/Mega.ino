// Include Arduino Wire library for I2C
#include <Wire.h>
#include <Keypad.h>
#include <Stepper.h>

// Define Slave I2C Address
#define SLAVE_ADDR 9
// Define Slave answer size
#define ANSWERSIZE 6
// Array for receiving inputs
char inTraffic[32] = {};
int i;
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


//motor
Stepper myStepper = Stepper(stepsPerRevolution, 2,4,3,5);

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
  //
  Serial.println("Write data to slave");

  // Write a charatre to the Slave
  Wire.beginTransmission(SLAVE_ADDR);

  Wire.write(newState);
  Wire.write(direction);
  Wire.write(destination);

  Wire.endTransmission();

  Serial.println("Receive data:");

  // Read response from Slave
  Wire.requestFrom(SLAVE_ADDR, ANSWERSIZE);

  // Add characters to string
  i = 0;
  while (Wire.available()) {
    inTraffic[i] = Wire.read();
    
    i++;
  }
  state = inTraffic[0];
  Serial.println(state);
  //
  char key = keypad.getKey();
   Serial.print(key); 

   // Step one revolution in one direction:
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
}
