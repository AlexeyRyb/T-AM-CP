#include <time.h>
#include <controlTaMcP.h>

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

controlTaMcP tankAM = controlTaMcP(pins);

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
  tankAM.setStopMoveMode();
} 

void loop() 
{ 
   int valUp = digitalRead(moveUp);
   int valBack = digitalRead(moveDown);
   int valRight = digitalRead(right);
   int valLeft = digitalRead(left);
   int valReverse = digitalRead(reverseMode);

   analogWrite(check, 255);

   int spdPlus = 1023;
   int spdMinus = -1023;
   
   tankAM.setReverse(valReverse);

   if ( valUp && !valBack && !valRight && !valLeft )
   {
    Serial.println("move up");
    tankAM.moveT(spdPlus, spdPlus);
   }

   if ( valBack && !valUp && !valRight && !valLeft)
   {
    Serial.println("move down");
    tankAM.moveT(spdMinus, spdMinus);
   }

   if ( valRight && !valBack && !valUp && !valLeft )
   {
    Serial.println("right");
    tankAM.moveT(spdPlus, spdMinus);
   }

   if ( valLeft && !valBack && !valRight && !valUp )
   {
    Serial.println("left");
    tankAM.moveT(spdMinus, spdPlus);
   }

   if ( valLeft && valBack  && !valRight && !valUp )
   {
    Serial.println("move down and left");
    tankAM.moveT(0, spdMinus);
   }

   if ( valLeft && valUp && !valRight && !valBack )
   {
    Serial.println("move up left");
    tankAM.moveT(0, spdPlus);
   }

   if ( valRight && valBack  && !valLeft && !valUp )
   {
    Serial.println("move down and right");
    tankAM.moveT(spdMinus, 0);
   }

   if ( valRight && valUp && !valLeft  && !valBack )
   {
    Serial.println("move up and right");
    tankAM.moveT(spdPlus, 0);
   }

   if ( !valRight && !valUp && !valLeft && !valBack )
   {
    Serial.println("stop move");
    tankAM.moveT(0, 0);
   }

}
