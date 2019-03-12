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
    _minSpd(10),
    _maxSpd(400.0),
    _maxValuePin(255.0),
    _epsDist(20.0),
    _epsSpd(10.0),
    _sendSpd(0.0),
    _numberOfSteps(500.0),
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

void caterpillarTaMcP::_moveDown()
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

void caterpillarTaMcP::_updateDistAndSpdEncoder()
{
    _encoderTaMcP.updateStatusEncoder(_moving);
    _distNow = _encoderTaMcP.getDist();
    _spdNow = _encoderTaMcP.getSpd();
}

void caterpillarTaMcP::setSpd(double spdIn)
{

    _spdMode = true;

    if  (abs(spdIn) <= _maxSpd)
    {
        _spdNeed = spdIn;
    }

    _encoderTaMcP.resetDist();

}

void caterpillarTaMcP::setSpdAndDist(double spdIn, double distIn)
{

    _spdMode = false;

    if  (abs(spdIn) <= _maxSpd)
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
    return abs(_distNeed - _distNow) <= _epsDist;
}

void caterpillarTaMcP::_standSpd()
{

    if (abs(_spdNeed) < _minSpd)
    {
        _stopMove();
    }
    else
    {

        double delta = abs(_spdNeed) - _spdNow;

        if (abs(delta) > _epsSpd) {

            _moving = true;
            _sendSpd += (_maxValuePin / _maxSpd) * delta / _numberOfSteps;  // 200 - maxSpeed 2.0 - koef PID

            if (_sendSpd < 0.) {
                _sendSpd = _minSpd;
            }

            if (_sendSpd > _maxValuePin) {
                _sendSpd = _maxValuePin;
            }

            _spdNeed > 0. ? _moveUp() : _moveDown();
            analogWrite(_pinSpd, _sendSpd);

        }

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

void caterpillarTaMcP::updateInternalData()
{

    _updateDistAndSpdEncoder();

    if (!_spdMode && _isDistReached())
    {
        _stopMove();
    }
    else
    {
        _standSpd();

    }

}

Array <double, 2> caterpillarTaMcP::getCurrentPosition()
{

    Array<double, 2> current;

    current.at(0) = _spdNow;
    current.at(1) = _distNow;

    return current;

}