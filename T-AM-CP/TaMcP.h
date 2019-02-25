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

    const int _numLog = 10;
    String _statusStr[10];

    int _spd;

    int _backLeft;
    int _upLeft;
    int _backRight;
    int _upRight;
    int _speedRight;
    int _speedLeft;
    bool _reverseMove;
    bool _stopMoveMode;
    int _maxSpd;

public:

    TaMcP(int pins[6]);

    void statusChange(String newStatus);
    String getStatus();

    void setReverse(bool statusMode);

    void moveUp();
    void moveBack();
    void right();
    void left();
    void moveUpRight();
    void moveUpLeft();
    void moveBackRight();
    void moveBackLeft();
    void stopMove();

    void moveSlow();
    void moveFast();
    void setSpd(int speedIn);

    void setStopMoveMode();





};

#endif
