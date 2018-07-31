//#include "ILI9341_P/hardware/lcd/lcd.h"
#include "TOUCH_PANEL/TouchPanel.h" 
#include "math.h"
#include "stm32f10x_adc.h"
#include "displays/ILI9341_P/hardware/TFT_P/tft_p.h"

//////////////////////////////////////////////////////////////////////////////////	 
//   ADS7846 Touch controller için bit banging ile SPI çalismasi yapiliyor
//////////////////////////////////////////////////////////////////////////////////

_m_tp_dev tp_dev=
{
	TP_Init,   //uint8_t (*init)(void)
	TP_Scan,   //uint8_t (*scan)(uint8_t)
	TP_Adjust, //void    (*adjust)(void)
	0,         //uint16_t x0
	0,         //uint16_t y0
 	0,         //uint16_t x
	0,         //uint16_t y
	0,         //uint8_t sta
	0.45,      //float xfac  Default: 320/711=0.45 (bu ve asagidaki degerler kalibrasyon ile degistirilebiliyor
	0.33,      //float yfac  Default: 240/727=0.33  bunun için PA03 deki kalibrasyon tusuna basarak TP_touch ça-
	-45,	  	 //short xoff	 x offset Default: -45   listirilir.)
	-40,       //short yoff  y offset Default: -40
	0,	  	 	 //touchtype	 default landscape modu
};					

uint8_t CMD_RDX=1;
uint8_t	CMD_RDY=2;
unsigned char buffer3[3];
uint8_t PEN;


/* *************      readAndDisplayTP   ******************************
*        By Selçuk ÖZBAYRAKTAR      Aug 2017
*        
*   TouchPanel X ve Y gerlim degerlerini okur ve görüntüler
***********************************************************************/

void readAndDisplayTP(void){
uint16_t xVal,yVal;

	POINT_COLOR=YELLOW;
	BACK_COLOR=BLUE;
    TP_Read_XY(&xVal,&yVal);
	  itoa(xVal,buffer3,10);
		TFT_ShowString(0,0,16,"    ",0);
		TFT_ShowString(0,0,16,buffer3,0);	
	  itoa(yVal,buffer3,10);
		TFT_ShowString(0,30,16,"    ",0);
		TFT_ShowString(0,30,16,buffer3,0);

//	while(1){
/*
		// Önce TPX degeri (Tam skala 1000, degerler no Touch=0..60, Touch Min:100 Touch Max: 850)
		
		TP_AN_InputRearrange(2,1);
    Delay_1ms(1);		
		anDeger=adcOku(1);
		itoa(anDeger,buffer,10);
		TP_AN_InputRearrange(2,0);
		TFT_ShowString(0,0,16,"    ",0);
		TFT_ShowString(0,0,16,buffer,0);		

		// Sonra da TPY degeri
		
		TP_AN_InputRearrange(1,1);
		Delay_1ms(1);
		anDeger=adcOku(2);
		itoa(anDeger,buffer,10);
		TP_AN_InputRearrange(1,0);
		TFT_ShowString(0,30,16,"    ",0);
		TFT_ShowString(0,30,16,buffer,0);
*/
		Delay_1ms(1000);
//	}
}

/* *******  ADC Configuration  ****************************************
*        By Selçuk ÖZBAYRAKTAR      July 2017
*        
***********************************************************************/
void ADC_Configuration(void)
{
  ADC_InitTypeDef ADC_InitStructure;
 
	// ADC1 konfigurasyonlari ------------------------------------------------------
 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO, ENABLE);
    
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	 
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;			 
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	 
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		    
  ADC_InitStructure.ADC_NbrOfChannel = 1;					 
  ADC_Init(ADC1, &ADC_InitStructure);

  // ADC1 kanal 0'da   (PA0) 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);  // PA0 ADC1 Channel 0
  ADC_Cmd(ADC1, ENABLE);                                  
  ADC_SoftwareStartConvCmd(ADC1,ENABLE); 
	
 /* ADC2 konfigurasyonlari ------------------------------------------------------*/	

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2 | RCC_APB2Periph_AFIO, ENABLE);
    
 
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	 
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;			 
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	 
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;		    
  ADC_InitStructure.ADC_NbrOfChannel = 1;					 

  ADC_Init(ADC2, &ADC_InitStructure);

  /* ADC1 kanal 1'da,  PA1*/ 
  ADC_RegularChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_55Cycles5);  // PA2 ADC1 Channel	1
  ADC_Cmd(ADC2, ENABLE);                                  
  ADC_SoftwareStartConvCmd(ADC2,ENABLE); 
}

