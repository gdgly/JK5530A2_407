/******************************************************************/
/* ���ƣHTIM2 PWM                                                */
/* Ч����                                                        */
/* ���ݣ�����һ��200HZ ��ռ�ձȣ�60.9% ��ռ�ձȣ�30.9%��PWM      */
/* ���ߣ�����                                                    */
/* ��ϵ��ʽ��QQ:363116119                                        */
/******************************************************************/
#include "my_register.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
/*****************************************************************/
extern struct bitDefine
{
	unsigned bit0: 1;
	unsigned bit1: 1;
	unsigned bit2: 1;
	unsigned bit3: 1;
	unsigned bit4: 1;
	unsigned bit5: 1;
	unsigned bit6: 1;
	unsigned bit7: 1;
} flagA,flagB,flagC,flagD;

vu32 abc_time;
vu32 ms_time;
/*****************************************************************/
void TIM4_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����GPIO�ṹ��
  /* ����GPIO�ܽŸ���*/
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_TIM4);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_TIM4);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//���������뺯��
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);//���������뺯��
}
/*****************************************************************/
void TIM4_PWM_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	/* TIM3 ������ ---------------------------------------------------
   TIM3 ����ʱ��(TIM3CLK) ����Ϊ APB2 ʱ�� (PCLK2)    
    => TIM3CLK = PCLK2 = SystemCoreClock
   TIM3CLK = SystemCoreClock, Prescaler = 0, TIM3 counter clock = SystemCoreClock
   SystemCoreClock Ϊ48 MHz */
  /* TIM4 ʱ��ʹ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	TIM4_Config();

  /* Time ��ʱ��������*/
  TIM_TimeBaseStructure.TIM_Prescaler = 6;//ʱ��Ԥ��Ƶ
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /* Time ��ʱ����Ϊ�����ؼ���ģʽ*/
  TIM_TimeBaseStructure.TIM_Period = 0;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

  TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);

  /* Ƶ��1��PWM ģʽ���� */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_Pulse = 0x0f;//ʹ��Ƶ��1����
  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
	
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_SetAutoreload(TIM4, 0x7530);//设置PWM分辨�?
	TIM_ARRPreloadConfig(TIM4, ENABLE);
  /* TIM4 使能*/
  TIM_Cmd(TIM4, ENABLE);
	
  /* TIM4 开始输�?*/
  TIM_CtrlPWMOutputs(TIM4, ENABLE);	
}
/**************************************************************************************/
void Temp_Comapre(void)	  //�¶������Ʒ���
{
	if(NTC_value<=1200)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_12);//��������
		flag_FAN_ON=1;
	}
	else if(NTC_value>=1300)
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_12);//�رշ���
		flag_FAN_ON=0;
	}
}

void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //ʹ�ö�ʱ��֮ǰ�����뿪����ʱ��ʱ�ӣ�������ʱ������APB1 �������衣
    //����Time x Clk�� x
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
   
		/*  �ۼ� TIM_Period �������һ�����»����ж�  */
    //  ����ʱ����0 ������4999����Ϊ5000 �Σ�Ϊһ����ʱ����
    TIM_TimeBaseInitStructure.TIM_Period = arr;  

    //��ʱ��ʱ��ԴTIMxCLK = 2 * PCLK1
    // PCLK1 = HCLK / 4
    // => TIMxCLK=HCLK/2=SystemCoreClock/2=90MHz
    // �趨��ʱ��Ƶ��Ϊ=TIMxCLK/(TIM_Prescaler+1)=10000Hz   
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc; 

    //������ʽ
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 

		//����ʱ�ӷ�Ƶ
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;

		//��ʼ����ʱ��TIMx, x[1,8]
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);

	  //����ʱ��x �����ж�
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 

		//��ʱ��x �ж�
    NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn;   

		// ������ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01; 

		// ���������ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;   

		/* ʹ���ж�ͨ�� */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

		// ʹ�ܶ�ʱ��
    TIM_Cmd(TIM3,ENABLE); 
}

void TIM3_IRQHandler(void){
	
	float ftemp = 0;
	float step_c = 0;
	int step_time = 0;
	
    if(TIM_GetITStatus(TIM3,TIM_IT_Update) == SET) 
    {
			
		if( ms_time > (1 << 31) - 2 )
			
			ms_time = 0;
		
		if( abc_time > (1 << 31) - 2 )
			
			abc_time = 0;
			
		
		switch(FLAG_WIN){
			
			//�̿ص�Դ
			case 0:{
				
				
			}
			break;
			
			//�������
			case 1:{
				
				if( trig_flag_state == 1 && C_DISCHARGE == 1 && abc_time > 1 ){		
					
					step_c =  (float)CLists.IR_Step_C / 1000 ;
					
					step_time = (int)CLists.IR_Step_Time;
					
					if( step_time < 1 ){
						
						step_time = 1;
					}
					
					if( old_I != 0 ){
						
						if( /*ms_time%step_time == 0 &&  DISS_Voltage * DISS_Current > 200 && */DISS_Current < old_I/* && (v_memory - DISS_Voltage >  v_memory * 0.5)*/){
							
							trig_flag_state = 0;
							Off_GPOI_ResetSet();  
							CLists.CSET_Current_Laod =  CLists.IR_Start_C;
						}	
					}								
					
					if( trig_flag_state != 0 )
					{						
						if( DISS_Current > old_I )
								
							old_I = DISS_Current;

						if( ms_time%step_time == 0)
						{
								
							max_discahrgeNum += step_c;
									
							CLists.CSET_Current_Laod = (int)(max_discahrgeNum * 1000);
						}
					}				
				}
	
			}
			break;
			
			//�̿ظ���	
			case 2:{
				
				
			}
			break;
			
			//��ŵ�
			case 3:{
				
				if( ms_time%1000 == 0 && ms_time > 999 ){
			
					if( C_DISCHARGE == 1 ){
							
						ftemp = DISS_Current * 1000 * 1/3600;
							
					}else{
							
						ftemp = DISS_POW_Current * 1000 * 1/3600;
					}
					
					CDC_CsumMah += ftemp ;
				}
			}
			break;
			//����ͼ
			case 4:{
				
				
			}
			break;
			//ϵͳ����
			case 5:{
				
				
			}	
			break;
			
			//ϵͳ��Ϣ
			case 6:{
				
				
			}  	  
			break;
		}
		
		if( ms_time % 1000 == 0 && ms_time > 999 )
			
			abc_time++; 
	
		ms_time++;
		
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	}
	  
}




