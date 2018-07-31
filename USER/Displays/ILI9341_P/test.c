#include "Displays/ILI9341_P/hardware/tft_p/tft_p.h"
#include "Displays/ILI9341_P/gui.h"
#include "Displays/ILI9341_P/test.h"

//////////////////////////////////////////////////////////////////////////////////
//STM32F103RBT6,72MHz 3.3V
//QDtech-TFT for STM32
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////
//========================variable==========================//
u16 ColorTab[5]={BRED,YELLOW,RED,GREEN,BLUE};
//=====================end of variable======================//

//******************************************************************
//DrawTestPage
//******************************************************************
void DrawTestPage(u8 *str)
{
//up
TFT_Fill(0,0,tft.width,20,BLUE);
//down
TFT_Fill(0,tft.height-20,tft.width,tft.height,BLUE);
POINT_COLOR=WHITE;
TFT_ShowString(0,2,16,str,0);
TFT_ShowString(0,tft.height-18,16,"Selcuk Ozbayraktar",0);
TFT_Fill(0,20,tft.width,tft.height-20,BLACK);
}

//******************************************************************
//Main_test
//******************************************************************
void main_test(void)
{
	DrawTestPage("DRAWING TEST PAGE");
	

	
	TFT_ShowString(0,30,16,"Bu bir satir",0);
	TFT_ShowString(0,60,16,"Bu ikinci satir",0);	
	TFT_ShowString(0,90,16," ILI9341 240X320",0);
	TFT_ShowString(0,120,16,"Selcuk Ozbayraktar 2017 Aug 24",0);
	Delay_1ms(3000);		
}

//******************************************************************
//Test_Color
//******************************************************************
void Test_Color(void)  
{
	DrawTestPage(".... 1: ...... ");
	TFT_Fill(20,20,tft.width-20,tft.height-20,WHITE);
	TFT_ShowString(tft.width-50,30,16,"White",0);Delay_1ms(1000);
	TFT_Fill(20,20,tft.width-20,tft.height-20,BLACK);
	TFT_ShowString(tft.width-50,30,16,"Black",0);Delay_1ms(1000);
	TFT_Fill(20,20,tft.width-20,tft.height-20,RED);
	TFT_ShowString(tft.width-50,30,16,"Red",0); Delay_1ms(1000);
	TFT_Fill(20,20,tft.width-20,tft.height-20,GREEN);
	TFT_ShowString(tft.width-50,30,16,"Green",0);Delay_1ms(1000);
	TFT_Fill(20,20,tft.width-20,tft.height-20,BLUE);
	TFT_ShowString(tft.width-50,30,16,"Blue",0);Delay_1ms(1000);
}

//******************************************************************
//Test_FillRec
//******************************************************************
void Test_FillRec(void)
{
	u8 i=0;
	DrawTestPage("....... 2:GUI ......");
	TFT_Fill(0,20,tft.width,tft.height-20,WHITE);
	for (i=0; i<5; i++) 
	{
		TFT_DrawRectangle(tft.width/2-80+(i*15),tft.height/2-80+(i*15),tft.width/2-80+(i*15)+60,tft.height/2-80+(i*15)+60); 
		POINT_COLOR=ColorTab[i];
	}
	Delay_1ms(1500);	
	TFT_Fill(0,20,tft.width,tft.height-20,WHITE); 
	for (i=0; i<5; i++) 
	{
		TFT_DrawFillRectangle(tft.width/2-80+(i*15),tft.height/2-80+(i*15),tft.width/2-80+(i*15)+60,tft.height/2-80+(i*15)+60); 
		POINT_COLOR=ColorTab[i];
	}
	Delay_1ms(1500);
}

//******************************************************************
//
//******************************************************************
void Test_Circle(void)
{
	u8 i=0;
	DrawTestPage("..... 3:GUI .........");
	TFT_Fill(0,20,tft.width,tft.height-20,WHITE);
	for (i=0; i<5; i++)  
	gui_circle(tft.width/2-80+(i*25),tft.height/2-50+(i*25),ColorTab[i],30,0);
	Delay_1ms(1500);	
	TFT_Fill(0,20,tft.width,tft.height-20,WHITE); 
	for (i=0; i<5; i++) 
	  	gui_circle(tft.width/2-80+(i*25),tft.height/2-50+(i*25),ColorTab[i],30,1);
	Delay_1ms(1500);
}

//******************************************************************
//Font_test
//******************************************************************
void English_Font_test(void)
{
	DrawTestPage(".... 4: ........");
	POINT_COLOR=YELLOW;
	BACK_COLOR=BLUE;
	TFT_ShowString(10,30,12,"6X12:abcdefghijklmnopqrstuvwxyz0123456789",0);
	TFT_ShowString(10,45,12,"6X12:ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789",1);
	TFT_ShowString(10,60,12,"6X12:~!@#$%^&*()_+{}:<>?/|-+.",0);
	TFT_ShowString(10,80,16,"8X16:abcdefghijklmnopqrstuvwxyz0123456789",0);
	TFT_ShowString(10,100,16,"8X16:ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789",1);
	TFT_ShowString(10,120,16,"8X16:~!@#$%^&*()_+{}:<>?/|-+.",0); 
	Delay_1ms(1200);
}




