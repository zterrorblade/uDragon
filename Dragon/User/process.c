/******************************************************************************
 * @file     process.c
 * @brief    uDragon Nuc442 I2S Driver effect process file
 * @version  1.0.1
 * @date     13, Janurary, 2017
 * @author   zterrorblade
 * @note
 ******************************************************************************/
 
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "NUC472_442.h"
#include "config.h"

/* !!!Maintain the consistency of declaration and definition */
#include "process.h"

#ifdef DATATYPE_UINT32
uint32_t volatile rx_buff[RXBUFF_LEN] = {0}; //receive buffer
uint32_t volatile tx_buff[TXBUFF_LEN] = {0}; //transmit buffer
#else
int32_t volatile rx_buff[RXBUFF_LEN] = {0};
int32_t volatile tx_buff[TXBUFF_LEN] = {0}; 
#endif

uint32_t volatile u32rx_buffpos = 0, u32tx_buffpos = 0; //Position of Rx and Tx Buffer
uint32_t volatile u32datacount = 0; //num need to be processed
/*uint32_t u32transpos = 0; //position of transform*/

void process()
{
	while(u32datacount > 0)
	{
		tx_buff[u32tx_buffpos++] = rx_buff[u32rx_buffpos++];
/*		u32transpos++ ;	 */
		u32datacount--;	
		if(u32rx_buffpos == RXBUFF_LEN)
			u32rx_buffpos -= RXBUFF_LEN;
		if(u32tx_buffpos == TXBUFF_LEN)
			u32tx_buffpos -= TXBUFF_LEN;	
	}
}
