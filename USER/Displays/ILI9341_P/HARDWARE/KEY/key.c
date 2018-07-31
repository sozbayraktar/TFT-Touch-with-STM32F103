#include "ILI9341_P/hardware/key/key.h"

//////////////////////////////////////////////////////////////////////////////////	 
//
//////////////////////////////////////////////////////////////////////////////////	
 	    
//PA0.15 PC5 
void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
} 


//KEY0_PRES KEY0   // Key Right
//KEY1_PRES KEY1
//WKUP_PRES WK_UP
//KEY0>KEY1>WK_UP!!
u8 KEY_Scan(void)
{	 	  
	if((KEY0==0||KEY1==0||WK_UP==1))  // herhangi bir tusa basilmis mi?
	{
		Delay_1ms(10);
		if(KEY0==0)return KEY0_PRES;       // (1) //Key Right
		else if(KEY1==0)return KEY1_PRES;  // (2)
		else if(WK_UP==1)return WKUP_PRES; // (3)
	}	     
	return 0;
}
