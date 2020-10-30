#pragma once
#include "Arduino.h"
#include "settings.h"

//wheel class

class Wheel {
public:
	Wheel(int pinF, int pinB) :forward_pin(pinF), backward_pin(pinB) {};

	void init(); // sets the pin mode to perform the read
	void moveForward();
	void moveBackward();
	void stop();
private:
	const int forward_pin;
	const int backward_pin;
};

//class of operations performend by truck

class Truck {
public:
	Truck(Wheel* LWheel_, Wheel* RWheel_, int pinS, int pinC, int pinO);

	void changeMode(); // changes from manual control to automatic and vice versa
	void turnLeft(int timeTurn = 0);
	void turnRight(int TimeTurn = 0);
	void rideForward(int TimeF = 0);
	void rideBackward(int TimeB = 0);
	void detour(int TimeForwardDetour1, int TimeTurnDetour , int TimeForwardDetour2);
	void discharge(int TDischarge);
	void beep(int TimeBeep);
	void handOn();
	void prepareAutopilot();
	void init(); // sets the pin mode to perform the read or write operations
	void workingAutoPilot();
	bool is_autopilot() { return autoPilot;}; //check whether autopilot is enabled

private:

	Wheel* LWheel;
	Wheel* RWheel;
	bool autoPilot = false; 
	bool cargoDischarge = false; //check whether the cargo have been delevered

	const int PIN_COLOR;
	const int PIN_OBSTACLE;
	const int SOLENOID;
};
