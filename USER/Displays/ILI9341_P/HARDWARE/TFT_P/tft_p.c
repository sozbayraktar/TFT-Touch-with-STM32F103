#include "displays/ILI9341_P/HARDWARE/tft_p/tft_p.h"
#include "displays/ILI9341_P/hardware/tft_p/font.h"
#include "displays/ILI9341_P/hardware/tft_p/bmp.h"
#include "stdlib.h"

/****************************************************************************************************
//=========================================TFT Genel================================================//
//5V DC 5V
//GND
//=======================================TFT Data pinleri==========================================//
//TFT_D0 PB8
//TFT_D1 PB9
//TFT_D2 PB10
//TFT_D3 PB11
//TFT_D4 PB12
//TFT_D5 PB13
//TFT_D6 PB14
//TFT_D7 PB15
//=======================================TFT Kontrol Pinleri==========================================//
//TFT_RST	>> Vcc
//TFT_CS	PA8	
//TFT_RS	PA11	
//TFT_WR	PA15
//TFT_RD	PA12
//TFT_RST PB0
//=========================================TFT Kontrol IC =========================================//
//XPT2046
**************************************************************************************************/	
	   
tft_device_typedef tft;


uint16_t POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;  
uint16_t DeviceCode;	 


//******************************************************************
//TFT_WR_REG
//******************************************************************
void TFT_WR_REG(uint8_t data)
{
#if TFT_USE8BIT_MODEL==1
	TFT_RS_CLR;
	TFT_CS_CLR;	
	DATAOUT(data<<8);
	TFT_WR_CLR;
	TFT_WR_SET;
	TFT_CS_SET;

#else
	TFT_RS_CLR;
	TFT_CS_CLR;
	DATAOUT(data);
	TFT_WR_CLR;
	TFT_WR_SET;
	TFT_CS_SET;
			
#endif	
	}

//******************************************************************
//TFT_WR_DATA  16 bit modunda:   16 bitlik veriyi B0..15 üzerinden gönderir
//              8 bit modunda:   16 bitin küçük 8 bitini B8..B15 üzerinden gönderir	
//******************************************************************
void TFT_WR_DATA(uint16_t data)
{
#if TFT_USE8BIT_MODEL==1// 8 bit veri 8 pin üzerinden (B8..B15)
	TFT_RS_SET;
	TFT_CS_CLR;
	DATAOUT(data<<8);
	TFT_WR_CLR;
	TFT_WR_SET;
	TFT_CS_SET;
	
	#else// 16 bit modu: 16 bitlik veriyi B0..B15 üzerine koyarak gönderir
	TFT_RS_SET;
	TFT_CS_CLR;
	DATAOUT(data);
	TFT_WR_CLR;
	TFT_WR_SET;
	TFT_CS_SET;			
#endif
}
//******************************************************************
//TFT_DrawPoint_16Bit
//******************************************************************
void TFT_DrawPoint_16Bit(uint16_t color)
{
#if TFT_USE8BIT_MODEL==1
	TFT_CS_CLR;
	TFT_RD_SET;
	TFT_RS_SET;
	DATAOUT(color);	
	TFT_WR_CLR;
	TFT_WR_SET;	
	DATAOUT(color<<8);	
	TFT_WR_CLR;
	TFT_WR_SET;	 
	TFT_CS_SET;
#else
	TFT_WR_DATA(color); 
#endif


}

//******************************************************************
//TFT_WriteReg
//******************************************************************
void TFT_WriteReg(uint8_t TFT_Reg, uint16_t TFT_RegValue)
{	
	TFT_WR_REG(TFT_Reg);  
	TFT_WR_DATA(TFT_RegValue);	    		 
}	   
	 
//******************************************************************
//TFT_WriteRAM_Prepare
//******************************************************************
void TFT_WriteRAM_Prepare(void)
{
	TFT_WR_REG(tft.wramcmd);
}	 

//******************************************************************
//TFT_DrawPoint
//******************************************************************
void TFT_DrawPoint(uint16_t x,uint16_t y)
{
	TFT_SetCursor(x,y);

#if TFT_USE8BIT_MODEL==1

	TFT_RS_SET; 
	TFT_CS_CLR;
	DATAOUT(POINT_COLOR);	
	TFT_WR_CLR;
	TFT_WR_SET;	
	DATAOUT(POINT_COLOR<<8);	
	TFT_WR_CLR;
	TFT_WR_SET;	 
	TFT_CS_SET;

#else
	TFT_WR_DATA(POINT_COLOR); 
#endif
}

