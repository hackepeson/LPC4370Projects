#ifndef __GPIO_H__
#define __GPIO_H__
#include <stdbool.h>

void gpioInit();
void gpioTogglePin();

bool gpioReadP11();

#endif