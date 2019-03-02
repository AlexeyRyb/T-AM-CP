#include "controlTaMcP.h"


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

controlTaMcP::controlTaMcP(int pinIn[6])
:
    _tank(pinIn),
    _spdRight(_spdMotor),
    _spdLeft(_spdMotor),
    _reverseMove(false),
    _maxSpd(_spdMotor),
    _stopMoveMode(true),
    _msecStop(100)
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

    #ifdef controlTaMcP_debug
        
        Serial.print("Speed: ");
        Serial.println(_spd);

    #endif

    _tank.setSpd(_spdLeft, _spdRight, _maxSpd);
    

}


void controlTaMcP::statusChange(String newStatus)
{

    for(int i = _numLog - 1; i > 0; i--)
    {
        _statusStr[i] = _statusStr[i-1];
    }

    _statusStr[0] = timePrint() + "  " + newStatus;

}



String  controlTaMcP::getStatus()
{
    String s = "";

    for (int i = 0; i < _numLog; i++)
    {
        s += _statusStr[i];
        s += "<br>";
    }

    return s;
}


void controlTaMcP::setReverse(bool statusReverse)
{
        _reverseMove = statusReverse;
}

void controlTaMcP::setMaxSpd(int speedIn)
{
    if (speedIn > 0)
    {
        _maxSpd = speedIn;
        statusChange("Max speed change MaxSpeed = " + String(_maxSpd));
    }
}

void controlTaMcP::setStopMoveMode(bool statusStopMoveMode)
{
    _stopMoveMode = statusStopMoveMode;
}

void controlTaMcP::setMsecStop(int msecIn)
{
    _msecStop = msecIn;
}

int controlTaMcP::getSpdRight()
{
    return _spdRight;
}

int controlTaMcP::getSpdLeft()
{
    return _spdLeft;
}

int controlTaMcP::getMaxSpd()
{
    return _maxSpd;
}

bool controlTaMcP::getStopMoveMode()
{
    return _stopMoveMode;
}

bool controlTaMcP::getReverseMode()
{
    return _reverseMove;
}

int controlTaMcP::getMsecStop()
{
    return _msecStop;
}

void controlTaMcP::setSpdT(int speedInLeft, int speedInRight)
{

    if ((speedInRight > 0) && (speedInRight <= _maxSpd) && (speedInLeft > 0) && (speedInLeft <= _maxSpd)) 
    {
        _spdRight = speedInRight;
        _spdLeft = speedInLeft;
        _tank.setSpd(speedInLeft, speedInRight, _maxSpd);

        statusChange("Speed change speed right = " + String(_spdRight * 100.0 / _maxSpd) + "% and speed left = " + String(_spdLeft * 100.0 / _maxSpd) + "%");
    }



}

void controlTaMcP::moveSlow(bool sideIn, int delta)
{

    if ( (delta < 100) && (delta > 0) )
    {

        double deltaPer = delta / 100.0;

        if (!sideIn)
        {

            if (_spdRight < _maxSpd * deltaPer)
            {
                _spdRight = 0;
            }
            else
            {
                _spdRight = _spdRight - _maxSpd * deltaPer;
            }

        }

        if (sideIn)
        {

            if (_spdLeft < _maxSpd * deltaPer)
            {
                _spdLeft = 0;
            }
            else
            {
                _spdLeft = _spdLeft - _maxSpd * deltaPer;
            }

        }

        #ifdef controlTaMcP_debug
            Serial.print(timePrint() + " move speed slow right = ");
            Serial.print(_spdRight);
            Serial.print(" and left = ");
            Serial.println(_spdLeft);
        #endif

        _tank.setSpd(_spdLeft, _spdRight, _maxSpd);

    }

}

void controlTaMcP::moveFast(bool sideIn, int delta)
{

    if ((delta < 100) && (delta > 0))
    {

        double deltaPer = delta / 100.0;

        if (!sideIn)
        {

            if (_spdRight > _maxSpd * (1 - deltaPer))
            {
                _spdRight = _maxSpd;
            }
            else
            {
                _spdRight = _spdRight + _maxSpd * deltaPer;
            }

        }

        if (sideIn)
        {

            if (_spdLeft > _maxSpd * deltaPer)
            {
                _spdLeft = _maxSpd;
            }
            else
            {
                _spdLeft = _spdLeft + _maxSpd * deltaPer;
            }

        }

        #ifdef controlTaMcP_debug
                    Serial.print(timePrint() + " move speed slow ");
                    Serial.println(_spd);
        #endif

        _tank.setSpd(_spdLeft, _spdRight, _maxSpd);

    }
}

void controlTaMcP::moveT(int speedLeft, int speedRight)
{
    if ( (speedRight <  _maxSpd + 1) && (speedRight > -_maxSpd - 1) && (speedLeft < _maxSpd + 1) && (speedLeft > -_maxSpd - 1))
    {

        if (_stopMoveMode)
        {
            _tank.stopMove(_msecStop);
        }

        if (_reverseMove)
        {

            setSpdT(abs(speedRight), abs(speedLeft));

            if ((speedRight < 0) && (speedLeft < 0))
            {
                statusChange("move up");
                _tank.moveUp();
            }

            if ((speedRight > 0) && (speedLeft < 0))
            {
                statusChange("left");
                _tank.left();
            }

            if ((speedRight > 0) && (speedLeft > 0))
            {
                statusChange("move down");
                _tank.moveDown();
            }

            if ((speedRight < 0) && (speedLeft > 0))
            {
                statusChange("right");
                _tank.right();
            }

            if ((speedRight == 0) && (speedLeft < 0))
            {
                statusChange("move up and left");
                _tank.moveUpLeft();
            }

            if ((speedRight < 0) && (speedLeft == 0))
            {
                statusChange("move up and right");
                _tank.moveUpRight();
            }

            if ((speedRight == 0) && (speedLeft > 0))
            {
                statusChange("move down and left");
                _tank.moveDownLeft();
            }

            if ((speedRight > 0) && (speedLeft == 0))
            {
                statusChange("move down and right");
                _tank.moveDownRight();
            }

            if ((speedRight == 0) && (speedLeft == 0))
            {
                statusChange("stop move");
                _tank.stopMove(_msecStop);
            }


        }
        else
        {
            setSpdT(abs(speedLeft), abs(speedRight));

            if ((speedRight > 0) && (speedLeft > 0))
            {
                statusChange("move up");
                _tank.moveUp();
            }

            if ((speedRight > 0) && (speedLeft < 0))
            {
                statusChange("left");
                _tank.left();
            }

            if ((speedRight < 0) && (speedLeft < 0))
            {
                statusChange("move down");
                _tank.moveDown();
            }

            if ((speedRight < 0) && (speedLeft > 0))
            {
                statusChange("right");
                _tank.right();
            }

            if ((speedRight > 0) && (speedLeft == 0))
            {
                statusChange("move up and left");
                _tank.moveUpLeft();
            }

            if ((speedRight == 0) && (speedLeft > 0))
            {
                statusChange("move up and right");
                _tank.moveUpRight();
            }

            if ((speedRight < 0) && (speedLeft == 0))
            {
                statusChange("move down and left");
                _tank.moveDownLeft();
            }

            if ((speedRight == 0) && (speedLeft < 0))
            {
                statusChange("move down and right");
                _tank.moveDownRight();
            }

            if ((speedRight == 0) && (speedLeft == 0))
            {
                statusChange("stop move");
                _tank.stopMove(100);
            }

        }


        



    }


}