//******************************************************************
//TFT_Clear
//******************************************************************
void TFT_Clear(uint16_t Color)
{
	u32 index=0;      
	TFT_SetWindows(0,0,tft.width-1,tft.height-1);	
#if TFT_USE8BIT_MODEL==1
	TFT_RS_SET;
	TFT_CS_CLR;	   
	for(index=0;index<tft.width*tft.height;index++)
	{
		DATAOUT(Color);	
		TFT_WR_CLR;
		TFT_WR_SET;	
		
		DATAOUT(Color<<8);	
		TFT_WR_CLR;
		TFT_WR_SET;	   
	}
	TFT_CS_SET;	
#else
	for(index=0;index<tft.width*tft.height;index++)
	{
		TFT_WR_DATA(Color);		  
	}
#endif
	
} 

//******************************************************************
//TFT_GPIOInit
//******************************************************************
void TFT_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

/*bunlar main içinde RCC Configuration ve GPIO configuration taraf1ndan ayarlaniyor zaten 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE); 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
*/
	
// TFT KONTROL PINLERI -  DIKKAT ! HEPSININ A Portunda oldugu kabul edilmistir!

	GPIO_InitStructure.GPIO_Pin = TFT_RS_PIN|TFT_CS_PIN|TFT_RD_PIN|TFT_WR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //GPIOA	
	GPIO_SetBits(GPIOA,TFT_RS_PIN|TFT_CS_PIN|TFT_RD_PIN|TFT_WR_PIN);	

// TFT Data pinleri (Data pinleri PB8..15)

	GPIO_InitStructure.GPIO_Pin =TFT_DATA_PINS;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB,TFT_DATA_PINS);
	
// TFT RST Pini

	GPIO_InitStructure.GPIO_Pin = TFT_RST_PIN;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(TFT_RST_PORT,TFT_RST_PIN);	
	
}

//******************************************************************
//TFT_Reset
//******************************************************************
void TFT_RESET(void)
{
	TFT_RST_CLR;
	Delay_1ms(100);	
	TFT_RST_SET;
	Delay_1ms(50);
}
 	 
//******************************************************************

//TFT_P init

