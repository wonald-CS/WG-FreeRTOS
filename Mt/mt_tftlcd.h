#ifndef ____MT_TFTLCD_H_
#define ____MT_TFTLCD_H_


void LCD_Fill(unsigned short xsta,unsigned short ysta,unsigned short xend,unsigned short yend,unsigned short color);
void Mt_Tftlcd_Clear(void);
void LCD_ShowString(unsigned short x,unsigned short y,const unsigned char *p,unsigned short fc,unsigned short bc,unsigned char sizey,unsigned char mode);
void LCD_ShowPicture32PixFont(unsigned short x,unsigned short y,unsigned char num,unsigned short fc,unsigned short bc,unsigned char mode);
#endif
