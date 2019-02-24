#include <time.h>
#include <TaMcP.h>


int pins[6] = {D5, D6, D7, D8, D1, D2};
const int moveUp =  9;
const int moveDown = 10;
const int right =   13;
const int left = 14;
const int reverse = 11;

TaMcP tank = TaMcP(pins);

void setup() 
{ 
  
  Serial.begin(115200); 
  Serial.setTimeout(100); 
  
  pinMode(moveUp, INPUT);
  pinMode(moveDown, INPUT);
  pinMode(right, INPUT);
  pinMode(left, INPUT);
  pinMode(reverse, INPUT);
  
  Serial.println("Ready");
  tank.setStopMoveMode();
} 

void loop() 
{ 
   int valUp = analogRead(moveUp);
   int valBack = analogRead(moveDown);
   int valRight = analogRead(right);
   int valLeft = analogRead(left);
   int valReverse = digitalRead(reverse);

   tank.setReverse(valReverse);

   if ( (valUp > 0) && (valBack == 0) && (valRight == 0) && (valLeft == 0))
   {
    
    tank.setSpd(valUp);
    tank.moveUp();
  
   }

   if ( (valBack > 0) && (valUp == 0) && (valRight == 0) && (valLeft == 0))
   {
    tank.setSpd(valBack);
    tank.moveBack();
   }

   if ( (valRight > 0) && (valBack == 0) && (valUp == 0) && (valLeft == 0))
   {
    tank.setSpd(valRight);
    tank.right();
   }

   if ( (valLeft > 0) && (valBack == 0) && (valRight == 0) && (valUp == 0))
   {
    tank.setSpd(valLeft);
    tank.left();
   }

   if ( (valLeft > 0) && (valBack > 0) && (valRight == 0) && (valUp == 0))
   {
    tank.setSpd( (valLeft + valBack) / 2);
    tank.moveBackLeft();
   }

   if ( (valLeft > 0) && (valUp > 0) && (valRight == 0) && (valBack == 0))
   {
    tank.setSpd( (valLeft + valUp) / 2);
    tank.moveUpLeft();
   }

   if ( (valRight > 0) && (valBack > 0) && (valRight == 0) && (valUp == 0))
   {
    tank.setSpd( (valRight + valBack) / 2);
    tank.moveBackRight();
   }

   if ( (valRight > 0) && (valUp > 0) && (valRight == 0) && (valUp == 0))
   {
    tank.setSpd( (valRight + valUp) / 2);
    tank.moveUpRight();
   }

   if ( (valRight == 0) && (valUp == 0) && (valRight == 0) && (valUp == 0))
   {
    tank.stopMove();
   }

}