//******************************************************************
void TFT_P_Init(void)
{       										 
	TFT_GPIOInit();
	
	TFT_CS_SET;	
 	TFT_RESET();  // Hard reset

  Delay_1ms(5);
	/* Software reset */
	TFT_WR_REG(0x01);  //0x01  Reset

	Delay_1ms(5);	

	//************* Start Initial Sequence **********//

  TFT_WR_REG(0xEF);
  TFT_WR_DATA(0x03);
  TFT_WR_DATA(0x80);
  TFT_WR_DATA(0x02);
	
	TFT_WR_REG(0xCF);   //OK  
	TFT_WR_DATA(0x00); 
	TFT_WR_DATA(0xC1); 
	TFT_WR_DATA(0X30);
	
	TFT_WR_REG(0xED);  //OK
	TFT_WR_DATA(0x64); 
	TFT_WR_DATA(0x03); 
	TFT_WR_DATA(0X12); 
	TFT_WR_DATA(0X81);
	
	TFT_WR_REG(0xE8);  //OK ??
	TFT_WR_DATA(0x85); 
	TFT_WR_DATA(0x10); //(?? 0x10>>0x00)
	TFT_WR_DATA(0x7A); //(?? 0x7A>>0x78)
	
	TFT_WR_REG(0xCB);  //OK
	TFT_WR_DATA(0x39); 
	TFT_WR_DATA(0x2C); 
	TFT_WR_DATA(0x00); 
	TFT_WR_DATA(0x34); 
	TFT_WR_DATA(0x02);
	
	TFT_WR_REG(0xF7);  //OK
  TFT_WR_DATA(0x20);
	
	TFT_WR_REG(0xEA);  //OK
	TFT_WR_DATA(0x00); 
	TFT_WR_DATA(0x00);
	
	TFT_WR_REG(0xC0);    //OK?? Power control 
	TFT_WR_DATA(0x23);   //VRH[5:0] (?? 0x1B>>0x23)
	
	TFT_WR_REG(0xC1);    //OK?? Power control 
	TFT_WR_DATA(0x10);   //SAP[2:0];BT[3:0] (?? 0x01>>0x10)
	
	TFT_WR_REG(0xC5);    //VCM control 
	TFT_WR_DATA(0x30); 	 //3F  (?? 0x30>>0x3E)
	TFT_WR_DATA(0x20); 	 //3C  (?? 0x30>>0x28)
	
	TFT_WR_REG(0xC7);    //VCM control2 
	TFT_WR_DATA(0XB7);   // (?? 0xB7>>0x86)
	
	TFT_WR_REG(0x36);    // Memory Access Control (Orientation kontrolu MADCTL)
	TFT_WR_DATA(0x28);   // *** TFT_SetParam() içinde tekrar deger ataniyor, buradaki etkisiz

  /*
	  B7:B6:B5:B4 0010 : Page/column selection 
	
	  B3:B2:B1:B0 1000
	              1001 : Vertical Flip (9h)
                1010 : Horizontal Flip (Ah)								
	
	Orientation_Portrait_1 writedata(0x58);
	Orientation_Portrait_2 writedata(0x88);
	Orientation_Landscape_1 writedata(0x28);
	Orientation_Landscape_2 writedata(0xE8);
	Orientation_Landscape_3 writedata(0xA8);	
	*/
		
	TFT_WR_REG(0x3A);	   //  OK ILI9341 PIX FMT
	TFT_WR_DATA(0x55);
	
	TFT_WR_REG(0xB1);    // OK  ILI9341 FRMCTRL
	TFT_WR_DATA(0x00);   
	TFT_WR_DATA(0x1A);   //  (?? 0x1A>>0x18 )
	
	TFT_WR_REG(0xB6);    // OK ILI9341_DFUNCTR Display Function Control 
	TFT_WR_DATA(0x0A);   // (?? 0x0A>>0x08)
	TFT_WR_DATA(0xA2);   // (?? 0xA2>>0x82)
	TFT_WR_DATA(0x27);   // (?? _x__>>0x27)
	
	TFT_WR_REG(0xF2);    // OK 3Gamma Function Disable 
	TFT_WR_DATA(0x00);
	
	TFT_WR_REG(0x26);    //OK Gamma curve selected 
	TFT_WR_DATA(0x01);
	
	TFT_WR_REG(0xE0);    //Set Gamma 
	TFT_WR_DATA(0x0F); 
	TFT_WR_DATA(0x2A);   // (?? 0x2A>>0x31) 
	TFT_WR_DATA(0x28);   // (?? 0x28>>0x2B)
	TFT_WR_DATA(0x08);   // (?? 0x08>>0x2C)
	TFT_WR_DATA(0x0E); 
	TFT_WR_DATA(0x08); 
	TFT_WR_DATA(0x54); 
	TFT_WR_DATA(0XA9); 
	TFT_WR_DATA(0x43); 
	TFT_WR_DATA(0x0A); 
	TFT_WR_DATA(0x0F); 
	TFT_WR_DATA(0x00); 
	TFT_WR_DATA(0x00); 
	TFT_WR_DATA(0x00); 
	TFT_WR_DATA(0x00);
	
	TFT_WR_REG(0XE1);    //Set Gamma 
	TFT_WR_DATA(0x00); 
	TFT_WR_DATA(0x15); 
	TFT_WR_DATA(0x17); 
	TFT_WR_DATA(0x07); 
	TFT_WR_DATA(0x11); 
	TFT_WR_DATA(0x06); 
	TFT_WR_DATA(0x2B); 
	TFT_WR_DATA(0x56); 
	TFT_WR_DATA(0x3C); 
	TFT_WR_DATA(0x05); 
	TFT_WR_DATA(0x10); 
	TFT_WR_DATA(0x0F); 
	TFT_WR_DATA(0x3F); 
	TFT_WR_DATA(0x3F); 
	TFT_WR_DATA(0x0F);


	TFT_WR_REG(0x2B);  // *** Set Page Address, TFT_SetParam içinde yeniden ayarlaniyor
	TFT_WR_DATA(0x00);
	TFT_WR_DATA(0x00);
	TFT_WR_DATA(0x01);
	TFT_WR_DATA(0x3f); 
	
	TFT_WR_REG(0x2A);  // *** Set Column Address, TFT_SetParam() içinde yeniden ayarlaniyor
	TFT_WR_DATA(0x00);
	TFT_WR_DATA(0x00);
	TFT_WR_DATA(0x00);
	TFT_WR_DATA(0xef);
	
	
	TFT_WR_REG(0x11); //Exit Sleep
	Delay_1ms(120);
	
	TFT_WR_REG(0x29); //display on	

	TFT_SetParam();
	TFT_Clear(BLUE);

}
  		  
