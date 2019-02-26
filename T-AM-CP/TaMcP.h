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

    int _brakeLeft;
    int _dirLeft;
    int _brakeRight;
    int _dirRight;
    int _speedRight;
    int _speedLeft;
    bool _reverseMove;
    bool _stopMoveMode;
    int _maxSpd;
    void moveBegin();

public:

    TaMcP(int pins[6]);

    void statusChange(String newStatus);
    String getStatus();

    void setReverse(bool statusMode);
    bool getReverseMode();

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
    int getSpd();
    int getMaxSpd();
    void setMaxSpd(int speedIn);

    void setStopMoveMode();
    bool getStopMoveMode();

    





};

#endif
