/******************************************************************/
/* 名称：main                                                    */
/* 效果：                                                        */
/* 内容：                                                       */
/* 作者：zhan                                                  */
/* 联系方式QQ:363116119                                        */
/******************************************************************/
#include "my_register.h"
#include "stm32f4xx.h"
#include "clock.h"
#include "tim6.h"
#include "tim1.h"
#include "tim4.h"
#include "adc.h"
#include "clock.h"
#include "gpio.h"
#include "bsp_SysTick.h"
#include "usart.h"
#include "ssd1963.h"
#include "iwdg.h"
#include "MainTask.h"
#include "IIC_24C01.h"
#include "flash.h"
#include "tm1650.h"
#include "ad5541.h"
#include "ad7689.h"
#include "usart.h"

struct bitDefine
{
    unsigned bit0: 1;
    unsigned bit1: 1;
    unsigned bit2: 1;
    unsigned bit3: 1;
    unsigned bit4: 1;
    unsigned bit5: 1;
    unsigned bit6: 1;
    unsigned bit7: 1;
} flagA, flagB,flagC,flagD,flagE,flagF,flagG;

int main(void)
	{
		u8 i;
	 int t=0;
	u16 len;	
	u8 datatemp[30];
	u8 len1 =0;
		
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);//开启CRC时钟，STEMWIN授权使用
	RCC_Configuration();
	SysTick_Init();
	GPIO_Conf();

    abc_time = 0;
    ms_time = 0;

	TIM1_PWM_Config();
	TIM4_PWM_Config();
	TIM3_Int_Init(10-1,8400-1);
	TIM6_Config();
	ADC1_DMA_Init();
//	USART_Configuration();
	USART2_Configuration();
	i2c_CfgGpio();//24C01引脚初始化
	TM1650_GPIO_INT();		/*TM1650引脚初始化 */
	AD5541_GPIOCoing();//DAC初始化
	AD7689_InitializeSPI1();//AD7689初始化
//	LCD_Initializtion();//液晶屏初始化
	//EEPROM_READ_Coeff();//读取校准参数
	RCONT_SW = 0;
	Read_Flash_Bytes();
	Off_GPOI_ResetSet();
	LOAD_MODE = 0;//负载CC模式
	LOAD_V_SW = 1;
	LOAD_I_SW = 0;
	GPIO_SetBits(GPIOC,GPIO_Pin_1);//OFF
	Flag_Swtich_ON = 0;
	flag_CC_MODE = 1;
	while(1)
	{
		if(USART_RX_STA&0x8000)	   //判断是否接收完数据
		{					   

//			len1=USART_RX_STA&0x3f;
			len1=USART_RX_CNT;
			len = SerialRemoteHandleL(len1,USART_RX_BUF);
			len1 =0;
			for(t=0;t<len;t++)								  //通过长度进行循环
			{
				USART_SendData(USART2, USART_RX_BUF[t]);      //将数组采集好的数据 一次向串口1发送
				while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
			}
			memset(USART_RX_BUF,0,USART_REC_LEN);
			
			USART_RX_STA=0;			  //清零 虚拟寄存器		  为一下次采集做准备
		}
		TIM_SetCompare3(TIM4,Contr_Voltage);//稳压电源电流DAC
		TIM_SetCompare4(TIM4,Contr_Current);//稳压电源电压DAC
		AD5541_Send(Contr_Laod);//加载DAC值
		MODE_CONT();//电压电流档位切换
		if(Flag_DAC_OFF==0)
		{
			Transformation_ADC();
		}
		if(Flag_ADC_Full==1)
		{
			Flag_ADC_Full=0;
			ADC_CH_Scan();//ADC滤波
		}
		AD7689_Scan_CH();//读取AD7689各通道AD值
	}
	//IWDG_Inte();
//	MainTask();
}


/******************* (C) COPYRIGHT 2017 STMicroelectronics *****END OF FILE****/
