/******************************************************************************
 * @file     process.c
 * @brief    uDragon Nuc442 I2S Driver effect process head file
 * @version  1.0.1
 * @date     13, Janurary, 2017
 * @author   zterrorblade
 * @note
 ******************************************************************************/
#include  "NUC472_442.h"
#include  "config.h" 

#ifndef __PROCESS_H__
#define __PROCESS_H__

typedef uint32_t I2STYPE;

extern I2STYPE volatile rx_buff[RXBUFF_LEN];
extern I2STYPE volatile tx_buff[TXBUFF_LEN];
extern uint32_t volatile u32datacount;
extern void process(void);

#endif
