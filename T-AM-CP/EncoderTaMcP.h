//
// Created by Алексей Рыбалкин on 07.03.2019.
//

#ifndef EncoderTaMcP_H
#define EncoderTaMcP_H

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "defines.h"
#include <Encoder.h>

class encoderTaMcP
{
    private:

        Encoder _encoder;
        double _dist;
        double _coefTomm;

        long _oldPosition;
        long _newPosition;

        long _prevTime;
        long _stepTime;
        double _spd;

    public:

        encoderTaMcP(int encoderIn1, int encoderIn2);
        void setCoef(double coefIn);

        double getDist();
        double getSpd();

        void checkDist(bool moving);
        void resetDist();


};



#endif //CONTROLT_AM_CP_ENCODERTAMCP_H
