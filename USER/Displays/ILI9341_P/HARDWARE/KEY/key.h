#ifndef __KEY_H
#define __KEY_H	 
#include "displays/ILI9341_P/system/sys/sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//
//////////////////////////////////////////////////////////////////////////////////	  	 


//#define KEY0   PCin(5)   	
//#define KEY1   PAin(15)	 //Key Right
//#define WK_UP  PAin(2)	 
 

#define KEY0  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)   //Key Right
#define KEY1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) 
#define WK_UP GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)
 

#define KEY0_PRES	1		//KEY0  Key Right
#define KEY1_PRES	2		//KEY1
#define WKUP_PRES	3		//WK_UP  

void KEY_Init(void);
u8 KEY_Scan(void);
extern void Delay_1ms(uint16_t time);
#endif
