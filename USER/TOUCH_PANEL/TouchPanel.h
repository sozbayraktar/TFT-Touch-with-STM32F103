#ifndef __TOUCH_H__
#define __TOUCH_H__
//#include "../system/sys/sys.h"
#include "inttypes.h"

//////////////////////////////////////////////////////////////////////////////////	 
//STM32F103RBT6,72M 3.3V
//QDtech-TFT for STM32 IO
//////////////////////////////////////////////////////////////////////////////////


//=========================================..............=========================================//
/***************************************************************************************************
//MO(MISO)	PC2	  //SPI
//MI(MOSI)	PC3	  //SPI
//PEN		    PC1	  //
//TCS		    PC13	// .. IC
//CLK		    PC0	  //SPI
**************************************************************************************************/	

#define TP_PRES_DOWN 0x80
#define TP_CATH_PRES 0x40 	  
										    
typedef struct
{
	uint8_t (*init)(void);	
	uint8_t (*scan)(uint8_t);	
	void (*adjust)(void);	
	uint16_t x0;	
	uint16_t y0;
	uint16_t x; 	
	uint16_t y;						   	    
	uint8_t  sta;	
								//b7: ...1 / ...0; 
                //b6:0 ...; 1 ...        			  
////////////////////////............../////////////////////////								
	float xfac;					
	float yfac;
	short xoff;
	short yoff;	   

//touchtype=0 .....
//touchtype=1 .....
	uint8_t touchtype;
}_m_tp_dev;


extern _m_tp_dev tp_dev;

#define TPX_Pin       GPIO_Pin_9  //(Her zaman digital output, TP_X ölçümü sirasinda 1 (Vcc) uygulaniyor)
#define TPX_Port      GPIOB
#define TPX           GPIOB,GPIO_Pin_9

#define TPY_Pin       GPIO_Pin_8  //(Her zaman digital output, TP_Y ölçümü sirasinda 1 (Vcc) uygulaniyor)
#define TPY_Port      GPIOB
#define TPY           GPIOB,GPIO_Pin_8

#define TFT_P_CS_Pin 	GPIO_Pin_1  // ASAGIDAKI TPX_M ile aynidir 
#define TFT_P_CS_Port GPIOA
#define TFT_P_CS      GPIOA,GPIO_Pin_1

#define TPX_M_Pin      GPIO_Pin_1   // (TP_Y ölçümü sirasinda ADC1 girisi olarak kullaniliyor, TPX ölçümü sirasinda "0" uygulaniyor)
#define TPX_M_Port     GPIOA
#define TPX_M          GPIOA,GPIO_Pin_1

#define TFT_P_RS_Pin   GPIO_Pin_0  //(ASAGIDAK0 TPY_M ile aynidir
#define TFT_P_RS_Port  GPIOA
#define TFT_P_RS       GPIOA,GPIO_Pin_0

#define TPY_M_Pin      GPIO_Pin_0  //(TP_X ölçümü sirasinda ADC2 girisi olarak kullaniliyor, TPY ölçümü sirasinda "0" uygulaniyor)
#define TPY_M_Port     GPIOA
#define TPY_M          GPIOA,GPIO_Pin_0

#define TPY_WR_Pin     GPIO_Pin_1
#define TPY_WR_Port    GPIOB
#define TPY_WR         GPIOB,GPIO_Pin_1

#define TPY_P_RD_Pin   GPIO_Pin_4
#define TPY_P_RD_Port  GPIOA
#define TPY_P_RD       GPIOA,GPIO_Pin_4



//#define PEN  PCin(1)   //PC1  INT

//#define DOUT PCin(2)   //PC2  MISO
//#define TDIN PCout(3)  //PC3  MOSI
//#define TCLK PCout(0)  //PC0  SCLK
//#define TCS  PCout(13) //PC13 CS    
     
void TP_AN_InputRearrange(unsigned char adcNo, unsigned char InputStatus);


void TP_Write_Byte(uint8_t num);		
uint16_t TP_Read_AD(uint8_t CMD);			
uint16_t TP_Read_XOY(uint8_t xy);		
uint8_t TP_Read_XY(uint16_t *x,uint16_t *y);		
uint8_t TP_Read_XY2(uint16_t *x,uint16_t *y);
uint8_t TP_Read_XY_pixels(uint16_t *x,uint16_t *y);
void TP_Draw_Touch_Point(uint16_t x,uint16_t y,uint16_t color);
void TP_Draw_Big_Point(uint16_t x,uint16_t y,uint16_t color);	
uint8_t TP_Scan(uint8_t tp);						
void TP_Save_Adjdata(void);
uint8_t TP_Get_Adjdata(void);		
void TP_Adjust(void);					
uint8_t TP_Init(void);			
																 
void TP_Adj_Info_Show(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t x3,uint16_t y3,uint16_t fac);//ÏÔÊ¾Ð£×¼ÐÅÏ¢
int16_t absolute(int16_t arg);

void TP_AN_InputRearrange(unsigned char adcNo, unsigned char InputStatus);
uint8_t checkPen(void);
uint16_t adcOku(unsigned char adcNo);
void reportStylus(uint16_t *x,uint16_t *y);


extern void Delay_1ms(uint16_t time);
extern void Delay_100us(uint16_t time);
extern void itoa(int i, unsigned char b[], uint8_t c);

#endif

















