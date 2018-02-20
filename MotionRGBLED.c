/*
\	Name:	MotionRGBLED.c 
/	Author:	Kasey Eljoundi
\
/	Description: Activate LED lights using a motion sensor.
\
/	Requirements:
\		Arduino board, PIR sensor, LEDs
/		Minimum 9v power supply recommended
\		ControlLED library by Kasey Eljoundi
*/

#include <ControlLED.h>


int inputPin = 13;               // input pin (for PIR sensor)
int inputVal = 0;               // variable for reading the pin status
int pirState = LOW;             
int calibrationTime = 20000;  //20 sec calibration delay

bool motionState = false;

ControlLED lightsR = ControlLED(3);	/* LED lights to activate */
ControlLED lightsG = ControlLED(5);	/* LED lights to activate */
ControlLED lightsB = ControlLED(6);	/* LED lights to activate */
ControlLED statusLED = ControlLED(17);	/* status LED - pro mini RX/TX LED */

unsigned long currentMillis = 0; //variable to hold current machine uptime (milliseconds)
unsigned long lightsTurnOnTime = 0;
unsigned int lightsInterval = 30000; //30 seconds

unsigned long statusLedOn = 0;
unsigned int statusLedInterval = 2000; //2 seconds
 

void setup() {		

	pinMode(inputPin, INPUT);     // declare pir sensor as input
	//delay after start for sensor "calibration"
	delayForCalibration(); 
	
}


void loop(){
	
	inputVal = digitalRead(inputPin);  // read input value from motion sensor
	currentMillis = millis();  
  
	//blink status led
	blinkStatusLED();  
  
	if (inputVal == HIGH) {    	
  
		/* show motion detection by LED */
		if (pirState == LOW) {	
			motionState = true;
			statusLED.on();
			pirState = HIGH; // We only want to print on the output change, not state
		}
		
        //turn on lights
		lightsR.linearFadeIn(2000);  
		lightsG.linearFadeIn(2000);  
		lightsB.linearFadeIn(2000); 
		lightsTurnOnTime = currentMillis;  //restart led interval timer
  
	} else{    
		
		//turn LED OFF **if interval is up**
		if( currentMillis - lightsTurnOnTime > lightsInterval ){
			lightsR.linearFadeOut(2000);
			lightsG.linearFadeOut(2000);
			lightsB.linearFadeOut(2000);
		}
		
		/* show motion detection by LED */
		if (pirState == HIGH){
			motionState = false;
			statusLED.off();
			pirState = LOW; // We only want to print on the output change, not state
		}  
	}
}


/*
\	Name: void blinkStatusLED()
/
\	Description: blink statusLED on even interval using background time comparisons,
/					blinking pauses if sensor is currently detecting motion
\
*/
void blinkStatusLED(){
	if( !motionState ){
		if( currentMillis - statusLedOn > statusLedInterval ){
			statusLedOn = currentMillis;
			statusLED.toggle();
		}
	}
}


/*
\	Name: void delayForCalibration()
/
\	Description: pause execution of program for sensor calibration,
/					manufacturer recommends "rest" after powering on sensor (10-60s)
\
*/
void delayForCalibration(){
    
    statusLED.on();
	
	delay(calibrationTime); 
 
    statusLED.toggle();
	delay(200);
	statusLED.toggle();
	delay(200);
	statusLED.toggle(); 
	delay(200);
	statusLED.toggle();
	delay(200);
	statusLED.toggle();
	delay(200);
	statusLED.toggle();
	delay(200);
	statusLED.off();
    
}


