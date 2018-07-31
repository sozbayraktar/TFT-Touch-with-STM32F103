#ifndef __TFT_H
#define __TFT_H		
#include "displays/ILI9341_P/system/sys/sys.h"	 
#include "inttypes.h"
#include "stm32f10x_conf.h"

/***********************************

//TFT_CS	PA08
//TFT_RS	PA11
//TFT_WR	PA15
//TFT_RD	PA12
//TFT_RST	PB0
********************************************************************************/	
typedef struct  
{										    
	uint16_t width;			
	uint16_t height;		
	uint16_t id;				//LCD ID
	uint8_t  dir;		
	uint16_t wramcmd;
	uint16_t setxcmd;
	uint16_t setycmd;
}tft_device_typedef; 	

extern tft_device_typedef tft;	

#define USE_HORIZONTAL  	1
#define TFT_USE8BIT_MODEL   1
#define TFT_INVERTED

//////////////////////////////////////////////////////////////////////////////////	  
#if USE_HORIZONTAL==1
#define TFT_W 320
#define TFT_H 240
#else
#define TFT_W 240
#define TFT_H 320
#endif
  
extern uint16_t  POINT_COLOR;    
extern uint16_t  BACK_COLOR;

////////////////////////////////////////////////////////////////////
//----------------- TFT CONTROL PINS -------------------------------
#define TFT_CS_PORT	GPIOA
#define TFT_CS_PIN	GPIO_Pin_1  // TP versiyonunda GPIO_Pin_1, Keypanel versiyonunda GPIO_Pin_8
#define TFT_CS GPIOA,GPIO_Pin_1 // TP versiyonunda GPIO_Pin_1, Keypanel versiyonunda GPIO_Pin_8

#define TFT_RS_PORT	GPIOA
#define TFT_RS_PIN	GPIO_Pin_0  //TP versiyonunda GPIO_Pin_0, Keypanel versiyonunda GPIPin_11
#define TFT_RS GPIOA,GPIO_Pin_0 //TP versiyonunda GPIO_Pin_0, Keypanel versiyonunda GPIPin_11

#define TFT_WR_PORT	GPIOA
#define TFT_WR_PIN	GPIO_Pin_15
#define TFT_WR GPIOA,GPIO_Pin_15

#define TFT_RD_PORT	GPIOA
#define TFT_RD_PIN	GPIO_Pin_12
#define TFT_RD GPIOA,GPIO_Pin_12

#define TFT_RST_PORT	GPIOB
#define TFT_RST_PIN	GPIO_Pin_0
#define TFT_RST GPIOB,GPIO_Pin_0

#define TFT_DATA_PINS GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15

//----------------- TFT KONTROL KOMUTLARI            ---------------- 

#define	TFT_CS_SET  GPIOA->BSRR=1<<1    //TP versiyonu için 1<<1, Keypanel için 1<<8
#define	TFT_RS_SET	GPIOA->BSRR=1   //TP versiyonu için =1 , keypanel için 1<<11
#define	TFT_WR_SET	GPIOA->BSRR=1<<15
#define	TFT_RD_SET	GPIOA->BSRR=1<<12
#define	TFT_RST_SET	GPIOB->BSRR=1
								    
#define	TFT_CS_CLR  GPIOA->BRR=1<<1    //TP versiyonu için 1<<1, Keypanel için 1<<8
#define	TFT_RS_CLR	GPIOA->BRR=1       //TP versiyonu için =1, Keypanel için 1<<11
#define	TFT_WR_CLR	GPIOA->BRR=1<<15
#define	TFT_RD_CLR	GPIOA->BRR=1<<12
#define	TFT_RST_CLR	GPIOB->BRR=1

//#define DATAOUT(x) GPIOB->ODR=x;  // 16 bit modunu kullansayd1k böyle olacakti
  #define DATAOUT(x) GPIOB->ODR=(x|(GPIOB->ODR&0x00FF));// Sadece üst 8 biti yani B8..B15 i kullaniyoruz
  #define DATAIN     GPIOB->IDR;   //
//////////////////////////////////////////////////////////////////////


#define L2R_U2D  0
#define L2R_D2U  1
#define R2L_U2D  2
#define R2L_D2U  3

#define U2D_L2R  4
#define U2D_R2L  5
#define D2U_L2R  6
#define D2U_R2L  7

#define DFT_SCAN_DIR  L2R_U2D

#define WHITE       0xFFFF
#define BLACK      	0x0000	  
#define BLUE       	0x001F  
#define BRED        0XF81F
#define GRED 			 	0XFFE0
#define GBLUE			 	0X07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define GREEN       0x07E0
#define CYAN        0x7FFF
#define YELLOW      0xFFE0
#define BROWN 			0XBC40
#define BRRED 			0XFC07


