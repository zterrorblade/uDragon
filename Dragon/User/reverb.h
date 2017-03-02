/******************************************************************************
 *@file       reverb.h
 *@brief      uDragon NUC442 I2S Driver effect reverb head file
 *@version    1.0.1
 *@date    	  1, March, 2017
 *@author     zterrorblade
 *@note       
 *****************************************************************************/
 
 #include <NUC472_442.h>
 #include <config.h>
 #include "process.h"
 
 #ifndef __REVERB_H__
 #define __REVERB_H__
 
 extern inline I2STYPE reverb(void);
 extern POS u32di_buffpos, u32da1_buffpos, u32da2_buffpos;
 
 #endif
 