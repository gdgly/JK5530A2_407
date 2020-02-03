/******************** (C) COPYRIGHT 2014 KUNKIN********************
 * �ļ���  ��my_register
 * ����    ������
 * ����    ���⺯����
 * ����    ��Define register����ϵͳʹ�õļĴ�����ȫ�ֱ���
 * ע��		 ��ͷ�ļ���ֻ���������������ж���
 * �޸����ڣ�2014-12-9
*********************************************************************/

#include "STM32F4xx.h"
#ifndef __my_register_h__
#define __my_register_h__
//=============================================================================

//=============================================================================
#define NOP   __nop();	  //��ָ��

//============================================================================
extern struct bitDefine flagA,flagB,flagC,flagD,flagE,flagF,flagG;
#define   Flag_ADC_Full             (flagA.bit1)//DMA������ɱ�־λ 
#define   Flag_BEEP						    	(flagA.bit2)//������������־
#define   Flag_Key_Scan							(flagA.bit3)//������ʱɨ���־
#define   UART_SEND_flag						(flagA.bit4)//���ڷ�����ɱ�־
#define   UART_Buffer_Rece_flag			(flagA.bit5)//���ڽ�����ɱ�־
#define   Flag_Swtich_ON      			(flagA.bit6)//������������ON��־
#define   Flag_0_5S      		      	(flagA.bit7)//��ʾ���õ�ѹ��˸��־

#define   Flag_1S      		         	(flagB.bit0)//��ʾ���õ�����˸��־
#define   Flag_5S      		        	(flagB.bit1)//�˳�����״̬��־
#define   Flag_Long                 (flagB.bit2)//����������־
#define   Flag_DAC_OFF              (flagB.bit3)//是否转换DAC控制值标�?
#define   Flag_SetA                 (flagB.bit5)//�������õ�����־λ
#define   Flag_SetV                 (flagB.bit6)//�������õ�ѹ��־λ 
#define   Flag_DisSet               (flagB.bit7)//��ʾ���õ�ѹ�͵�����־λ

#define   Flag_Beep_OFF             (flagC.bit6)//�������رձ�־

#define   flag_Tim_USART            (flagD.bit0)//���ڼ��������־
#define   flag_Test                 (flagD.bit1)//�Զ�������ͣ��־
#define   flag_Test_PASS            (flagD.bit2)//�Զ�����PASS��־
#define   flag_Test_FAIL            (flagD.bit3)//�Զ�����FAIL��־
#define   flag_Test_Time            (flagD.bit4)//�Զ����Եȴ�ʱ���־
#define   flag_Test_Start           (flagD.bit5)//�Զ����ԱȽϿ�ʼ��־
#define   flag_Test_Min             (flagD.bit6)//�Զ��������ޱ�־
#define   flag_Load_CC              (flagD.bit7)//����CC/CVģʽ��־λ

#define   flag_Test_Door             (flagE.bit0)//�Զ������ż���־
#define   flag_Test_SetTime          (flagE.bit1)//�Զ�����ʱ���־ 
#define   flag_OVP                   (flagE.bit2)//OVP��־
#define   flag_OCP                   (flagE.bit3)//OCP��־
#define   flag_Doub                  (flagE.bit4)//����˫��
#define   flag_t_Doub                (flagE.bit5)//����˫����ʱ��־
#define   flag_Time_SW               (flagE.bit6)//��ʱ���ܿ�����־
#define   flag_CC_MODE               (flagE.bit7)//����ģʽ��־

#define   flag_Adjust                (flagF.bit0)//
#define   flag_ADJ_ON                (flagF.bit1)//
#define   flag_ADJ_VL                (flagF.bit2)//
#define   flag_ADJ_VH                (flagF.bit3)//
#define   flag_ADJ_ALCC              (flagF.bit4)//
#define   flag_ADJ_AHCC              (flagF.bit5)//
#define   flag_ADJ_ALCV              (flagF.bit6)//
#define   flag_ADJ_AHCV              (flagF.bit7)//

#define   flag_DisVL                 (flagG.bit0)//
#define   flag_OverV                 (flagG.bit1)//
#define   flag_OverACC               (flagG.bit2)//
#define   flag_OverACV               (flagG.bit3)//
#define   flag_Just_Usart            (flagG.bit4)//
#define   flag_FAN_ON                (flagG.bit5)//
#define   flag_ADJ_FAN               (flagG.bit6)//
#define   flag_TM7707_CH						 (flagG.bit7)//
//=============================================================================
extern vu32 GE_V;
extern vu32 SHI_V;
extern vu32 BAI_V;
extern vu32 QIAN_V;  //�����ɨ�����
extern vu32 GE_A;
extern vu32 SHI_A;
extern vu32 BAI_A;
extern vu32 QIAN_A;