/* *************     TP_AN_InputRearrange   ***************************************
*        By Selçuk ÖZBAYRAKTAR      Aug 2017
*        
*   Touch Panel de X/Y gerilim degerlerini okumak üzere X+/X_M , Y+/Y_M pinlerinin
*   ayarlarini yapar
//========= ILI9341 2.4" 8bit paralel girisli TFT ekranin TP pinleri ==============//
//TFT_P_D1  TPX+    PB9  (Her zaman digital output, TP_X ölçümü sirasinda 1 (Vcc) uygulaniyor)
//TFT_P_D0  TPY+    PB8  (Her zaman digital output, TP_Y ölçümü sirasinda 1 (Vcc) uygulaniyor)
//TFT_P_CS	TPX_M   PA1  (TP_Y ölçümü sirasinda ADC1 girisi olarak kullaniliyor, TPX ölçümü sirasinda "0" uygulaniyor)
//TFT_P_RS	TPY_M   PA0  (TP_X ölçümü sirasinda ADC2 girisi olarak kullaniliyor, TPY ölçümü sirasinda "0" uygulaniyor)

Manuel ölçüm sonuçlari:

Y Ölçümü PA0-TFT_RS (TPY_M) >> GND; PB8-TFT_P_D0 (TPY+) >> Vcc;  PA1-TFT_CS (TPX_M)>> AIN; PB8-TFT_P_D0 (TPX+)>>IPD

No touch : 0V 40

0,47V    0.46      0.45   105
1.6      1.6       1.6    460
2.8      2.8       2.8    850

X Ölçümü PA1-TFT_CS (TPX_M) >> GND; PB9-TFT_P_D1 (TPX+) >> Vcc;  PA0-TFT_RS (TPY_M)>> AIN; PB9-TFT_P_D1 (TPY+)>>IPD

No touch : 0V

0,36V     1.5       2.65   105
0.36      1.5       2.65   460
0.36      1.5       2.65   850
***********************************************************************/
void TP_AN_InputRearrange(unsigned char adcNo, unsigned char InputStatus)
	{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	switch (adcNo){
	case 1:{
	if(InputStatus==1)
	  {				// TPY için TPX_M üzerinden ölçüm yapilacak, TPX_M yi analog input yap, TPY+ >> Vcc, TPY_M >>GND 
	GPIO_InitStructure.GPIO_Pin =  TPX_M_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //TPX_M Analog Input to measure TP
  GPIO_Init(TPX_M_Port, &GPIO_InitStructure);
			
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD; // TPX+ (TFT_P_D1 pinini Pull Down yap)
			GPIO_InitStructure.GPIO_Pin=TPX_Pin;
			GPIO_Init(TPX_Port,&GPIO_InitStructure);
			
			GPIO_SetBits(TPY);  // TPY+=Vcc
			GPIO_ResetBits(TPY_M); // TPY_M=0
			
		} else  // TPX_M yi Tekrar output (TFT_P_CS) yapip display moduna geç
	{GPIO_InitStructure.GPIO_Pin =  TPX_M_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //TPX_M (TFT_P_CS) Output ve PushPull
  GPIO_Init(TPX_M_Port, &GPIO_InitStructure);
		
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; // TPX+ pinini tekrar normal konumuna (Output to TFT_P_D1) alalim
			GPIO_InitStructure.GPIO_Pin=TPX_Pin;
			GPIO_Init(TPX_Port,&GPIO_InitStructure);

  GPIO_SetBits(TFT_P_CS);
	}	
		break;
 		    }
	case 2:{
	if(InputStatus==1)
	  {				// TPX için TPY_M üzerinden ölçüm yapilacak, TPY_M yi analog input yap, TPX+>> Vcc, TPX_M >>GND 
	GPIO_InitStructure.GPIO_Pin =  TPY_M_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; //TPY_M Analog Input to measure TP (TFT_P_RS)
  GPIO_Init(TPY_M_Port, &GPIO_InitStructure);
			
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD; // TPY+ (TFT_P_D0) pinini Pull Down yap
			GPIO_InitStructure.GPIO_Pin=TPY_Pin;
			GPIO_Init(TPY_Port,&GPIO_InitStructure);
			
			GPIO_SetBits(TPX);  // TPX+=Vcc
			GPIO_ResetBits(TPX_M); // TPX_M=0
			
		} else  // TPY_M yi Tekrar output (TFT_P_RS) yapip display moduna geç
	{GPIO_InitStructure.GPIO_Pin =  TPY_M_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //TPY_M (TFT_P_RS) Output ve PushPull
  GPIO_Init(TPY_M_Port, &GPIO_InitStructure);
		
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; // TPY+ pinini tekrar normal (OUT PP) konumuna (Output to TFT_P_D0) alalim
			GPIO_InitStructure.GPIO_Pin=TPY_Pin;
			GPIO_Init(TPY_Port,&GPIO_InitStructure);
		
	GPIO_SetBits(TFT_P_RS);
	}			
		break;
		}
}
	}


