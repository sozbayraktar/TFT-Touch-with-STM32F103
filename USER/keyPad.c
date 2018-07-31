#include "inttypes.h"
#include "stm32f10x_gpio.h"
#include "systick.h"

struct pinTypeDef
{
	GPIO_TypeDef* GPIOx;
	uint16_t PinNo;
};

struct pinTypeDef PINx;
struct pinTypeDef* pPINx=&PINx;


//  *****  Keypanel Matrix Definitions

#define kpC1Port	GPIOA
#define kpC1Pin	  GPIO_Pin_0
#define kpC1      kpC1Port,kpC1Pin

#define kpC2Port	GPIOA
#define kpC2Pin	  GPIO_Pin_1
#define kpC2      kpC2Port,kpC2Pin

#define kpC3Port	GPIOA
#define kpC3Pin	  GPIO_Pin_2
#define kpC3      kpC3Port,kpC3Pin

#define kpC4Port	GPIOA
#define kpC4Pin	  GPIO_Pin_3
#define kpC4      kpC4Port,kpC4Pin

#define kpR1Port	GPIOB
#define kpR1Pin	  GPIO_Pin_1
#define kpR1      kpR1Port,kpR1Pin

#define kpR2Port	GPIOB
#define kpR2Pin	  GPIO_Pin_3
#define kpR2      kpR2Port,kpR2Pin

#define kpR3Port	GPIOB
#define kpR3Pin	  GPIO_Pin_4
#define kpR3      kpR3Port,kpR3Pin

#define kpR4Port	GPIOB
#define kpR4Pin	  GPIO_Pin_5
#define kpR4      kpR4Port,kpR4Pin

		
		
struct pinTypeDef keyPadColumnPin[4];
//struct keyPadPinStruct* pKeyPadColumnPin;		

struct pinTypeDef keyPadRowPin[4];
//struct keyPadPinStruct* pKeyPadRowPin;		

// ************ END OF KEYPANEL MATRIX DEFINITIONS ************


void keyPad_setup(void); 
char readKeyPad(void);

void GPIO_KeyPad_Configuration(void); 
char debounceInputPin(struct pinTypeDef* pin, char pinState);
void invertOutputPin(struct pinTypeDef* pin);

extern void Delay_100us(uint16_t time);
extern void Delay_1ms(uint16_t time);

void keyPad_setup(void) {
GPIO_KeyPad_Configuration();
	
		keyPadColumnPin[0].GPIOx=kpC1Port;
		keyPadColumnPin[0].PinNo=kpC1Pin;

		keyPadColumnPin[1].GPIOx=kpC2Port;
		keyPadColumnPin[1].PinNo=kpC2Pin;
		
		keyPadColumnPin[2].GPIOx=kpC3Port;
		keyPadColumnPin[2].PinNo=kpC3Pin;
		
		keyPadColumnPin[3].GPIOx=kpC4Port;
		keyPadColumnPin[3].PinNo=kpC4Pin;
		
		
		keyPadRowPin[0].GPIOx=kpR1Port;
		keyPadRowPin[0].PinNo=kpR1Pin;

		keyPadRowPin[1].GPIOx=kpR2Port;
		keyPadRowPin[1].PinNo=kpR2Pin;
		
		keyPadRowPin[2].GPIOx=kpR3Port;
		keyPadRowPin[2].PinNo=kpR3Pin;
		
		keyPadRowPin[3].GPIOx=kpR4Port;
		keyPadRowPin[3].PinNo=kpR4Pin;
}


/* ********************** Read Key Pad *******************************************
// hiçbir tusa basilmadiysa "0", basildi ise butonun nosu 11..44 arasinda döner
* ********************** readKeyPad *******************************************/
char readKeyPad(void) 
	{
      				
  uint8_t column, row, i;
  char keyPressed=0x00;

		for(i=0;i<4;i++){ // Önce tüm kolonlara "0" uygulayarak herhangi bir tusa basilip basilmadigina bakacaz
			GPIO_ResetBits(keyPadColumnPin[i].GPIOx,keyPadColumnPin[i].PinNo); 		
		}

//		satirlari tarayalim
row=0;
		for(i=0;i<4;i++){
		if (debounceInputPin(&keyPadRowPin[i],Bit_RESET)==1) {
                                 			row=i+1;
                                			break;
		                                  }
}
		
column=0;		
		if(row!=0){ // Bir tusa basilmis ise, hangi kolonda olduguna bakalim
		for(i=0;i<4;i++){ 
			GPIO_SetBits(keyPadColumnPin[i].GPIOx,keyPadColumnPin[i].PinNo); // Kolonlari sira ile "1" yapalim
      if(debounceInputPin(&keyPadRowPin[row-1],Bit_SET)==1) {
																				column=i+1;
																				break;			
																				}	
}
																																														
}
if(column!=0) {GPIO_ResetBits(keyPadColumnPin[column-1].GPIOx,keyPadColumnPin[column-1].PinNo);//Kolonu tekrar 0 yap
	while(debounceInputPin(&keyPadRowPin[row-1],Bit_SET)==0);  //basilan tus serbest birakilana kadar bekle
              } 

keyPressed=column*10;
keyPressed+=row;
//if(keyPressed!=0) printf("KeyPressed : %d \n", keyPressed);
return keyPressed;
}
	

/*******************   INVERT AN OUTPUT PIN ********************************/
void invertOutputPin(struct pinTypeDef* pin)
{
pin->GPIOx->ODR^=pin->PinNo;	
}

/*******************   CHECK STATUS OF AN INPUT PIN ********************************/

char debounceInputPin(struct pinTypeDef* pin, char pinState)
{char result=0;
if(GPIO_ReadInputDataBit(pin->GPIOx,pin->PinNo)==pinState){
                Delay_1ms(1);
	              if(GPIO_ReadInputDataBit(pin->GPIOx,pin->PinNo)==pinState) result=1;
	              else {   // okumalar uyusmadi 100us sonra bir kere daha bakalim
									Delay_1ms(1);
									if(GPIO_ReadInputDataBit(pin->GPIOx,pin->PinNo)==pinState) result=1; 
									else	result=0;		// Gene olmadi yanlis alarm	
								     }										
}
return result;
}


/*******************Giris - Cikis Pinleri Konfigurasyonlari*********************/
void GPIO_KeyPad_Configuration(void)
{
	
  GPIO_InitTypeDef GPIO_InitStructure;
	
/***** Setting KEYPANEL COLUMNS (Scanning Outputs) and ROWS (Scanning Inputs) *******/


  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //Output ve PushPull

  GPIO_InitStructure.GPIO_Pin =  kpC1Pin;
  GPIO_Init(kpC1Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  kpC2Pin;
  GPIO_Init(kpC2Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  kpC3Pin;
  GPIO_Init(kpC3Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  kpC4Pin;
  GPIO_Init(kpC4Port, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //Input ve PullUp

  GPIO_InitStructure.GPIO_Pin =  kpR1Pin;
  GPIO_Init(kpR1Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  kpR2Pin;
  GPIO_Init(kpR2Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  kpR3Pin;
  GPIO_Init(kpR3Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  kpR4Pin;
  GPIO_Init(kpR4Port, &GPIO_InitStructure);

/************** End of setting KEYPANEL COLUMNS and ROWS ****************************/

}

