//
// Created by Алексей Рыбалкин on 08.03.2019.
//

#ifndef caterpillarTaMcP_H
#define caterpillarTaMcP_H

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "defines.h"
#include "EncoderTaMcP.h"
#include <Array.h>

class caterpillarTaMcP
{

    private:

        double _distNow;
        double _distNeed;

        double _spdNow;
        double _spdNeed;

        double _maxSpd;
        double _maxSpdPin;

        encoderTaMcP _encoderTaMcP;

        int _pinDown;
        int _pinUp;
        int _pinSpd;

        bool _spdMode;

        double _epsDist;
        double _spdMin;
        double _epsSpd;
        double _sendSpd;
        double _numberOfStaps;

        bool _moving;

        double _coefDown;
        double _coefUp;

        void _moveUp();
        void _moveBack();
        void _stopMove();

        void _changeDistAndSpd();
        void _standSpd();

        bool _isDistReached();


    public:

        caterpillarTaMcP(int pins[5]);

        void setSpdAndDist(double spdIn, double distIn);
        void setSpd(double spdIn);

        void setCoef(double coefDownIn, double coefUpIn);

        void checkMove();

        Array <double, 2> currentPosition();

};


#endif //CONTROLT_AM_CP_CATERPILLAR_H
