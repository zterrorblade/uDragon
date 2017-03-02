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
#include "echo.h"
#include "reverb.h"
#include "flange.h"
#include "chorus.h"
#include "phase.h"

/*# ifdef DATATYPE_UINT32 */
I2STYPE rx_buff[RXBUFF_LEN] = {0}; //receive buffer，init as zero in order to make it more convinent 
I2STYPE tx_buff[TXBUFF_LEN] = {0}; //transmit buffer
/* #else
int32_t volatile rx_buff[RXBUFF_LEN] = {0};
int32_t volatile tx_buff[TXBUFF_LEN] = {0}; 
#endif */

POS u32rx_buffpos = 0, u32tx_buffpos = 0; //Position of Rx and Tx Buffer
POS u32datacount = 0; //num need to be processed
/*uint32_t u32transpos = 0; //position of transform*/
//换成定义u32rxd_buffpos和u32txd_buffpos?

void process()
{
	while(u32datacount-- > 0)
	{
	/*	U32VOL u32del_buffpos;
		if(u32rx_buffpos >= size)
	 		u32del_buffpos = u32rx_buffpos - DELAY_SIZE;
 		else
 			u32del_buffpos = u32rx_buffpos - DELAY_SIZE + RXBUFF_LEN;*/
		#ifdef ECHOING
			tx_buff[u32tx_buffpos] = singleecho(DELAY_SIZE, DECAY);
			tx_buff[u32tx_buffpos] = multiecho(DELAY_SIZE, DECAY, NUM);//只考虑NUM=0的情况 
			tx_buff[u32tx_buffpos] = allpass(rx_buff, RXBUFF_LEN, \
			rx_buff, TXBUFF_LEN, u32rx_buffpos, u32tx_buffpos, DELAY_SIZE, DECAY);
		#endif
		#ifdef REVERBING
			tx_buff[u32tx_buffpos] = reverb();
			if(++u32di_buffpos >= BUFF_HALF_LEN)
				u32di_buffpos -= BUFF_HALF_LEN;
			if(++u32da1_buffpos >= BUFF_HALF_LEN)
				u32da1_buffpos -= BUFF_HALF_LEN;
			if(++u32da2_buffpos >= BUFF_HALF_LEN)
				u32da2_buffpos -= BUFF_HALF_LEN;
		#endif
		#ifdef FLANGING
			flange();
		#endif
		#ifdef CHORUSING
			chorus();
		#endif
		#ifdef PHASING
			phase();
		#endif
//		tx_buff[u32tx_buffpos++] = rx_buff[u32rx_buffpos++];
/*		u32transpos++ ;	 */
		if(++u32rx_buffpos >= RXBUFF_LEN)
			u32rx_buffpos -= RXBUFF_LEN;
		if(++u32tx_buffpos >= TXBUFF_LEN)
			u32tx_buffpos -= TXBUFF_LEN;
	}
}
