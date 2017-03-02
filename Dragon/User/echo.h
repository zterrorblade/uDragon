/******************************************************************************
 *@file       echo.h
 *@brief      uDragon NUC442 I2S Driver effect echo head file
 *@version    1.0.1
 *@date    	  27, February, 2017
 *@author     zterrorblade
 *@note       effect echo include singleecho mutliecho and allpass filter
 *****************************************************************************/
 
 #include <NUC472_442.h>
 #include <config.h>
 #include "process.h"
 
 #ifndef __ECHO_H__
 #define __ECHO_H__
 
 #define DELAY_SIZE 960
 #define DECAY 0.5
 #define NUM 0

 
 extern inline I2STYPE singleecho(POS, float);//single echo
 extern inline I2STYPE multiecho(POS, float, uint32_t);//infinite echo, ingore finite echo, namely NUM=0
 extern inline I2STYPE allpass(I2STYPE *, POS, I2STYPE *, POS, POS, POS, POS, float);
 //extern inline void echo(U32VOL, U32VOL); 
 
 #endif
 