/*************************************************
*TFT_SetWindows
*************************************************/
void TFT_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
{	
	TFT_WR_REG(tft.setxcmd);	
	TFT_WR_DATA(xStar>>8);
	TFT_WR_DATA(0x00FF&xStar);		
	TFT_WR_DATA(xEnd>>8);
	TFT_WR_DATA(0x00FF&xEnd);

	TFT_WR_REG(tft.setycmd);	
	TFT_WR_DATA(yStar>>8);
	TFT_WR_DATA(0x00FF&yStar);		
	TFT_WR_DATA(yEnd>>8);
	TFT_WR_DATA(0x00FF&yEnd);	

	TFT_WriteRAM_Prepare();				
}   

/*************************************************
TFT_SetCursor
*************************************************/
void TFT_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
	TFT_WR_REG(tft.setxcmd);	
	TFT_WR_DATA(Xpos>>8);
	TFT_WR_DATA(0x00FF&Xpos);
	TFT_WR_DATA(Xpos>>8);        // *** bu satiri ve asagidakini ILI9381 3.5" TFT_P için eklemem gerekti
  TFT_WR_DATA(0x00FF&Xpos);	

	
	TFT_WR_REG(tft.setycmd);	 
	TFT_WR_DATA(Ypos>>8);
	TFT_WR_DATA(0x00FF&Ypos);
	TFT_WR_DATA(Ypos>>8);        // *** bu satiri ve asagidakini ILI9381 3.5" TFT_P için eklemem gerekti
  TFT_WR_DATA(0x00FF&Ypos);	

	TFT_WriteRAM_Prepare();	
} 


void TFT_SetParam(void)
{ 
  /*
	  B7:B6:B5:B4 0010 : Page/column selection 
	
	  B3:B2:B1:B0 1000
	              1001 : Vertical Flip (0x09)
                1010 : Horizontal Flip (0x0A)								
	
	Orientation_Portrait_1 writedata(0x58);
	Orientation_Portrait_2 writedata(0x88);
	Orientation_Landscape_1 writedata(0x28);
	Orientation_Landscape_2 writedata(0xE8);
	Orientation_Landscape_3 writedata(0xA8);  //TFT Inverted & Flip Vertical
	Orientation_Landscape_4 writedata(0x68);  //TFT Inverted & no vertical flip	
	*/
	
	tft.wramcmd=0x2C;
	tft.width=TFT_W;
	tft.height=TFT_H;
	tft.setxcmd=0x2A;
	tft.setycmd=0x2B;
	
#if USE_HORIZONTAL==1	  
	tft.dir=1;
#ifdef TFT_INVERTED// TFT tipine göre ekran1n yatay ve düsey tarama yönlerini ayarlamak gerekiyor
	TFT_WriteReg(0x36,0x68);
	#else
	TFT_WriteReg(0x36,0x28);	
	#endif
#else
	tft.dir=0;			 	 		
	TFT_WriteReg(0x36,0xC9);// Buraya da yukaridaki gibi TFT Tipine uygun olan parametreler konulmali
#endif
}

//******************************************************************
//      TFT_Fill
//          sx:
//        	sy:
//			ex:
//			ey:
//        	color:
//******************************************************************
void TFT_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{  	

	uint16_t i,j;			
	uint16_t width=ex-sx+1;
	uint16_t height=ey-sy+1;
	TFT_SetWindows(sx,sy,ex-1,ey-1);

#if TFT_USE8BIT_MODEL==1
	TFT_RS_SET;
	TFT_CS_CLR;	   
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		{
			DATAOUT(color);	
			TFT_WR_CLR;
			TFT_WR_SET;	
			
			DATAOUT(color<<8);	
			TFT_WR_CLR;
			TFT_WR_SET;	
		}   
	}
	TFT_CS_SET;	
#else
	for(i=0;i<height;i++)
	{
		for(j=0;j<width;j++)
		TFT_WR_DATA(color);	 
	}
#endif
	TFT_SetWindows(0,0,tft.width-1,tft.height-1);
}

//******************************************************************
//           TFT_DrawLine
//          x1,y1:
//        	x2,y2:
//****************************************************************** 
void TFT_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1;
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; 
	else if(delta_x==0)incx=0; 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )
	{  
		TFT_DrawPoint(uRow,uCol);
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
} 

//******************************************************************
//                        TFT_DrawRectangle
//         (x1,y1),(x2,y2):
//******************************************************************  
void TFT_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	TFT_DrawLine(x1,y1,x2,y1);
	TFT_DrawLine(x1,y1,x1,y2);
	TFT_DrawLine(x1,y2,x2,y2);
	TFT_DrawLine(x2,y1,x2,y2);
}  

