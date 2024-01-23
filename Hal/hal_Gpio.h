#ifndef ____HAL_GPIO_H_
#define ____HAL_GPIO_H_
 
#define CHECK_ACSTATE_PORT       GPIOB
#define CHECK_ACSTATE_PIN        GPIO_Pin_1

typedef enum
{
	STA_AC_BREAK = 0,
	STA_AC_LINK,
}en_AcLinkSta;

void hal_Gpio_Init(void);
en_AcLinkSta hal_Gpio_AcStateCheck(void);


void hal_GPIO_WIFIPowerEN_H(void);
void hal_GPIO_WIFIPowerEN_L(void);

void hal_GPIO_4GPowerKey_H(void);
void hal_GPIO_4GPowerKey_L(void);


#endif
//////

