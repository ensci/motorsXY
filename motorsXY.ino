#include <rotary.h>

#define INPUT_A 8 
#define INPUT_B 9
#define INPUT_C 10
#define INPUT_D 11

Rotary r = Rotary(2, 3);
Rotary r2 = Rotary(4, 5);

volatile int posX, posY;

int X=A0;
int Y=A1;
int valeurX = 0;
int valeurY = 0;

char mem[5];


void setup (){
  Serial.begin(9600);
  Serial.println("start...");

  // déclarer les pins comme sorties numériques
  pinMode(INPUT_A, OUTPUT);
  pinMode(INPUT_B, OUTPUT);
  pinMode(INPUT_C, OUTPUT);
  pinMode(INPUT_D, OUTPUT);


  PCICR |= (1 << PCIE2) | (1 << PCIE1);
  PCMSK2 |= (1 << PCINT18) | (1 << PCINT19) | (1 << PCINT20) | (1 << PCINT21);
  sei();

}

// loop est vide car tous ce passe dans les callbacks, serialEvent - quand qqch arrive dans le port série et ISR quand un encodeur 
// enregistre changement

void loop (){


}

/// serial callback
void serialEvent(){
// lire le port série jusqu'à le caractere 'e' et stocker les valeurs dans la liste mem[]
// découper en 5 bytes maximum, si plus que 5 bytes est envoyé il reécrit dans la liste
// donc envoter le caractere 'e' est cruciale

  Serial.readBytesUntil('e', mem, 5);
// ici FIFO est vidé

// voir ce qu'il a reçu
//  for (int i=0; i<5; i++) 
//    Serial.println(mem[i]);

// conversion des caracteres en integers
// le premier caractere est une lettre, donc il faut créer une nouvelle liste sans
// le premiere caractere
  char nmbr[4]; // nouvelle liste qui va contenir que des chiffres
  for (int i=0; i<4; i++) 
    nmbr[i] = mem[i+1];
  
  int number = atoi(nmbr); // fonction qui traduit les caracteres en integers
  
  Serial.print("received ");
  Serial.println(number);


  for (int i=0; i<5; i++) 
    mem[i]=0;



  /*
  int val = Serial.read(); 
   
   switch(val) {
   
   case 'x' :
   avanceMotor('x');
   break;
   
   case 'y' :
   avanceMotor('y');
   break;
   
   case 's' :
   
   stopMotor('x');
   stopMotor('y');
   break;
   
   }
   */
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

void RAZ() {

  /// au debut va doucement dans la direction de l'interrupteur
  /*
 /// allume le moteur
   while(interrupteurX!=1) {}
   posX = 0;
   
   while(interrupteurY!=1) {}
   posY = 0;
   */
}

void avanceMotor(int motorXY) { // enter 'X' or 'Y'
  if ((motorXY=='X') || (motorXY=='x')) {
    digitalWrite(INPUT_A, HIGH); 
    digitalWrite(INPUT_B, LOW);  
  } 
  else if ((motorXY=='Y') || (motorXY=='y')) {
    digitalWrite(INPUT_C, HIGH); 
    digitalWrite(INPUT_D, LOW);
  }

}

void reculeMotor(int motorXY) {
  if ((motorXY=='X') || (motorXY=='x')) {
    digitalWrite(INPUT_A, LOW); 
    digitalWrite(INPUT_B, HIGH);  
  } 
  else if ((motorXY=='Y') || (motorXY=='y')) {
    digitalWrite(INPUT_C, LOW); 
    digitalWrite(INPUT_D, HIGH);
  }
}

void stopMotor(int motorXY) {
  if ((motorXY=='X') || (motorXY=='x')) {
    digitalWrite(INPUT_A, LOW); 
    digitalWrite(INPUT_B, LOW);  
  } 
  else if ((motorXY=='Y') || (motorXY=='y')) {
    digitalWrite(INPUT_C, LOW); 
    digitalWrite(INPUT_D, LOW);
  }
}










