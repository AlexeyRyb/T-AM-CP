#ifndef TaMcP_H
#define TaMcP_H

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "defines.h"


class TaMcP
{
    private:

        int _brakeLeft;
        int _dirLeft;
        int _brakeRight;
        int _dirRight;
        int _speedRight;
        int _speedLeft;
         
        void moveBegin();

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
