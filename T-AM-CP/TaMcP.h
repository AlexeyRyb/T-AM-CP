#ifndef TaMcP_H
#define TaMcP_H

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "defines.h"

String timePrint();

class TaMcP
{
private:

    int _downLeft;
    int _upLeft;
    int _downRight;
    int _upRight;
    int _speedRight;
    int _speedLeft;

public:

    TaMcP(int pins[6]);

    void moveUp();
    void moveDown();
    void right();
    void left();
    void moveUpRight();
    void moveUpLeft();
    void moveDownRight();
    void moveDownLeft();
    void stopMove(int msec);

    void setSpd(int speedInLeft, int speedInRight, int maxSpd);

};

#endif
