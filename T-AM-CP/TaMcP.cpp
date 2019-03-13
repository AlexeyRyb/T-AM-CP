#include "TaMcP.h"

TaMcP::TaMcP(int pinLeftIn[5], int pinRightIn[5])
:
    _caterpillarLeft(pinLeftIn),
    _caterpillarRight(pinRightIn),
    _spdMode(false),
    _done(false),
    _syncrhonizationMode(true)

{

}

void TaMcP::setMoveSpd(double spdLeftIn, double spdRightIn)
{

    if ( _caterpillarLeft.setSpd(spdLeftIn) && _caterpillarRight.setSpd(spdRightIn) )
    {
        _spdMode = true;
    }
    else
    {
        Serial.println("Error: input data not correct(speed mode)");
    }
}

void TaMcP::setMoveSpdAndDist(double spdLeftIn, double distLeftIn,
                                double spdRightIn, double distRightIn, bool synchronization)
{

    if ( _caterpillarLeft.setSpdAndDist(spdLeftIn, distLeftIn) &&
         _caterpillarRight.setSpdAndDist(spdRightIn, distRightIn) )
    {
        _spdMode = false;
        _done = true;
        _syncrhonizationMode = synchronization;
    }
    else
    {
        Serial.println("Error: input data not correct(speed and distance mode)");
    }
}

void TaMcP::setCoefEncoder(double coefLeftDownIn, double coefLeftUpIn,
                            double coefRightDownIn, double coefRightUpIn)
{

    if ( !_caterpillarLeft.setCoef(coefLeftDownIn, coefLeftUpIn) ||
         !_caterpillarRight.setCoef(coefRightDownIn, coefRightUpIn) )
    {
        Serial.println("Error: coef encoder not correct");
    }
}

void TaMcP::updateInternalDataT() {

    _caterpillarLeft.updateInternalData();
    _caterpillarRight.updateInternalData();

    if (_done)
    {

        if (_syncrhonizationMode)
        {

            if (_caterpillarRight.isDistReached())
            {
                _caterpillarLeft.setSpd(0);
                _done = false;
                Serial.println("done");
            }

            if (_caterpillarLeft.isDistReached() && _done)
            {
                _caterpillarRight.setSpd(0);
                _done = false;
                Serial.println("done");
            }

        }
        else
        {

            if (_caterpillarRight.isDistReached() && _caterpillarLeft.isDistReached())
            {
                _done = false;
                Serial.println("done");
            }
        }

    }
}

Array<double, 4> TaMcP::getCurrentPositionT()
{

    Array<double, 2> positionLeft = _caterpillarLeft.getCurrentPosition();
    Array<double, 2> positionRight = _caterpillarRight.getCurrentPosition();
    Array<double, 4> position;

    position.at(0) = positionLeft.at(0);
    position.at(1) = positionLeft.at(1);
    position.at(2) = positionRight.at(0);
    position.at(3) = positionRight.at(1);
    return position;
}

