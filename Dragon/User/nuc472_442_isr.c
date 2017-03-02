/******************************************************************************
 * @file     config.h
 * @brief    uDragon Nuc442 I2S Driver interrupt handler file
 * @version  1.0.1
 * @date     13, Janurary, 2017
 * @author   zterrorblade
 * @note
 ******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "NUC472_442.h"
#include "process.h"

  /*  I2S Interrupt Request Handler  */
void I2S1_IRQHandler(void)
{
	uint32_t u32reg;/* get interrupt register flag */
	uint32_t u32len;/* get data number of input */
	static I2STYPE *prx_buff = rx_buff, *ptx_buff = tx_buff;
	
	u32reg = I2S_GET_INT_FLAG(I2S1, I2S_STATUS_TXIF_Msk | I2S_STATUS_RXIF_Msk);
	
	/*  receive data  */
	if (u32reg & I2S_STATUS_RXIF_Msk) 
	{
		u32len = I2S_GET_RX_FIFO_LEVEL(I2S1);
		u32datacount += u32len;
		while(u32len-- > 0)
		{
			*prx_buff++ = I2S_READ_RX_FIFO(I2S1);
			if(prx_buff >= rx_buff + RXBUFF_LEN)
				prx_buff = rx_buff;
		}
/*		for ( i = 0; i < u32len; i++ ) 
		{
			*prx_buff++ = I2S_READ_RX_FIFO(I2S1);
			if(prx_buff >= rx_buff + RXBUFF_LEN)
				prx_buff = rx_buff;
		}*/
	}	
	
	/*  transmit data   */
	if (u32reg & I2S_STATUS_TXIF_Msk) 
	{
		u32len = 8 - I2S_GET_TX_FIFO_LEVEL(I2S1);
		while(u32len-- > 0)
		{
			I2S_WRITE_TX_FIFO(I2S1, *ptx_buff++);
			if(ptx_buff >= tx_buff + TXBUFF_LEN)
				ptx_buff = tx_buff;
		}
/*		for ( i = 0; i < u32len; i++ ) 
		{
			I2S_WRITE_TX_FIFO(I2S1, *ptx_buff++);
			if(ptx_buff >= tx_buff + TXBUFF_LEN)
				ptx_buff = tx_buff;
		}*/
	}	
}

