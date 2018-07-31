/******************************************************************************
*
* Dosya ismi:   main.h
* Amaç:         main.c dosyasinda kullanilan parametrelerin tanmlanmasi
*
* Yazan:Selçuk ÖZBAYRAKTAR                    Tarih: MAYIS 2017 
******************************************************************************

***************  48 ve 64 pin konfigürasyonlar için Port atamalari ******************

VCC   (VCC J2-20)                       
GND   (GND J2-19)                      
PA0		(11 J1-08) USART2 CTS/ADC12_IN0/TIM2_CH1_ETR (3.3V)         KEYPANEL C1 (output)  Touch panel için TPY_M ve TFT_P_RS
PA1   (10 J1-09) USART2 RTS/ADC12_IN1/TIM2_CH2		 (3.3V)					KEYPANEL C2 (output)  Touch Panel için TPX_M ve TFT_P_CS
PA2		(09 J1-10) USART2	TX/ADC12_IN2/TIM2_CH3      (3.3V)					KEYPANEL C3 (output)
PA3		(08 J1-11) USART2	RX/ADC12_IN3/TIM2_CH4      (3.3V)         KEYPANEL C4 (output)    

PA4		(07 J1-12) USART2 CK/ADC12_IN4/SPI1_NSS      (3.3V)                  
PA5		(06 J1-13) SPI1_SCK/ADC12_IN5                (3.3V)         			
PA6   (05 J1-14) SPI1_MISO/ADC12_IN6/TIM3_CH1      (3.3V)                  
PA7		(04 J1-15) SPI1_MOSI/ADC12_IN7/TIM3_CH2      (3.3V)         
PA8		(27 J2-05) USART1_CK/TIM1_CH1/MCO            (5V)           TFT_P_CS  (Touch Panel için TFT_P_CS yi PA1 e atiyoruz)
PA9		(26 J2-06) USART1 TXD/TIM1_CH2               (5V)          
PA10	(25 J2-07) USART1	RXD/TIM1_CH3               (5V)           
PA11	(24 J2-08) USART1 CTS/TIM1_CH4/CAN_RX/USB_DP (5V)           TFT_P_RS  (Touch Panel için TFT_P_RS yi PA0 a atiyoruz)
PA12	(23 J2-09) USART1 RTS/TM1_ETR/CAN_TX/USB_DM  (5V)           TFT_P_RD          
PA13	(22 J2-10) SW DIO/JTMS / REMAP  ile I/O      (5V)           SW Programlama
PA14	(21 J2-11) SW CLK/JTCK / REMAP ile I/O       (5V)           SW Programlama
PA15	(20 J2-12) JTDI        (5V)  (ST-LINK için,I/O için REMAP)  TFT_P_WR                  

PB0		(03 J1-16) ADC12_IN8/TIM3_CH3 (3.3V)                        TFT_P_RST/LED2            
PB1		(33 -----) ADC12_IN9/TIM3_CH4 (3.3V)                        KEYPANEL_R1 
PB2		(02 J1-17) Boot1							                              Boot1
PB3		(19 J2-13) JTDO/REMAP ile PB3/SPI1_SCK                      KEYPANEL_R2     
PB4		(18 J2-14) JNTRST/REMAP ile PB4/SPI1_MISO                   KEYPANEL R3
PB5		(17 J2-15) IO/I2C1_SMBA/REMAP ile SPI1_MOSI       (3.3V)    KEYPANEL R4
PB6		(16 J2-16) IO/I2C1_SCL/TIM4_CH1/REMAP USART1_TX     (5V)    
PB7   (15 J2-17) IO/I2C1_SDA/TIM4_CH2/REMAP USART1_RX     (5V)    
PB8		(14 J2-18) IO/TIM4_CH3                              (5V)    TFT_P_D0
PB9		(DISC ---) IO/TIM4_CH4                              (5V)    TFT_P_D1            
PB10	(01 J1-18) I2C2_SCL/USART3_TX                       (5V)    TFT_P_D2
PB11	(00 J1-19) I2C2_SDA/USART3_RX                       (5V)    TFT_P_D3                 
PB12	(31 J2-01) SPI2 NSS/I2C2_SMBA/USART3_CK/TIM1_BIN    (5V)    TFT_P_D4          
PB13	(30 J2-02) SPI2 SCK/USART3_CTS/TIM1_CH1N            (5V)    TFT_P_D5         
PB14	(29 J2-03) SPI2 MISO/USART3_RTS/TIM1_CH2N           (5V)    TFT_P_D6  
PB15	(28 J2-04) SPI2 MOSI/TIM1_CH3N                      (5V)    TFT_P_D7  

******  PC0--12  pinleri 48 pinli olan MAPLE da ve STM32 NANO minimum konfigürasyonda bulunmuyor
                 64 pinli STM32F103 lerde bulunyor.

PC0   (64 Pin) IO/ADC12_IN10(3.3V)                                 
PC1   (64 Pin) IO/ADC12_IN11(3.3V)                                 
PC2   (64 Pin) IO/ADC12_IN12(3.3V)                                 
PC3   (64 Pin) IO/ADC12_IN13(3.3V)                                 
PC4   (64 Pin) IO/ADC12_IN14(3.3V)                                 
PC5   (64 Pin) IO/ADC12_IN15(3.3V)                                 
PC6   (64 Pin) IO/TIM3_CH1  (5V)                                   
PC7   (64 Pin) IO/TIM3_CH2  (5V)
PC8   (64 Pin) IO/TIM3_CH3  (5V)                                   
PC9   (64 Pin) IO/TIM3_CH4  (5V)                                   
PC10  (64 Pin) IO/REMAP USART3_TX (5V)                             
PC11  (64 Pin) IO/REMAP USART3_RX (5V)                             
PC12  (64 Pin) IO/REMAP USART3_CK (5V)       
***********************************
PC13  (14 J1-04) IO/TAMPER RTC                             
PC14	(13 J1-05) IO/OSC32 IN
PC15	(12 J1-06) IO/OSC32 OUT

//=======================================2.4" 8b paralel girisli TFT Data girisleri ==========================================//
//8 Paralel giris:
//TFT_P_D0 TPY+   PB8      (Touch panel ortak pini)
//TFT_P_D1 TPX+   PB9      (Touch panel ortak pini)
//TFT_P_D2        PB10
//TFT_P_D3        PB11
//TFT_P_D4        PB12
//TFT_P_D5        PB13
//TFT_P_D6        PB14
//TFT_P_D7        PB15

//========= ILI9341 2.4" 8bit paralel girisli TFT ekranin TP ve kontrol pinleri ==============//
//TFT_P_RST	        PB0 (CNC de böyle.Bazi projelerde bu pin 3V3 e baglandi.)
//TFT_P_D0  TPY+    PB8  (Her zaman digital output, TP_Y ölçümü sirasinda 1 (Vcc) uygulaniyor)
//TFT_P_D1  TPX+    PB9  (Her zaman digital output, TP_X ölçümü sirasinda 1 (Vcc) uygulaniyor)
//   *** Touch panel kullanimi halinde asagidaki iki pin ADC girisi olan port pinlerine baglanacak sekilde
//       degisiklik yapilmalidir ''' ***
//TFT_P_CS	TPX_M   PA08 (PA1 ile TP_Y ölçümü sirasinda ADC1 girisi olarak kullaniliyor, TPX ölçümü sirasinda "0" uygulaniyor)
//TFT_P_RS	TPY_M   PA11 (PA0 ile TP_X ölçümü sirasinda ADC2 girisi olarak kullaniliyor, TPY ölçümü sirasinda "0" uygulaniyor)
//TFT_P_RD          PA12
//TFT_P_WR          PA15
*******************************************************************************/

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "systick.h"
#include "stdlib.h"

