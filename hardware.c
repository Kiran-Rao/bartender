#ifndef VALVE_C
#define VALVE_C

#include "helper.c"

const int _motorSpeed = 50;

//#define TEST_HARDWARE // For testing purposes

// Closes valve by running motor at _motorSpeed.
// When the motors stops moving, the motor
// retreats and restarts moving.
// This is due to physical constraints of the materials
void closeValve (short m, short attempts)
{
	nMotorEncoder[m] = 0;
	for (short i = 0; i < attempts; i++)
	{
		// Sets motor forward
		motor[m] = _motorSpeed;

		bool breakVal = true;
		short last = nMotorEncoder[m];

		while(breakVal)
		{
			wait1Msec(300);

			if (nMotorEncoder[m] == last)
				breakVal = false;
			last = nMotorEncoder[m];
		}

		motor[m] = -_motorSpeed;

		wait1Msec(200);
	}

	motor[m] = _motorSpeed;

	wait1Msec(400);

	motor[m] = 0;
}

// Opens valve by running motor at _motorSpeed.
// When the motors stops moving, the motor
// retreats and restarts moving.
// This is due to physical constraints of the materials
void openValve (short m, short attempts)
{
	nMotorEncoder[m] = 0;
	for (short i = 0; i < attempts; i++)
	{
		bool breakVal = true;
		short last = nMotorEncoder[m];

		motor[m] = -_motorSpeed;

		// Continues until valve stops moving
		while(breakVal)
		{
			wait1Msec(300);

			if (nMotorEncoder[m] == last)
				breakVal = false;
			last = nMotorEncoder[m];
		}

		// Backup 200 ms
		motor[m] = _motorSpeed;
		wait1Msec(200);
	}
	// Keep going for 400ms before stopping
	motor[m] = -_motorSpeed;
	wait1Msec(400);
	motor[m] = 0;
}

// Task opens the mountain dew
// Number of attempts calibrated for the specific valve
task openDew()
{
	openValve(motorA, 7);
}

// Task opens the orange soda
// Number of attempts calibrated for the specific valve
task openOrange()
{
	openValve(motorB, 6);
}

// Task opens the coke
// This valve operates differently from the rest.
// In a proudction model, all valves will be similar to the coke valve.
// However, this was not possible in the prototype.

// The open coke task keeps the motor running until the speed of
// the motor reaches 5degrees per 100ms, or 1 degree per second.
task openCoke()
{
	motor[motorC] = -30;

	int last = nMotorEncoder[motorC];

	do{
		last = nMotorEncoder[motorC];
		wait1Msec(100);
	}while (!tol(nMotorEncoder[motorC], last, 5));

	motor[motorC] = 0;
}

// Task closes the mountain dew
// Number of attempts calibrated for the specific valve
task closeDew()
{
	closeValve(motorA, 9);
}

// Task closes the orange soda
// Number of attempts calibrated for the specific valve
task closeOrange()
{
	closeValve(motorB, 7);
}

// Task closes the coke
// This valve operates differently from the rest.
// In a proudction model, all valves will be similar to the coke valve.
// However, this was not possible in the prototype.

// The close coke task keeps the motor running until the speed of
// the motor reaches 10degrees per 100ms, or 1 degree per second.
task closeCoke()
{
	motor[motorC] = 30;

	int last = nMotorEncoder[motorC];

	do {

		last = nMotorEncoder[motorC];
		wait1Msec(100);
	}while(!tol(nMotorEncoder[motorC], last, 10));

	motor[motorC] = 0;
}


// A hardware test to test and calibrate the hardware.
// Used to make sure everything operates correctly
// Proper coding practices may not have been used, as it is not production code
#ifdef TEST_HARDWARE
task main()
{
	SensorType[S1] = sensorSONAR;

	openValve(motorA, 6);
	openValve(motorB, 6);

	/*
	nMotorEncoder[motorC] = 0;
	motor[motorC] = -_motorSpeed;
	bool breakVal = true;
	int last = nMotorEncoder[motorC];
	while(breakVal)
	{
		wait1Msec(200);

		if (nMotorEncoder[motorC] == last)
			breakVal = false;

		last = nMotorEncoder[motorC];
	}
	motor[motorC] = _motorSpeed;
	wait1Msec(200);
	motor[motorC] = -_motorSpeed;
	wait1Msec(400);
	motor[motorC] = 0;
	*/

	while(nNxtButtonPressed == -1);

	closeValve(motorA, 6);
	closeValve(motorB, 6);
	/*
	nMotorEncoder[motorC] = 0;
	motor[motorC] = _motorSpeed;
	breakVal = true;
	last = nMotorEncoder[motorC];
	while(breakVal)
	{
		wait1Msec(200);

		if (nMotorEncoder[motorC] == last)
			breakVal = false;

		last = nMotorEncoder[motorC];
	}
	motor[motorC] = -_motorSpeed;
	wait1Msec(200);
	motor[motorC] = _motorSpeed;
	wait1Msec(400);
	motor[motorC] = 0;
	*/
}
#endif


#endif
