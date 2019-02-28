#include "TaMcP.h"



TaMcP::TaMcP(int pins[6])
:
    _brakeLeft(pins[0]),
    _dirLeft(pins[1]),
    _brakeRight(pins[2]),
    _dirRight(pins[3]),
    _speedRight(pins[4]),
    _speedLeft(pins[5])

{
    

    #ifdef TaMcP_debug
        Serial.println(timePrint()+" TaMcP init");
        Serial.print("Pins: ");
        for (int i = 0; i < 6; i++)
        {
            Serial.print(pins[i]);
            Serial.print(" ");
        }
        
    #endif
    pinMode(_dirRight, OUTPUT);
    pinMode(_dirLeft, OUTPUT);
    pinMode(_brakeLeft, OUTPUT);
    pinMode(_brakeRight, OUTPUT);
    pinMode(_speedRight, OUTPUT);
    pinMode(_speedLeft, OUTPUT);
}



void TaMcP::setSpd(int speedInRight, int speedInLeft, int maxSpd)
{
    if ((speedInRight > 0) && (speedInRight <= maxSpd)) 
    {
        analogWrite(_speedRight, speedInRight);
    
    }
    if ((speedInRight > 0) && (speedInRight <= maxSpd)) 
    {
        analogWrite(_speedLeft, speedInLeft);
    
    }
    
}


void TaMcP::stopMove(int sec)
{

    #ifdef TaMcP_debug
        Serial.println(timePrint() + " stopMove");
    #endif

    digitalWrite(_brakeLeft, true);
    digitalWrite(_brakeRight, true);

    delay(sec);

}


void TaMcP::moveBegin()
{

    digitalWrite(_brakeLeft, false);
    digitalWrite(_brakeRight, false);

}

void TaMcP::moveUp()
{

    #ifdef TaMcP_debug
        Serial.println(timePrint() + " move up");
    #endif

    statusChange("move up");
    moveBegin();

    digitalWrite(_dirRight, true);
    digitalWrite(_dirLeft, true);

}

void TaMcP::moveBack()
{

    #ifdef TaMcP_debug
        Serial.println(timePrint() + " move back");
    #endif

    statusChange("move down");
    moveBegin();

    digitalWrite(_dirRight, false);
    digitalWrite(_dirLeft, false);
}

void TaMcP::right()
{

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

    #ifdef TaMcP_debug
        Serial.println(timePrint() + " move up and right");
    #endif

    statusChange("move up and right");
    moveBegin();

    digitalWrite(_brakeRight, true);
    digitalWrite(_dirLeft, true);
}

void TaMcP::moveUpLeft()
{

    #ifdef TaMcP_debug
        Serial.println(timePrint() + " move up and left");
    #endif

    statusChange("move up and left");
    moveBegin();
    digitalWrite(_brakeLeft, true);
    digitalWrite(_dirRight, true);
}

void TaMcP::moveBackRight()
{

    #ifdef TaMcP_debug
        Serial.println(timePrint() + " move back and right");
    #endif

    statusChange("move down and right");
    moveBegin();

    digitalWrite(_brakeRight, true);
    digitalWrite(_dirLeft, false);
}

void TaMcP::moveBackLeft()
{

    #ifdef TaMcP_debug
        Serial.println(timePrint() + " move back and left");
    #endif

    statusChange("move down and left");
    moveBegin();
    digitalWrite(_brakeLeft, true);
    digitalWrite(_dirRight, false);
}

