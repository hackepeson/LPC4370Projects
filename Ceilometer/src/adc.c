#include <stdio.h>
#include "chip.h"
#include "adc.h"
#include "gpio.h"


#define HSADC_DMA_READ    8

#define DMA_CH            7
#define NUM_SAMPLE        DMA_TRANSFER_SIZE
uint32_t sample[NUM_SAMPLE];
uint32_t summedDataVector[DMA_TRANSFER_SIZE];

void initADC()
{
  // Setup the clock, feeding the ADC ~30 MHz
  Chip_USB0_Init(); // Initialize the USB0 PLL to 480 MHz
  Chip_Clock_SetDivider(CLK_IDIV_A, CLKIN_USBPLL, 4); // Source DIV_A from USB0PLL, and set divider to 2 (Max div value supported is 4) [IN 480 MHz; OUT 240 MHz
  Chip_Clock_SetDivider(CLK_IDIV_B, CLKIN_IDIVA, 2); // Source DIV_B from DIV_A, [IN 240 MHz; OUT 80 MHz
  Chip_Clock_SetBaseClock(CLK_BASE_ADCHS, CLKIN_IDIVB, true, false); // Source ADHCS base clock from DIV_B

   /////////////////////////////////////////// HSADC settings ////////////////////////////////////////////////////////////////
  LPC_ADCHS->INTS[0].CLR_EN   = 0x7F; // disable interrupt 0
  LPC_ADCHS->INTS[0].CLR_STAT = 0x7F; // clear interrupt status
  while(LPC_ADCHS->INTS[0].STATUS & 0x7D); // wait for status to clear, have to exclude FIFO_EMPTY

  LPC_ADCHS->INTS[1].CLR_EN   = 0x7F;
  LPC_ADCHS->INTS[1].CLR_STAT = 0x7F;
  while(LPC_ADCHS->INTS[1].STATUS & 0x7D);

  /* Initialize HSADC */
  LPC_ADCHS->POWER_DOWN = 0;
  LPC_ADCHS->FLUSH = 1;
  Chip_HSADC_Init(LPC_ADCHS);
  LPC_ADCHS->FIFO_CFG      = (15 << 1) /* FIFO_LEVEL*/ /* | (1) */ /* PACKED_READ*/;
  LPC_ADCHS->DSCR_STS = 1;
  LPC_ADCHS->DESCRIPTOR[0][0] = (1 << 31) /* UPDATE TABLE*/ | (1 << 24) /* RESET_TIMER*/ | (0 << 22) /* THRESH*/ | (0xA00 << 8) /* MATCH*/ | (0x10 << 6) /* BRANCH*/;
  LPC_ADCHS->DESCRIPTOR[1][0] = (1 << 31) /* UPDATE TABLE*/ | (1 << 24) /* RESET_TIMER*/ | (0 << 22) /* THRESH*/ | (0x01 << 8) /* MATCH*/ | (0x01 << 6) /* BRANCH*/;
  LPC_ADCHS->CONFIG        = (0x90 << 6) /* RECOVERY_TIME*/ | (0 << 5) /* CHANNEL_ID_EN*/| (0x01) /* TRIGGER_MASK*/;

  uint8_t DGEC = 0xE; // 80 MHz
  LPC_ADCHS->ADC_SPEED     = (DGEC << 16) | (DGEC << 12) | (DGEC << 8) | (DGEC << 4) | (DGEC);

  //Didn't set threshold registers as they aren't used

  LPC_ADCHS->POWER_CONTROL = (1 << 18) /* BGAP*/ | (1 << 17) /* POWER*/ | (0 << 16) /* TWOS */ | (1 << 10) /* DC in ADC0*/ | (1 << 4) | (0x4) /* CRS*/;
  Chip_HSADC_FlushFIFO(LPC_ADCHS);

  ////////////////////////////////////////////////////// DMA settings ///////////////////////////////////////////////////////////////////

  Chip_GPDMA_Init(LPC_GPDMA);
  LPC_GPDMA->CONFIG =   0x01;

  while( !(LPC_GPDMA->CONFIG & 0x01) );
  /* Clear all DMA interrupt and error flag */

  LPC_GPDMA->INTTCCLEAR = 0xFF; //clears channel terminal count interrupt
  LPC_GPDMA->INTERRCLR = 0xFF; //clears channel error interrupt.

  LPC_GPDMA->CH[DMA_CH].SRCADDR  =  (uint32_t) &LPC_ADCHS->FIFO_OUTPUT[0];
  LPC_GPDMA->CH[DMA_CH].DESTADDR = ((uint32_t) &sample);
  LPC_GPDMA->CH[DMA_CH].CONTROL  =  (DMA_TRANSFER_SIZE) // transfer size
                                    | (0x0 << 12)       // src burst size
                                    | (0x0 << 15)       // dst burst size
                                    | (0x2 << 18)       // src transfer width
                                    | (0x2 << 21)       // dst transfer width
                                    | (0x1 << 24)       // src AHB master select
                                    | (0x0 << 25)       // dst AHB master select
                                    | (0x0 << 26)       // src increment: 0, src address not increment after each trans
                                    | (0x1 << 27)       // dst increment: 1, dst address     increment after each trans
                                    | (0x1 << 31);      // terminal count interrupt enable bit: 1, enabled




    LPC_GPDMA->CH[DMA_CH].LLI      =  1;

}

