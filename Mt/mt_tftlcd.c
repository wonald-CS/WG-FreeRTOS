#include "mt_tftlcd.h"
#include "hal_tftlcd.h"
#include "lcdfont.h"


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

/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/
static void LCD_DrawPoint(unsigned short x,unsigned short y,unsigned short color)
{
	LCD_Address_Set(x,y,x,y);//设置光标位置 
	LCD_WR_DATA(color);
} 



///////////////////////////////////////////
/******************************************************************************
      函数说明：显示单个字符
      入口数据：x,y显示坐标
                num 要显示的字符
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChar(unsigned short x,unsigned short y,unsigned char num,unsigned short fc,unsigned short bc,unsigned char sizey,unsigned char mode)
{
	unsigned char temp,sizex,t,m=0;
	unsigned short i,TypefaceNum;//一个字符所占字节大小
	unsigned short x0=x;
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	num=num-' ';    //得到偏移后的值
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //设置光标位置 
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==12)temp=ascii_1206[num][i];		       //调用6x12字体
		else if(sizey==16)temp=ascii_1608[num][i];		 //调用8x16字体
		else if(sizey==24)temp=ascii_2412[num][i];		 //调用12x24字体
		else if(sizey==32)temp=ascii_3216[num][i];		 //调用16x32字体
		else if(sizey==48)temp=ascii_4824[num][i];		 //调用24x48字体	
		else return;
		for(t=0;t<8;t++)
		{
			if(!mode)//非叠加模式
			{
				if(temp&(0x01<<t))LCD_WR_DATA(fc);
				else LCD_WR_DATA(bc);
				m++;
				if(m%sizex==0)
				{
					m=0;
					break;
				}
			}
			else//叠加模式
			{
				if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);//画一个点
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
	}   	 	  
}

/******************************************************************************
      函数说明：显示字符串
	  入口数据：x，y显示坐标
	  			*p 要显示的字符串
				fc 字的颜色
				bc 字的背景色
				mode： 0非叠加模式  1叠加模式
	   返回值：无
******************************************************************************/
void LCD_ShowString(unsigned short x,unsigned short y,const unsigned char *p,unsigned short fc,unsigned short bc,unsigned char sizey,unsigned char mode)
{         
	while(*p!='\0')
	{       
		LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x+=sizey/2;
		p++;
	}  
}


/******************************************************************************
      函数说明：小图标显示     32*32
      入口数据：x,y显示坐标       
                num 要显示的图标
                fc 图标的颜色
                bc 图标背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD_ShowPicture32PixFont(unsigned short x,unsigned short y,unsigned char num,unsigned short fc,unsigned short bc,unsigned char mode)
{
	unsigned char temp,t;
	unsigned short i,TypefaceNum;////一个图标所占字节大小	
	TypefaceNum=128;
	
	LCD_Address_Set(x,y,x+31,y+31);   //设置光标位置    
	
	for(i=0;i<TypefaceNum;i++)
	{ 
		temp=gImage_wifiSigal[num][i];
		///0X70  0111 0000 
		for(t=0;t<8;t++)
		{
			if(!mode)//非透明模式
			{ 
				if(temp&(0x80>>t))
					LCD_WR_DATA(fc);
				else 
					LCD_WR_DATA(bc);
			}
		}
	}   	 	  
}



