// Include Arduino Wire library for I2C
#include <Wire.h>

// Define Slave I2C Address
#define SLAVE_ADDR 9

// Define Slave answer size
#define ANSWERSIZE 6
char inTraffic[10] = {};
int i;


void setup() {

  // Initialize I2C communications as Slave
  Wire.begin(SLAVE_ADDR);

  // Function to run when data requested from master
  Wire.onRequest(requestEvent);

  // Function to run when data received from master
  Wire.onReceive(receiveEvent);

  // Setup Serial Monitor
  Serial.begin(9600);
  Serial.println("Lift Slaaf x");
}

void receiveEvent() {

  // Read while data received
  i = 0;
  while (0 < Wire.available()) {
    //put into array to read out later
    inTraffic[i] = Wire.read();
    i++;
    Serial.println(inTraffic[i]);
  }


}

void requestEvent() {
  //First write isnt recognized
  Wire.write("f");
  Wire.write("g");
  Wire.write("0");
  Wire.write("1");
}

void loop() {

  // Time delay in loop
  delay(500);
}