#include "stdbool.h"

#include "displays/ILI9341_P/hardware/tft_p/tft_p.h"
#include "displays/ILI9341_P/hardware/tft_p/buttonBmps.h"
#include "TOUCH_PANEL/TouchPanel.h"

#define DEBUG

#define messageWindow tft.height-66
#define statusLine    tft.height-65

	int value;
	int generalScaleFactor=1;
	int TFT_x0,TFT_y0,TFT_x,TFT_y;	



/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/

#define LED2_PORT	GPIOB
#define LED2_PIN	GPIO_Pin_1
#define LED2 GPIOB,GPIO_Pin_1

#define DENEME_Pin GPIO_Pin_15 
#define DENEME_Port GPIOA


#define ECLK                8000000 //this is BUSCLK 

   
/* Private functions ---------------------------------------------------------*/

void itoa(int i, unsigned char b[], uint8_t c);

/* Global degiskenler  */

uint16_t** pointerToPointer;



//uint16_t BACK_COLOR, POINT_COLOR ; 

struct tBox{ 
  int x0; 
  int y0;
  uint8_t width;
  uint8_t height;	
  uint16_t labelIndex;
  uint8_t pressed;
  uint8_t gap;
  uint8_t errMargin;	
};
	
struct tBox button;
struct tBox* pButton=&button;

struct coord{
int x;
int y;
int z;	
};



/* Private variables ---------------------------------------------------------*/

unsigned char buffer[4];

  long i;

struct pinTypeDef
{
	GPIO_TypeDef* GPIOx;
	uint16_t PinNo;
};

/* Fonksiyon Prototipleri-----------------------------------------------------*/

void RCC_Configuration(void);
void GPIO_Configuration(void);

void testKeypad(void);

void Delay_100us(uint16_t time);
void Delay_1ms(uint16_t time);

void checkKeyPad(void);
void jogPad(void);
uint8_t evetHayir(void);

extern void keyPad_setup(void); 
extern char readKeyPad(void);

#define textOrientation TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_2)  // landscape_1 (0,0) sag alt kösede
                                                        // landscape_2 (0,0) sol üst kösede, yazilar OK
                                                        // landscape_3 (0,0) sol alt kösede, yazilar ters
#define graphicOrientation TM_ILI9341_Rotate(TM_ILI9341_Orientation_Landscape_3)																												

void TFT_displayFloat(float Number, uint16_t displayLine, uint16_t cursorPos);

void itoa(int i, unsigned char b[], uint8_t c);
void drawButtonAndLabel(struct tBox* pButton);
void drawButtonFrame(struct tBox* dugme);
void drawButtons(short nrOfColumns, short nrOfRows, short lblOffset);
void eraseButtons(short nrOfColumns, short nrOfRows);
void koordinat(void);
void options(void);
void testTftDisplay(void);
void biseyYap(unsigned char* arg1);
void testBitmap(void);

/* Global Degiskenler--- -----------------------------------------------------*/