// ********************************* ADC den deger okuma **********************************
uint16_t adcOku(unsigned char adcNo)
{uint16_t AD_value;	
switch (adcNo){
	case 1:{
	if(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)==SET)
	  {
    AD_value=(ADC_GetConversionValue(ADC1)/4.096); // Voltaj degerini % olarak veriyor (%100>>3.3V) 
		}
		break;
 		    }
	case 2:{
			  if(ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC)==SET)
	  {
    AD_value=(ADC_GetConversionValue(ADC2)/4.096); // Voltaj degerini % olarak veriyor (%100>>3.3V) 
			}
		break;
		}
		case 3:{ // Reserved - ileride kullanilacak
	  {
    AD_value=0;
					}
		break;
 		    }
	case 4:{ // Reserved - ileride kullanilacak
	  {
    AD_value=0;
			}
		break;
		}
	default: {AD_value=0xFFFF; break;}
}
return AD_value;
}
 	 			    			 		 
	   
/*
u16 TP_Read_AD(u8 CMD)	  
{ 	 
	u8 count=0; 	  
	u16 Num=0; 
	TCLK=0;	 	 
	TDIN=0; 	
	TCS=0; 	
	TP_Write_Byte(CMD);
	Delay_1us(6); //ADS7846 ....
	TCLK=0; 	     	    
	Delay_1us(1);    	                
	TCLK=1;		    	    
	TCLK=0; 	     	    
	for(count=0;count<16;count++) 
	{ 				  
		Num<<=1; 	 
		TCLK=0;  	    	   
		TCLK=1;
		if(DOUT)Num++; 		 
	}  	
	Num>>=4;
	TCS=1;
	return(Num);   
}
*/

u16 TP_Read_AD(u8 CMD) // Modified for 4 wire touch panel version	 Sozbayraktar 
{ 	  
	u16 Num=0; 
	if(CMD==1){ //read X	
  	TP_AN_InputRearrange(2,1);
    Delay_1ms(1);		
		Num=adcOku(1);
		TP_AN_InputRearrange(2,0);				
	} else // read Y
	{
   	TP_AN_InputRearrange(1,1);
		Delay_1ms(1);
		Num=adcOku(2);
		TP_AN_InputRearrange(1,0);
	}
	
	return(Num);
}	

#define READ_TIMES 5
#define LOST_VAL 1

u16 TP_Read_XOY(u8 xy)
{
	u16 i, j;
	u16 buf[READ_TIMES];
	u16 sum=0;
	u16 temp;
	for(i=0;i<READ_TIMES;i++)buf[i]=TP_Read_AD(xy);		 		    
	for(i=0;i<READ_TIMES-1; i++)
	{
		for(j=i+1;j<READ_TIMES;j++)
		{
			if(buf[i]>buf[j])
			{
				temp=buf[i];
				buf[i]=buf[j];
				buf[j]=temp;
			}
		}
	}	  
	sum=0;
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++)sum+=buf[i];
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 

u8 TP_Read_XY(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=TP_Read_XOY(1);
	ytemp=TP_Read_XOY(2);	  												   
	//if(xtemp<100||ytemp<100)return 0;
	*x=xtemp;
	*y=ytemp;
	return 1;
}

uint8_t TP_Read_XY_pixels(u16 *x,u16 *y)
{
	u16 xtemp,ytemp;			 	 		  
	xtemp=TP_Read_XOY(1);
	ytemp=TP_Read_XOY(2);
	if(xtemp<100||ytemp<100)return 0;
*x=xtemp*tp_dev.xfac+tp_dev.xoff; //Pixele çevirme katsayisi ve offset ile pozisyon hesaplamasini yap
*y=ytemp*tp_dev.yfac+tp_dev.yoff;	
	reportStylus(x,y);//Status bar üzerinde geri bildirim
	return 1;
}

