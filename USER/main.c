/********************************************************************************
** Developeded by: Selcuk OZBAYRAKTAR             
** Date: 16.04.2017           
** Version: V 6.0 
** bu satir degistirildi 4 Aug 2020
** bu da yeni bransh uzerinde iken eklendi
**				 
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "main.h"
extern const unsigned char* pBmpFile;
/*************************Ana Program*************************/
int main(void)
{
	RCC_Configuration();
	GPIO_Configuration();//Giris Cikislari Ayarla		
	
TFT_P_Init();
TP_Init();	
	
BACK_COLOR=BLUE;
POINT_COLOR=YELLOW;	

Delay_1ms(1000);
TFT_Clear(BLUE);


testBitmap();
while(1);

//keyPad_setup(); // Touch Panel versiyonunda Keypanel yok

Delay_1ms(1000);

/*   TESTING THE KEYPAD   */

//testKeypad(); // Fonksiyon içinde sonsuz döngü var

testTftDisplay();

} //  *******************   END OF MAIN ********************************

/**********               testBitmap **************************************************
*           Basit ölçeklenebilir monokrom bitMap grafik görüntüleme                   *
* Ekrana her satirda daha büyük semboller kullanarak 5 satir halinde,                 *
* 9 rakami dan baslayip 11 adet hareket (Blok oklar, Exit, Check vb) sembolü çizer.   *
* Ekrana sigip sigmadigini kontrol etmez, dolayisi ile 320x480 den küçük ekranlarda   *
* tasma ve bozulmalar olabilir                                                        *
***************************************************************************************/
void testBitmap(void)
{
unsigned char size;
BACK_COLOR=BLUE;
POINT_COLOR=YELLOW;

for(size=1;size<5;size++){
for(i=0;i<176;i=i+16){
TFT_ShowBmp(i/2*(size+1),(size-1)*64,YELLOW, RED, pBmpFile+NINE+i,8,16,size,0);
}
}
// Bir de minik çember çizelim
TFT_circle(100, 100,YELLOW,2,1);
}


/* *************      draw buttons   **********************************
*        By Selçuk ÖZBAYRAKTAR      Aug 2017
*        
***********************************************************************/
void drawButtons(short nrOfColumns, short nrOfRows, short lblOffset){
char col=0;
short buttonQTY;
	int ilkX0;
	
button.width=tft.width/nrOfColumns-1;
if(button.width*nrOfRows>tft.height){button.width=tft.height/nrOfRows-1;} //Yükseklige göre ayar gerekebilir
button.height=button.width;           //tuslar kare olsun
button.x0=tft.width-(button.width+1)*nrOfColumns;  //saga yanastiriyoruz
button.y0=0;
button.gap=2;
button.labelIndex=lblOffset;
button.errMargin=0;

ilkX0=button.x0;
	
buttonQTY=nrOfColumns*nrOfRows;

button.pressed=0;
	

for(i=0;i<buttonQTY;i++){   // Key Panel'i çizelim
	button.labelIndex=*(pLabelList+lblOffset+i);
	drawButtonAndLabel(pButton);
	if(col<nrOfColumns-1 ) {    // yandaki butona geç
           button.x0+=(button.width+button.gap);
           col++;} else {     // alt siraya geç,satir basi yap
						             button.x0=ilkX0;
                         button.y0+=(button.height+button.gap);
                         col=0;}
}	

}

/* *************      draw buttons   **********************************
*        By Selçuk ÖZBAYRAKTAR      Aug 2017
*        
***********************************************************************/
void clickButton(short nrOfColumns, short nrOfRows, short lblOffset, short buttonColRow){
	
short buttonNo,buttonColumn,buttonRow;

button.width=tft.width/nrOfColumns-1;
if(button.width*nrOfRows>tft.height){button.width=tft.height/nrOfRows-1;} //Yükseklige göre ayar gerekebilir
button.height=button.width;           //tuslar kare olsun
button.x0=tft.width-(button.width+1)*nrOfColumns;  //saga yanastiriyoruz
button.y0=0;
button.gap=2;
button.labelIndex=lblOffset;
button.errMargin=0;

buttonColumn=buttonColRow/10;
buttonRow=buttonColRow-buttonColumn*10;
button.x0+=button.width*(buttonColumn-1);
button.y0+=button.height*(buttonRow-1);
	
buttonNo=(buttonRow-1)*nrOfColumns+buttonColumn-1;

button.pressed=1;
	
	button.labelIndex=*(pLabelList+lblOffset+buttonNo);
	drawButtonAndLabel(pButton);

Delay_1ms(250);

}


