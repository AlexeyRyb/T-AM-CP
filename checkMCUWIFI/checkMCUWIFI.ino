
#include <time.h>


#include "caterpillarTaMcP.h"

const int downLeft = 22;
const int upLeft = 23;
const int downRight = 24;
const int upRight = 25;
const int spdLeft = 7;
const int spdRight = 6;

const int encoderRight1 = 18;
const int encoderRight2 = 19;
const int encoderLeft1 = 20;
const int encoderLeft2 = 21;

int pinsLeft[5] = {downLeft, upLeft, spdLeft, encoderLeft1, encoderLeft2};
int pinsRight[5] = {downRight, upRight, spdRight, encoderRight1, encoderRight2};

Array <double, 2> curPos;

double coefLeftDown = 1.08168;
double coefLeftUp = 0.545555;
double coefRightDown = 0.6475;
double coefRightUp = 0.736462;
caterpillarTaMcP caterpillarLeft(pinsLeft);
caterpillarTaMcP caterpillarRight(pinsRight);

void setup() 
{ 
  
  Serial.begin(115200); 
  Serial.setTimeout(10); 
  caterpillarRight.setCoef(coefRightDown, coefRightUp);
  caterpillarLeft.setCoef(coefLeftDown, coefLeftUp);
  
} 

void loop() 
{ 

  if (Serial.available() > 0)
  {
    int ctrl = Serial.parseInt();

    if (ctrl == 1)
    {
        double spd = Serial.parseFloat();
        caterpillarLeft.setSpd(spd);
        caterpillarRight.setSpd(spd);
    }

    if (ctrl == 2)
    {
      double spd = Serial.parseFloat();
      double dist = Serial.parseFloat();
      caterpillarLeft.setSpdAndDist(spd, dist);
      caterpillarRight.setSpdAndDist(spd, dist);
    }
  }
  Serial.print("Left: ");
  caterpillarLeft.checkMove();
 Serial.print("Right: ");
 caterpillarRight.checkMove();
  Serial.println();
  /*curPos = caterpillarLeft.currentPosition();
  Serial.print(curPos[0]);
  Serial.print("  ");
  Serial.println(curPos[1]);*/
  
  
}
