// Include Arduino Wire library for I2C
#include <Wire.h>

// Define Slave I2C Address
#define SLAVE_ADDR 9
// Define Slave answer size
#define ANSWERSIZE 6
// Array for receiving inputs
char input[10] = {};
int index;
int newState;
int direction;
int destination;
int state;

void setup() {

  // Initialize I2C communications as Master
  Wire.begin();

  // Setup serial monitor
  Serial.begin(9600);
  Serial.println("Lift Master");
  newState = 3;
  direction = 1;
  destination = 4;
}

void loop() {

  delay(500);
  Serial.println("Write data to slave");

  // Write a charatre to the Slave
  Wire.beginTransmission(SLAVE_ADDR);

  Wire.write(newState);
  Wire.write(direction);
  Wire.write(destination);

  Wire.endTransmission();
  Serial.println("Transmission done");
  Serial.println("Receive data:");

  // Read response from Slave
  Wire.requestFrom(SLAVE_ADDR, ANSWERSIZE);

  // Add characters to string
  index = 0;
  while (Wire.available()) {
    input[index] = Wire.read();
    index++;
  }
  state = input[0];
  Serial.println(state);
}
