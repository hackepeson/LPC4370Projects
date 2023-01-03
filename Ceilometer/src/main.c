#include <stdbool.h>
#include <string.h>
#include "chip.h"
#include "board.h"
#include "adc.h"
#include "timer.h"
#include "gpio.h"



////////////////////// SPI STUFF /////////////////////////////
#define LPC_SSP   (LPC_SSP1)
#define BUFFER_SIZE                         (2000)
#define SSP_DATA_BITS                       (SSP_BITS_16)
#define SSP_DATA_BIT_NUM(databits)          (databits+1)
#define SSP_DATA_BYTES(databits)            (((databits) > SSP_BITS_16) ? 2:1)
#define SSP_LO_BYTE_MSK(databits)           ((SSP_DATA_BYTES(databits) > 1) ? 0xFF:(0xFF>>(8-SSP_DATA_BIT_NUM(databits))))
#define SSP_HI_BYTE_MSK(databits)           ((SSP_DATA_BYTES(databits) > 1) ? (0xFF>>(16-SSP_DATA_BIT_NUM(databits))):0)
#define SSP_IRQ           					(SSP1_IRQn)
#define SSPIRQHANDLER SSP1_IRQHandler
#define SSP_MODE_SEL                        (0x31)
#define SSP_TRANSFER_MODE_SEL               (0x32)
#define SSP_MASTER_MODE_SEL                 (0x31)
#define SSP_SLAVE_MODE_SEL                  (0x32)
#define SSP_POLLING_SEL                     (0x31)
#define SSP_INTERRUPT_SEL                   (0x32)
#define SSP_DMA_SEL                         (0x33)

static SSP_ConfigFormat ssp_format;
static Chip_SSP_DATA_SETUP_T xf_setup;
static uint16_t Tx_Buf[BUFFER_SIZE];
static uint16_t Rx_Buf[BUFFER_SIZE];
static volatile uint8_t  isXferCompleted = 0;

void SSPIRQHANDLER(void)
{
	Chip_SSP_Int_Disable(LPC_SSP);	/* Disable all interrupt */

	Chip_SSP_Int_RWFrames16Bits(LPC_SSP, &xf_setup);

	if ((xf_setup.rx_cnt != xf_setup.length) || (xf_setup.tx_cnt != xf_setup.length)) {
		Chip_SSP_Int_Enable(LPC_SSP);	/* enable all interrupts */
	}
	else {
		isXferCompleted = 1;
	}
}

static void Buffer_Init(void)
{
	uint16_t i;
	uint8_t ch = 0;

	for (i = 0; i < BUFFER_SIZE; i++) {
		Tx_Buf[i] = ch++;
		Rx_Buf[i] = 0x0000;
	}
}

////////////////////// UART STUFF /////////////////////////////
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

	Board_SSP_Init(LPC_SSP);
	Chip_SSP_Init(LPC_SSP);

	////////////// SPI
	ssp_format.frameFormat = SSP_FRAMEFORMAT_SPI;
	ssp_format.bits = SSP_DATA_BITS;
	ssp_format.clockMode = SSP_CLOCK_MODE0;
	Chip_SSP_SetFormat(LPC_SSP, ssp_format.bits, ssp_format.frameFormat, ssp_format.clockMode);
	Chip_SSP_Enable(LPC_SSP);
	Chip_SSP_SetMaster(LPC_SSP, 0); // Slave
	NVIC_EnableIRQ(SSP_IRQ);

	xf_setup.length = BUFFER_SIZE*2;
	xf_setup.tx_data = Tx_Buf;
	xf_setup.rx_data = Rx_Buf;


	while (1)
	{
		Buffer_Init();
		xf_setup.rx_cnt = xf_setup.tx_cnt = 0;

		// Interrupt mode
/*
		isXferCompleted = 0;
	    Chip_SSP_Int_FlushData(LPC_SSP); // flush dummy data from SSP FiFO
		Chip_SSP_Int_RWFrames16Bits(LPC_SSP, &xf_setup);
		Chip_SSP_Int_Enable(LPC_SSP);	// enable interrupt
		while (!isXferCompleted) {}
*/
	    // Polled mode
		Chip_SSP_Int_FlushData(LPC_SSP); // flush dummy data from SSP FiFO
		Chip_SSP_RWFrames_Blocking(LPC_SSP, &xf_setup);
		for (int i = 0; i < 20; i++)
		{
			printf("%d\n", (uint16_t)Rx_Buf[i]);
		}
		//Chip_SSP_WriteFrames_Blocking(LPC_SSP, Tx_Buf, 2);
		//uint32_t Chip_SSP_WriteFrames_Blocking(LPC_SSP_T *pSSP, const uint8_t *buffer, uint32_t buffer_len)
	}

	///////////// End SPI

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
			/*
			while (gpioReadP11())
				while (0) {
					__NOP();
				}
			*/
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

		/*for (int i = 0; i < MinNoOfSampleData; i++) {
			printf("%lu\n", sampleVectorSUM[i]);
		}
		printf("Min no of sample point = %d\n", MinNoOfSampleData);
		*/

	}
	return 0;
}