/* *************      erase buttons   *********************************
*        By Selçuk ÖZBAYRAKTAR      Jan 2018
* "Draw buttons" fonksiyonu ile senkron ölçülerde dikdörtgen alani temizler
* Draw Buttons degistirilirse bunun da ona göre degismesi gerekir
***********************************************************************/
void eraseButtons(short nrOfColumns, short nrOfRows){

int x0,y0,x1,y1;
short xButtonSize,yButtonSize;	

xButtonSize=tft.width/nrOfColumns-1;
if(xButtonSize*nrOfRows>tft.height){xButtonSize=tft.height/nrOfRows-1;} //Yükseklige göre ayarla
yButtonSize=xButtonSize;           //tuslar kare
	
x0=tft.width-(xButtonSize+1)*nrOfColumns;  //saga yanastiriyoruz
y0=0;

x1=x0+nrOfColumns*(xButtonSize+2);
y1=y0+nrOfRows*(yButtonSize+2);
	
TFT_Fill(x0,y0,x1,y1,BLUE);	
}

/* *************      drawButtonAndLabel   ****************************
*        By Selçuk ÖZBAYRAKTAR      Jan 2018
*        
*   ekran üzerinde bir Keyboard butonu çizer, içine bmp etiketi yerlestirir
***********************************************************************/
void drawButtonAndLabel(struct tBox* buttonPtr){
short scale,leadSpacing;
uint16_t	yPos;
const unsigned char* pLabel;
uint8_t	bmpWidth=8;
uint8_t	bmpHeight=16;
	
drawButtonFrame(buttonPtr);
	
POINT_COLOR=YELLOW;	
scale=buttonPtr->height/16;
	
// ** etiketi butonun büyüklügüne uygun olarak yerlestirelim
yPos=(buttonPtr->height-scale*bmpHeight)/2;	
leadSpacing=(buttonPtr->width-scale*bmpWidth)/2;	
	
pLabel=pBmpFile+buttonPtr->labelIndex;   // tanim : const unsigned char *pBMP
TFT_ShowBmp(buttonPtr->x0+leadSpacing,buttonPtr->y0+yPos,POINT_COLOR, BACK_COLOR, pLabel,bmpWidth,bmpHeight,scale,0);

}
/* *************      drawButtonFrame  ********************************
*        By Selçuk ÖZBAYRAKTAR      Jan 2018
*        
* 3D buton Çerçevesi çizer
***********************************************************************/
void drawButtonFrame(struct tBox* dugme){
short i;
	uint16_t x0,y0,x1,y1;
	x0=dugme->x0;
	y0=dugme->y0;
	x1=dugme->x0+dugme->width;
	y1=dugme->y0+dugme->height;
	
BACK_COLOR=LGRAY;
POINT_COLOR=DARKGRAY;
TFT_Fill(x0 ,y0,x1,y1,BACK_COLOR);

i=0;	
	
while(i<5){
if(dugme->pressed==0)POINT_COLOR=LIGHTGRAY; else POINT_COLOR=BLACK;
TFT_DrawLine(x0+i,y0+i,x0+i,y1-i); //Left edge
if(dugme->pressed==0)POINT_COLOR=LGRAY2;	else POINT_COLOR=DARKGRAY;
TFT_DrawLine(x1-i,y0+i,x0+i,y0+i);//Top edge

if(dugme->pressed==0) POINT_COLOR=BLACK; else POINT_COLOR=LIGHTGRAY;
TFT_DrawLine(x1-i,y1-i,x1-i,y0+i); //Right edge
if(dugme->pressed==0) POINT_COLOR=DARKGRAY; else POINT_COLOR=LGRAY2;	
TFT_DrawLine(x0+i,y1-i,x1-i,y1-i);//Bottom edge
i++;}
}

