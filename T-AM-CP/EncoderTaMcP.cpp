//
// Created by Алексей Рыбалкин on 07.03.2019.
//
#include "EncoderTaMcP.h"

encoderTaMcP::encoderTaMcP(int encoderIn1, int encoderIn2)
:
    _encoder(encoderIn1, encoderIn2),
    _dist(0.0),
    _oldPosition(-999),
    _prevTime(micros()),
    _spd(0),
    _OneStepToMm(0.7)
{
}

//Need to change the coefficient on the move, as they are different for moving forward and backward
void encoderTaMcP::setCoef(double coefIn)
{
    if (coefIn > 0)
    {
        _OneStepToMm = coefIn;
    }
}
double encoderTaMcP::getDist()
{
    return _dist;
}

double encoderTaMcP::getSpd()
{
    return _spd;
}
void encoderTaMcP::resetDist()
{
    _dist = 0.0;
}

void encoderTaMcP::updateStatusEncoder(bool moving)
{

    _newPosition = _encoder.read();

    if (_newPosition != _oldPosition)
    {

        _stepTime = micros() - _prevTime;
        _prevTime = micros();
        _spd = _OneStepToMm / (_stepTime / 1e6);

        _oldPosition = _newPosition;
       _dist += _OneStepToMm;

    }

    if (!moving)
    {
        _spd = 0.0;
    }

}