//******************************************************************
//                         TFT_DrawFillRectangle
//      (x1,y1),(x2,y2):
//******************************************************************   
void TFT_DrawFillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	TFT_Fill(x1,y1,x2,y2,POINT_COLOR);

}
 
//******************************************************************
//                        _draw_circle_8
//       (xc,yc) :
// 			(x,y):
//           	c:
//******************************************************************  
void _draw_circle_8(int xc, int yc, int x, int y)
	{
	TFT_DrawPoint(xc + x, yc + y);

	TFT_DrawPoint(xc - x, yc + y);

	TFT_DrawPoint(xc + x, yc - y);

	TFT_DrawPoint(xc - x, yc - y);

	TFT_DrawPoint(xc + y, yc + x);

	TFT_DrawPoint(xc - y, yc + x);

	TFT_DrawPoint(xc + y, yc - x);

	TFT_DrawPoint(xc - y, yc - x);
}

//******************************************************************
//                        TFT_circle
//         (xc,yc) :
//         	c:
//		 	r:
//		 	fill:
//******************************************************************  
void TFT_circle(int xc, int yc,uint16_t c,int r, int fill)
{
	int x = 0, y = r, yi, d;

	d = 3 - 2 * r;
	if (fill) 
	{

		while (x <= y) {
			for (yi = x; yi <= y; yi++)
				_draw_circle_8(xc, yc, x, yi);

			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	} else 
	{

		while (x <= y) {
			_draw_circle_8(xc, yc, x, y);
			if (d < 0) {
				d = d + 4 * x + 6;
			} else {
				d = d + 4 * (x - y) + 10;
				y--;
			}
			x++;
		}
	}
}

//******************************************************************
//                          TFT_ShowBmp
//      (x,y): Resmin ekran üzerindeki pozisyonu
//      fc:  ForeColor
//			bc:  BackColor
//			bmpID: Görüntülenecek BMP nin bmpFile daki baslangiç satiri
//			width: genislik
//      height: yükseklik
//			mode: 0, ;1
//******************************************************************  
void TFT_ShowBmp(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc, const unsigned char *pBMP,uint8_t width,uint8_t height,short carpan, uint8_t mode)
{  
  int i;
	uint8_t temp;
  uint8_t numberOfChars,column;
	short reDrawPix,reDrawRow;      
  numberOfChars=height*width/8;
	
	TFT_SetWindows(x,y,x+width*carpan-1,y+height*carpan-1);  //BMP nin çizilecegi alani seç

				for(i=0;i<numberOfChars;i++) 
		{   for(reDrawRow=0;reDrawRow<carpan;reDrawRow++){ // Çizimi büyütmek için ayni satiri tekrar kullaniyoruz
			temp=*(pBMP+i);
			for(column=0;column<8;column++)  //karakterin her biti bir pixel
		    {
        for(reDrawPix=0;reDrawPix<carpan;reDrawPix++){ //Çizimi büyütmek için ayni pixeli tekrar kullaniyoruz	
		    if(temp&0x01)TFT_DrawPoint_16Bit(fc);           // raster üzerindeki bit "1" ise front color, yoksa back color ile çiz
				        else if(!mode) TFT_DrawPoint_16Bit(bc); // Mode "0" da bos pixellerde arka plani siliyoruz
				}// End loop yatay pixel tekrari
        temp=temp>>1;            // sonraki bite geç
		    } 
			} // End loop satir tekrari				
		}	
	TFT_SetWindows(0,0,tft.width-1,tft.height-1);
} 


//******************************************************************
//                          TFT_ShowChar
//      (x,y):
//      fc:  ForeColor
//			bc:  BackColor
//			num: Görüntülenecek karakter
//			size: Font Size
//			mode: 0, ;1
//******************************************************************  
void TFT_ShowChar(uint16_t x,uint16_t y,uint16_t fc, uint16_t bc, uint8_t num,uint8_t size,uint8_t mode)
{  
  uint8_t temp;
  uint8_t pos,pos2,t;
	uint16_t colortemp=POINT_COLOR;      
		   
	num=num-' '; //karakterin Font dizini içindeki indeksini hesapliyoruz
	TFT_SetWindows(x,y,x+size/2-1,y+size-1);  //Karakterin çizilecegi alani seç (genislik=fontSize/2)
	if(!mode)  // Mode "0" ise arka plani silerek üzerine yaz
	{pos2=0;
				for(pos=0;pos<size;pos++) // Font Boyutu (yani yüksekligi)ni satir satir tara
		{ if(((size==32)||(size==24))&&(pos!=(pos/2)*2)) pos2++; //iki kat büyüklükte olan fontlarda 
			                                                       //her iki satirda bir satir ilerleyek yeni piksel oku
			if(size==12)temp=asc2_1206[num][pos]; // Font ve satir seçimi (12 ve 16 fontlar ve bunlarin iki katlarini kullaniyoruz)
			else if(size==16) temp=asc2_1608[num][pos];  
			else if(size==24) temp=asc2_1206[num][pos2];  //24 lük font
			else temp=asc2_1608[num][pos2];  //32 lik font
			
			for(t=0;t<size/2;t++)  //Font'un bir satirini tara
		    {                 
		    if(temp&0x01)TFT_DrawPoint_16Bit(fc);       // raster üzerindeki bit "1" ise front color, yoksa back color ile çiz
				        else TFT_DrawPoint_16Bit(bc);       // Mode "0" da bos pixellerde arka plani siliyoruz
if((size==12)||(size==16)) temp>>=1;
       else if(t!=(t/2)*2) temp>>=1;					//2x Büyük fontlarda her iki kolonda bir yeni pixel oku
		    }			
		}	
	}else //Mode "1" ise arka plani silmeden yaziyoruz
	  {pos2=0;
		for(pos=0;pos<size;pos++)
		{if(((size==32)||(size==24))&&(pos!=(pos/2)*2)) pos2++; //iki kat büyüklükte olan fontlarda 
			                                                       //her iki satirda bir satir ilerleyek yeni piksel oku
			if(size==12)temp=asc2_1206[num][pos]; // Font ve satir seçimi (12 ve 16 fontlar ve bunlarin iki katlarini kullaniyoruz)
			else if(size==16) temp=asc2_1608[num][pos];  
			else if(size==24) temp=asc2_1206[num][pos2];  //24 lük font
			else temp=asc2_1608[num][pos2];  //32 lik font
			
			for(t=0;t<size/2;t++)
		    {   
				POINT_COLOR=fc;
				if(temp&0x01)TFT_DrawPoint(x+t,y+pos);	 
		    if((size==12)||(size==16)) temp>>=1;
               else if(t!=(t/2)*2) temp>>=1;					//2x Büyük fontlarda her iki kolonda bir yeni pixel oku
		    }
		}
	}
	POINT_COLOR=colortemp;	
	TFT_SetWindows(0,0,tft.width-1,tft.height-1);
	
} 

//******************************************************************
//
//******************************************************************  	  
void TFT_ShowString(uint16_t x,uint16_t y,uint8_t size,uint8_t *p,uint8_t mode)
{         
    while((*p<='~')&&(*p>=' '))  // Basilamayan (non-printable char) ise görüntülemeye son ver 
    {   
		if(x>(tft.width-1)||y>(tft.height-1))return; //Ekrana sigmiyorsa sonlandir
			     
        TFT_ShowChar(x,y,POINT_COLOR,BACK_COLOR,*p,size,mode);
        x+=size/2;  //Font genisligi kadar ilerle
        p++; // siradaki karaktere geç
    }  
} 

//******************************************************************
//
//******************************************************************
/*void TFT_ShowNum(uint16_t x,uint16_t y,uint8_t size,uint16_t number,uint8_t mode)
{unsigned char buffer[5];
	itoa(number,buffer,10);
	TFT_ShowString(x,y,size,buffer,mode);
} 	
*/

//******************************************************************
//
//******************************************************************  
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}

//******************************************************************
//
//******************************************************************  			 
void TFT_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				TFT_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	TFT_ShowChar(x+(size/2)*t,y,POINT_COLOR,BACK_COLOR,temp+'0',size,0); 
	}
} 


//******************************************************************
//  TFT_Drawbmp16
//******************************************************************  
void TFT_Drawbmp16(uint16_t x,uint16_t y,const unsigned char *p)
{
  	int i; 
	unsigned char picH,picL; 
	TFT_SetWindows(x,y,x+40-1,y+40-1);
    for(i=0;i<40*40;i++)
	{	
	 	picL=*(p+i*2);
		picH=*(p+i*2+1);				
		TFT_DrawPoint_16Bit(picH<<8|picL);  						
	}	
	TFT_SetWindows(0,0,tft.width-1,tft.height-1);

}


