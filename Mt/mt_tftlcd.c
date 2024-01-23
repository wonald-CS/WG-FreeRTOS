#include "mt_tftlcd.h"
#include "hal_tftlcd.h"
#include "lcdfont.h"


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

/******************************************************************************
      ����˵������ָ��λ�û���
      ������ݣ�x,y ��������
                color �����ɫ
      ����ֵ��  ��
******************************************************************************/
static void LCD_DrawPoint(unsigned short x,unsigned short y,unsigned short color)
{
	LCD_Address_Set(x,y,x,y);//���ù��λ�� 
	LCD_WR_DATA(color);
} 



///////////////////////////////////////////
/******************************************************************************
      ����˵������ʾ�����ַ�
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾ���ַ�
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChar(unsigned short x,unsigned short y,unsigned char num,unsigned short fc,unsigned short bc,unsigned char sizey,unsigned char mode)
{
	unsigned char temp,sizex,t,m=0;
	unsigned short i,TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	unsigned short x0=x;
	sizex=sizey/2;
	TypefaceNum=(sizex/8+((sizex%8)?1:0))*sizey;
	num=num-' ';    //�õ�ƫ�ƺ��ֵ
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //���ù��λ�� 
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==12)temp=ascii_1206[num][i];		       //����6x12����
		else if(sizey==16)temp=ascii_1608[num][i];		 //����8x16����
		else if(sizey==24)temp=ascii_2412[num][i];		 //����12x24����
		else if(sizey==32)temp=ascii_3216[num][i];		 //����16x32����
		else if(sizey==48)temp=ascii_4824[num][i];		 //����24x48����	
		else return;
		for(t=0;t<8;t++)
		{
			if(!mode)//�ǵ���ģʽ
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
			else//����ģʽ
			{
				if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);//��һ����
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
      ����˵������ʾ�ַ���
	  ������ݣ�x��y��ʾ����
	  			*p Ҫ��ʾ���ַ���
				fc �ֵ���ɫ
				bc �ֵı���ɫ
				mode�� 0�ǵ���ģʽ  1����ģʽ
	   ����ֵ����
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
      ����˵����Сͼ����ʾ     32*32
      ������ݣ�x,y��ʾ����       
                num Ҫ��ʾ��ͼ��
                fc ͼ�����ɫ
                bc ͼ�걳��ɫ
                sizey �ֺ�
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowPicture32PixFont(unsigned short x,unsigned short y,unsigned char num,unsigned short fc,unsigned short bc,unsigned char mode)
{
	unsigned char temp,t;
	unsigned short i,TypefaceNum;////һ��ͼ����ռ�ֽڴ�С	
	TypefaceNum=128;
	
	LCD_Address_Set(x,y,x+31,y+31);   //���ù��λ��    
	
	for(i=0;i<TypefaceNum;i++)
	{ 
		temp=gImage_wifiSigal[num][i];
		///0X70  0111 0000 
		for(t=0;t<8;t++)
		{
			if(!mode)//��͸��ģʽ
			{ 
				if(temp&(0x80>>t))
					LCD_WR_DATA(fc);
				else 
					LCD_WR_DATA(bc);
			}
		}
	}   	 	  
}



