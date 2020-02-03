/******************************************************************/
/* 名称uasrt                                           */
/* 效果：                                                        */
/* 内容：                                                        */
/* 作者：李振                                                    */
/* 联系方式：QQ:363116119                                        */
/******************************************************************/
#include "my_register.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx.h"
#include "usart.h"

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
vu8 UART_Buffer_Rece[16];
vu8 UART_Buffer_Send[20];
vu8 UART_Buffer_Size;
vu8 Transmit_BUFFERsize;
vu8 t_USART;

u16 USART_RX_STA=0;       //接收状态标记
u16 USART_RX_CNT=0;       //接收计数
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

char CmdStr[CmdNumb][CmdLen] =
{
	{"STATUS_\0"},        //状态读取
	{"MOD_\0"},           //模式切换   
	{"TRIG_\0"},           //打开关闭
	{"SETPARA_\0"},           //模式切换    1个字符  
	{"LOADI_\0"},           //读取CS1237电流的数据 
	{"POWV_\0"},          //ADC电流
	{"POWI_\0"},            //启动LM5116 1个字符 
	{"ADCV_\0"},          //ADC电压
	{"LOWI_\0"},          //被拉低时过流 1个字符 
	{"HORL_\0"},          //切高低档位  1个字符 
	{"LVL_\0"},           //DAC8562输出 A路 0~5V
	{"SWITCH_\0"},        //总开关     1个字符 
	{"CALV_\0"},        //13电压校准     1个字符 
	{"CALI_\0"},        //14电流校准     1个字符 
	{"CTRLV_\0"},        //15控制电压校准     1个字符 
	{"CLEARI_\0"},        //16电流清零
	{"OVER_\0"},        //17输入输出保护
	{"TEMP_\0"},        //18温度
};


/*****************************************************************/
unsigned short IsDigitChar(unsigned char a)             //如果a的值是数字的话返回1，否则返回0
{
	int b;
  if(a<0x30 || a>0x39)                     //数字1-9
  	{
  	   b=0;
  	}
  else
  	{
  	   b=1;
  	}
  return b;
}


u16 SetErr_ACK(char *buf, u8 addr ,u8 ucErr)
{
		u16 i=0;
		u8 len = 0;
		char stradd[3] = {0};
		int addlen=0;
		memset(buf,0,sizeof(buf));
    	buf[i++] = ChrStartS;
		sprintf(stradd,"%d",addr);
		addlen=strlen(stradd);
		strncat(&buf[i],stradd,addlen);
		i+=addlen;
		buf[i++] = '/';
		switch(ucErr)
        {
          case 1:         //
          	   len = 9;
          	   memmove(&buf[i],"PARA ERR!",len);   
          	   break;
	      case 2:         //
          	   len = 8;
          	   memmove(&buf[i],"CMD ERR!",len);   
          	   break;
	      case 3:         //
          	   len = 8;
          	   memmove(&buf[i],"REM ERR!",len);   
          	   break;
		  default:    
               len = 8;
          	   memmove(&buf[i],"CMD ERR!",len);   
               break;
	    }
	    i+=len;
		buf[i++] = ChrEndR;
		buf[i++] = ChrEndS;
		return i;
}

