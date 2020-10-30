#include "Truck.h"

void Wheel::init() {
	pinMode(forward_pin, OUTPUT);
	pinMode(backward_pin, OUTPUT);

	stop(); // set LOW to pins because lags
};

void Wheel::moveForward() {
	digitalWrite(forward_pin, HIGH);
};

void Wheel::moveBackward() {
	digitalWrite(backward_pin, HIGH);
};

void Wheel::stop() {
	digitalWrite(forward_pin, LOW);
	digitalWrite(backward_pin, LOW);
};

Truck::Truck(Wheel* LWheel_, Wheel* RWheel_, int pinS, int pinC, int pinO) : SOLENOID(pinS), PIN_COLOR(pinC), PIN_OBSTACLE(pinO)
{
	LWheel = LWheel_;
	RWheel = RWheel_;
};

void Truck::init() {

	LWheel->init();
	RWheel->init();
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(SOLENOID, OUTPUT);
	pinMode(PIN_COLOR, INPUT);
	pinMode(PIN_OBSTACLE, INPUT);
	digitalWrite(SOLENOID, LOW);
};

void Truck::changeMode() {
	if (autoPilot)
	{
		autoPilot = !autoPilot;
		cargoDischarge = !cargoDischarge;
		digitalWrite(SOLENOID, LOW);
	}
	else
	{
		autoPilot = !autoPilot;
		prepareAutopilot();
	}
};

void Truck::prepareAutopilot() {

	if (digitalRead(PIN_COLOR)) // truck outside the black zone
	{
		changeMode();
	}
	else // truck inside the black zone
	{
		return 0;
	}
};

void Truck::workingAutoPilot() {

	if (cargoDischarge)
	{
		if (digitalRead(PIN_COLOR)) // truck outside the black zone
		{
			rideBackward();
		}
		else // truck inside the black zone
		{
			changeMode();
		}
	}
	else
	{
		if (digitalRead(PIN_OBSTACLE)) // no barrier
		{
			rideForward();
		}
		else // barrier
		{
			if (digitalRead(PIN_COLOR)) // truck outside the black field
			{
				beep(TIMEBEEPDETOUR);
				detour(TIMEFORWARDDETOUR1, TIMETURNDDETOUR, TIMEFORWARDDETOUR2);
			}
			else // truck inside the black field
			{
				discharge(TIMEDISCHANGE);
				beep(TIMEBEEPDISCHANGE);

				rideBackward(TIMEBACKWARD);
			}
		}
	}
};

void Truck::rideForward(int TimeF) {

	LWheel->moveForward();
	RWheel->moveForward();

	delay(++TimeF);

	LWheel->stop();
	RWheel->stop();
};

void Truck::rideBackward(int TimeB) {

	LWheel->moveBackward();
	RWheel->moveBackward();

    //TimeB+=10;
	delay(TimeB);

	LWheel->stop();
	RWheel->stop();
};

void Truck::turnRight(int TimeTurn) {

	LWheel->moveForward();
	RWheel->moveBackward();

	delay(++TimeTurn);

	LWheel->stop();
	RWheel->stop();
};

void Truck::turnLeft(int TimeTurn) {

	LWheel->moveBackward();
	RWheel->moveForward();

	delay(++TimeTurn);

	LWheel->stop();
	RWheel->stop();
};

void Truck::detour(int TimeForwardDetour1, int TimeTurnDetour , int TimeForwardDetour2) {

	LWheel->stop();
	RWheel->stop();
	delay(400);
	turnLeft(TimeTurnDetour);
    LWheel->stop();
    RWheel->stop();
    delay(400);
	rideForward(TimeForwardDetour1);
    LWheel->stop();
    RWheel->stop();
    delay(400);
	turnRight(TimeTurnDetour);
    LWheel->stop();
    RWheel->stop();
    delay(400);
	rideForward(TimeForwardDetour2);
    LWheel->stop();
    RWheel->stop();
    delay(400);
	turnRight(TimeTurnDetour);
    LWheel->stop();
    RWheel->stop();
    delay(400);
	rideForward(TimeForwardDetour1);
    LWheel->stop();
    RWheel->stop();
    delay(400);
	turnLeft(TimeTurnDetour);
    LWheel->stop();
    RWheel->stop();
    delay(400);
};

void Truck::beep(int TimeBeep) {

	analogWrite(LED_BUILTIN, 100);
	delay(TimeBeep);
	analogWrite(LED_BUILTIN, 0);
};

void Truck::discharge(int TDischarge) {
	digitalWrite(SOLENOID, HIGH);
	delay(++TDischarge);

	cargoDischarge = true;

    digitalWrite(SOLENOID, LOW);
};

void Truck::handOn() {
	if (Serial.available())
	{
		//Serial.read()  // read first byte because unicode
		switch (Serial.read())
		{
		case UP: {
			rideForward(TIMEF);
			break;
		}
		case DOWN: {
			rideBackward(TIMEB);
			break;
		}
		case LEFT: {
			turnLeft(TIMETURN);
			break;
		}
		case RIGHT: {
			turnRight(TIMETURN);
			break;
		}
		case START: {
			changeMode();
			break;
		}
		case DISCHARGE:{
			discharge(TIMEDISCHANGE);
			break;
		} // TODO: delete for release
		default:
			break;
		}
	}
};