void startADC()
{
  Chip_HSADC_SWTrigger(LPC_ADCHS);

}

void stopADC()
{

}

uint16_t performADCAndFetchOneVector(uint16_t* vector)
{
  uint16_t noOfADCData = 0;
   /* Clear all DMA interrupt and error flag */
  LPC_GPDMA->INTTCCLEAR = 0xFF; //clears channel terminal count interrupt
  LPC_GPDMA->INTERRCLR = 0xFF; //clears channel error interrupt.

  LPC_GPDMA->CH[DMA_CH].SRCADDR  =  (uint32_t) &LPC_ADCHS->FIFO_OUTPUT[0];
  LPC_GPDMA->CH[DMA_CH].DESTADDR = ((uint32_t) &sample);
  LPC_GPDMA->CH[DMA_CH].CONTROL  =  (DMA_TRANSFER_SIZE)   // transfer size
    | (0x0                << 12)  // src burst size
    | (0x0                << 15)  // dst burst size
    | (0x2                << 18)  // src transfer width
    | (0x2                << 21)  // dst transfer width
    | (0x1                << 24)  // src AHB master select
    | (0x0                << 25)  // dst AHB master select
    | (0x0                << 26)  // src increment: 0, src address not increment after each trans
    | (0x1                << 27)  // dst increment: 1, dst address     increment after each trans
    | (0x1                << 31); // terminal count interrupt enable bit: 1, enabled

   Chip_HSADC_FlushFIFO(LPC_ADCHS);


   LPC_GPDMA->CH[DMA_CH].CONFIG   =      (0x1                << 0)   // enable bit: 1 enable, 0 disable
                                    | (HSADC_DMA_READ     << 1)   // src peripheral: set to 8   - HSADC
                                    | (0x0                << 6)   // dst peripheral: no setting - memory
//                                    | (0x6                << 11)  // flow control: peripheral to memory - DMA control
                                    | (0x1                << 14)  // IE  - interrupt error mask
                                    | (0x1                << 15)  // ITC - terminal count interrupt mask
                                    | (0x0                << 16)  // lock: when set, this bit enables locked transfer
//                                    | (0x1                << 18); // Halt: 1, enable DMA requests; 0, ignore further src DMA req
;
//gpioTogglePin();
setGPIO1_0_High();




   while ((LPC_GPDMA->RAWINTTCSTAT & 0x80) != 0x80);
   setGPIO1_0_Low();
   for (int i = 0; i < DMA_TRANSFER_SIZE; i++)
   {
      if (sample[i] != 0x8000)
      {
        *vector++ = sample[i];
        noOfADCData++;
      }
   }
   setGPIO1_0_Low();
   return noOfADCData;
}

void performADCAndFetchCumlativeVector(uint32_t* vector)
{
  LPC_GPDMA->CH[DMA_CH].CONFIG   =      (0x1                << 0)   // enable bit: 1 enable, 0 disable
                                    | (HSADC_DMA_READ     << 1)   // src peripheral: set to 8   - HSADC
                                    | (0x0                << 6)   // dst peripheral: no setting - memory
//                                    | (0x6                << 11)  // flow control: peripheral to memory - DMA control
                                    | (0x1                << 14)  // IE  - interrupt error mask
                                    | (0x1                << 15)  // ITC - terminal count interrupt mask
                                    | (0x0                << 16)  // lock: when set, this bit enables locked transfer
//                                    | (0x1                << 18); // Halt: 1, enable DMA requests; 0, ignore further src DMA req
;

}