u16 SerialRemoteHandleL(u8 len,char* buf)
{

  
    u16 currCharNum;
    u32 temp1;
  u8 Gmode_Vale;
	u8 i,j;
	
	u8 addr = 0;
	char str[200]={0};
	int cmd_flag=255;
	u8 templen =0;
	u8 tmpFg;
	u8 LM_S_Vale,LOW_I_Vale,H_L_Vale,SWITCH_Vale;
	u16 I_ADC_Vale,V_ADC_Vale;
	int32_t V_CS_Vale,I_CS_Vale;
	float Lvl_Vale,fadcx,fsingal, fVale;
 
//	u8 len = 11;
	u8 pntlen = 0;


	
	currCharNum=0;
	if((buf[currCharNum] != ChrStartR) || (buf[len-2] != ChrEndR)||(buf[len-1] != ChrEndS))
    {
		return SetErr_ACK(buf,addr ,CMD_ERR); 
    } 
	str[currCharNum++] = ChrStartR;
	sprintf(&str[currCharNum],"%d",addr);
	strncat(str,"/",1);
	currCharNum =strlen(str);
	cmd_flag=strncmp(buf,str,strlen(str));
	//testflag = cmd_flag;
	if(0!=cmd_flag)
	{
		return SetErr_ACK(buf, addr ,CMD_ERR);
		
	}
	for (j=0;j<CmdNumb;j++)
    {
		templen = strlen(CmdStr[j]);
		cmd_flag=strncmp(&buf[currCharNum],CmdStr[j],templen);
	    if(0==cmd_flag)
	    {

		  buf[0] = ChrStartS;
		  currCharNum+=templen;
		  switch(j)
          {
			case 0:
				pntlen = 5;    //
				if(buf[currCharNum]=='?')
				{

					
					sprintf(&buf[currCharNum],"%05d",CLists.CVoltage);
					currCharNum+=pntlen;
					
					buf[currCharNum++]=(',');
					sprintf(&buf[currCharNum],"%05d",CLists.CLaod_Current);
					currCharNum+=pntlen;
					
					buf[currCharNum++]=(',');
					sprintf(&buf[currCharNum],"%05d",CLists.CPOW_Voltage);
					currCharNum+=pntlen;
					
					buf[currCharNum++]=(',');
					sprintf(&buf[currCharNum],"%05d",CLists.CCurrent);
					currCharNum+=pntlen;
					
					buf[currCharNum++]=(',');
					sprintf(&buf[currCharNum],"%05d",CLists.CR_VLUE);
					currCharNum+=pntlen;
					
//					buf[currCharNum++]=(',');
//					sprintf(&buf[currCharNum],"%d",H_L);
//					currCharNum++;
					
//					buf[currCharNum++]=(',');
//					sprintf(&buf[currCharNum],"%d",LM_S);
//					currCharNum++;
					
//					buf[currCharNum++]=(',');
//					sprintf(&buf[currCharNum],"%02d",(int)temperature);
//					currCharNum+=2;
					
				  buf[currCharNum++ ] = ChrEndR;
				  buf[currCharNum++ ] = ChrEndS;
				}
				else
				{
					return SetErr_ACK(buf, addr ,CMD_ERR);
				}
				break;
			case 1:
				pntlen = 1;
				if(buf[currCharNum]=='?')
				{
					  sprintf(&buf[currCharNum],"%1d",FLAG_WIN);
				    currCharNum+=pntlen;
				    buf[currCharNum ++] = ChrEndS;
					
				}
				else
				{
					tmpFg = 1;
		            for(i=0;i<pntlen;i++)
		            {
					   if(IsDigitChar(buf[currCharNum+i])==1)
					   {
					      tmpFg = (tmpFg&0x01);
					   }
					   else
					   {
					      tmpFg = 0;
					   }
		            }
					if(tmpFg!=1||buf[currCharNum+pntlen]!=ChrEndR)
					{
						return SetErr_ACK(buf, addr ,CMD_ERR);
						
					}					
					for(i=0,temp1=0;i<pntlen;i++)
		            {
		                  temp1 = temp1*10+(buf[currCharNum++]-0x30);
					}		                   
				    if(temp1 > 5)
			         {
			             return SetErr_ACK(buf, addr ,PARA_ERR);
						 
			         }
							 FLAG_WIN = temp1;
					 buf[currCharNum++] = ChrEndS;
		        }
				break;
				case 2:
				pntlen = 1;
				if(buf[currCharNum]=='?')
				{
					  sprintf(&buf[currCharNum],"%1d",C_DISCHARGE);
				    currCharNum+=pntlen;
				    buf[currCharNum ++] = ChrEndS;
					
				}
				else
				{
					tmpFg = 1;
		            for(i=0;i<pntlen;i++)
		            {
					   if(IsDigitChar(buf[currCharNum+i])==1)
					   {
					      tmpFg = (tmpFg&0x01);
					   }
					   else
					   {
					      tmpFg = 0;
					   }
		            }
					if(tmpFg!=1||buf[currCharNum+pntlen]!=ChrEndR)
					{
						return SetErr_ACK(buf, addr ,CMD_ERR);
						
					}					
					for(i=0,temp1=0;i<pntlen;i++)
		            {
		                  temp1 = temp1*10+(buf[currCharNum++]-0x30);
					}		                   
				    if(temp1 > 5)
			         {
			             return SetErr_ACK(buf, addr ,PARA_ERR);
						 
			         }
					 switch(FLAG_WIN)
					 {
						 
					 }
							 FLAG_WIN = temp1;
					 buf[currCharNum++] = ChrEndS;
		        }
				break;
				case 3:
				pntlen = 1;
				if(buf[currCharNum]=='?')
				{
					  sprintf(&buf[currCharNum],"%1d",C_DISCHARGE);
				    currCharNum+=pntlen;
				    buf[currCharNum ++] = ChrEndS;
					
				}
				else
				{
					if(currCharNum == 0x0b)
					{						
						for(i=0,temp1=0;i<5;i++)
						{
							temp1 = temp1*10+(buf[currCharNum++]-0x30);
						}
					}					
				    if(temp1 > 5)
			         {
			             return SetErr_ACK(buf, addr ,PARA_ERR);
						 
			         }
					 switch(FLAG_WIN)
					 {
						 
					 }
							 FLAG_WIN = temp1;
					 buf[currCharNum++] = ChrEndS;
		        }
				break;
//			case 2:
//				pntlen = 5;    //
//				if(buf[currCharNum]=='?')
//				{
////					V_CS_Vale = Read_CS1237V();
//					
//					buf[currCharNum++]=(TEMPV>=0?'+':'-');
//					
//					sprintf(&buf[currCharNum],"%05d",TEMPV);
//					currCharNum+=pntlen;
//					buf[currCharNum++]=(TEMPI>=0?'+':'-');
//					sprintf(&buf[currCharNum],"%05d",abs(TEMPI));
//					currCharNum+=pntlen;
//					
////					currCharNum++;
//					buf[currCharNum++]=(',');
//					sprintf(&buf[currCharNum],"%d",H_L);
//					currCharNum++;
//					
//					buf[currCharNum++]=(',');
//					sprintf(&buf[currCharNum],"%d",LM_S);
//					currCharNum++;
//					
//					buf[currCharNum++]=(',');
//					sprintf(&buf[currCharNum],"%02d",(int)temperature);
//					currCharNum+=2;
//					
//				  buf[currCharNum++ ] = ChrEndR;
//				  buf[currCharNum++ ] = ChrEndS;
//				}
//				else
//				{
//					return SetErr_ACK(buf, addr ,CMD_ERR);
//				}
//				break;
//			case 3:
//				pntlen = 1;
//				if(buf[currCharNum]=='?')
//				{
//					sprintf(&buf[currCharNum],"%1d",Gmode_Vale);
//				    currCharNum+=pntlen;
//					buf[currCharNum] = ChrEndR;
//					currCharNum++;
////				    buf[currCharNum ++] = ChrEndS;
//					
//				}
//				else
//				{
//					tmpFg = 1;
//		            for(i=0;i<pntlen;i++)
//		            {
//					   if(IsDigitChar(buf[currCharNum+i])==1)
//					   {
//					      tmpFg = (tmpFg&0x01);
//					   }
//					   else
//					   {
//					      tmpFg = 0;
//					   }
//		            }
//					if(tmpFg!=1||buf[currCharNum+pntlen]!=ChrEndR)
//					{
//						return SetErr_ACK(buf, addr ,CMD_ERR);
//						
//					}					
//					for(i=0,temp1=0;i<pntlen;i++)
//		            {
//		                  temp1 = temp1*10+(buf[currCharNum++]-0x30);
//					}		                   
////				    if(temp1 >Mode_H)
////			         {
////			             return SetErr_ACK(buf, addr ,PARA_ERR);
////						 
////			         }
//		        
//							 calflag=temp1;
//							if(calflag == 3)
//							{
//								LVL_DA = 5000;
//							}
////							Change_MODE_Val(Mode);
//					 buf[currCharNum++] = ChrEndS;
//		        }
//				break;
//			case 4:
//				pntlen = 10;    //
//				if(buf[currCharNum]=='?')
//				{
//					I_CS_Vale = Read_CS1237();
//					
//					buf[currCharNum++]=(TEMPI>=0?'+':'-');
//					sprintf(&buf[currCharNum],"%05d",TEMPI);
//					currCharNum+=pntlen;
//				  buf[currCharNum++ ] = ChrEndR;
//				  buf[currCharNum++ ] = ChrEndS;
//					
//				}
//				else
//				{
//					return SetErr_ACK(buf, addr ,CMD_ERR);
//		    }
//				break;
//			case 5:
//				pntlen = 7;
//				if(buf[currCharNum]=='?')
//				{
//					I_ADC_Vale = Get_Adc_Average(ADC_Channel_14,3);
//					fadcx=(Vref*(((float)I_ADC_Vale/4096))); 
//					sprintf(&buf[currCharNum],"%01.5f",fadcx);
//				//	sprintf(&buf[currCharNum],"%07d",I_ADC_Vale);
//				    currCharNum+=pntlen;
//				    buf[currCharNum++ ] = ChrEndS;
//					
//				}
//				else
//				{
//					return SetErr_ACK(buf, addr ,CMD_ERR);
//		    }
//		  
//				break;
//			case 6:
//				pntlen = 1;
//				if(buf[currCharNum]=='?')
//				{
//					sprintf(&buf[currCharNum],"%1d",LM_S_Vale);
//				    currCharNum+=pntlen;
//				    buf[currCharNum++ ] = ChrEndR;
//					
//				}
//				else
//				{
//					tmpFg = 1;
//		            for(i=0;i<pntlen;i++)
//		            {
//					   if(IsDigitChar(buf[currCharNum+i])==1)
//					   {
//					      tmpFg = (tmpFg&0x01);
//					   }
//					   else
//					   {
//					      tmpFg = 0;
//					   }
//		            }
//					if(tmpFg!=1||buf[currCharNum+pntlen]!=ChrEndR)
//					{
//						return SetErr_ACK(buf, addr ,CMD_ERR);
//						
//					}					
//					for(i=0,temp1=0;i<pntlen;i++)
//		            {
//		                  temp1 = temp1*10+(buf[currCharNum++]-0x30);
//					}		                   
//				    if(temp1>LM_S_H)
//			         {
//			             return SetErr_ACK(buf, addr ,PARA_ERR);
//						 
//			         }
//		        	 LM_S=temp1;
////					 if(calflag == 0)
////					 {
//						Change_H_L_Vale(1);
////					 }
////					 SWITCH_Vale=temp1;
////					 H_L_Vale = temp1;
////					 Change_SWITCH_Val(SWITCH_Vale);
////					 SWITCH_A = temp1;
////					 Switch(0);
////					  if(temp1 == 0)
////					  {
////						 Output_DAC8562(Voltage_Convert(0), Voltage_Convert(0));
////					  }
////					 Change_LM_Val(LM_S_Vale);
//					
////					 Change_H_L_Vale(H_L_Vale);
//					 buf[currCharNum++] = ChrEndR;
//		        }
//				break;
//			case 7:
//				pntlen = 7;
//				if(buf[currCharNum]=='?')
//				{
//					V_ADC_Vale = Get_Adc_Average(ADC_Channel_15,3);
//					fadcx=(Vref*(((float)V_ADC_Vale/4096))); 
//							sprintf(&buf[currCharNum],"%01.5f",fadcx);
//					//V_ADC_Vale = Get_Adc_Average(15,5);
//					//sprintf(&buf[currCharNum],"%07d",V_ADC_Vale);
//				  currCharNum+=pntlen;
//				  buf[currCharNum++ ] = ChrEndS;
//				}
//				else
//				{
//					return SetErr_ACK(buf, addr ,CMD_ERR);
//		    }
//				break;
//			case 8:
//				pntlen = 1;
//				if(buf[currCharNum]=='?')
//				{
//					LOW_I_Vale = OverI();
//					LOW_I = LOW_I_Vale;
//					sprintf(&buf[currCharNum],"%1d",LOW_I_Vale);
//				    currCharNum+=pntlen;
//				    buf[currCharNum++ ] = ChrEndS;
//					
//				}
//				else
//				{
//					tmpFg = 1;
//		            for(i=0;i<pntlen;i++)
//		            {
//					   if(IsDigitChar(buf[currCharNum+i])==1)
//					   {
//					      tmpFg = (tmpFg&0x01);
//					   }
//					   else
//					   {
//					      tmpFg = 0;
//					   }
//		            }
//					if(tmpFg!=1||buf[currCharNum+pntlen]!=ChrEndR)
//					{
//						return SetErr_ACK(buf, addr ,CMD_ERR);
//						
//					}					
//					for(i=0,temp1=0;i<pntlen;i++)
//		            {
//		                  temp1 = temp1*10+(buf[currCharNum++]-0x30);
//					}		                   
//				    if(temp1>SWITCH_A_H)
//			         {
//			             return SetErr_ACK(buf, addr ,PARA_ERR);
//						 
//			         }
//		        	 LOW_I_Vale=temp1;
//							Change_LOW_I_Val(LOW_I_Vale);
//					 buf[currCharNum++] = ChrEndR;
//		        }
//				
//				break;
//			case 9:
//				pntlen = 1;
//				if(buf[currCharNum]=='?')
//				{
//					sprintf(&buf[currCharNum],"%1d",H_L_Vale);
//				    currCharNum+=pntlen;
//				    buf[currCharNum ++] = ChrEndS;
//					
//				}
//				else
//				{
//					tmpFg = 1;
//		            for(i=0;i<pntlen;i++)
//		            {
//					   if(IsDigitChar(buf[currCharNum+i])==1)
//					   {
//					      tmpFg = (tmpFg&0x01);
//					   }
//					   else
//					   {
//					      tmpFg = 0;
//					   }
//		            }
//					if(tmpFg!=1||buf[currCharNum+pntlen]!=ChrEndR)
//					{
//						return SetErr_ACK(buf, addr ,CMD_ERR);
//						
//					}					
//					for(i=0,temp1=0;i<pntlen;i++)
//		            {
//		                  temp1 = temp1*10+(buf[currCharNum++]-0x30);
//					}		                   
//				    if(temp1 >1)
//			         {
//			             return SetErr_ACK(buf, addr ,PARA_ERR);
//						 
//			         }

//							 H_L_Vale=temp1;
//							 H_L = temp1;
//							Change_H_L_Vale(H_L_Vale);
//					 buf[currCharNum++] = ChrEndR;
//		        }
//				break;
//			case 10:
//				pntlen = 8;
//				if(buf[currCharNum]=='?')
//				{
//					buf[currCharNum++]=(Lvl_Vale>=0?'+':'-');
//								sprintf(&buf[currCharNum],"%03.3f",Lvl_Vale);
//				    currCharNum+=pntlen;
//				    buf[currCharNum++ ] = ChrEndS;
//				}
////				else
////				{
////					tmpFg = 1;
////		            for(i=0;i<pntlen;i++)
////		            {
////									if(i==0||i==4)
////										continue;
////					   if(IsDigitChar(buf[currCharNum+i])==1)
////					   {
////					      tmpFg = (tmpFg&0x01);
////					   }
////					   else
////					   {
////					      tmpFg = 0;
////					   }
////		            }
////					if(tmpFg!=1||buf[currCharNum+pntlen]!=ChrEndR)
////					{
////						return SetErr_ACK(buf, addr ,CMD_ERR);
////						
////					}
////					temp1=0;
////					for(i=0;i<pntlen;i++)
////		            {
////		               if(i==0)
////					   {
////						   fsingal = (buf[currCharNum++]=='+'?1:(-1));
////						   continue; 
////					   } 
////					   else if(i==4)
////					   {
////						   currCharNum++;
////						   continue; 
////					   }
////					   else									 
////					   temp1 = temp1*10+(buf[currCharNum++]-0x30);
////					}		                   
//					if(buf[currCharNum]=='+')
//					{
//						fsingal = 1;
//						temp1 = 0;
//						currCharNum++;
//						for(i=0;i<7;i++)
//						{
//							if(i == 3)
//							{
//								currCharNum++;
//								continue;
//							}else{
//								temp1 = temp1*10+(buf[currCharNum++]-0x30);
//							}
//						}
////						Lvl_Vale=temp1*0.0001*fsingal;
//						Lvl_Vale=temp1;
//						LVL_DA =Lvl_Vale;
//	//					currCharNum++;
//						temp1 = 0;
//						
//						if(buf[currCharNum]==',')
//						{
//							currCharNum++;
//							for(i=0;i<7;i++)
//							{
//								if(i == 3)
//								{
//									currCharNum++;
//									continue;
//								}else{
//									temp1 = temp1*10+(buf[currCharNum++]-0x30);
//								}
//							}
//						}						
//						ChargePC = temp1;
//						temp1 = 0;
//						
//						if(buf[currCharNum]==',')
//						{
//							currCharNum++;
//							for(i=0;i<7;i++)
//							{
//								if(i == 3)
//								{
//									currCharNum++;
//									continue;
//								}else{
//									temp1 = temp1*10+(buf[currCharNum++]-0x30);
//								}
//							}
//						}						
//						LoadPC = temp1;
//						temp1 = 0;
//						
//						if(buf[currCharNum]==',')
//						{
//							currCharNum++;
//							for(i=0;i<3;i++)
//							{
//								temp1 = temp1*10+(buf[currCharNum++]-0x30);
//							}
//						}						
//						charget = temp1;
//						temp1 = 0;
//						
//						if(buf[currCharNum]==',')
//						{
//							currCharNum++;
//							for(i=0;i<3;i++)
//							{
//								temp1 = temp1*10+(buf[currCharNum++]-0x30);
//							}
//						}						
//						loadt = temp1;
//						
////						temp1 = 0;
////						currCharNum++;
////						if(buf[currCharNum++] == ',')
////						{
////							temp1 = temp1*10+(buf[currCharNum++]-0x30);
////							Irange = temp1;
////						}
////					}
//               
////							  Lvl_Vale=temp1*0.0001*fsingal;
////						    LVL_DA =Lvl_Vale;
////							Voltage_Convert(LVL_DA);
//					 buf[currCharNum++] = ChrEndR;
//		        }
//				break;
//			case 11:
//				pntlen = 1;
//				if(buf[currCharNum]=='?')
//				{
//					sprintf(&buf[currCharNum],"%1d",SWITCH_Vale);
//				    currCharNum+=pntlen;
//				    buf[currCharNum++ ] = ChrEndS;
//					
//				}
//				else
//				{
//					tmpFg = 1;
//		            for(i=0;i<pntlen;i++)
//		            {
//					   if(IsDigitChar(buf[currCharNum+i])==1)
//					   {
//					      tmpFg = (tmpFg&0x01);
//					   }
//					   else
//					   {
//					      tmpFg = 0;
//					   }
//		            }
//					if(tmpFg!=1||buf[currCharNum+pntlen]!=ChrEndR)
//					{
//						return SetErr_ACK(buf, addr ,CMD_ERR);
//						
//					}					
//					for(i=0,temp1=0;i<pntlen;i++)
//		            {
//		                  temp1 = temp1*10+(buf[currCharNum++]-0x30);
//					}		                   
//				    if(temp1>SWITCH_A_H)
//			         {
//			             return SetErr_ACK(buf, addr ,PARA_ERR);
//						 
//			         }
//		        	 SWITCH_Vale=temp1;
//							Change_SWITCH_Val(SWITCH_Vale);
//					 buf[currCharNum++] = ChrEndR;
//		        }
//				break;
//				case 12:
//				{
//					pntlen = 8;
//					if(buf[currCharNum]=='?')
//					{
//						buf[currCharNum++]=(Lvl_Vale>=0?'+':'-');
//									sprintf(&buf[currCharNum],"%03.3f",Lvl_Vale);
//						currCharNum+=pntlen;
//						buf[currCharNum++ ] = ChrEndS;
//					}
//					else
//					{
//						tmpFg = 1;
//						for(i=0;i<pntlen;i++)
//						{
//						   if(i==0||i==4)
//							  continue;
//						   if(IsDigitChar(buf[currCharNum+i])==1)
//						   {
//							  tmpFg = (tmpFg&0x01);
//						   }
//						   else
//						   {
//							  tmpFg = 0;
//						   }
//						}
//						if(tmpFg!=1||buf[currCharNum+pntlen]!=ChrEndR)
//						{
//							return SetErr_ACK(buf, addr ,CMD_ERR);
//							
//						}
//						temp1=0;
//						for(i=0;i<pntlen;i++)
//						{
//						   if(i==0)
//						   {
//							   fsingal = buf[currCharNum++]-0x30;
//							   continue; 
//						   } 
//						   else if(i==4)
//						   {
//							   currCharNum++;
//							   continue; 
//						   }
//						   else									 
//						   temp1 = temp1*10+(buf[currCharNum++]-0x30);
//						}	
//						CalHandle(0,fsingal,temp1);
//						
//					}
//				}break;
//				case 13:
//				{
//					pntlen = 8;
//					if(buf[currCharNum]=='?')
//					{
//						buf[currCharNum++]=(Lvl_Vale>=0?'+':'-');
//									sprintf(&buf[currCharNum],"%03.3f",Lvl_Vale);
//						currCharNum+=pntlen;
//						buf[currCharNum++ ] = ChrEndS;
//					}
//					else
//					{
//						tmpFg = 1;
//						for(i=0;i<pntlen;i++)
//						{
//						   if(i==0||i==4)
//							  continue;
//						   if(IsDigitChar(buf[currCharNum+i])==1)
//						   {
//							  tmpFg = (tmpFg&0x01);
//						   }
//						   else
//						   {
//							  tmpFg = 0;
//						   }
//						}
//						if(tmpFg!=1||buf[currCharNum+pntlen]!=ChrEndR)
//						{
//							return SetErr_ACK(buf, addr ,CMD_ERR);
//							
//						}
//						temp1=0;
//						for(i=0;i<pntlen;i++)
//						{
//						   if(i==0)
//						   {
//							   fsingal = buf[currCharNum++]-0x30;
//							   continue; 
//						   } 
//						   else if(i==4)
//						   {
//							   currCharNum++;
//							   continue; 
//						   }
//						   else									 
//						   temp1 = temp1*10+(buf[currCharNum++]-0x30);
//						}	
//						CalHandle(1,fsingal,temp1);
//						
//					}
//				}break;
//				case 14://控制电压校准
//				{
//					pntlen = 8;
//					if(buf[currCharNum]=='?')
//					{
//						buf[currCharNum++]=(Lvl_Vale>=0?'+':'-');
//									sprintf(&buf[currCharNum],"%03.3f",Lvl_Vale);
//						currCharNum+=pntlen;
//						buf[currCharNum++ ] = ChrEndS;
//					}
//					else
//					{
//						tmpFg = 1;
//						for(i=0;i<pntlen;i++)
//						{
//						   if(i==0||i==4)
//							  continue;
//						   if(IsDigitChar(buf[currCharNum+i])==1)
//						   {
//							  tmpFg = (tmpFg&0x01);
//						   }
//						   else
//						   {
//							  tmpFg = 0;
//						   }
//						}
//						if(tmpFg!=1||buf[currCharNum+pntlen]!=ChrEndR)
//						{
//							return SetErr_ACK(buf, addr ,CMD_ERR);
//							
//						}
//						temp1=0;
//						for(i=0;i<pntlen;i++)
//						{
//						   if(i==0)
//						   {
//							   fsingal = buf[currCharNum++]-0x30;
//							   continue; 
//						   } 
//						   else if(i==4)
//						   {
//							   currCharNum++;
//							   continue; 
//						   }
//						   else									 
//						   temp1 = temp1*10+(buf[currCharNum++]-0x30);
//						}
//						CalHandle(2,fsingal,temp1);
//						
//					}
//				}break;
//				case 15:
//				{
//					pntlen = 1;
//					if(buf[currCharNum]=='?')
//					{
//						sprintf(&buf[currCharNum],"%1d",Gmode_Vale);
//						currCharNum+=pntlen;
//						buf[currCharNum] = ChrEndR;
//						currCharNum++;
//	//				    buf[currCharNum ++] = ChrEndS;
//						
//					}
//					else
//					{
//						tmpFg = 1;
//						for(i=0;i<pntlen;i++)
//						{
//						   if(IsDigitChar(buf[currCharNum+i])==1)
//						   {
//							  tmpFg = (tmpFg&0x01);
//						   }
//						   else
//						   {
//							  tmpFg = 0;
//						   }
//						}
//						if(tmpFg!=1||buf[currCharNum+pntlen]!=ChrEndR)
//						{
//							return SetErr_ACK(buf, addr ,CMD_ERR);
//							
//						}					
//						for(i=0,temp1=0;i<pntlen;i++)
//						{
//							  temp1 = temp1*10+(buf[currCharNum++]-0x30);
//						}		                   
//						if(temp1 >Mode_H)
//						 {
//							 return SetErr_ACK(buf, addr ,PARA_ERR);
//							 
//						 }
//						 if(H_L == 1)
//						 {
//							CalHandle(3,1,temp1);
//						 }else{
//							 CalHandle(3,0,temp1);
//						 }
//						 
////								 Mode=temp1;
////								Change_MODE_Val(Mode);
////						 buf[currCharNum++] = ChrEndS;
//					}
//					break;
//				}break;
//				case 16:
//				{
//					pntlen = 3;
//					if(buf[currCharNum]=='?')
//					{
//						sprintf(&buf[currCharNum],"%1d",Gmode_Vale);
//						currCharNum+=pntlen;
//						buf[currCharNum] = ChrEndR;
//						currCharNum++;
//	//				    buf[currCharNum ++] = ChrEndS;
//						
//					}
//					else
//					{
//						tmpFg = 1;
//						for(i=0;i<pntlen;i++)
//						{
//						   if(IsDigitChar(buf[currCharNum+i])==1)
//						   {
//							  tmpFg = (tmpFg&0x01);
//						   }
//						   else
//						   {
//							  tmpFg = 0;
//						   }
//						}
//						if(tmpFg!=1||buf[currCharNum+pntlen]!=ChrEndR)
//						{
//							return SetErr_ACK(buf, addr ,CMD_ERR);
//							
//						}					
//						for(i=0,temp1=0;i<pntlen;i++)
//						{
//							  temp1 = temp1*10+(buf[currCharNum++]-0x30);
//						}		                   
//						 overflag = temp1;
//						 
////								 Mode=temp1;
////								Change_MODE_Val(Mode);
////						 buf[currCharNum++] = ChrEndS;
//					}
//					break;
//				}break;
//				case 17:
//				{
//					pntlen = 2;
//					if(buf[currCharNum]=='?')
//					{
////						buf[currCharNum++]=(Lvl_Vale>=0?'+':'-');
//									sprintf(&buf[currCharNum],"%02d",(int)temperature);
//						currCharNum+=pntlen;
//						buf[currCharNum++ ] = ChrEndR;
//						buf[currCharNum++ ] = ChrEndS;
//					}
//				}break;
			default:    //ERR
           		 return SetErr_ACK(buf, addr ,CMD_ERR);
				  
		  }
		  break;
		}
		
	}
	if(j>=CmdNumb)
	{
		return SetErr_ACK(buf, addr ,CMD_ERR);			  
	}
	return currCharNum+1;
}



