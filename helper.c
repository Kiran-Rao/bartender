// Simple Helper Functions (sometimes trivial), to make coding easier

#ifndef HELPER_C
#define HELPER_C

// Creates a delay lasting n seconds
void wait1Sec(short n)
{
	for (; n > 0; n--)
		wait10Msec(100);
}

// Returns true if variables are within tolerance of each other
bool tol(short a, short b, short t)
{
	return (abs(a-b) <= t);
}

// Returns the maximum of three variables
int max (int a, int b, int c)
{
	a = (a > b)? a : b;
	return (a > c)? a : c;
}

// Draws a Right facing arrow in the text position given
void nxtDrawRArrow(short row, short column)
{
	row = 63 - 8 * row;
	column *= 6;
	for (short i = 0; i < 4; i++)
		nxtDrawLine(column + i, row - i, column + i, row - 6 + i);
}

// Draws a Left facing arrows in the text position given
void nxtDrawLArrow(short row, short column)
{
	row = 60 - 8 * row;
	column *= 6;
	for (short i = 0; i < 4; i++)
		nxtDrawLine(column + i, row + i, column + i, row - i);
}

#endif
