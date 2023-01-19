#ifndef __GPIO_H__
#define __GPIO_H__
#include <stdbool.h>

void gpioInit();
void gpioTogglePin();

bool gpioReadP11();

void setGPIO1_0_High();
void setGPIO1_0_Low();

#endif
