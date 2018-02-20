/*
\	FadeLED.h - Library for fading LED on and off without using delay() function to prevent halting of main program
/	Created by Kasey Eljoundi, July 22, 2015
\
*/

#include "Arduino.h"
#include "ControlLED.h"

ControlLED::ControlLED(uint8_t pin)
{
	pinMode(pin, OUTPUT);
	this->_pin = pin;
	this->state = LOW;
	this->_brightness = 0;
}


/*
\	Name: void on()
/
\	Description: turn on LED
*/
void ControlLED::on(void)
{
	digitalWrite(_pin, HIGH);
	this->_brightness = 255;
	this->state = HIGH;
}


/*
\	Name: void on(int brightnessPercent)
/
\	Description: turn on LED to specified brightness
/
\	Params:
/		brightnessPercent - int - brightness of LED (% of full brightness), constricted to 0-100 
*/
void ControlLED::on(int brightnessPercent)
{
	if( brightnessPercent == 0 ){
		off();
	}else{
		int pwm = constrain(brightnessPercent, 1, 100); /* constrain input between 1-100 */
		_brightness = map(pwm, 1, 100, 1, 255); 		/* map input between 1-255 */
		analogWrite(_pin, _brightness);
		this->state = HIGH;
	}
}


/*
\	Name: void off()
/
\	Description: turn off LED
*/
void ControlLED::off(void)
{
	digitalWrite(_pin, LOW);
	this->_brightness = 0;
	this->state = LOW;
}


/*
\	Name: void off()
/
\	Description: toggle LED state; if LED is on, turn off. if LED is off, turn on
*/
void ControlLED::toggle(void){
	state ? off() : on();
}


/*
\	Name: bool getState()
/
\	Description: return state of LED (on or off)
/
\	Return:
/		state - bool - on/off (1 or 0 respectively) state of LED	
\					***ONLY returns "1" when _brightness == 255 (max, HIGH)***
*/
bool ControlLED::getState(void)
{
	return state; 
}


/*
\	Name: int getBrightness()
/
\	Description: return current brightness of LED (on or off)
/
\	Return:
/		brightness - int - brightness (0-255 PWM value) of LED
\		
*/
int ControlLED::getBrightness()
{
	return this->_brightness;
}


/*
\	Name: void linearFadeIn(int duration)
/
\	Description: fade LED to full brightness over specified time. linearFadeIn() acts as a constructor for the fade process, pLinearFadeIn performs stepped analog writing
\
/	Params:
\		duration - int - length of time for fade to take
*/
void ControlLED::linearFadeIn(int duration)
{
	if( _fading ){
		pLinearFadeIn();
	}else if( this->state != HIGH ){
		this-> _duration = duration;
		this->_fadeStart = millis();
		this-> _brightness = 0;					//start fade at min
		_fading = true;							//flag set to currently fading LED	
	}
}
void ControlLED::pLinearFadeIn()
{
	if( _brightness < 255 ){
		_timeNow = millis();
		_brightness = (int)( ( (float) 255/(float) _duration)*(float)( _timeNow - _fadeStart ) );		/*  b = (255/duration)t + 255  */
		analogWrite(_pin, _brightness);	//write to pin			
	}else if( _brightness >= 255 ){		//fade is complete
		_fading = false;
		this->on();
	}
}
/*
\	END fadeIn(), pFadeIn()
*/


/*
\	Name: void linearFadeOut(int duration)
/
\	Description: fade LED to full brightness over specified time. linearFadeOut() acts as a constructor for the fade process, pLinearFadeOut performs stepped analog writing
\
/	Params:
\		duration - int - length of time for fade to take
*/
void ControlLED::linearFadeOut(int duration)
{
	if( _fading ){	
		pLinearFadeOut();
	}else if( this->state != LOW ){
		this-> _duration = duration;
		this->_fadeStart = millis();
		this-> _brightness = 255;				//start fade at max
		_fading = true;							//flag set to currently fading LED
	}
}
void ControlLED::pLinearFadeOut()
{			
	if( _brightness > 0 ){
		_timeNow = millis();
		_brightness = 255 - (int) ( ((float) 255/(float) _duration)*(float)( _timeNow - _fadeStart ) );		/*  b = (-255/duration)t + 255  */
		analogWrite(_pin, _brightness);		
	}else if( _brightness <= 0 ){		//fade is complete
		_fading = false;
		this->off();
	}
}
/*
\	END fadeOut(), pFadeOut()
*/


void ControlLED::sinFadeIn(int duration)
{
	if( _fading ){	
		pSinFadeIn();
	}else if( this->state != HIGH ){
		this-> _duration = duration;
		this->_fadeStart = millis();
		this-> _brightness = 0;					//start fade at min
		_fading = true;							//flag set to currently fading LED
	}
}
void ControlLED::pSinFadeIn()
{				
	if( _brightness < 255 ){
		_timeNow = millis();
		
		int t = constrain(_timeNow - _fadeStart, 0, _duration); 
		//float time = mapFloat( (float)t, (float)0, (float)_duration, (float)0, (float) 3.14159 );
		
		//_brightness = sin(time/2)*255;
		
		float time = mapFloat( (float)t, 0, _duration, 0, 1 );
		
		_brightness = sin(   (float)3.14159 / (float)2 * time  )* (float)255;
		
		Serial.println(_brightness);
			
		analogWrite(_pin, _brightness);	//write to pin			
	}else if( _brightness >= 255 ){		//fade is complete
		_fading = false;
		this->on();
	}
}

float ControlLED::mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}




