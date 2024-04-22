#include "stm32F10x.h"
#include "hal_adc.h"


void hal_AdcInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO, ENABLE);
  
	//PB0-电池电量检测 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


////设置ADC分频因子6 72M/8=9,ADC最大时间不能超过14M
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);

	/* ADC1 configuration ------------------------------------------------------*/
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;		// 独立工作模式;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;				//多通道扫描模式;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;		//连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	// 转换触发方式：转换由软件触发启动;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC 数据右对齐 ;
	ADC_InitStructure.ADC_NbrOfChannel = 2;					//进行规则转换的 ADC 通道的数目为3; 
	ADC_Init(ADC1, &ADC_InitStructure);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);

	/* 复位 ADC1 的校准寄存器 */	
	ADC_ResetCalibration(ADC1);

	/* 等待 ADC1 校准寄存器复位完成 */	
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* 开始 ADC1 校准 */	
	ADC_StartCalibration(ADC1);

	/* 等待 ADC1 校准完成 */	
	while(ADC_GetCalibrationStatus(ADC1));
	 
	/* 启动 ADC1 转换 */ 
	//ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}



