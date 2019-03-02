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
  tankAM.moveT(0, 0);
} 

void loop() 
{ 
  
   if (Serial.available() > 0)
   {
      int ctrl = Serial.parseInt();
      
      if (ctrl == 1)
      {
        int leftSpd = Serial.parseInt();
        int rightSpd = Serial.parseInt();

        tankAM.moveT(leftSpd, rightSpd);
      }

      if (ctrl == 2)
      {
        int leftSpd = Serial.parseInt();
        int rightSpd = Serial.parseInt();

        tankAM.setSpdT(leftSpd, rightSpd);
      }

      if (ctrl == 3)
      {

        bool sideMove = Serial.parseInt();
        int delt = Serial.parseInt();

        if (delt > 0)
        {
          tankAM.moveFast(sideMove, delt);
        }
        else
        {
          tankAM.moveSlow(sideMove, -delt);
        }
        
      }

      if (ctrl == 4)
      {
        bool stopMode = Serial.parseInt();

        tankAM.setStopMoveMode(stopMode);
        
      }

      if (ctrl == 5)
      {
        bool reverseMode = Serial.parseInt();

        tankAM.setReverse(reverseMode);
      }

      if (ctrl == 6)
      {
        int msec = Serial.parseInt();

          tankAM.setMsecStop(msec);
      }
      
      
   }
}