//--------------------------
extern vu8 NewKey;
extern vu8 DAC_Flag;
extern vu8 Beep_Flag;
extern vu8 t_beep;
extern vu8 t_KeyScan;
extern vu8 t_LEDScan;  //��ʱ������
extern vu8 t_KEYON;
extern vu16 t_0_5S;
extern vu16 t_1S;
extern vu16 t_5S;
extern vu16 t_Basket;
extern vu16 Basket_V;
extern vu16 Basket_A;
extern vu16 t_OFF_V;
extern vu16 t_ON_V;
extern vu16 t_OFF_A;
extern vu16 t_ON_A;
extern vu8 t_USART;
extern vu16 ADJ_Write;//У׼ʱ��д���ʵ��ֵ
extern vu8 OFFSET_Zore;//���ֵ
extern vu8 ADDR;
extern vu8 t_lock;
extern vu8 t_onoff;
extern vu16 MAX_limit;
extern vu16 MIN_limit;
extern vu16 Test_Time;
extern vu16 Test_Time1;
extern vu16 Time_sw;
extern vu16 Test_Daley;
extern vu16 Test_C_Door;
extern vu16 TEMP_OFFSET;//�¶Ȳ���ֵ
//--------------------------
extern vu8 UART_Buffer_Size;//���ڽ������ݳ���
extern vu8 Transmit_BUFFERsize;
//---------------------------
extern vu16 NTC_value;   //ADֵ
extern vu16 Vmon_POWE_value;//��Դ��ѹ
extern vu16 Imon_POWE_value;//��Դ����
extern vu16 Imon_Load_value;//���ص���
extern vu16 Vmon_Load_value;//���ص�ѹ
extern vu16 Rmon_value;//����
extern vu16 R1mon_value;//ʶ�����1
extern vu16 R2mon_value;//ʶ�����2

extern float DISS_Voltage;
extern float DISS_POW_Voltage;
extern float DISS_Current;
extern float DISS_POW_Current;
extern float DISS_R;

extern vu32 Run_Control[17];	
#define onoff_Load                    Run_Control[0]  //ON/OFF
#define onoff_POWE                    Run_Control[1]  //ON/OFF
#define SET_Voltage 	    	          Run_Control[2]  //��Դ�趨��ѹ
#define SET_Current	    	    		    Run_Control[3]  //��Դ�趨����
#define SET_Voltage_Laod 					    Run_Control[4]  //�����趨��ѹ
#define SET_Current_Laod					    Run_Control[5]  //�����趨����
#define LOAD_MODE								      Run_Control[6]  //���ع���ģʽ
#define LOAD_V_SW								      Run_Control[7]  //���ص�ѹ��λ
#define LOAD_I_SW							        Run_Control[8]  //���ص�����λ
#define RCONT_SW							   	    Run_Control[9]  //���������λ

#define POW_Voltage	    	            Run_Control[10]  //��Դ������ѹ
#define Voltage	    	                Run_Control[11]  //���ز�����ѹ
#define Current	    	    		        Run_Control[12]  //��Դ��������
#define Laod_Current	    		        Run_Control[13]  //���ز�������
#define R_VLUE											  Run_Control[14]  //�������ֵ
#define R1_VLUE											  Run_Control[15]  //ʶ�����1
#define R2_VLUE											  Run_Control[16]  //ʶ�����2

extern vu16 Contr_Voltage;//
extern vu16 Contr_Current;//
extern vu16 Contr_Laod;//
extern vu32 Correct_Parametet[17];
#define  REG_Load_CorrectionV_LOW  			  Correct_Parametet[0]//�����ѹ���������͵�
#define  REG_Load_CorrectionV_HIG   			Correct_Parametet[1]//�����ѹ���������ߵ�
#define  REG_Load_CorrectionA_LOW			    Correct_Parametet[2]//����CCģʽ�������������͵�
#define  REG_Load_CorrectionA_HIG		      Correct_Parametet[3]//����CCģʽ�������������ߵ�
#define  SET_Load_CorrectionV_LOW			    Correct_Parametet[4]//����CVģʽ���Ƶ�ѹ�͵�
#define  SET_Load_CorrectionV_HIG			    Correct_Parametet[5]//����CVģʽ���Ƶ�ѹ�ߵ�
#define  SET_Load_CorrectionA_LOW			    Correct_Parametet[6]//����CCģʽ���Ƶ����͵�
#define  SET_Load_CorrectionA_HIG			    Correct_Parametet[7]//����CCģʽ���Ƶ����ߵ�

#define  REG_POWER_CorrectionV			      Correct_Parametet[8]//��Դ������ѹ����
#define  REG_POWER_CorrectionA_CC			    Correct_Parametet[9]//��ԴCCģʽ������������
#define  REG_POWER_CorrectionA_CV			    Correct_Parametet[10]//��ԴCVģʽ������������
#define  SET_POWER_CorrectionV 			      Correct_Parametet[11]//��Դ��ѹ���Ʋ���
#define  SET_POWER_CorrectionA 			      Correct_Parametet[12]//��Դ�������Ʋ���

#define  REG_CorrectionR_LOW              Correct_Parametet[13]//������������͵�
#define  REG_CorrectionR_HIG              Correct_Parametet[14]//������������ߵ�

#define  REG_CorrectionR1                 Correct_Parametet[15]//ʶ�����1����
#define  REG_CorrectionR2                 Correct_Parametet[16]//ʶ�����2����

extern vu32 Correct_Strong[17];//У׼ϵ��
#define  REG_LoadV_Offset_LOW             Correct_Strong[0]//�����ѹ��������
#define  REG_LoadV_Offset_HIG             Correct_Strong[1]//�����ѹ��������
#define  REG_LoadA_Offset_LOW             Correct_Strong[2]//����CCģʽ������������
#define  REG_LoadA_Offset_HIG             Correct_Strong[3]//����CCģʽ������������
#define  SET_LoadV_Offset_LOW             Correct_Strong[4]//����CVģʽ���Ƶ�ѹ����
#define  SET_LoadV_Offset_HIG  					  Correct_Strong[5]//����CVģʽ���Ƶ�ѹ����
#define  SET_LoadA_Offset_LOW   					Correct_Strong[6]//����CCģʽ���Ƶ�������
#define  SET_LoadA_Offset_HIG   					Correct_Strong[7]//����CCģʽ���Ƶ�������

#define  REG_POWERV_Offset                Correct_Strong[8]//��ԴCVģʽ��ѹ��������
#define  REG_POWERA_Offset_CC             Correct_Strong[9]//��ԴCCģʽ������������
#define  REG_POWERA_Offset_CV             Correct_Strong[10]//��ԴCCģʽ������������
#define  SET_POWERV_Offset	              Correct_Strong[11]//��ԴCVģʽ��ѹ���Ʋ���
#define  SET_POWERA_Offset                Correct_Strong[12]//��ԴCCģʽ�������Ʋ�������

#define  REG_ReadR_Offset_LOW             Correct_Strong[13]//�����������
#define  REG_ReadR_Offset_HIG             Correct_Strong[14]//�����������

#define  REG_ReadR1_Offset             Correct_Strong[15]//�����������
#define  REG_ReadR2_Offset             Correct_Strong[16]//�����������

extern vu8  correct_por[8];
#define  Polar0            correct_por[0]//���ص�ѹ�����Ϳ���ϵ��
#define  Polar1            correct_por[1]//���ص��������Ϳ���ϵ��
#define  Polar2						 correct_por[2]//��Դ��ѹ���ƺͲ���ϵ��
#define  Polar3						 correct_por[3]//��Դ���������Ϳ���ϵ��
#define  Polar4						 correct_por[4]//��������ߵ͵�ϵ��
#define  Polar5						 correct_por[5]//ʶ��������ϵ��
#define  Polar6						 correct_por[6]//ʶ��������ϵ��
#define  Polar7						 correct_por[7]//ʶ��������ϵ��

//---------------------------
extern vu8 UART_Buffer_Rece[16];
extern vu8 UART_Buffer_Send[20];

