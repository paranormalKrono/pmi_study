#ifndef	_TIME_DEBUG_H_
#define	_TIME_DEBUG_H_
#include "MathPMI.h"
#include <time.h>

typedef enum clock_index {
	clock1, clock2
} clock_type;

void startTimer();
void stopTimer();
double getTimerClock();
double getClock(clock_type ct);

#endif // _TIME_DEBUG_H_