#include <time.h>
#include <TaMcP.h>

const int brakeLeft = 9;
const int dirLeft = 12;
const int brakeRight = 8;
const int dirRight = 13;
const int speedLeft = 11;
const int speedRight = 3;

int pins[6] = {brakeLeft, dirLeft, brakeRight, dirRight, speedLeft, speedRight};

const int moveUp =  3;
const int moveDown = 6;
const int right =   2;
const int left = 4;
const int reverseMode = 7;

const int check = 5;

TaMcP tank = TaMcP(pins);

void setup() 
{ 
  
  Serial.begin(115200); 
  Serial.setTimeout(100); 
  
  pinMode(moveUp, INPUT);
  pinMode(moveDown, INPUT);
  pinMode(right, INPUT);
  pinMode(left, INPUT);
  pinMode(reverseMode, INPUT);
  pinMode(check, OUTPUT);
  
  Serial.println("Ready");
  tank.setStopMoveMode();
} 

void loop() 
{ 
   int valUp = digitalRead(moveUp);
   int valBack = digitalRead(moveDown);
   int valRight = digitalRead(right);
   int valLeft = digitalRead(left);
   int valReverse = digitalRead(reverseMode);

   analogWrite(check, 255);

   tank.setReverse(valReverse);

   if ( valUp && !valBack && !valRight && !valLeft )
   {
    Serial.println("move up");
    tank.moveUp();
   }

   if ( valBack && !valUp && !valRight && !valLeft)
   {
    Serial.println("move back");
    tank.moveBack();
   }

   if ( valRight && !valBack && !valUp && !valLeft )
   {
    Serial.println("right");
    tank.right();
   }

   if ( valLeft && !valBack && !valRight && !valUp )
   {
    Serial.println("left");
    tank.left();
   }

   if ( valLeft && valBack  && !valRight && !valUp )
   {
    Serial.println("move back and left");
    tank.moveBackLeft();
   }

   if ( valLeft && valUp && !valRight && !valBack )
   {
    Serial.println("move up left");
    tank.moveUpLeft();
   }

   if ( valRight && valBack  && !valLeft && !valUp )
   {
    Serial.println("move back and right");
    tank.moveBackRight();
   }

   if ( valRight && valUp && !valLeft  && !valBack )
   {
    Serial.println("move up and right");
    tank.moveUpRight();
   }

   if ( !valRight && !valUp && !valLeft && !valBack )
   {
    Serial.println("stop move");
    tank.stopMove();
   }

}
