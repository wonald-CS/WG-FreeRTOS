#include "mt_tftlcd.h"
#include "hal_tftlcd.h"

unsigned char ColorBuf[640];


/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2)
{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1+2);
		LCD_WR_DATA(x2+2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1+1);
		LCD_WR_DATA(y2+1);
		LCD_WR_REG(0x2c);//储存器写
}


/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
								color       要填充的颜色
      返回值：  无
******************************************************************************/

void LCD_Fill(unsigned short xsta,unsigned short ysta,unsigned short xend,unsigned short yend,unsigned short color)
{          
	unsigned short i; 
	LCD_Address_Set(xsta,ysta,xend-1,yend-1);//设置显示范围
	for(i=0;i<xend;i++)
  {
		ColorBuf[i++] = color>>8;
		ColorBuf[i] = color;
	}
	for(i=ysta;i<yend*2;i++)
	{		
		 DMA_SPI3_TX(ColorBuf,xend);
	}	
}
