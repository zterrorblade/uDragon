/******************************************************************************
 * @file     reverb.c
 * @brief    uDragon NUC442 I2S Driver effect reverb file
 * @version  1.0.1
 * @date     1, March, 2017
 * @author   zterrorblade
 * @note     
 ******************************************************************************/
 
#include <NUC472_442.h>
#include <config.h>
#include "process.h"
#include "echo.h"
#include "reverb.h"
 
POS delaysize[] = {700, 900, 600, 400, 450, 390};//delay
float attenuation[] = {0.6, 0.4, 0.2, 0.1, 0.7, 0.6, 0.8};//attenuation (可不可以加const)
I2STYPE d_iir[BUFF_HALF_LEN], d_all1[BUFF_HALF_LEN], d_all2[BUFF_HALF_LEN];//three temp buffer to store temp date
POS u32di_buffpos = 0, u32da1_buffpos = 0, u32da2_buffpos = 0;//record position of the temp buffer

inline void reverb(void)
{
	d_iir[u32di_buffpos] = multiecho(delaysize[0], attenuation[0], NUM) + \
						   multiecho(delaysize[1], attenuation[1], NUM) + \
						   multiecho(delaysize[2], attenuation[2], NUM) + \
						   multiecho(delaysize[3], attenuation[3], NUM);
	d_all1[u32da1_buffpos] = allpass(d_iir, BUFF_HALF_LEN, d_all1, BUFF_HALF_LEN, \
	u32di_buffpos, u32da1_buffpos, delaysize[4], attenuation[4]);
	d_all2[u32da2_buffpos] = allpass(d_all1, BUFF_HALF_LEN, d_all2, BUFF_HALF_LEN, \
	u32da1_buffpos, u32da2_buffpos, delaysize[5], attenuation[5]);
	return (rx_buff[u32rx_buffpos] + attenuation[6] * d_all2[u32da2_buffpos]);
}
