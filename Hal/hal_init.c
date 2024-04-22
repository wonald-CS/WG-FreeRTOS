#include "hal_led.h"
#include "hal_usart.h"
#include "hal_init.h"
#include "hal_GPIO.h"
#include "hal_flash.h"
#include "hal_tftlcd.h"
#include "hal_wTn6.h"
#include "hal_timer.h"
#include "hal_al6630.h"
#include "hal_key.h"
#include "hal_adc.h"



void Hal_Init(void)
{
	hal_LedInit();	
	Hal_Usart_Init();
	hal_Gpio_Init();
	hal_spi2Init();
	hal_tftlcd_Init();
	LCD_Open_Init();
	hal_Wtn6_Init();
	hal_timer_Init();
	hal_Al6630_Init();
	hal_Key_Init();
    hal_AdcInit();
}
