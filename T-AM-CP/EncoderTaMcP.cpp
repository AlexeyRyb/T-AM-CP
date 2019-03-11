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
    _coefTomm(0.7)
{
}

void encoderTaMcP::setCoef(double coefIn)
{
    if (coefIn > 0)
    {
        _coefTomm = coefIn;
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

void encoderTaMcP::checkDist(bool moving)
{

    _newPosition = _encoder.read();

   // Serial.println(_newPosition);

    if (_newPosition != _oldPosition)
    {

        if (moving)
        {
            _stepTime = micros() - _prevTime;
            _prevTime = micros();
            _spd = _coefTomm / (_stepTime / 1e6);
        }
        else
        {
            _spd = 0.0;
        }

        _oldPosition = _newPosition;
       _dist += _coefTomm;
    }

}