typedef struct {
	
	vu32 Conoff_Load;                	//ON/OFF
	vu32 Conoff_POWE;               	//ON/OFF  
	vu32 CSET_Voltage; 	    	   		//��Դ�趨��ѹ     
	vu32 CSET_Current;	    	   		//��Դ�趨���� 		   
	vu32 CSET_Voltage_Laod; 			//�����趨��ѹ
	vu32 CSET_Current_Laod;				//�����趨����	   
	vu32 CLOAD_MODE;					//���ع���ģʽ		    
	vu32 CLOAD_V_SW;					//���ص�ѹ��λ		     
	vu32 CLOAD_I_SW;					//���ص�����λ	       
	vu32 CRCONT_SW;						//���������λ   	    

	vu32 CPOW_Voltage;	    	    	//��Դ������ѹ     
	vu32 CVoltage;	    	        	//���ز�����ѹ    
	vu32 CCurrent;    	    			//��Դ��������     
	vu32 CLaod_Current;	    			//���ز�������       
	vu32 CR_VLUE;						//�������ֵ					 
	vu32 CR1_VLUE;						//ʶ�����1						
	vu32 CR2_VLUE;						//ʶ�����2							

	vu32 CREG_Load_CorrectionV_LOW;  	//�����ѹ���������͵�			 
	vu32 CREG_Load_CorrectionV_HIG;  	//�����ѹ���������ߵ�		
	vu32 CREG_Load_CorrectionA_LOW;		//����CCģʽ�������������͵�		    
	vu32 CREG_Load_CorrectionA_HIG;		//����CCģʽ�������������ߵ�    
	vu32 CSET_Load_CorrectionV_LOW;		//����CVģʽ���Ƶ�ѹ�ֵ�	   
	vu32 CSET_Load_CorrectionV_HIG;		//����CVģʽ���Ƶ�ѹ�ߵ�	   
	vu32 CSET_Load_CorrectionA_LOW;		//����CCģʽ���Ƶ����͵�	   
	vu32 CSET_Load_CorrectionA_HIG;		//����CCģʽ���Ƶ����ߵ�	    

	vu32 CREG_POWER_CorrectionV;		//��Դ������ѹ����	    
	vu32 CREG_POWER_CorrectionA_CC;		//��ԴCCģʽ������������
	vu32 CREG_POWER_CorrectionA_CV;		//��ԴCVģʽ������������
	vu32 CSET_POWER_CorrectionV; 		//��Դ��ѹ���Ʋ���
	vu32 CSET_POWER_CorrectionA; 		//��Դ�������Ʋ���		     

	vu32 CREG_CorrectionR_LOW;       	//������������͵�     
	vu32 CREG_CorrectionR_HIG;       	//������������ߵ�

	vu32 CREG_CorrectionR1;        		//ʶ�����1����        
	vu32 CREG_CorrectionR2;          	//ʶ�����2����     

	vu32 CREG_LoadV_Offset_LOW;     	//�����ѹ��������        
	vu32 CREG_LoadV_Offset_HIG;     	//�����ѹ��������
	vu32 CREG_LoadA_Offset_LOW;     	//����CCģʽ������������        
	vu32 CREG_LoadA_Offset_HIG;      	//����CCģʽ������������    
	vu32 CSET_LoadV_Offset_LOW;      	//����CVģʽ���Ƶ�ѹ����     
	vu32 CSET_LoadV_Offset_HIG;  		//����CVģʽ���Ƶ�ѹ����  				
	vu32 CSET_LoadA_Offset_LOW;   		//����CCģʽ���Ƶ�������  				
	vu32 CSET_LoadA_Offset_HIG;    		//����CCģʽ���Ƶ�������  				
	
	vu32 CREG_POWERV_Offset;        	//��ԴCVģʽ��ѹ��������       
	vu32 CREG_POWERA_Offset_CC;     	//��ԴCCģʽ������������      
	vu32 CREG_POWERA_Offset_CV;     	//��ԴCCģʽ������������       
	vu32 CSET_POWERV_Offset;	        //��ԴCVģʽ��ѹ���Ʋ���      
	vu32 CSET_POWERA_Offset;         	//��ԴCCģʽ�������Ʋ���         

	vu32 CREG_ReadR_Offset_LOW;      	//�������ϵ��  
	vu32 CREG_ReadR_Offset_HIG;      	//�������ϵ��       

	vu32 CREG_ReadR1_Offset;     		//�������ϵ��     
	vu32 CREG_ReadR2_Offset;     		//�������ϵ��         

	vu8 CPolar0;           				//���ص�ѹ�����Ϳ���ϵ��
	vu8 CPolar1;           				//���ص��������Ϳ���ϵ��
	vu8 CPolar2;			  			//��Դ��ѹ���ƺͲ���ϵ��
	vu8 CPolar3;			  			//��Դ���������Ϳ���ϵ��
	vu8 CPolar4;			  			//��������ߵ͵�ϵ��
	vu8 CPolar5;						//ʶ��������ϵ��			
	vu8 CPolar6;						//ʶ��������ϵ��		
	vu8 CPolar7;						//ʶ��������ϵ��	
	
	/* ������� */
	vu16 IR_Start_C;   			//��ʼ����     
	vu16 IR_Step_C;				//�������� 
	vu32 IR_Threshold_V;		//�ż���ѹ
	vu8 IR_Step_Time;			//����ʱ��

	/* �̿ص�Դ */
	vu16 POWER_OutPut_V;		//�����ѹ    
	vu16 POWER_Limit_C;			//���Ƶ��� 

	/* �̿ظ��� */
	vu16 LOAD_C;				//���ص���
	vu16 LOAD_V;				//���ص���

	/* ��ŵ� */
	vu16 CDC_OutPut_V;          //�����ѹ     
	vu16 CDC_Limit_C;			//���Ƶ���
	vu32 CDC_Ccutoff_V;			//��ֹ��ѹ
	vu16 CDC_Ccutoff_C;			//��ֹ����
	vu16 CDC_Load_C;			//���ص���    
	vu32 CDC_Dcutoff_V;			//��ֹ��ѹ  
	vu8  CDC_Cycle_Time;		//ѭ������

	/* ϵͳ���� */
	vu8 SYS_Comparator;			//��ѡ����
	vu8 SYS_Overflow;			//�������� --- FLAG_OVTest 
	vu8 SYS_Beep;				//��������
	vu32 SYS_UL_V;				//��ѹ����
	vu32 SYS_LL_V;				//��ѹ����
	vu16 SYS_UL_II;				//��������
	vu16 SYS_LL_IR;				//��������
	vu16 SYS_UL_Overflow;		//��������
	vu16 SYS_LL_Overflow;		//��������

	vu8 SYS_BatteryType; 		//�������
	vu8 SYS_LANUGAGE;  			//���� ---  FLAG_LANG 
	
	/* ϵͳ��Ϣ */
	vu16 SYS_SYEARS;
	vu8  SYS_SMONTHS;
	vu8	 SYS_SDAYS;
	vu32 SYS_SerialNO;
	
}CorrectList;

