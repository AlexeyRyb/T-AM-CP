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
    _stopMoveMode(true)
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
        
        Serial.print("Speed: ");
        Serial.println(_spd);

    #endif

    _tank.setSpd(_spdRight, _spdLeft, _maxSpd);
    

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

void controlTaMcP::setStopMoveMode()
{
    _stopMoveMode = !_stopMoveMode;
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

void controlTaMcP::setSpdT(int speedInRight, int speedInLeft)
{

    if ((speedInRight > 0) && (speedInRight <= _maxSpd) && (speedInLeft > 0) && (speedInLeft <= _maxSpd)) 
    {
        _spdRight = speedInRight;
        _spdLeft = speedInLeft;
        _tank.setSpd(speedInRight, speedInLeft, _maxSpd);
    }

    statusChange("Speed change speed right = " + String(_spdRight * 100.0 / _maxSpd) + "% and speed left = " + String(_spdLeft * 100.0 / _maxSpd) + "%");

}

void controlTaMcP::moveSlow(bool rightIn, bool leftIn, int del1, int del2)
{
    int delPer1 = 0;
    int delPer2 = 0;
    
    if (del1 < 100)
    {
        delPer1 = del1 / 100;
    }

    if (del2 < 100)
    {
        delPer2 = del2 / 100;
    }

    if (rightIn)
    {
        if (_spdRight < _maxSpd * del1)
        {
            _spdRight = 0;
        }
        else
        {
            _spdRight = _spdRight - _maxSpd * del1;

        }
        statusChange("speed slow right speed = " + String(_spdRight * 100.0 / _maxSpd) + "%");
    

    }
    
    if (leftIn)
    {
        if (_spdLeft < _maxSpd * del2)
        {
            _spdLeft = 0;
        }
        else
        {
            _spdLeft = _spdLeft - _maxSpd * del2;

        }
        statusChange("speed slow left speed = " + String(_spdLeft * 100.0 / _maxSpd) + "%");
    

    }
    #ifdef TaMcP_debug
        Serial.print(timePrint() + " move speed slow right = ");
        Serial.print(_spdRight);
        Serial.print(" and left = ");
        Serial.println(_spdLeft);
    #endif

    _tank.setSpd(_spdRight, _spdLeft, _maxSpd);
}

void controlTaMcP::moveFast(bool rightIn, bool leftIn, int del1, int del2)
{
    int delPer1 = 0;
    int delPer2 = 0;
    
    if ((del1 < 100) && (del1 > 0))
    {
        delPer1 = del1 / 100;
    }

    if ((del2 < 100) && (del2 > 0))
    {
        delPer2 = del2 / 100;
    }

    if (rightIn)
    {
        if (_spdRight > _maxSpd * (1 - del1) )
        {
            _spdRight = _maxSpd;
        }
        else
        {
            _spdRight = _spdRight + _maxSpd * del1;

        }
        statusChange("speed slow right speed = " + String(_spdRight * 100.0 / _maxSpd) + "%");
    

    }
    
    if (leftIn)
    {
        if (_spdLeft > _maxSpd * del2)
        {
            _spdLeft = _maxSpd;
        }
        else
        {
            _spdLeft = _spdLeft + _maxSpd * del2;

        }
        statusChange("speed slow left speed = " + String(_spdLeft * 100.0 / _maxSpd) + "%");
    

    }
    #ifdef TaMcP_debug
        Serial.print(timePrint() + " move speed slow ");
        Serial.println(_spd);
    #endif

    _tank.setSpd(_spdRight, _spdLeft, _maxSpd);
}

void controlTaMcP::moveT(int speedLeft, int speedRight)
{
    if ( (speedRight <  1024) && (speedRight > -1024) && (speedLeft < 1024) && (speedLeft > -1024))
    {
        setSpdT(abs(speedRight), abs(speedLeft));

        //CLUTCH
        if (_reverseMove)
        {
            speedRight = (-1) * speedRight;
            speedLeft = (-1) * speedLeft;
        }

        if (_stopMoveMode)
        {
            _tank.stopMove(100);
        }

        
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
        
        if ((speedRight > 0) && (speedLeft = 0))
        {
            statusChange("move up and left");
            _tank.moveUpLeft();
        }
        
        if ((speedRight = 0) && (speedLeft > 0))
        {
            statusChange("move up and right");
            _tank.moveUpRight();
        }
        
        if ((speedRight < 0) && (speedLeft = 0))
        {
            statusChange("move down and left");
            _tank.moveDownLeft();
        }
        
        if ((speedRight = 0) && (speedLeft < 0))
        {
            statusChange("move down and right");
            _tank.moveDownRight();
        }
        
        if ((speedRight = 0) && (speedLeft = 0))
        {
            statusChange("stop move");
            _tank.stopMove(100);
        }


    }


}