void reportStylus(u16 *x,u16 *y)
{
		TFT_ShowString(310,224,16,"X",0);
    itoa(*x,buffer3,10);
	  TFT_ShowString(210,224,16,"x=    ",0);
		TFT_ShowString(226,224,16,buffer3,0);	
	  itoa(*y,buffer3,10);
		TFT_ShowString(260,224,16,"y=    ",0);
		TFT_ShowString(275,224,16,buffer3,0);	
Delay_1ms(200);
TFT_ShowString(310,224,16," ",0);	
}

#define ERR_RANGE 50
u8 TP_Read_XY2(u16 *x,u16 *y) 
{
	u16 x1,y1;
 	u16 x2,y2;
 	u8 flag;    
    flag=TP_Read_XY(&x1,&y1);   
    if(flag==0)return(0);
    flag=TP_Read_XY(&x2,&y2);	   
    if(flag==0)return(0);   
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))
    &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }else return 0;	  
}  
//////////////////////////////////////////////////////////////////////////////////		  

void TP_Draw_Touch_Point(uint16_t x,uint16_t y,uint16_t color)
{
	POINT_COLOR=color;
	TFT_DrawLine(x-12,y,x+13,y);
	TFT_DrawLine(x,y-12,x,y+13);
	TFT_DrawPoint(x+1,y+1);
	TFT_DrawPoint(x-1,y+1);
	TFT_DrawPoint(x+1,y-1);
	TFT_DrawPoint(x-1,y-1);
	TFT_circle(x,y,POINT_COLOR,6,0);
}	  

void TP_Draw_Big_Point(u16 x,u16 y,u16 color)
{	    
	POINT_COLOR=color;
	TFT_DrawPoint(x,y); 
	TFT_DrawPoint(x+1,y);
	TFT_DrawPoint(x,y+1);
	TFT_DrawPoint(x+1,y+1);	 	  	
}						  
//////////////////////////////////////////////////////////////////////////////////
/********************* CheckPen *************************************************
* TFT mizde interrupt pini olmadigindan polling yaparak ekrana dokunulup 
* dokunulmadigini kontrol ediyoruz
**********************************************************************************/
uint8_t checkPen(void)
{uint16_t xVal,yVal;

	POINT_COLOR=YELLOW;
	BACK_COLOR=BLUE;
    TP_Read_XY(&xVal,&yVal);
	  itoa(xVal,buffer3,10);
		TFT_ShowString(84,100,16,"x=    ",0);
		TFT_ShowString(100,100,16,buffer3,0);	
	  itoa(yVal,buffer3,10);
		TFT_ShowString(84,130,16,"x=    ",0);
		TFT_ShowString(100,130,16,buffer3,0);

if((xVal>100)&&(yVal>100)) PEN=0; // Ekrana dokunulmus
else PEN=1;	//dokunulmamis

return PEN;
}
//////////////////////////////////////////////////////////////////////////////////
u8 TP_Scan(u8 tp)
{		   
	if(checkPen()==0)  //ekrana dokunulmus ise
	{
		if(tp)TP_Read_XY2(&tp_dev.x,&tp_dev.y);  //tp "!=0" olarak verilmis ise
		else if(TP_Read_XY2(&tp_dev.x,&tp_dev.y)) // tp "0" olarak verilmis ise de ekrani oku, uygulanacak islem farkli
		{
	 		tp_dev.x=tp_dev.xfac*tp_dev.x+tp_dev.xoff;
			tp_dev.y=tp_dev.yfac*tp_dev.y+tp_dev.yoff;  
	 	} 
		if((tp_dev.sta&TP_PRES_DOWN)==0)  //TP_PRES_DOWN(0x80) "0" ise
		{		 
			tp_dev.sta=TP_PRES_DOWN|TP_CATH_PRES; //TP_PRES_DOWN ve TP_CATH_PRES(0x40) set ediliyor 
			tp_dev.x0=tp_dev.x;  // dokunulunca koordinatlar x0/y0 olarak kaydediliyor
			tp_dev.y0=tp_dev.y;
	
		}			   
	}else  //ekrana dokunulmamis ise
	{
		if(tp_dev.sta&TP_PRES_DOWN)  //
		{
			tp_dev.sta&=~(1<<7);	//TP_CATH_PRES biti sifirlaniyor
		}else
		{
			tp_dev.x0=0;
			tp_dev.y0=0;
			tp_dev.x=0xffff;
			tp_dev.y=0xffff;
		}	    
	}
	return tp_dev.sta&TP_PRES_DOWN;
}	  

