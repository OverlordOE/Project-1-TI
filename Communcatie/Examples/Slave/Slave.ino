// Include Arduino Wire library for I2C
#include <Wire.h>
 
// Define Slave I2C Address
#define SLAVE_ADDR 9
 
// Define Slave answer size
#define ANSWERSIZE 6
#define led 7
byte x = 0;

// Define string with response to Master
String answer = "Led on";
 
void setup() {
 
  // Initialize I2C communications as Slave
  Wire.begin(SLAVE_ADDR);
  
  // Function to run when data requested from master
  Wire.onRequest(requestEvent); 
  
  // Function to run when data received from master
  Wire.onReceive(receiveEvent);
  
  // Setup Serial Monitor 
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  Serial.println("I2C Slave Demonstration");
}
 
void receiveEvent() {
 
  // Read while data received
  while (0 < Wire.available()) {
     x = Wire.read();
  }
  Serial.println(x);
  
}
 
void requestEvent() {
 

  if( x== 1){

     Wire.write("Led on ",6);

  }
   if( x== 0){

     Wire.write("Led off",7);

  }
  
 
  digitalWrite(led, x);
  
  
}
 
void loop() {
 
  // Time delay in loop
  delay(50);
}
