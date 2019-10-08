// Include Arduino Wire library for I2C
#include <Wire.h>
 
// Define Slave I2C Address
#define SLAVE_ADDR 9
 
// Define Slave answer size
#define ANSWERSIZE 7

#define knop 7
 
void setup() {
 
  // Initialize I2C communications as Master
  Wire.begin();
  
  // Setup serial monitor
  Serial.begin(9600);
  pinMode(knop, INPUT);
  Serial.println("I2C Master Demonstration");
}
 
void loop() {

  Serial.println(digitalRead(knop));
  delay(50);
  Serial.println("Write data to slave");
  
  // Write a charatre to the Slave
  Wire.beginTransmission(SLAVE_ADDR);
 
  Wire.write(digitalRead(knop));
  
  Wire.endTransmission();
    
  Serial.println("Receive data:");
  
  // Read response from Slave
  // Read back 7 characters
  Wire.requestFrom(SLAVE_ADDR,ANSWERSIZE);
  
  // Add characters to string
  String response = "";
  while (Wire.available()) {
      char b = Wire.read();
      response += b;
  } 
  
  // Print to Serial Monitor
  Serial.println(response);
}
