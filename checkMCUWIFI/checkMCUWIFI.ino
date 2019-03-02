#include <time.h>
#include <controlTaMcP.h>

const int brakeLeft = 9;
const int dirLeft = 12;
const int brakeRight = 8;
const int dirRight = 13;
const int speedLeft = 11;
const int speedRight = 3;

int pins[6] = {brakeLeft, dirLeft, brakeRight, dirRight, speedLeft, speedRight};

controlTaMcP tankAM = controlTaMcP(pins);

void setup() 
{ 
  
  Serial.begin(115200); 
  Serial.setTimeout(100); 
  
  Serial.println("Ready");
  tankAM.setMaxSpd(255);
} 

void loop() 
{ 
  
   if (Serial.available() > 0)
   {
      int leftSpd = Serial.parseInt();
      int rightSpd = Serial.parseInt();

      tankAM.moveT(leftSpd, rightSpd);
   }
}
