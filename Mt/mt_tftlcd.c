#include "mt_tftlcd.h"
#include "hal_tftlcd.h"

unsigned char ColorBuf[640];


/******************************************************************************
      ����˵����������ʼ�ͽ�����ַ
      ������ݣ�x1,x2 �����е���ʼ�ͽ�����ַ
                y1,y2 �����е���ʼ�ͽ�����ַ
      ����ֵ��  ��
******************************************************************************/
void LCD_Address_Set(unsigned short x1,unsigned short y1,unsigned short x2,unsigned short y2)
{
		LCD_WR_REG(0x2a);//�е�ַ����
		LCD_WR_DATA(x1+2);
		LCD_WR_DATA(x2+2);
		LCD_WR_REG(0x2b);//�е�ַ����
		LCD_WR_DATA(y1+1);
		LCD_WR_DATA(y2+1);
		LCD_WR_REG(0x2c);//������д
}


/******************************************************************************
      ����˵������ָ�����������ɫ
      ������ݣ�xsta,ysta   ��ʼ����
                xend,yend   ��ֹ����
								color       Ҫ������ɫ
      ����ֵ��  ��
******************************************************************************/

void LCD_Fill(unsigned short xsta,unsigned short ysta,unsigned short xend,unsigned short yend,unsigned short color)
{          
	unsigned short i; 
	LCD_Address_Set(xsta,ysta,xend-1,yend-1);//������ʾ��Χ
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
