/*-----------------------------------------------------------------------------------------------------*/
/*- Libraries -----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------*/
#include <Wire.h> //I2C Library

/*-----------------------------------------------------------------------------------------------------*/
/*- Pins ----------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------*/

/* Pins for 7-Segment Display */
const int latchPin = 12; //Pin connected to ST_CP of 74HC595
const int clockPin = 8; //Pin connected to SH_CP of 74HC595 
const int dataPin = 11; //Pin connected to DS of 74HC595 
byte datArray[10] = {125, 48, 110, 122, 51, 91, 95, 112, 127, 123}; // array without the decimal

/* Pins for REED */
const byte reedPin = 2;
boolean reedState = false;

/* Pins for Buttons */
const byte  buttonPin[]  = {3,4};
const byte  ledPin[]     = {5,6};
boolean buttonDownState = false;
boolean buttonUpState = false;

/* pins for leds */
//Todo

/*-----------------------------------------------------------------------------------------------------*/
/*- Config --------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------*/

/* Elevator Config */
byte minState = 0; //Lowest floor number
byte maxState = 4; //Highest floor number
byte state = 0; //Starting Floor number

/* Local Config */
byte defaultState = 2; // Floor number of local Floor
#define SLAVE_ADDR 9 // Slave I2C Address
bool upButton;
bool downButton;

/* Slave Config */
#define ANSWERSIZE 6 // Define Slave answer size
String input; //input from master
String answer = "Led on"; // Define string with response to Master

/*-----------------------------------------------------------------------------------------------------*/
/*- Functions -----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------*/

void setup() {
  /* Setup for pins */
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(reedPin, INPUT_PULLUP);

  for (byte i=0; i<sizeof(buttonPin); i++) pinMode(buttonPin[i], INPUT_PULLUP);
  for (byte i=0; i<sizeof(ledPin);    i++) pinMode(ledPin[i],  OUTPUT);

  /* Set on default floor */
  changeState(state);

  /* Checks if all buttons are available by the location of the elevator */
  upButton = (defaultState < maxState) ? (true) : (false);
  downButton = (defaultState > minState) ? (true) : (false);

  /* I2C communication startup */
  Wire.begin(SLAVE_ADDR); // Initialize I2C communications as Slave
  Wire.onRequest(requestEvent); // Function to run when data requested from master
  Wire.onReceive(receiveEvent); // Function to run when data received from master
  
  /* Setup Serial Monitor */
  Serial.begin(9600);
  Serial.println((String)"Floor: "+defaultState+" Online!");
}


void receiveEvent() {
  // Read while data received
  int index = 0;
  while (0 < Wire.available()) {
    input = Wire.read(); //newState, direction, destination
  }
}


void requestEvent() {
  int newState = (int)input[0]; 
  bool direction = (bool)input[1]; // 0 = down ,1 = up
  int destination = (int)input[2];

  checkDefaultState();

  //Wire.write();
}


void loop() {
  if(button(0)){
    buttonDownState = true;
    digitalWrite(ledPin[0], HIGH);
  }
  if(button(0)){
    buttonUpState = true;
    digitalWrite(ledPin[2], HIGH);
  }

  
  // Time delay in loop
  delay(50);
}



void checkDefaultState() {
  if (!digitalRead(reedPin) && !reedState){
    /* Set State*/
    reedState = true;
    state = defaultState;

    changeState(defaultState);
  } else {
    reedState = false;
  }
}



void changeState(int newState){
  if(minState > newState < maxState){//test if this works
    state = newState;
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, datArray[++state]);
    digitalWrite(latchPin, HIGH);
  }
}



boolean button(byte i)         // geeft DIRECT EENMALIG een '1' als knop i ingedrukt wordt
{                              // knop i moet 50 ms los zijn voordat een nieuwe '1' gegeven kan worden 
                               // in dit voorbeeld is bereik i: [0..3]

  const  unsigned long debounce  =  50;
  static unsigned long buttonTime[sizeof(buttonPin)];   // static array!!
  static boolean       buttonFlg[sizeof(buttonPin)];    // static array!!

  if (!digitalRead(buttonPin[i]))
  {
    buttonTime[i]=millis();
    if (!buttonFlg[i])
    {
      buttonFlg[i]=1;
      return(1);
    }
  }
  else  
  {
    if ((millis()-buttonTime[i])>debounce) buttonFlg[i]=0;
  }
  return(0);
}

/*-----------------------------------------------------------------------------------------------------*/
/*- Unused Functions ----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------*/

/* Unused Loop Function */
void segmentDisplayLoop(){
  /* loop from 0 to 9 */
  for(int num = 0; num < 10; num++)
  {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, datArray[num]);
    digitalWrite(latchPin, HIGH);
    delay(1000); //wait for a second
  }
}
