/******************************************************************************
 * @file     echo.c
 * @brief    uDragon NUC442 I2S Driver effect echo file
 * @version  1.0.1
 * @date     27, February, 2017
 * @author   zterrorblade
 * @note     
 ******************************************************************************/
 
 #include <NUC472_442.h>
 #include <config.h>
 #include "process.h"
 #include "echo.h"
 
 //?βε?????????β????????????????buffpos????????????????????????? 
 
 inline I2STYPE singleecho(POS size, float a)
 {
 	POS u32del_buffpos;
	if(u32rx_buffpos >= size)
		u32del_buffpos = u32rx_buffpos - size;
	else
		u32del_buffpos = u32rx_buffpos - size + RXBUFF_LEN;
	return (rx_buff[u32rx_buffpos] + a * rx_buff[u32del_buffpos]);
 }
 inline I2STYPE multiecho(POS size, float a, uint32_t n)
 {
	if(n == 0)
	{
		POS u32del_buffpos;
		if(u32tx_buffpos >= size)
	 		u32del_buffpos = u32tx_buffpos - size;
 		else
 			u32del_buffpos = u32tx_buffpos - size + TXBUFF_LEN;	
		return (rx_buff[u32rx_buffpos] + a * tx_buff[u32del_buffpos]);
	}
	else
	{
		POS u32del_buffpos;
		if(u32rx_buffpos >= size)
	 		u32del_buffpos = u32rx_buffpos - size;
 		else
 			u32del_buffpos = u32rx_buffpos - size + RXBUFF_LEN;
		return  (rx_buff[u32rx_buffpos] + a * rx_buff[u32del_buffpos]);
	}
 }
 inline I2STYPE allpass(I2STYPE *rxbuf, POS rsize, I2STYPE *txbuf, POS tsize, POS rxpos, POS txpos, POS size, float a)
 {
 	POS u32rdel_buffpos, u32tdel_buffpos;//delay position of receive and transmit buffer 
 	if(rxpos >= size)
 		u32rdel_buffpos = rxpos - size;
	else
		u32rdel_buffpos = rxpos -size + rsize;
	if(txpos >= size)
		u32tdel_buffpos = txpos - size;
	else
		u32tdel_buffpos = txpos - size + tsize;
 	return  (rxbuf[u32rdel_buffpos] + a * (rxbuf[rxpos] - txbuf[u32tdel_buffpos]));
 }