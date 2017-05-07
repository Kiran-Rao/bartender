//#define DEBUG

#include "helper.c"
#include "ui.c"
#include "hardware.c"
#include "animation.c"
#include "ultrasonic.c"

const int _nDrinks = 12;
const int _nDrinkSizes = 6;
const int _nLines = 7;
const string _drinkNames[] = {"Dirty Mutt",
															"Coke",
															"Orange",
															"Dew",
															"Coke O",
															"Coke D",
															"Dew O",
															"Dew C",
															"Orange Dew",
															"Cheapass",
															"Corange",
															"O Splash"};

const int _dewRatio[]    = {1,0,0,1,0,1,2,2,1,0,0,2};
const int _orangeRatio[] = {1,0,1,0,1,0,1,0,1,0,1,1};
const int _cokeRatio[]   = {1,1,0,0,2,2,0,1,0,0,1,0};

const string _drinkSizeNames[] = {"Custom",
															    "2oz        60mL",
															    "Half Cup  125mL",
															    "Cup       250mL",
															    "Can       355mL",
															    "Bottle    591mL"};

const int _drinkSizes[] = { 125, 60, 125, 250, 355, 591};

bool isExit = false; // Required for multitasking

// Calculates the amount of time each sensor needs to be open
void ratioCalc(int totalVolume, int ratio1, int ratio2, int ratio3, int & tim1, int & tim2, int & tim3)
{
  int totalRatio = ratio1 + ratio2 + ratio3;

  if (totalRatio != 0)
  {
    tim1 = (ratio1 * totalVolume / totalRatio - 8)/3.5; 
    tim2 = (ratio2 * totalVolume / totalRatio - 8)/4.2;
    tim3 = (ratio3 * totalVolume / totalRatio - 15)/5; 
  }
  else
  {
    tim1 = 0;
    tim2 = 0;
    tim3 = 0;
  }

  if (tim1 < 0)
  	tim1 = 0;
  if (tim2 < 0)
  	tim2 = 0;
  if (tim3 < 0)
  	tim3 = 0;
}

// While the button is held down,
// Coke will pour freely and the UI will be suspended
task buttonPressed()
{
	while(!isExit)
	{
		// Waits for the button to be pressed
		while(SensorValue[S2] == 0 && !isExit) wait1Msec(1); // Wait1Msec frees the CPU for other processes
		isSuspended = true;
		pourStaticAnim();
		StartTask(openCoke);
		EndTimeSlice();

		// Waits for the button to be unpressed
		while(SensorValue[S2] == 1 && !isExit)  wait1Msec(1);
		StopTask(openCoke); // Making sure there's not confliction
		pourCompAnim();
		StartTask(closeCoke);
		EndTimeSlice();
		isSuspended = false;
	}
}

// Checks the sonor sensor,
// Updates the botom part of the UI with current volume levels.
task updateVolumeStatus()
{
	while (!isExit)
	{
		// Gets the sonar sensor, computes size of bar
		short a = sensorValue[S1];
		if (a == 255) a = 5;
		else a = (5 - a/5);
		
		// Draws status bar on bottom line
		nxtDisplayString(7, "Orange Volume");
		nxtDrawLine(0, 7, 99, 7);
		for (short i = 1; i < 7; i++)
			nxtEraseLine(80, i, 82, i);
		for (short i = 1; i < a + 1; i++)
			nxtDrawLine(80, i, 82, i);
		wait10Msec(20);
	}
}

// Initilizes
void initilize()
{
	nNxtExitClicks = 5;
	SensorType[S1] = sensorSONAR;
	SensorType[S2] = sensorTouch;

	StartTask(buttonPressed);
	StartTask(updateVolumeStatus);
	//initilizeMotors();
}

task main()
{
	initilize();
	while (!isExit)
	{
		// Selects which Drink
		short drinkSel = UIListSelection(_drinkNames, _nDrinks, _nLines);

		// Ends program if back button selected
		if (drinkSel == -1) isExit = true;
		else
		{
			// Selects what drink size
			int drinkSize = UIListSelection(_drinkSizeNames, _nDrinkSizes, _nLines);
			if (drinkSize != -1)
			{
				// Calculates time and opens appropreate valves
				int dew, orange, coke, maxTime;
				ratioCalc(_drinkSizes[drinkSize], _dewRatio[drinkSel], _orangeRatio[drinkSel], _cokeRatio[drinkSel], dew, orange, coke);
				maxTime = max(dew, orange, coke);
				if (dew)    StartTask (openDew);
				if (orange) StartTask (openOrange);
				if (coke)   StartTask (openCoke);

				animationTimeRequired = maxTime * 100 / 20;
				StartTask(pourAnim); // Starts pouring animation

				// Calculates when to close valves
				for (int i = 0; i < maxTime + 1; i++)
				{
					if (i == dew && i != 0)
					{
						StartTask (closeDew);
						dew = maxTime + 2;
					}
					if (i == orange && i != 0)
					{
						StartTask (closeOrange);
						orange = maxTime + 2;
					}
					if (i == coke && i != 0)
					{
						StartTask (closeCoke);
						coke = maxTime + 2;
					}
					wait1Sec(1);
				}

				// Pauses to ensure valves closed.
				wait1Sec(5);
				eraseDisplay();
			}
		}
	}

	// Ends program gracefully
	StartTask (closeDew);
	StartTask (closeOrange);
	StartTask (closeCoke);
	wait1Sec(10);
}
