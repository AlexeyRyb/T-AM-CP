#ifndef TaMcP_H
#define TaMcP_H

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "caterpillarTaMcP.h"
#include "defines.h"

String timePrint();

class TaMcP
{
private:

    caterpillarTaMcP _caterpillarLeft;
    caterpillarTaMcP _caterpillarRight;

    bool _spdMode;
    bool _syncrhonizationMode;
    bool _done;



public:

    TaMcP(int pinLeftIn[5], int pinRightIn[5]);

    void setMoveSpd(double spdLeftIn, double spdRightIn);

    void setMoveSpdAndDist(double spdLeftIn, double distLeftIn,
                            double spdRightIn, double distRightIn, bool synchronization);

    void setCoefEncoder(double coefLeftDownIn, double coefLeftUpIn,
                        double coefRightDownIn, double coefRightUpIn);

    void updateInternalDataT();

    Array<double, 4> getCurrentPositionT();

};

#endif