typedef struct {
	/* ������� */
	vu16 IR_Start_C;   			//��ʼ����     
	vu16 IR_Step_C;				//�������� 
	vu32 IR_Threshold_V;		//�ż���ѹ
	vu8 IR_Step_Time;			//����ʱ��

	/* �̿ص�Դ */
	vu16 POWER_OutPut_V;		//�����ѹ    
	vu16 POWER_Limit_C;			//���Ƶ���
	vu16 POWER_Cutoff_C;		//��ֹ���� 	

	/* �̿ظ��� */
	vu16 LOAD_C;				//���ص���
	vu16 LOAD_V;				//���ص���
	vu16 LOAD_Cutoff_V;			//��ֹ��ѹ

	/* ��ŵ� */
	vu16 CDC_OutPut_V;          //�����ѹ     
	vu16 CDC_Limit_C;			//���Ƶ���
	vu32 CDC_Ccutoff_V;			//��ֹ��ѹ
	vu16 CDC_Ccutoff_C;			//��ֹ����
	vu16 CDC_Load_C;			//���ص���    
	vu32 CDC_Dcutoff_V;			//��ֹ��ѹ  
	vu8  CDC_Cycle_Time;		//ѭ������
}SETPARA;
extern CorrectList CLists;

extern vu32 abc_time;      //��ʱ�� -s
extern vu32 ms_time;       //��ʱ�� -ms

extern vu8 FLAG_FBUTWIN;    //��¼��ǰҳ��  Ϊsetup����
extern vu8 FLAG_WIN;      	//�����־  0/�̿ص�Դ  1/�������   2/�̿ظ���   3/��ŵ�   4/����  5/ϵͳ����   6/ϵͳ��Ϣ
extern vu8 FLAG_LANG;  		//���Ա��
extern vu8 FLAG_OVTest;  	//�Ƿ�����������
extern vu8 FLAG_BEEP;   	//�������
extern vu8 FLAG_SORT;   	//��ѡ���ر��
extern vu8 FLAG_BATTERY;  	//���
extern vu8 FLAG_INPUTAUTH;  //����Ȩ�ޱ��  0/��������״̬  1/������״̬
extern char displayStr[20];  //������ַ�
extern vu8 FLAG_FIRSTWIN[8]; //�жϼ���ҳ�����Ϊ�Ƿ�Ϊ�ϵ�����״̬

extern vu8 POWER_RECHARGE;   //���
extern vu8 C_DISCHARGE;       //�ŵ�

extern float CDC_CsumMah;         //ÿ���ۼӵ�����

extern vu8 trig_flag_state; 

extern float max_discahrgeNum;    
extern float old_I;  
extern vu8 off_state; 
extern float v_memory;

//============================================================================= 
#define Receive_BUFFERSIZE   10
//=============================================================================
#endif
/******************* (C) COPYRIGHT 2015 KUNKIN *****END OF FILE*************************/