const u8* TP_REMIND_MSG_TBL="Please use the stylus click the cross on the screen.The cross will always move until the screen adjustment is completed.";
 					  
void TP_Adj_Info_Show(u16 x0,u16 y0,u16 x1,u16 y1,u16 x2,u16 y2,u16 x3,u16 y3,u16 fac)
{	  
	POINT_COLOR=YELLOW;
	TFT_ShowString(40,160,16,"x1:",1);
 	TFT_ShowString(40+80,160,16,"y1:",1);
 	TFT_ShowString(40,180,16,"x2:",1);
 	TFT_ShowString(40+80,180, 16,"y2:",1);
	TFT_ShowString(40,200, 16,"x3:",1);
 	TFT_ShowString(40+80,200, 16,"y3:",1);
	TFT_ShowString(40,220, 16,"x4:",1);
 	TFT_ShowString(40+80,220, 16,"y4:",1);  
 	TFT_ShowString(40,240, 16,"fac is:",1);
// Tanim: TFT_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)	
	TFT_ShowNum(40+24,160,x0,4,16);	
	TFT_ShowNum(40+24+80,160,y0,4,16);
	TFT_ShowNum(40+24,180,x1,4,16);	
	TFT_ShowNum(40+24+80,180,y1,4,16);
	TFT_ShowNum(40+24,200,x2,4,16);	
	TFT_ShowNum(40+24+80,200,y2,4,16);
	TFT_ShowNum(40+24,220,x3,4,16);	
	TFT_ShowNum(40+24+80,220,y3,4,16);	
 	TFT_ShowNum(40+56,tft.width,fac,3,16); 
}
		 