static void USART1_NVIC_Config(void)//串口接收中断配置
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USART_Configuration(void)//串口初始化函数
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART1 clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 19200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	/******使能串口接收中断******************/
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
	USART_Cmd(USART1, ENABLE);//使能串口1
  USART_ClearFlag(USART1, USART_FLAG_TC);
  
  USART1_NVIC_Config();//串口中断配置
}			


static void USART2_NVIC_Config(void)//串口接收中断配置
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void USART2_Configuration(void)//串口初始化函数
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* config USART1 clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.02) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.03) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	
	/******使能串口接收中断******************/
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
//	USART_ITConfig(USART2, USART_IT_ORE, ENABLE);	
	USART_Cmd(USART2, ENABLE);//使能串口1
  USART_ClearFlag(USART2, USART_FLAG_TC);
  
  USART2_NVIC_Config();//串口中断配置
}

void USART2_IRQHandler(void)
{
	u8 Res;
 	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
		Res=USART_ReceiveData(USART2);

		if((USART_RX_STA&0x8000)==0)//接收未完成
		{
		if(USART_RX_STA&0x4000)//接收到了0x0d
			{
			if(Res!=0x0a)
				USART_RX_STA=0;//接收错误,重新开始
			else 
			{
		//		SerialRemoteHandleL(USART_RX_STA,USART_RX_BUF);
				USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;			//将接收的数据 存入数组中
				USART_RX_STA++;
				USART_RX_CNT = USART_RX_STA;
				USART_RX_STA|=0x8000;	//接收完成了 			  //接收到回车的后字节  置位状态寄存器 
			}
			}
		else //还没收到0X0D
			{	
			if(Res==0x0d)
			{
				USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;			//将接收的数据 存入数组中
				USART_RX_STA++;	
				USART_RX_STA|=0x4000;					 //接收到回车的前一字节  置位状态寄存器
			}
			else
				{
				USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;			//将接收的数据 存入数组中
					USART_RX_STA++;									//长度+1 为下一次做准备
				if(USART_RX_STA>(USART_REC_LEN-1))
					USART_RX_STA=0;//接收数据错误,重新开始接收	  
				}		 
			}
		} 
	}
}
void UART2_Send(void)
{
	static vu8 UART_Buffer_Send_pointer=0;
		if (UART_Buffer_Send_pointer < Transmit_BUFFERsize)
         {
					 if(UART_Buffer_Send[0]==ADDR)
					 {
						USART_SendData(USART2,UART_Buffer_Send[UART_Buffer_Send_pointer]);
						while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);//等待发送完成
						UART_Buffer_Send_pointer++;
					}
         }
		else 
		{
			UART_Buffer_Send_pointer=0;
			UART_SEND_flag=0;//发送完成一帧数据
		}
}

void UART1_Send(void)
{
	static vu8 UART_Buffer_Send_pointer=0;
		if (UART_Buffer_Send_pointer < Transmit_BUFFERsize)
         {
					 if(UART_Buffer_Send[0]==ADDR)
					 {
						USART_SendData(USART1,UART_Buffer_Send[UART_Buffer_Send_pointer]);
						while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);//等待发送完成
						UART_Buffer_Send_pointer++;
					}
         }
		else 
		{
			UART_Buffer_Send_pointer=0;
			UART_SEND_flag=0;//发送完成一帧数据
		}
}

