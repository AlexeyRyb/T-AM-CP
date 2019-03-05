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

		int _msecStop;

        TaMcP _tank;

        void moveT();\

  	public:

  		controlTaMcP(int pinIn[6]);

  		void statusChange(String newStatus);
        String getStatus();

        void setReverse(bool statusMode);
        bool getReverseMode();

        int getSpdRight();
        int getSpdLeft();
        int getMaxSpd();
        void setMaxSpd(int speedIn);

        void setStopMoveMode(bool statusStopMoveMode);
        bool getStopMoveMode();

        void setSpdT(int spdInRight, int spdInLeft);
        void moveSlow(bool sideIn, int delta);
        void moveFast(bool sideIn, int delta);

        void moveT(int speedRight, int speedLeft);

		void setMsecStop(int msecIn);
		int getMsecStop();

		void restRun();

		void restStart();





};

#endif