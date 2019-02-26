#include "TaMcP.h"

String timePrint()
{
    String timeStr = "";
    int timeNow = millis() / 1000;

    if (timeNow / 60 / 60 < 10)
    {
        timeStr +="0";
    }

    timeStr += String(timeNow / 60 / 60);

    timeStr += ":";
    if ( (timeNow / 60) % 60 < 10)
    {
        timeStr += "0";
    }

    timeStr += String( (timeNow / 60) % 60);
    timeStr += ":";
    if (timeNow % 60 < 10)
    {
        timeStr += "0";
    }

    timeStr += String(timeNow % 60);

    return timeStr;
}


TaMcP::TaMcP(int pins[6])
:
    _brakeLeft(pins[0]),
    _dirLeft(pins[1]),
    _brakeRight(pins[2]),
    _dirRight(pins[3]),
    _speedRight(pins[4]),
    _speedLeft(pins[5]),
    _spd(1023),
    _reverseMove(false),
    _stopMoveMode(true),
    _maxSpd(1023)

{
    for (int i = 0; i < _numLog; i++)
    {
        if (i == 0)
        {
            _statusStr[i] = timePrint() + "  Start";
        }
        else
        {
            _statusStr[i] = "";
        }
    }

    #ifdef TaMcP_debug
        Serial.println(timePrint()+" TaMcP init");
        Serial.print("Pins: ");
        for (int i = 0; i < 6; i++)
        {
            Serial.print(pins[i]);
            Serial.print(" ");
        }
        Serial.println();
        Serial.print("Speed: ");
        Serial.println(_spd);

    #endif
    pinMode(_dirRight, OUTPUT);
    pinMode(_dirLeft, OUTPUT);
    pinMode(_brakeLeft, OUTPUT);
    pinMode(_brakeRight, OUTPUT);
    pinMode(_speedRight, OUTPUT);
    pinMode(_speedLeft, OUTPUT);
    analogWrite(_speedRight, _spd);
    analogWrite(_speedLeft, _spd);

}

void TaMcP::statusChange(String newStatus)
{

    for(int i = _numLog - 1; i > 0; i--)
    {
        _statusStr[i] = _statusStr[i-1];
    }

    _statusStr[0] = timePrint() + "  " + newStatus;

}

void TaMcP::setReverse(bool statusReverse)
{
        _reverseMove = statusReverse;
}

void TaMcP::setSpd(int speedIn)
{
    if ((speedIn > 0) && (speedIn <= _maxSpd)) 
    {
        _spd = speedIn;
    }

    analogWrite(_speedRight, _spd);
    analogWrite(_speedLeft, _spd);
    statusChange("Speed change speed = " + String(_spd * 100.0 / _maxSpd) + "%");

}

void TaMcP::setMaxSpd(int speedIn)
{
    if (speedIn > 0)
    {
        _maxSpd = speedIn;
        statusChange("Max speed change MaxSpeed = " + String(_maxSpd));
    }
}
void TaMcP::setStopMoveMode()
{
    _stopMoveMode = !_stopMoveMode;
}
void TaMcP::stopMove()
{

    #ifdef TaMcP_debug
        Serial.println(timePrint() + " stopMove");
    #endif
    digitalWrite(_brakeLeft, true);
    digitalWrite(_brakeRight, true);
    delay(100);

}

int TaMcP::getSpd()
{
    return _spd;
}

int TaMcP::getMaxSpd()
{
    return _maxSpd;
}

bool TaMcP::getStopMoveMode()
{
    return _stopMoveMode;
}

bool TaMcP::getReverseMode()
{
    return _reverseMove;
}

void TaMcP::moveBegin()
{
    digitalWrite(_brakeLeft, false);
    digitalWrite(_brakeRight, false);

}

void TaMcP::moveUp()
{
    if (_stopMoveMode)
    {
        stopMove();
    }

    #ifdef TaMcP_debug
        Serial.println(timePrint() + " move up");
    #endif

    statusChange("move up");
    moveBegin();
    if (_reverseMove)
    {
        digitalWrite(_dirRight, false);
        digitalWrite(_dirLeft, false);
    }
    else
    {
        digitalWrite(_dirRight, true);
        digitalWrite(_dirLeft, true);
    }
}

