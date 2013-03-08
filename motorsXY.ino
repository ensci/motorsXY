// find excellent rotary library under https://github.com/brianlow/Rotary
#include <rotary.h>

// define input pins for LD293D
#define INPUT_A 8 
#define INPUT_B 9
#define INPUT_C 10
#define INPUT_D 11

// create two rotary objects
Rotary r = Rotary(2, 3);
Rotary r2 = Rotary(4, 5);

// this is current position for motors
volatile int posX, posY;

int targetX, targetY;

void setup (){
  Serial.begin(9600);
  Serial.println("start...");

  // declare inputs for LD293D
  pinMode(INPUT_A, OUTPUT);
  pinMode(INPUT_B, OUTPUT);
  pinMode(INPUT_C, OUTPUT);
  pinMode(INPUT_D, OUTPUT);

  // settings for rotary encoders
  PCICR |= (1 << PCIE2) | (1 << PCIE1);
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21);
  sei();

}


void loop (){


}

#define MAX_RCV_BYTES 5 // define length of messages, for example x1234 is 5 bytes long 

/// serial handler
void serialEvent(){
  // Serial handler is activates when there is something in serial port
  // idea is to send coordinates in format :
  // x100e
  // y2452e
  // s
  // letter 'e' is for end, that's delimiter, s - is for stop
  // 
  // read serial port until letter 'e' than put infos in mem array
  char mem[MAX_RCV_BYTES];

  Serial.readBytesUntil('e', mem, MAX_RCV_BYTES);
  // after this FIFO will be empty

  // print contents of mem
  //  for (int i=0; i<MAX_RCV_BYTES; i++) 
  //    Serial.println(mem[i]);

  // Convert from characters to integers
  // first caracter is letter so create a new list without him
  char nmbr[MAX_RCV_BYTES-1]; // new array only for numbers
  for (int i=0; i<MAX_RCV_BYTES-1; i++) 
    nmbr[i] = mem[i+1];

  int number = atoi(nmbr); // convert caracters to numbers

  Serial.print("received "); // print what is received
  Serial.println(number); 

  switch(mem[0]) {

  case 'x' :
    targetX = number;
    break;

  case 'y' :
    targetY = number;
    break;

  case 's' : // stop
    
    break;

  case 'r' : // reset to initial position

    break;


  }

}

ISR(PCINT2_vect) {
  char result = r.process();
  char result2 = r2.process();
  if (result) {

    if (result == DIR_CW) {
      posX++; 
    } 
    else {
      posX--;
    }
    Serial.print(" X ");
    Serial.println(posX);
  }

  if (result2) {
    if (result2 == DIR_CW) {
      posY++; 
    } 
    else {
      posY--;
    }
    Serial.print(" Y ");
    Serial.println(posY);

    //Serial.println(result2 == DIR_CW ? "2Right" : "2Left");
  }
}

void motorForward(int motorXY) { // enter 'X' or 'Y'
  if ((motorXY=='X') || (motorXY=='x')) {
    digitalWrite(INPUT_A, HIGH); 
    digitalWrite(INPUT_B, LOW);  
  } 
  else if ((motorXY=='Y') || (motorXY=='y')) {
    digitalWrite(INPUT_C, HIGH); 
    digitalWrite(INPUT_D, LOW);
  }

}

void motorBackward(int motorXY) {
  if ((motorXY=='X') || (motorXY=='x')) {
    digitalWrite(INPUT_A, LOW); 
    digitalWrite(INPUT_B, HIGH);  
  } 
  else if ((motorXY=='Y') || (motorXY=='y')) {
    digitalWrite(INPUT_C, LOW); 
    digitalWrite(INPUT_D, HIGH);
  }
}

void motorStop(int motorXY) {
  if ((motorXY=='X') || (motorXY=='x')) {
    digitalWrite(INPUT_A, LOW); 
    digitalWrite(INPUT_B, LOW);  
  } 
  else if ((motorXY=='Y') || (motorXY=='y')) {
    digitalWrite(INPUT_C, LOW); 
    digitalWrite(INPUT_D, LOW);
  }
}