/* *************      drawMenuBar  ***************************************
*        By Selçuk ÖZBAYRAKTAR      Aug 2017
*        
* TFT ekran üzerinde ana menu satirini çizer, buton etiketlerini yazar
***********************************************************************/
void drawMenuBar(void){
uint8_t temp, labelOffset=12;

while(1){

	
drawButtons(4,2,labelOffset);

temp=0;
		
while(temp==0)temp=readKeyPad();

if((temp-(temp/10)*10)<=2) {clickButton(4,2,labelOffset,temp);
                            drawButtons(4,2,labelOffset);}

//printf("Temp Keypad = %d \n",temp);
if(temp==42) {// EXIT Basilmis
	    eraseButtons(4,2);	
			return;}
		
	switch(temp)
{
	case 11: {
		        break;}
	case 21: {
        		eraseButtons(4,2);
		        jogPad();  //Manuel hareket
		        break;}
	case 31: {
		       eraseButtons(4,2);
		       koordinat();  // koordinat girisi ile hareket
		       break;
	         }
	case 32: {
		         TFT_Clear(BLUE);  // CLEAR SCREEN
			     drawButtons(4,2,12);
		       break;
	         }
  case 41: {
		       eraseButtons(4,2);
		       options();    // seçenekler
		       break;
	         }		 
	default : break;
}	// END Switch

}	// END While(1)
}

/* *************      EVET_HAYIR KUTUSU  ***************************************
*        By Selçuk ÖZBAYRAKTAR      Aug 2017
*        
*  ekran üzerinde Evet Hayir diyalog menüsünü çizer
***********************************************************************/
uint8_t evetHayir(void)
	{
uint8_t temp,labelOffset=19;
while(1){	

drawButtons(1,1,labelOffset);		
temp=0;
while(temp==0)temp=readKeyPad();
	
	switch(temp)
{
	case 11: {return 1;}
	default : break;
}	
}
	
}



/* *************      drawJogPad  ***************************************
*        By Selçuk ÖZBAYRAKTAR      Aug 2017
*        
*      ekran üzerinde 4x4 jogPad menüsü çizer
***********************************************************************/
void jogPad(void){
uint8_t temp,labelOffset=36;
	
drawButtons(4,4,labelOffset);

while(1){	
temp=0;
while(temp==0) temp=readKeyPad();

clickButton(4,4,labelOffset,temp);	
drawButtons(4,4,labelOffset);
	
switch(temp)
{
	case 14: {//EXIT BASILMIS
		        TFT_Clear(BLUE);
		        return;}
	case 11: {
biseyYap("JogPad 11");
            break;		
		        }

	case 12: {

biseyYap("JogPad 12");		
					  break;			
					  }
	
	case 13: {
biseyYap("JogPad 13");
			      break;			
			      }
	case 21: {
biseyYap("JogPad 21");
            break;		
		        }
	case 22: {	
biseyYap("JogPad 22");		
					  break;			
					  }
	case 23: { 
biseyYap("JogPad 23");
			      break;			
			      }
	case 31: {
biseyYap("JogPad 31");	
            break;		
		        }

		case 32: {
biseyYap("JogPad 32");	
					  break;			
					  }
						
	case 33: {
biseyYap("JogPad 33");
			      break;			
			      }
	default : break;
}	
}
}


/* *************      options  ***************************************
*        By Selçuk ÖZBAYRAKTAR      sept 2017
*        
*   Ekran üzerinde 4x4 matrix keyboard çizer
***********************************************************************/
void options(void){
uint8_t temp,labelOffset=68;

drawButtons(4,4,labelOffset);

	while(1){	
temp=0;
while(temp==0)temp=readKeyPad();
clickButton(4,4,labelOffset,temp);
drawButtons(4,4,labelOffset);
		
switch(temp)
{
	case 14: {//EXIT BASILMIS
		        TFT_Clear(BLUE);
		        return; }
	case 11: {   
biseyYap("Options 11");
	           break;	
		        }
	case 12: { 
biseyYap("Options 12");		
	           break;			
					  }
	case 13: {
biseyYap("Options 13");
	           break;			
			      }
	case 31: {   
biseyYap("Options 31");
	           break;		
		        }
	case 33: { 
biseyYap("Options 33");
	           break;			
					  }
  case 41: {
biseyYap("Options 41");		
	           break;
	}
	case 42: {
biseyYap("Options 42");
	}		
             break;

	case 44: { 
biseyYap("Options 44");		
            return;			
					  }
	default : break;
}	
}
}

