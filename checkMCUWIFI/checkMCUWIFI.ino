
#include <time.h>


#include "TaMcP.h"

const int downLeft = 50;
const int upLeft = 51;
const int downRight = 53;
const int upRight = 52;

const int spdLeft = 7;
const int spdRight = 6;

const int encoderRight1 = 18;
const int encoderRight2 = 19;
const int encoderLeft1 = 20;
const int encoderLeft2 = 21;

int pinsLeft[5] = {downLeft, upLeft, spdLeft, encoderLeft1, encoderLeft2};
int pinsRight[5] = {downRight, upRight, spdRight, encoderRight1, encoderRight2};

double coefLeftDown = 1.08168;
double coefLeftUp = 0.545555;
double coefRightDown = 0.6475;
double coefRightUp = 0.736462;

TaMcP tank(pinsLeft, pinsRight);

void setup() 
{ 
  
  Serial.begin(115200); 
  Serial.setTimeout(10);  
  tank.setCoefEncoder(coefLeftDown, coefLeftUp, coefRightDown, coefRightUp);
  Serial.println("Ready");
  
} 

void loop() 
{ 

  if (Serial.available() > 0)
  {
    int ctrl = Serial.parseInt();

    if (ctrl == 1)
    {
      double spd = Serial.parseFloat();
        
      tank.setMoveSpd(spd, spd);
    }

    if (ctrl == 2)
    {
      double spd = Serial.parseFloat();
      double dist = Serial.parseFloat();
      
      tank.setMoveSpdAndDist(spd, dist, spd, dist, false);
    }
  }

  tank.updateInternalDataT();

  Array<double, 4> positionNow = tank.getCurrentPositionT();

  Serial.print("Right:    ");
  Serial.print(positionNow.at(0));
  Serial.print("  ");
  Serial.print(positionNow.at(1));
  Serial.print("    Left:    ");
  Serial.print(positionNow.at(2));
  Serial.print("  ");
  Serial.println(positionNow.at(3));
}
