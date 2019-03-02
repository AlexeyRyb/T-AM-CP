#include "TaMcP.h"

TaMcP::TaMcP(int pins[6])
:
    _downLeft(pins[0]),
    _upLeft(pins[1]),
    _downRight(pins[2]),
    _upRight(pins[3]),
    _speedLeft(pins[4]),
    _speedRight(pins[5])

{

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
    pinMode(_upRight, OUTPUT);
    pinMode(_upLeft, OUTPUT);
    pinMode(_downLeft, OUTPUT);
    pinMode(_downRight, OUTPUT);
    pinMode(_speedRight, OUTPUT);
    pinMode(_speedLeft, OUTPUT);

}

void TaMcP::setSpd(int speedInLeft, int speedInRight, int maxSpd)
{
    if ((speedInRight > 0) && (speedInRight <= maxSpd))
    {
        analogWrite(_speedRight, speedInRight);

    }

    if ((speedInLeft > 0) && (speedInLeft <= maxSpd))
    {
        analogWrite(_speedLeft, speedInLeft);

    }

}

void TaMcP::stopMove(int msec)
{

    #ifdef TaMcP_debug
        Serial.println(timePrint() + " stopMove");
    #endif

    digitalWrite(_upRight, false);
    digitalWrite(_upLeft, false);
    digitalWrite(_downLeft, false);
    digitalWrite(_downRight, false);

    delay(msec);

}

void TaMcP::moveUp()
{

    #ifdef TaMcP_debug
        Serial.println(timePrint() + " move up");
    #endif

     digitalWrite(_upLeft, true);
     digitalWrite(_downLeft, false);
     digitalWrite(_upRight, true);
     digitalWrite(_downRight, false);

}

void TaMcP::moveDown()
{

    #ifdef TaMcP_debug
        Serial.println(timePrint() + " move back");
    #endif

     digitalWrite(_upLeft, false);
     digitalWrite(_downRight, true);
     digitalWrite(_upRight, false);
     digitalWrite(_downLeft, true);

}

void TaMcP::right()
{

    #ifdef TaMcP_debug
        Serial.println(timePrint() + " move right");
    #endif

    digitalWrite(_upLeft, true);
    digitalWrite(_downLeft, false);
    digitalWrite(_upRight, false);
    digitalWrite(_downRight, true);

}

void TaMcP::left()
{

    #ifdef TaMcP_debug
        Serial.println(timePrint() + " move left");
    #endif

    digitalWrite(_upLeft, false);
    digitalWrite(_downLeft, true);
    digitalWrite(_upRight, true);
    digitalWrite(_downRight,false);

}


void TaMcP::moveUpRight()
{

    #ifdef TaMcP_debug
        Serial.println(timePrint() + " move up and right");
    #endif

    digitalWrite(_upLeft, true);
    digitalWrite(_downLeft, false);
    digitalWrite(_upRight, false);
    digitalWrite(_downRight, false);

}

void TaMcP::moveUpLeft()
{


    #ifdef TaMcP_debug
        Serial.println(timePrint() + " move up and left");
    #endif

    digitalWrite(_upLeft, false);
    digitalWrite(_downLeft, false);
    digitalWrite(_upRight, true);
    digitalWrite(_downRight, false);

}

void TaMcP::moveDownRight()
{

    #ifdef TaMcP_debug
        Serial.println(timePrint() + " move back and right");
    #endif

    digitalWrite(_upLeft, false);
    digitalWrite(_downLeft, true);
    digitalWrite(_upRight, false);
    digitalWrite(_downRight, false);

}

void TaMcP::moveDownLeft()
{

    #ifdef TaMcP_debug
        Serial.println(timePrint() + " move back and left");
    #endif

    digitalWrite(_upLeft, false);
    digitalWrite(_downLeft, false);
    digitalWrite(_upRight, false);
    digitalWrite(_downRight, true);

}
