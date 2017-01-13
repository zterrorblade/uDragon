/******************************************************************************
 * @file     main.c
 * @brief    uDragon Nuc442 I2S Driver main file
 * @version  1.0.1
 * @date     13, Janurary, 2017
 * @author   zterrorblade
 * @note
 ******************************************************************************/
 
#include <stdio.h>
#include <string.h>
#include "NUC472_442.h"
#include "process.h"

/* uint8_t item; */
void Delay(uint32_t count)
{
	uint32_t volatile i;
	for (i = 0; i < count ; i++);
}

void SYS_Init(void)
{
	/*---------------------------------------------------------------------------------------------------------*/
	/* Init System Clock                                                                                       */
	/*---------------------------------------------------------------------------------------------------------*/
	/* Unlock protected registers */
	SYS_UnlockReg();

	/* Enable External XTAL (4~24 MHz) */
	CLK_EnableXtalRC(CLK_PWRCTL_HXTEN_Msk);

	/* Waiting for 12.288MHz clock ready */
	/* !!!Modify XTAL in head file in "system_NUC_472_442.h" __HXT */
	CLK_WaitClockReady( CLK_STATUS_HXTSTB_Msk);

	/* Switch HCLK clock source to HXT */
	CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_HXT,CLK_CLKDIV0_HCLK(1));

	/* Set PLL to power down mode and PLL_STB bit in CLKSTATUS register will be cleared by hardware.*/
	CLK->PLLCTL |= CLK_PLLCTL_PD_Msk;

	/* Set PLL frequency */
	/* Modify __HXT in clk.h line 58*/
	/* Thus the actual frequence of PLL is 12.288MHz*7=86.016MHz */
	/* Here not modify the #define in the head file clk.h*/
	CLK->PLLCTL = CLK_PLLCTL_84MHz_HXT;

	/* Waiting for clock ready */
	CLK_WaitClockReady(CLK_STATUS_PLLSTB_Msk);

	/* Switch HCLK clock source to PLL */
	CLK_SetHCLK(CLK_CLKSEL0_HCLKSEL_PLL,CLK_CLKDIV0_HCLK(1));

	/* Enable IP clock */
	CLK_EnableModuleClock(UART0_MODULE);
	CLK_EnableModuleClock(TMR0_MODULE);
	CLK_EnableModuleClock(I2S1_MODULE);

	/* Select IP clock source */
	CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UARTSEL_HXT, CLK_CLKDIV0_UART(1));
	CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_HXT, 0); 

	/* Update System Core Clock */
	/* User can use SystemCoreClockUpdate() to calculate SystemCoreClock. */
	SystemCoreClockUpdate();

	/*---------------------------------------------------------------------------------------------------------*/
	/* Init I/O Multi-function                                                                                 */
	/*---------------------------------------------------------------------------------------------------------*/
	/* Set GPG multi-function pins for UART0 RXD and TXD */
	SYS->GPG_MFPL = SYS_GPG_MFPL_PG1MFP_UART0_RXD | SYS_GPG_MFPL_PG2MFP_UART0_TXD ;

	/* Set multi function pin for I2S1 */
	/* GPG7, GPG8, GPG9, GPG5, GPG6 */
	SYS->GPG_MFPL = (SYS->GPG_MFPL & ~SYS_GPG_MFPL_PG7MFP_Msk) | SYS_GPG_MFPL_PG7MFP_I2S1_MCLK;
	SYS->GPG_MFPH = (SYS->GPG_MFPH & ~(SYS_GPG_MFPH_PG8MFP_Msk | SYS_GPG_MFPH_PG9MFP_Msk)) | (SYS_GPG_MFPH_PG8MFP_I2S1_DO | SYS_GPG_MFPH_PG9MFP_I2S1_DI);
	SYS->GPG_MFPL = (SYS->GPG_MFPL & ~(SYS_GPG_MFPL_PG5MFP_Msk | SYS_GPG_MFPL_PG6MFP_Msk)) | (SYS_GPG_MFPL_PG5MFP_I2S1_BCLK | SYS_GPG_MFPL_PG6MFP_I2S1_LRCK);

	/* Lock protected registers */
	SYS_LockReg();
	}

	/*---------------------------------------------------------------------------------------------------------*/
	/*  Main Function                                                                                          */
	/*---------------------------------------------------------------------------------------------------------*/
	int32_t main (void)
	{
	/* Init System, IP clock and multi-function I/O */
	SYS_Init();

	/* Init UART to 115200-8n1 for print message */
	UART_Open(UART0, 115200);

	printf("+------------------------------------------------------------------------+\n");
	printf("|                       I2S Driver Sample Code with                      |\n");
	printf("+------------------------------------------------------------------------+\n");
	printf("         NOTE: This sample code works with AK5357 and AK4386.             \n");

	GPIO_SetMode(PD,BIT3,GPIO_MODE_OUTPUT);
	PD3 = 1;   //PIN 24

	// ADC define
	GPIO_SetMode(PE,BIT12,GPIO_MODE_OUTPUT);  // AD.CKS1
	PE12 = 1;   //PIN 1
	GPIO_SetMode(PE,BIT13,GPIO_MODE_OUTPUT);  // AD.CKS2
	PE13 = 0;   //PIN 2
	GPIO_SetMode(PE,BIT14,GPIO_MODE_OUTPUT);  // AD.DIF
	PE14 = 1;   //PIN 3
	GPIO_SetMode(PE,BIT15,GPIO_MODE_OUTPUT);  // AD.PDN
	PE15 = 1;   //PIN 4
	GPIO_SetMode(PF,BIT9,GPIO_MODE_OUTPUT);  // AD.CKS0
	PF9 = 0;    //PIN 5

	// DAC define
	GPIO_SetMode(PF,BIT10,GPIO_MODE_OUTPUT);  // DA.PDN
	PF10 = 1;   //PIN 6
	GPIO_SetMode(PF,BIT11,GPIO_MODE_OUTPUT);  // DA.DFS0
	PF11 = 0;   //PIN 7
	GPIO_SetMode(PF,BIT12,GPIO_MODE_OUTPUT);  // DA.DFS1
	PF12 = 0;   //PIN 8
	GPIO_SetMode(PF,BIT13,GPIO_MODE_OUTPUT);  // DA.DEM
	PF13 = 0;   //PIN 9
	GPIO_SetMode(PF,BIT14,GPIO_MODE_OUTPUT);  // DA.DIF0
	PF14 = 1;   //PIN 10
	GPIO_SetMode(PF,BIT15,GPIO_MODE_OUTPUT);  // DA.DIF1
	PF15 = 1;   //PIN 13

	NVIC_EnableIRQ(I2S1_IRQn);/* enable nested vector interrput controller */

	// select source from HXT(12.288MHz)
	CLK_SetModuleClock(I2S1_MODULE,CLK_CLKSEL3_I2S1SEL_HXT,0); 

	//SET I2S AS THE "STEREO" OUTPUT
	I2S_Open(I2S1, I2S_MODE_SLAVE, 48000, I2S_DATABIT_24, I2S_MONO, I2S_FORMAT_I2S, I2S_I2S); 

	// Set MCLK and enable MCLK
	I2S_EnableMCLK(I2S1,12288000);  

	// I2S will store data in left channel
	I2S_SET_MONO_RX_CHANNEL(I2S1, I2S_MONO_LEFT);   

	I2S_EnableInt(I2S1, I2S_IEN_RXTHIEN_Msk);  // Enable Rx threshold level interrupt   
	I2S_ENABLE_RX(I2S1);  // Enable I2S Rx function to receive data

	// Enable I2S Tx function to send data when data in the buffer is more than half of buffer size
	I2S_EnableInt(I2S1, I2S_IEN_TXTHIEN_Msk);
	I2S_ENABLE_TX(I2S1);

	while(1)
	{
		process();    //process data
	/* while(TransPos < u32RxBuffPos)
	{	
	TxBuff[TransPos] = RxBuff[TransPos];

	TransPos++ ;	   
	}   */
	/*     item = getchar();
	printf("%c\n",item);  */
	}
}

/*** (C) COPYRIGHT 2016 Eno Technology Corp. ***/