/* *************     debug fonksiyonu biseyYap()  ********************
*        By Selçuk ÖZBAYRAKTAR      Aug 2017
*        
*   Argüman olarak verilen texti ekrana yazar 
***********************************************************************/
void biseyYap(unsigned char* txt1){ 
	TFT_ShowString(0,18,16,"ISLEM :",0);
	TFT_ShowString(60,18,16,txt1,0);
	return;
}

/* *************      koordinat  ***************************************
*        By Selçuk ÖZBAYRAKTAR      Aug 2017
*        
*   Ekran üzerinde matrix nümerik giris keyboard çizer
***********************************************************************/
void koordinat(void){
	uint8_t temp,labelOffset=52;

drawButtons(4,4,labelOffset);

	while(1){	
temp=0;
while(temp==0)temp=readKeyPad();	
clickButton(4,4,labelOffset,temp);
drawButtons(4,4,labelOffset);

switch(temp)
{
	case 14: {//EXIT BASILMIS
		        TFT_Clear(BLUE);
		        return; }
	case 44: {
		biseyYap("Numerik 44");
	          break;}							
	case 41: {
biseyYap("Numerik 41");		
          break;		
		        }
	case 42: { 
biseyYap("Numerik 42");		
            break;		
		        }
	case 43: {
biseyYap("Numerik 43");			   
            break;		
		        }
	default : break;
}	
}
}



/**********************   Test TFT Display   **************************
*        By Selçuk ÖZBAYRAKTAR      Apr 2017
*        
*    
***********************************************************************/

void testTftDisplay(void){
	
  Delay_1ms(100);

  while (1) {
		
if(readKeyPad()==44) {
											drawMenuBar(); // KEY_PAD komutlari ile çalis, EXIT ile Seri Moda geri dön
	                    POINT_COLOR=BACK_COLOR; //Menüyü sil 
	                    TFT_DrawFillRectangle(0,0,170,40);
	                    POINT_COLOR=YELLOW;
}
}
}


void RCC_Configuration(void)
{
  /* Enable peripheral clocks ------------------------------------------------*/
  /* GPIOA,GPIOB Periph clock enable */
	
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE); 
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE);
		
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO  , ENABLE);//Alt Fonksiyonlara atadigimiz PB3,PB4 ve PA15 için
	}


/*******************Giris - Cikis Pinleri Konfigurasyonlari*********************/
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Asagidaki komut PB3,PB4,PA15 i I/O olarak kullanabilmek için REMAP ! -----------*/

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

  GPIO_InitStructure.GPIO_Pin =  DENEME_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(DENEME_Port, &GPIO_InitStructure);
	
//TFT_P_RS ve TFT_P_CS pinleri PA11/PA0 ve PA8/PA1	paralel bagli oldugundan
//PA0 ve PA1 ile çalisirken PA8 ve PA11 in yüksek empedansli konumda oldugundan emin olalim

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8|GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}


/*****************************************************************************
** Delay  (Timer/Interrupt kullanmayan gecikme)
*****************************************************************************/
void Delay_100us(uint16_t time)
{
	uint16_t i,j;
	for(i=time;i>0;i--)
	{
		for(j=796;j>0;j--);
	}
}

/*****************************************************************************
** Delay  (Timer/Interrupt kullanmayan gecikme)
*****************************************************************************/
void Delay_1ms(uint16_t time)
{
	uint16_t i,j;
	for(i=time;i>0;i--)
	{
		for(j=7960;j>0;j--);
	}
}

/****************************************************************************
 Function :   iToA(int i, char b[], uint8_t c)
 Description: integer degiskeni string'e çevirir
 Parameters : i: integer deger, b string adresi, c : 10,16 ya da 2 (desimal, hex ve binary çevirme için)
 Returns :    *b[] ye yerlestirilmis string
 ****************************************************************************/ 
void itoa(int i, unsigned char b[], uint8_t c){
    char const digit[] = "0123456789ABCDEF";
	int shifter;
    unsigned char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/c;
    }while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%c];
        i = i/c;
    }while(i);

}
