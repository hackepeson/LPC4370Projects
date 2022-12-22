#include <stdbool.h>
#include <string.h>
#include "chip.h"
#include "board.h"
#include "adc.h"
#include "timer.h"
#include "gpio.h"

#define LPC_UARTX (LPC_USART2)

int main() {
	SystemCoreClockUpdate();
	Board_Init();
	Board_UART_Init(LPC_UARTX);
	Board_LED_Set(0, false);

	/* Setup UART for 115.2K8N1 */
	Chip_UART_Init(LPC_UARTX);
	Chip_UART_SetBaud(LPC_UARTX, 115200);
	Chip_UART_ConfigData(LPC_UARTX, (UART_LCR_WLEN8 | UART_LCR_SBS_1BIT));
	Chip_UART_SetupFIFOS(LPC_UARTX, (UART_FCR_FIFO_EN | UART_FCR_TRG_LEV2));
	Chip_UART_TXEnable(LPC_UARTX);

	initADC();
	startADC();
	gpioInit();

	uint16_t sampleVector[DMA_TRANSFER_SIZE];
	uint32_t sampleVectorSUM[DMA_TRANSFER_SIZE];
	uint16_t MinNoOfSampleData = DMA_TRANSFER_SIZE + 1;

#define NO_OF_SAMPLE (8000)

	timerSetup();

	/*
	 while (1)
	 {
	 printf("%d\n",gpioReadP11());
	 }
	 */
	//  bool toggle;
	char txData[8];
	while (1) {
		memset(sampleVectorSUM, 0, sizeof(sampleVectorSUM));
		MinNoOfSampleData = DMA_TRANSFER_SIZE + 1;
		for (int i = 0; i < NO_OF_SAMPLE; i++) {
			uint16_t noOfData;
			//timerWaitForInterrupt1ms();

			//if (gpioReadP11())
			if (1) {
				noOfData = performADCAndFetchOneVector(sampleVector);
			}
			while (gpioReadP11())
				while (0) {
					__NOP();
				}
			/*
			 for (int i = 0; i < noOfData; i++)
			 {
			 printf("%d\n", sampleVector[i]);
			 }
			 */

			if (MinNoOfSampleData > noOfData) {
				MinNoOfSampleData = noOfData;
			}
			for (int n = 0; n < DMA_TRANSFER_SIZE; n++) {
				sampleVectorSUM[n] += sampleVector[n];
			}
		}

		//toggle = !toggle;
		for (int i = 0; i < MinNoOfSampleData; i++) {
			sprintf(txData, "%d ", sampleVector[i]);
			//uint32_t lsr = Chip_UART_ReadLineStatus(LPC_UARTX);

			Chip_UART_SendBlocking(LPC_UARTX, txData, strlen(txData));
		}

		// Add carrigereturn
		txData[0] = '\r';
		txData[1] = '\n';
		txData[2] = 0;

		Chip_UART_SendBlocking(LPC_UARTX, txData, strlen(txData));

		/*
		 if (toggle)
		 {
		 Board_LED_Set(0, false);
		 }
		 else
		 {
		 Board_LED_Set(0, true);
		 }
		 */

		for (int i = 0; i < MinNoOfSampleData; i++) {
			printf("%d\n", sampleVectorSUM[i]);
		}
		printf("Min no of sample point = %d\n", MinNoOfSampleData);

	}
	return 0;
}
