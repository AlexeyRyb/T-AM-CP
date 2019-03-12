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

        const double _minSpd;
        const double _maxSpd;
        const double _maxValuePin;

        encoderTaMcP _encoderTaMcP;

        const int _pinDown;
        const int _pinUp;
        const int _pinSpd;

        bool _spdMode;

        const double _epsDist;
        const double _epsSpd;

        double _sendSpd;
        double _numberOfSteps;

        bool _moving;

        double _coefDown;
        double _coefUp;

        void _moveUp();
        void _moveDown();
        void _stopMove();

        void _updateDistAndSpdEncoder();
        void _standSpd();

        bool _isDistReached();


    public:

        caterpillarTaMcP(int pins[5]);

        void setSpdAndDist(double spdIn, double distIn);
        void setSpd(double spdIn);

        void setCoef(double coefDownIn, double coefUpIn);

        void updateInternalData();

        Array <double, 2> getCurrentPosition();

};


#endif //CONTROLT_AM_CP_CATERPILLAR_H