#define LIGHTGRAY       0XEF5B  // 11101 111010 11011  (normal)
#define LGRAY 			 		0XC618  // 11000 110000 11000  (darker)
#define LGRAY2 			 		0XC618  // 10100 101000 10100  (a bit more darker)
#define DARKGRAY  			0X8430  // 10000 100001 10000 (darkest)

#define DARKBLUE      	 0X01CF 
#define LIGHTBLUE      	 0X7D7C  
#define GRAYBLUE       	 0X5458
#define LIGHTGREEN     	0X841F


#define LGRAYBLUE      	0XA651
#define LBBLUE          0X2B12
	    															  
extern uint16_t BACK_COLOR, POINT_COLOR ;  

void TFT_P_Init(void);
void TFT_DisplayOn(void);
void TFT_DisplayOff(void);
void TFT_Clear(uint16_t Color);	 
void TFT_SetCursor(uint16_t Xpos, uint16_t Ypos);
void TFT_DrawPoint(uint16_t x,uint16_t y);
uint16_t  TFT_ReadPoint(uint16_t x,uint16_t y);
void TFT_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void TFT_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);		   
void TFT_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd);
void TFT_DrawPoint_16Bit(uint16_t color);
uint16_t TFT_RD_DATA(void);							    
void TFT_WriteReg(uint8_t TFT_Reg, uint16_t TFT_RegValue);
void TFT_WR_DATA(uint16_t data);
uint16_t TFT_ReadReg(uint8_t TFT_Reg);
void TFT_WriteRAM_Prepare(void);
void TFT_WriteRAM(uint16_t RGB_Code);
uint16_t TFT_ReadRAM(void);		   
uint16_t TFT_BGR2RGB(uint16_t c);
void TFT_SetParam(void);

void TFT_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color);
void TFT_ShowChar(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc, uint8_t num,uint8_t size,uint8_t mode);
void TFT_ShowString(uint16_t x,uint16_t y,uint8_t size,uint8_t *p,uint8_t mode);
void TFT_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size);
void TFT_Drawbmp16(uint16_t x,uint16_t y,const unsigned char *p);
void TFT_ShowBmp(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc, const unsigned char *pBMP,uint8_t width,uint8_t height,short zoom, uint8_t mode);
void TFT_circle(int xc, int yc,uint16_t c,int r, int fill);
void TFT_DrawFillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
extern void itoa(int i, unsigned char b[], uint8_t c);


extern void Delay_1ms(uint16_t time);

//9320/9325 LCD

#define R0             0x00
#define R1             0x01
#define R2             0x02
#define R3             0x03
#define R4             0x04
#define R5             0x05
#define R6             0x06
#define R7             0x07
#define R8             0x08
#define R9             0x09
#define R10            0x0A
#define R12            0x0C
#define R13            0x0D
#define R14            0x0E
#define R15            0x0F
#define R16            0x10
#define R17            0x11
#define R18            0x12
#define R19            0x13
#define R20            0x14
#define R21            0x15
#define R22            0x16
#define R23            0x17
#define R24            0x18
#define R25            0x19
#define R26            0x1A
#define R27            0x1B
#define R28            0x1C
#define R29            0x1D
#define R30            0x1E
#define R31            0x1F
#define R32            0x20
#define R33            0x21
#define R34            0x22
#define R36            0x24
#define R37            0x25
#define R40            0x28
#define R41            0x29
#define R43            0x2B
#define R45            0x2D
#define R48            0x30
#define R49            0x31
#define R50            0x32
#define R51            0x33
#define R52            0x34
#define R53            0x35
#define R54            0x36
#define R55            0x37
#define R56            0x38
#define R57            0x39
#define R59            0x3B
#define R60            0x3C
#define R61            0x3D
#define R62            0x3E
#define R63            0x3F
#define R64            0x40
#define R65            0x41
#define R66            0x42
#define R67            0x43
#define R68            0x44
#define R69            0x45
#define R70            0x46
#define R71            0x47
#define R72            0x48
#define R73            0x49
#define R74            0x4A
#define R75            0x4B
#define R76            0x4C
#define R77            0x4D
#define R78            0x4E
#define R79            0x4F
#define R80            0x50
#define R81            0x51
#define R82            0x52
#define R83            0x53
#define R96            0x60
#define R97            0x61
#define R106           0x6A
#define R118           0x76
#define R128           0x80
#define R129           0x81
#define R130           0x82
#define R131           0x83
#define R132           0x84
#define R133           0x85
#define R134           0x86
#define R135           0x87
#define R136           0x88
#define R137           0x89
#define R139           0x8B
#define R140           0x8C
#define R141           0x8D
#define R143           0x8F
#define R144           0x90
#define R145           0x91
#define R146           0x92
#define R147           0x93
#define R148           0x94
#define R149           0x95
#define R150           0x96
#define R151           0x97
#define R152           0x98
#define R153           0x99
#define R154           0x9A
#define R157           0x9D
#define R192           0xC0
#define R193           0xC1
#define R229           0xE5							  		 
#endif  

	 
	 



