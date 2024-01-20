#include "hal_led.h"
#include "hal_usart.h"
#include "hal_init.h"
#include "hal_GPIO.h"
#include "hal_flash.h"



void Hal_Init(void)
{
	hal_LedInit();	
	Hal_Usart_Init();
	hal_Gpio_Init();
	hal_spi2Init();
}
