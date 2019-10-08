const int latchPin = 12; //Pin connected to ST_CP of 74HC595
const int clockPin = 8; //Pin connected to SH_CP of 74HC595 
const int dataPin = 11; //Pin connected to DS of 74HC595 
/*=======================================================================================================
//display 0,1,2,3,4,5,6,7,8
Number 0 :  01111101     125
Number 1 :  00110000     48
Number 2 :  01101110     110
Number 3 :  01111010     122
Number 4 :  00110011     51
Number 5 :  01011011     91
Number 6 :  01011111     95
Number 7 :  01110000     112
Number 8 :  01111111     127
Number 9 :  01111011     123

To display the dot, simply add 128.
*/
/*In order to display zero your array has decimal number 126. However using the shift register
it is actuall the binary number that is uploaded into the resitsre: zie the table above for zero: 01111110. 
*/
byte datArray[10] = {125, 48, 110, 122, 51, 91, 95, 112, 127, 123}; // array without the decimal point

byte floorNumber = 0;



const byte reedPin = 2;
boolean reedState = false;

const byte buttonDownPin = 3;
const byte buttonUpPin = 4;
boolean buttonDownState = false;
boolean buttonUpState = false;
const byte ledDownPin = 5;
const byte ledUpPin = 6;



void setup ()
{
  //set pins to output

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(reedPin, INPUT_PULLUP);

  pinMode(buttonDownPin, INPUT_PULLUP);
  pinMode(buttonUpPin, INPUT_PULLUP);
  pinMode(ledDownPin, OUTPUT);
  pinMode(ledUpPin, OUTPUT);

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, datArray[floorNumber]);
  digitalWrite(latchPin, HIGH);
}


void loop()
{
	/*
  //loop from 0 to 9
  for(int num = 0; num < 10; num++)
  {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, LSBFIRST, datArray[num]);
    digitalWrite(latchPin, HIGH);
    delay(1000); //wait for a second
  }
	*/


  if (!digitalRead(reedPin)){
    if (!reedState){
      reedState = true;
      // Set value
      floorNumber = 0;
      displayNumber();
    }
  } else {
    reedState = false;
  }
  
	if (!digitalRead(buttonDownPin)){
		if (!buttonDownState){
			buttonDownState = true;
			// Count down
			if (floorNumber > 0){
        floorNumber--;
				displayNumber();
			}
		}
	} else {
		buttonDownState = false;
	}

	if (!digitalRead(buttonUpPin)){
		if (!buttonUpState){
			buttonUpState = true;
			// Count up
			if (floorNumber < 9){
        floorNumber++;
				displayNumber();
			}
		}
	} else {
		buttonUpState = false;
	}
	
  // LED
	digitalWrite(ledDownPin, !digitalRead(buttonDownPin));
	digitalWrite(ledUpPin, !digitalRead(buttonUpPin));
}

void displayNumber(){
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, datArray[floorNumber]);
  digitalWrite(latchPin, HIGH);
}
