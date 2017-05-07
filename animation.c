#ifndef ANIMATION_C
#define ANIMATION_C

#include "helper.c"

// A variable used to pass the amount of time required for the pouring animation 
short animationTimeRequired = 250; 

// Draws a cup on the screen
void drawCup()
{
	nxtDrawLine(40,15,60,15); //draws bottom line
	nxtDrawLine(40,14,60,14);
	nxtDrawLine(40,15,35,40); //left line
	nxtDrawLine(39,15,34,40);
	nxtDrawLine(60,15,65,40); //right line
	nxtDrawLine(61,15,66,40);
}

// Draws a smile within the filled cup
void drawSmile()
{
	nxtEraseLine(45,30,45,26); //draw left eye
	nxtEraseLine(46,30,46,26);
	nxtEraseLine(55,30,55,26); //draw right eye
	nxtEraseLine(54,30,54,26);

	nxtEraseLine(45,22,49,19); //left smile
	nxtEraseLine(46,22,50,19);
	nxtEraseLine(55,22,51,19); //right smile
	nxtEraseLine(54,22,50,19);
	nxtClearPixel(50,19); //middle smile
	nxtClearPixel(50,20);
}

// Draws a single line within the cup
void drawFillCup(short i)
{
	nxtDrawLine(40-i/4,i+15,60+i/4,i+15);
}

// Draws a line indicating liquid is flowing
void drawFlowLine()
{
	nxtDrawLine(50,99,50,15);
}

// Erases the flow line (when animation is complete)
void eraseFlowLine()
{
	nxtEraseLine(50,99,50,35);
}

// Draws a static animation for when the pour is complete
void pourCompAnim()
{
	eraseDisplay();
	drawCup();
	for (short i = 0; i < 20; i++)
		drawFillCup(i);
	drawSmile();
}

// Draws a static animation for when pouring
void pourStaticAnim()
{
	eraseDisplay();
	drawCup();
	for (short i = 0; i < 20; i++)
		drawFillCup(i);

}

// Animates the cup filling. Time is based on the variable animationTimeRequired
task pourAnim()
{
	short animTime = animationTimeRequired;
	eraseDisplay();
	drawCup();
	drawFlowLine();
	for (short i = 0; i < 20; i++)
	{
		drawFillCup(i);
		wait10Msec(animTime);
	}
	eraseFlowLine();
	drawSmile();
}



#endif
