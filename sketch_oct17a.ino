
#include "Truck.h"

Wheel leftWheel(8, 7);
Wheel rightWheel(10, 11);
Truck truck(&leftWheel, &rightWheel, 3, 2, 5);

void setup() {
  // put your setup code here, to run once:
	Serial.begin(9600);

	truck.init();
}

void loop() {
  // put your main code here, to run repeatedly:
	if (truck.is_autopilot())
	{ 
		truck.workingAutoPilot(); 
	}
	else 
	{
		truck.handOn(); 
	}
}