void TaMcP::moveBack()
{

    if (_stopMoveMode)
    {
        stopMove();
    }
    #ifdef TaMcP_debug
        Serial.println(timePrint() + " move back");
    #endif

    statusChange("move down");
    moveBegin();
    if (_reverseMove)
    {
        digitalWrite(_dirRight, true);
        digitalWrite(_dirLeft, true);
    }
    else {
        digitalWrite(_dirRight, false);
        digitalWrite(_dirLeft, false);
    }
}

void TaMcP::right()
{

    if (_stopMoveMode)
    {
        stopMove();
    }
    #ifdef TaMcP_debug
        Serial.println(timePrint() + " move right");
    #endif

    statusChange("right");
    moveBegin();
    digitalWrite(_dirRight, false);
    digitalWrite(_dirLeft, true);
}

void TaMcP::left()
{

    if (_stopMoveMode)
    {
        stopMove();
    }
    #ifdef TaMcP_debug
        Serial.println(timePrint() + " move left");
    #endif

    statusChange("left");
    moveBegin();
    digitalWrite(_dirRight, true);
    digitalWrite(_dirLeft, false);
}


void TaMcP::moveUpRight()
{

    if (_stopMoveMode)
    {
        stopMove();
    }
    #ifdef TaMcP_debug
        Serial.println(timePrint() + " move up and right");
    #endif

    statusChange("move up and right");
    moveBegin();

    if (_reverseMove)
    {
        digitalWrite(_brakeLeft, true);
        digitalWrite(_dirRight, false);
    }
    else
    {
        digitalWrite(_brakeRight, true);
        digitalWrite(_dirLeft, true);
    }
}

void TaMcP::moveUpLeft()
{

    if (_stopMoveMode)
    {
        stopMove();
    }
    #ifdef TaMcP_debug
        Serial.println(timePrint() + " move up and left");
    #endif

    statusChange("move up and left");
    moveBegin();
    if (_reverseMove)
    {
        digitalWrite(_brakeRight, true);
        digitalWrite(_dirLeft, false);
    }
    else
    {
        digitalWrite(_brakeLeft, true);
        digitalWrite(_dirRight, true);
    }
}

void TaMcP::moveBackRight()
{

    if (_stopMoveMode)
    {
        stopMove();
    }
    #ifdef TaMcP_debug
        Serial.println(timePrint() + " move back and right");
    #endif

    statusChange("move down and right");
    moveBegin();
    if (_reverseMove)
    {
        digitalWrite(_brakeLeft, true);
        digitalWrite(_dirRight, true);
    }
    else
    {
        digitalWrite(_brakeRight, true);
        digitalWrite(_dirLeft, false);
    }
}

void TaMcP::moveBackLeft()
{

    if (_stopMoveMode)
    {
        stopMove();
    }
    #ifdef TaMcP_debug
        Serial.println(timePrint() + " move back and left");
    #endif

    statusChange("move down and left");
    moveBegin();
    if (_reverseMove)
    {
        digitalWrite(_brakeRight, true);
        digitalWrite(_dirLeft, true);
    }
    else
    {
        digitalWrite(_brakeLeft, true);
        digitalWrite(_dirRight, false);
    }
}

void TaMcP::moveSlow()
{

    if (_spd < _maxSpd * 0.1)
    {
        _spd = 0;
    }
    else
    {
        _spd = _spd - _maxSpd * 0.1;
    }

    #ifdef TaMcP_debug
        Serial.print(timePrint() + " move speed slow ");
        Serial.println(_spd);
    #endif

    statusChange("speed slow  speed = " + String(_spd * 100.0 / _maxSpd) + "%");
    analogWrite(_speedRight, _spd);
    analogWrite(_speedLeft, _spd);

}

void TaMcP::moveFast()
{


    if (_spd > _maxSpd * 0.9)
    {
        _spd = _maxSpd;
    }
    else
    {
        _spd = _spd + _maxSpd * 0.1;
    }
    #ifdef TaMcP_debug
        Serial.print(timePrint() + " move speed fast ");
        Serial.println(_spd);
    #endif

    statusChange("speed fast speed = " + String(_spd * 100.0 / _maxSpd) + "%");
    analogWrite(_speedRight, _spd);
    analogWrite(_speedLeft, _spd);

}

String TaMcP::getStatus()
{
    String s = "";

    for (int i = 0; i < _numLog; i++)
    {
        s += _statusStr[i];
        s += "<br>";
    }

    return s;
}
