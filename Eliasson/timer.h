#ifndef __TIMER_H__
#define __TIMER_H__

void timerSetup(void);

void timerWaitForInterrupt1ms();

void timerWaitMS(uint16_t timeValueMs);


#endif