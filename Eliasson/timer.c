//#include "chip.h"
#include "board.h"
#include "timer.h"

#define SAMPLERATE (1000)

static bool timerInterruptOccured  = false;

void timerSetup(void)
{
  /* Enable timer 1 clock and reset it */
  Chip_TIMER_Init(LPC_TIMER1);
  Chip_RGU_TriggerReset(RGU_TIMER1_RST);
  while (Chip_RGU_InReset(RGU_TIMER1_RST)) {}

  /* Timer setup for match and interrupt at SAMPLERATE */
  Chip_TIMER_Reset(LPC_TIMER1);
  Chip_TIMER_MatchEnableInt(LPC_TIMER1, 1);
  Chip_TIMER_SetMatch(LPC_TIMER1, 1, (Chip_Clock_GetRate(CLK_MX_TIMER1) / SAMPLERATE));
  Chip_TIMER_ResetOnMatchEnable(LPC_TIMER1, 1);
  Chip_TIMER_Enable(LPC_TIMER1);

  /* Enable timer interrupt */
  NVIC_EnableIRQ(TIMER1_IRQn);
  NVIC_ClearPendingIRQ(TIMER1_IRQn);
}

void TIMER1_IRQHandler(void)
{
  /* Clear timer interrupt */
  Chip_TIMER_ClearMatch(LPC_TIMER1, 1);

  /* Reset descriptor pointer to table 0, descriptor 0 and force SW trigger */
  //Chip_HSADC_SetActiveDescriptor(LPC_ADCHS, 0, 0);
  //Chip_HSADC_SWTrigger(LPC_ADCHS);
  timerInterruptOccured = true;
}

void timerWaitForInterrupt1ms()
{
  // Spinlock
  while (!timerInterruptOccured);
  timerInterruptOccured = false;
}


void timerWaitMS(uint16_t timeValueMs)
{
   for (int i = 0; i < timeValueMs; i++);
}
