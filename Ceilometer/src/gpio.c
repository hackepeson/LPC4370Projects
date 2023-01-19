#include <stdbool.h>
#include "board.h"
#include "chip.h"
#include "gpio.h"

#define PININT_IRQ_HANDLER  GPIO0_IRQHandler	/* GPIO interrupt IRQ function name */
#define PININT_NVIC_NAME    PIN_INT0_IRQn	/* GPIO interrupt NVIC interrupt name */
#define PININT_INDEX   0	/* PININT index used for GPIO mapping */

#define TEST_INPUT_PORT         1	/* GPIO port number mapped to PININT */
#define TEST_INPUT_PIN          0	/* GPIO pin number mapped to PININT */

#define TEST_INPUT_PIN_PORT     1
#define TEST_INPUT_PIN_BIT      7
void PININT_IRQ_HANDLER(void)
{
  Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(PININT_INDEX));
  Board_LED_Toggle(0);
}


void setGPIO1_0_High()
{

	Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1, 0, false);
	Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 1, 0);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1, 0, true);
}

void setGPIO1_0_Low()
{
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1, 0, false);
	 Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 1, 0);
	Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1, 0, false);
}


void gpioInit()
{
  // P1_0 Output
  //Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1, 0, false);
  //Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 1, 0);
  // P1_1 Input

  Chip_SCU_PinMuxSet(TEST_INPUT_PIN_PORT,TEST_INPUT_PIN_BIT,SCU_MODE_INBUFF_EN); // Enable input buffer for GPIO Port 1 Pin 0 on the LPC-LINK 2

  Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, TEST_INPUT_PORT, TEST_INPUT_PIN);

  Chip_SCU_GPIOIntPinSel(0, TEST_INPUT_PORT, TEST_INPUT_PIN);

  Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(PININT_INDEX));
  Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(PININT_INDEX));
  //Chip_PININT_EnableIntLow(LPC_GPIO_PIN_INT, PININTCH(PININT_INDEX));
  Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(PININT_INDEX));

  /* Enable interrupt in the NVIC */
  NVIC_ClearPendingIRQ(PININT_NVIC_NAME);
  NVIC_EnableIRQ(PININT_NVIC_NAME);



}

void gpioTogglePin()
{
  Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1, 0, false);
  Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1, 0, true);
  Chip_GPIO_SetPinState(LPC_GPIO_PORT, 1, 0, false);

}

bool gpioReadP11()
{
  return Chip_GPIO_GetPinState(LPC_GPIO_PORT, 1, 0);

}
