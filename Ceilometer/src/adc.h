#ifndef __ADC_H__
#define __ADC_H__
#include <stdint.h>

#define DMA_TRANSFER_SIZE 1500 // max. 4095

void initADC();
void startADC();
void stopADC();
void performADCAndFetchCumulativeVector(uint32_t* vector);
uint16_t performADCAndFetchOneVector(uint16_t* vector);




#endif