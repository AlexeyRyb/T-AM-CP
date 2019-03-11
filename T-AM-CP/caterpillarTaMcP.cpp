#include "caterpillarTaMcP.h"

caterpillarTaMcP::caterpillarTaMcP(int pins[5])
:
    _pinDown(pins[0]),
    _pinUp(pins[1]),
    _pinSpd(pins[2]),
    _encoderTaMcP(pins[3],pins[4]),
    _distNow(0),
    _distNeed(0),
    _spdNow(0),
    _spdNeed(0),
    _maxSpd(400.0),
    _maxSpdPin(255.0),
    _epsDist(20.0),
    _epsSpd(10.0),
    _spdMin(1.0),
    _sendSpd(0.0),
    _numberOfStaps(500.0),
    _moving(false),
    _spdMode(false),
    _coefDown(0.7),
    _coefUp(0.7)

{
    pinMode(_pinDown, OUTPUT);
    pinMode(_pinUp, OUTPUT);
    pinMode(_pinSpd, OUTPUT);
}

void caterpillarTaMcP::_moveUp()
{
    _encoderTaMcP.setCoef(_coefUp);
    digitalWrite(_pinUp, true);
    digitalWrite(_pinDown, false);
}

void caterpillarTaMcP::_moveBack()
{
    _encoderTaMcP.setCoef(_coefDown);
    digitalWrite(_pinUp, false);
    digitalWrite(_pinDown, true);
}

void caterpillarTaMcP::_stopMove()
{
    digitalWrite(_pinUp, false);
    digitalWrite(_pinDown, false);
    _encoderTaMcP.resetDist();
    _distNeed = 0.0;
    _moving = false;
}

void caterpillarTaMcP::_changeDistAndSpd()
{
    _encoderTaMcP.checkDist(_moving);
    _distNow = _encoderTaMcP.getDist();
    _spdNow = _encoderTaMcP.getSpd();

   // Serial.print(_distNow);
   // Serial.print("    ");
   // Serial.println(_spdNow);

}

void caterpillarTaMcP::setSpd(double spdIn)
{
    _spdMode = true;
    if  ((spdIn >= -_maxSpd) && (spdIn <= _maxSpd))
    {
        _spdNeed = spdIn;
    }
    _encoderTaMcP.resetDist();
}

void caterpillarTaMcP::setSpdAndDist(double spdIn, double distIn)
{

    _spdMode = false;

    if  ((spdIn >= -_maxSpd) && (spdIn <= _maxSpd))
    {
        _spdNeed = spdIn;
    }

    if  (distIn >= 0)
    {
        _distNeed = distIn;
    }
    _encoderTaMcP.resetDist();

}

bool caterpillarTaMcP::_isDistReached()
{
    //Serial.println( abs(_distNeed - _distNow) );
    return abs(_distNeed - _distNow) <= _epsDist;
}

void caterpillarTaMcP::_standSpd()
{

    double delta = abs(_spdNeed) - _spdNow;
    Serial.print("delta = ");
    Serial.println(delta);
    if (abs(delta) > _epsSpd)
    {
        _moving = true;
        _sendSpd += (_maxSpdPin / _maxSpd) * delta / _numberOfStaps;  // 200 - maxSpeed 2.0 - koef PID

        if (_sendSpd < 0.)
        {
            _sendSpd = _spdMin;
        }

        if (_sendSpd > _maxSpdPin)
        {
            _sendSpd = _maxSpdPin;
        }
        _spdNeed > 0. ? _moveUp() : _moveBack();
        analogWrite(_pinSpd, _sendSpd);
    }
}

void caterpillarTaMcP::setCoef(double coefDownIn, double coefUpIn)
{
    if (coefDownIn > 0)
    {
        _coefDown = coefDownIn;
    }

    if (coefUpIn > 0)
    {
        _coefUp = coefUpIn;
    }
}

void caterpillarTaMcP::checkMove()
{

    _changeDistAndSpd();

    if (!_spdMode && _isDistReached())
    {
        _stopMove();
    }
    else
    {
        //_standSpd();

        if (abs(_spdNeed)  > _spdMin)
        {
            _spdNeed > 0. ? _moveUp() : _moveBack();
            analogWrite(_pinSpd, abs(_spdNeed)*(_maxSpdPin/_maxSpd));
        }
        else
        {
            _stopMove();
        }
    }

    Serial.print(_distNow);


}

Array <double, 2> caterpillarTaMcP::currentPosition()
{

    _changeDistAndSpd();

    Array<double, 2> current;

    current.at(0) = _spdNow;
    current.at(1) = _distNow;

    return current;

}