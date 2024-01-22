#ifndef ____APP_TFTLCD_H_
#define ____APP_TFTLCD_H_


#define LCD_W 320
#define LCD_H 240




///RGB565  
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE           	 0x001F  
#define BRED             0XF81F
#define GRED 		   		 	 0XFFE0
#define GBLUE	        	 0X07FF
#define RED              0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			     0XBC40 //棕色
#define BRRED 			     0XFC07 //棕红色
#define GRAY  			     0X8430 //灰色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			     0XC618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)


#define COOR_ICON_AC_X   1
#define COOR_ICON_AC_Y   1

#define COOR_ICON_BAT_X   33
#define COOR_ICON_BAT_Y   1

#define  COOR_ICON_SERVER_X   210
#define  COOR_ICON_SERVER_Y    1

#define  COOR_ICON_WIFI_X   250
#define  COOR_ICON_WIFI_Y    1

#define  COOR_ICON_SIM_X   282
#define  COOR_ICON_SIM_Y    1

#define  COOR_ICON_SYSTEMODE_X   70
#define  COOR_ICON_SYSTEMODE_Y   100


enum
{
	FORTSIZE_12 = 12,
	FORTSIZE_16 = 16,	
	FORTSIZE_24 = 24,
	FORTSIZE_32 = 32,	
	FORTSIZE_48 = 48,	
};

#define HUE_LCD_FONT     WHITE
#define HUE_LCD_BACK     BLACK//YELLOW  //BLACK//
#define HUE_FONT_BACK    GRAY


void TftLcd_Task(void *pvParameters);
#endif

