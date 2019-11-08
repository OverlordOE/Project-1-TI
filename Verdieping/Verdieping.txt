/*-----------------------------------------------------------------------------------------------------*/
/*- Libraries -----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------*/
#include <Wire.h> // I2C Library

/*-----------------------------------------------------------------------------------------------------*/
/*- Pins ----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------*/

/* Pins for 7-Segment Display */
const int latchPin = 12; // Pin connected to ST_CP of 74HC595
const int clockPin = 8; // Pin connected to SH_CP of 74HC595
const int dataPin = 11; // Pin connected to DS of 74HC595
byte segData[10] = {125, 48, 110, 122, 51, 91, 95, 112, 127, 123}; // Array without the decimal

/* Pins for REED */
const byte reedPin = 2;
boolean reedState = false;

/* Pins for Buttons */
const byte buttonPin[2] = {3, 4}; // [0] = Down, [1] = Up
bool buttonAvailable[2];
bool buttonState[2] = {false, false};

/* pins for leds */
const byte ledPin[] = {5, 6};
const byte liftDoorLedPin = 13;

/*-----------------------------------------------------------------------------------------------------*/
/*- Config --------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------*/

/* Elevator Config */
const byte localFloor = 0; // Floor number of local Floor (0 tm 4)
const byte minFloor = 0; // Lowest floor number
const byte maxFloor = 4; // Highest floor number

bool elevatorDirection;
byte destinationFloor;
byte currentFloor;

byte displayNumber;

/* Slave Config */
const byte floorAddress[5] = {9,10,11,12,13}; // Slave I2C Address 9t/m13
#define ANSWERSIZE 3 // Define Slave answer size
char input[2]; // Input from master


/*-----------------------------------------------------------------------------------------------------*/
/*- Functions -----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------*/

void setup() {
  /* Setup for pins */
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(reedPin, INPUT_PULLUP);

  for (byte i = 0; i < sizeof(buttonPin); i++) pinMode(buttonPin[i], INPUT_PULLUP);
  for (byte i = 0; i < sizeof(ledPin); i++) pinMode(ledPin[i], OUTPUT);
  pinMode(liftDoorLedPin, OUTPUT);


  /* Checks if all buttons are available by the location of the elevator */
  buttonAvailable[0] = (localFloor > minFloor) ? (true) : (false);
  buttonAvailable[1] = (localFloor < maxFloor) ? (true) : (false);

  /* I2C communication startup */
  Wire.begin(floorAddress[localFloor]); // Initialize I2C communications as Slave
  Wire.onRequest(requestEvent); // Function to run when data requested from master
  Wire.onReceive(receiveEvent); // Function to run when data received from master

  /* Setup Serial Monitor */
  Serial.begin(9600);
  Serial.println((String)"Floor: " + localFloor + " Online!");

  setDisplay(currentFloor);
}


void loop() {
  if (button(0) && buttonAvailable[0]) { // Turn LEDs on when the button is pressed
    buttonState[0] = true;
    digitalWrite(ledPin[0], HIGH);
  }
  if (button(1) && buttonAvailable[1]) {
    buttonState[1] = true;
    digitalWrite(ledPin[1], HIGH);
  }

  reedState = !digitalRead(reedPin);
  if (reedState && destinationFloor == currentFloor) {
    digitalWrite(liftDoorLedPin, HIGH);
  } else {
    digitalWrite(liftDoorLedPin, LOW);
  }

  if (currentFloor == localFloor){ // Turn LEDs off
    buttonState[elevatorDirection] = false;
    digitalWrite(ledPin[elevatorDirection], LOW);
  }

  if(currentFloor != displayNumber){ // Only update the 7 segment display when the currentFloor changes
    setDisplay(currentFloor);
    displayNumber = currentFloor;
  }

  delay(50);
}



void receiveEvent() {
  // Read while data received
  while (0 < Wire.available()) {
    currentFloor = (byte)Wire.read();
    elevatorDirection = (bool)Wire.read(); // 0 = down ,1 = up
    destinationFloor = (byte)Wire.read();
  }
}



void requestEvent() {
  Wire.write(reedState);
  Wire.write(buttonState[0]);
  Wire.write(buttonState[1]);
}



void setDisplay(int number) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, segData[(number)]);
  digitalWrite(latchPin, HIGH);
}



boolean button(byte i)         // geeft DIRECT EENMALIG een '1' als knop i ingedrukt wordt
{ // knop i moet 50 ms los zijn voordat een nieuwe '1' gegeven kan worden
  // in dit voorbeeld is bereik i: [0..3]

  const  unsigned long debounce  =  50;
  static unsigned long buttonTime[sizeof(buttonPin)];   // static array!!
  static boolean       buttonFlg[sizeof(buttonPin)];    // static array!!

  if (!digitalRead(buttonPin[i]))
  {
    buttonTime[i] = millis();
    if (!buttonFlg[i])
    {
      buttonFlg[i] = 1;
      return (1);
    }
  }
  else
  {
    if ((millis() - buttonTime[i]) > debounce) buttonFlg[i] = 0;
  }
  return (0);
}
