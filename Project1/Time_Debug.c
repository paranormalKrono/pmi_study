#include "Time_Debug.h"


clock_t c1, c2;
void startTimer()
{
	c1 = clock();
}
void stopTimer()
{
	c2 = clock();
}
double getTimerClock()
{
	return ((double)c2 - (double)c1) / CLOCKS_PER_SEC;
}

double getClock(clock_type ct) {
	switch (ct)
	{
	case clock1:
		return (double)c1 / CLOCKS_PER_SEC;
	case clock2:
		return (double)c2 / CLOCKS_PER_SEC;
	default:
		break;
	}
}

