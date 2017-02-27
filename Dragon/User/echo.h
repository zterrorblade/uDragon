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
 
 #ifndef __ECHO_H__
 #define __ECHO_H__
 
 extern void singleecho(void);
 extern void mutliecho(void);
 extern void allpass(void);
 
 #endif
 