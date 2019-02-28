#ifndef controlTaMcP_H
#define controlTaMcP_H

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "defines.h"
#include "TaMcP.h"

String timePrint();

class controlTaMcP
{
	private:
	
		const int _numLog = 10;
		const int _spdMotor = 1023; 
        String _statusStr[10];

        int _spdRight;
        int _spdLeft;

        bool _reverseMove;
        bool _stopMoveMode;
        int _maxSpd;
        TaMcP tank;
        void moveT();

  	public:

  		controlTaMcP(int pins[6]);

  		void statusChange(String newStatus);
        String getStatus();

        void setReverse(bool statusMode);
        bool getReverseMode();

        int getSpd();
        int getMaxSpd();
        void setMaxSpd(int speedIn);

        void setStopMoveMode();
        bool getStopMoveMode();

        void moveSlow();
        void moveFast();



};
