const short len = 5;

string str[] = {"aoeu","htns","another string","ebay", "bbob"};
int in[] = {5, 6, 7, 8, 9, 10};
void wait1Sec(short n);

void useString(string * s)
{
	string b;
	for (short i = 0; i < len; i++)
	{
		b = *s++; // s[i] does not work
		nxtDisplayString(i, "%s aoetu", b);
		//nxtDisplayString(1, b);
		//nxtDisplayString(3, "%s ");
	}
}

task main()
{
	/*
	nxtDisplayString(2, "%s bobto", str[3]);
	string b = str[3];
	nxtDisplayString(3, "%s  aoeu", b);
	*/
	useString(str);

	wait1Sec(5);
}



void wait1Sec(short n)
{
	for ( ; n > 0; n--)
		wait1Msec(1000);
}
