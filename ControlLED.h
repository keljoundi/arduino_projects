/*
\	FadeLED.h - Library for fading LED on and off without using delay() function to prevent halting of main program
/	Created by Kasey Eljoundi, July 22, 2015
\
*/

#ifndef ControlLED_h
#define ControlLED_h

#include "Arduino.h"

class ControlLED
{

	public:
		ControlLED(uint8_t pin);
		
		bool state; 
		
		void on();
		void on(int brightnessPercent);
		void off();
		void toggle();
		bool getState();
		void linearFadeIn(int duration);
		void linearFadeOut(int duration);
		int getBrightness();
		
		void sinFadeIn(int duration);
		
	private:
		uint8_t _pin;
		
		int _brightness;	
		int _duration;
		
		bool _fading;
		
		unsigned long _lastStep;
		unsigned long _timeNow;
		unsigned long _fadeStart;
		
		void pLinearFadeIn();
		void pLinearFadeOut();
		
		void pSinFadeIn();
		
		float mapFloat(float x, float in_min, float in_max, float out_min, float out_max);

};

#endif