void TP_Adjust(void)
{								 
	u16 pos_temp[4][2];
	u8  cnt=0;	
	u16 d1,d2;
	uint32_t tem1,tem2;
	float fac; 	
	u16 outtime=0;
 	cnt=0;				
	POINT_COLOR=BLUE;
	BACK_COLOR =WHITE;
	TFT_Clear(WHITE);   
	POINT_COLOR=RED; 
	TFT_Clear(WHITE);	   
	POINT_COLOR=BLACK;
	TFT_ShowString(10,40,16,"Please use the stylus click the",1);
	TFT_ShowString(10,56,16,"cross on the screen.The cross will",1);
	TFT_ShowString(10,72,16,"always move until the screen ",1);
	TFT_ShowString(10,88,16,"adjustment is completed.",1);

	TP_Draw_Touch_Point(20,20,RED); 
	tp_dev.sta=0;
	tp_dev.xfac=0;	 
	while(1)
	{
		tp_dev.scan(1);
		if((tp_dev.sta&0xc0)==TP_CATH_PRES)
		{	
			outtime=0;		
			tp_dev.sta&=~(1<<6);
						   			   
			pos_temp[cnt][0]=tp_dev.x;
			pos_temp[cnt][1]=tp_dev.y;
			cnt++;	  
			switch(cnt)
			{			   
				case 1:						 
					TP_Draw_Touch_Point(20,20,WHITE);	 
					TP_Draw_Touch_Point(tft.width-20,20,RED);
				break;
				case 2:
 					TP_Draw_Touch_Point(tft.width-20,20,WHITE);
					TP_Draw_Touch_Point(20,tft.height-20,RED);	
					break;
				case 3:
 					TP_Draw_Touch_Point(20,tft.height-20,WHITE);	
 					TP_Draw_Touch_Point(tft.width-20,tft.height-20,RED);
					break;
				case 4:
					tem1=absolute(pos_temp[0][0]-pos_temp[1][0]);//x1-x2
					tem2=absolute(pos_temp[0][1]-pos_temp[1][1]);//y1-y2
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);  // Ortalama hesabi
					
					tem1=absolute(pos_temp[2][0]-pos_temp[3][0]);//x3-x4
					tem2=absolute(pos_temp[2][1]-pos_temp[3][1]);//y3-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05||d1==0||d2==0)
					{
						cnt=0;
 				    	TP_Draw_Touch_Point(tft.width-20,tft.height-20,WHITE);
   	 					TP_Draw_Touch_Point(20,20,RED);								//»­µã1
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//ÏÔÊ¾Êý¾Ý   
 						continue;
					}
					tem1=absolute(pos_temp[0][0]-pos_temp[2][0]);//x1-x3
					tem2=absolute(pos_temp[0][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);
					
					tem1=absolute(pos_temp[1][0]-pos_temp[3][0]);//x2-x4
					tem2=absolute(pos_temp[1][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)
					{
						cnt=0;
 				    	TP_Draw_Touch_Point(tft.width-20,tft.height-20,WHITE);
   	 					TP_Draw_Touch_Point(20,20,RED);	
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//ÏÔÊ¾Êý¾Ý   
						continue;
					}
								   
					tem1=absolute(pos_temp[1][0]-pos_temp[2][0]);//x1-x3
					tem2=absolute(pos_temp[1][1]-pos_temp[2][1]);//y1-y3
					tem1*=tem1;
					tem2*=tem2;
					d1=sqrt(tem1+tem2);
	
					tem1=absolute(pos_temp[0][0]-pos_temp[3][0]);//x2-x4
					tem2=absolute(pos_temp[0][1]-pos_temp[3][1]);//y2-y4
					tem1*=tem1;
					tem2*=tem2;
					d2=sqrt(tem1+tem2);
					fac=(float)d1/d2;
					if(fac<0.95||fac>1.05)
					{
						cnt=0;
 				    	TP_Draw_Touch_Point(tft.width-20,tft.height-20,WHITE);
   	 					TP_Draw_Touch_Point(20,20,RED);
 						TP_Adj_Info_Show(pos_temp[0][0],pos_temp[0][1],pos_temp[1][0],pos_temp[1][1],pos_temp[2][0],pos_temp[2][1],pos_temp[3][0],pos_temp[3][1],fac*100);//ÏÔÊ¾Êý¾Ý   
						continue;
					}
					tp_dev.xfac=(float)(tft.width-40)/(pos_temp[1][0]-pos_temp[0][0]);	//Faktör hesaplaniyor, iki nokta arasi pixel sayisi/delta gerilim
					tp_dev.xoff=(tft.width-tp_dev.xfac*(pos_temp[1][0]+pos_temp[0][0]))/2; //offset hesabi
						  
					tp_dev.yfac=(float)(tft.height-40)/(pos_temp[2][1]-pos_temp[0][1]);
					tp_dev.yoff=(tft.height-tp_dev.yfac*(pos_temp[2][1]+pos_temp[0][1]))/2;  
					if(absolute(tp_dev.xfac)>2||absolute(tp_dev.yfac)>2)
					{
						cnt=0;
 				    	TP_Draw_Touch_Point(tft.width-20,tft.height-20,WHITE);
   	 					TP_Draw_Touch_Point(20,20,RED);
						TFT_ShowString(40,26, 16,"TP Need readjust!",1);
						tp_dev.touchtype=!tp_dev.touchtype;
						if(tp_dev.touchtype==0)
						{
							CMD_RDX=1;
							CMD_RDY=2;	 
						}else
						{
							CMD_RDX=2;
							CMD_RDY=1;	 
						}			    
						continue;
					}		
					POINT_COLOR=YELLOW;
					TFT_Clear(BLUE);
					TFT_ShowString(35,110, 16,"Touch Screen Adjust OK!",1);
					Delay_1ms(1000);
//					TP_Save_Adjdata();  //24CXX kullanildiginda bunu aç
					return;				 
			}
		}
		Delay_1ms(10);
		outtime++;
		if(outtime>1000)
		{
//			TP_Get_Adjdata();     //24CXX kullanildiginda bunu aç
			break;
	 	} 
 	}
}		  

int16_t absolute(int16_t arg){
	int16_t x=arg;
	if(x<0) x=-x;
	return x;
}

u8 TP_Init(void)
{
GPIO_InitTypeDef GPIO_InitStructure;	//GPIO			    		   

ADC_Configuration();
			    		   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_0|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
 	   

  TP_Read_XY(&tp_dev.x,&tp_dev.y);	 
										    
		  TFT_Clear(DARKBLUE);
	    TP_Adjust();	

	return 1; 									 
}


