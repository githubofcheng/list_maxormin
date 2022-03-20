#include "rs422.h"
#include "key.h"
#include "math.h"
#include "delay.h"
#include "led.h"
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os ʹ��	  
#endif

#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 



UART_HandleTypeDef UART1_Handler; //UART1���
UART_HandleTypeDef UART2_Handler; //UART2���
		float FW_angle,FW_ans;
		float FY_angle,FY_ans;
		float LW_angle1,LW_ans1;
		float LW_angle2,LW_ans2;
		int FW_change=0;
		int FY_change=0;
	  float ZT1;
		float ZT2;
		float LW1;
		float LW2;
		float HY,ZY,SY,Height;
		int NA_JZ=0,NA_LW=0;
		float ans=0;
	  int mode[2]={0};//�ȶ�ģʽ�µĲ˵�
		int nums=0;//�����л��˵���
		u8 BZ=0;
		int SD_BZ=0;//������־
		int BEEP_BZ=0;
		int FH_BZ1=1,FH_BZ2=1,FH_BZ3=1,FH_BZ4=1;
		int Reuse[2]={0,0};//���ù��ܰ�������
u8 rdata1[17]={0};
u8 rdata2[11]={0};
u8 tdata[8]={0X5A,0X10,0X00,0X00,0X00,0X00,0X00,0X00};//��һ���ֽڣ��ڶ����ֽ��ǹ̶���



extern int state;


void uart1_init()//����һ��ʼ��
{
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=115200;				    //������
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ

	 
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()��ʹ��UART1		
		HAL_UART_Receive_IT(&UART1_Handler,rdata1,sizeof(rdata1));  //ʹ�ܴ��ڽ����ж�

}
void uart2_init()//���ڶ���ʼ��
{
	UART2_Handler.Instance=USART2;					    //USART2
	UART2_Handler.Init.BaudRate=115200;				    //������
	UART2_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART2_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART2_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART2_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART2_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	
	
   HAL_UART_Init(&UART2_Handler);	
	 HAL_UART_Receive_IT(&UART2_Handler,rdata2,sizeof(rdata2));  //ʹ�ܴ��ڽ����ж�

}
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO�˿�����
   	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_9|GPIO_PIN_10;			//PA9 10
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//����ΪUSART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��
		
		
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//��ռ���ȼ�3�������ȼ�3
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//ʹ��USART1�ж�ͨ��
	
		
	}
	if(huart->Instance==USART2)//����Ǵ���2�����д���2 MSP��ʼ��
		{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART2_CLK_ENABLE();			//ʹ��USART2ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_3|GPIO_PIN_2;			//PA0,1
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF7_USART2;	//����ΪUSART2
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��

		
		
		HAL_NVIC_SetPriority(USART2_IRQn,3,3);			//��ռ���ȼ�3�������ȼ�3
		HAL_NVIC_EnableIRQ(USART2_IRQn);				//ʹ��USART2�ж�ͨ��
	
		
	}

}

 void USART1_IRQHandler()
{
	
					#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
					OSIntEnter();    
				#endif
	
		HAL_UART_IRQHandler(&UART1_Handler);
		HAL_UART_Receive_IT(&UART1_Handler,rdata1,sizeof(rdata1));   //ʹ�ܴ��ڽ����ж�
	
	#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
    #endif
}

void USART2_IRQHandler()
{
	
		#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
					OSIntEnter();    
				#endif
	
	
		HAL_UART_IRQHandler(&UART2_Handler);
		HAL_UART_Receive_IT(&UART2_Handler,rdata2,sizeof(rdata2));   //ʹ�ܴ��ڽ����ж�
	
	#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)//���ڽ����жϴ�����
{
	u8 sum=0x00;
	
	if(huart->Instance==USART1)
{
	int sum=0X00;
						
				
			if(rdata1[0]==0XA5&&rdata1[1]==0X81)//ת̨�������ݶ���
		{	
			NA_LW=1;
			for(int d=0;d<16;d++)  sum=sum^rdata1[d];
//			if(rdata1[16]==sum){
			
			                      ZT1=uncoding1(4);		
														ZT2=uncoding1(8);											
														LW_ans1=uncoding2(12);
														LW_ans2=uncoding2(14);			
		
	//		}
			
		}
	
}//����һ
//HAL_UART_Transmit(&UART1_Handler,rdata1,sizeof(rdata1),1000);//����1����
	
	if(huart->Instance==USART2)//����2
	{	
		int JY=0X00;

												if(rdata2[0]==0XA5&&rdata2[1]==0X82)
													{		
															NA_JZ=1;
															for(int d=0;d<10;d++) JY=JY^rdata2[d];
//														if(rdata2[10]==JY)
//														{
																	HY=uncoding3(2);
																	ZY=uncoding3(4);
																	SY=uncoding3(6);
																	Height=uncoding3(8);
														
														
														
														
														
			//											}
													}									
	}

}

void coding1(float num,char num1)//
{
int a;
a=num*1000;
tdata[num1+3]=a;
tdata[num1+2]=a>>8;
tdata[num1+1]=a>>16;
tdata[num1]=a>>24;
}
void coding2(float num,char num1)
{
    short a;
   a=num*100;
   tdata[num1+1]=a;
   tdata[num1]=a>>8;
}
float uncoding1(char num1)
{
    int a;
    a=rdata1[num1];
    a=a<<8;
    a|=rdata1[num1+1];
    a=a<<8;
    a|=rdata1[num1+2];
    a=a<<8;
    a|=rdata1[num1+3];
    return a/1000.0;
}
float uncoding2(char num1)
{
    short a;
    a=rdata1[num1];
    a=a<<8;
    a|=rdata1[num1+1];
    return a/100.0;
}

float uncoding3(char num1)
{
    short a;
    a=rdata2[num1];
    a=a<<8;
    a|=rdata2[num1+1];
    return a/100.0;
}
float Read_Number()//�Ӽ����ж�ȡ���ָ�����λ��
{

	int number[7]={0};//{2,3,10,2,3,12}
	int piont;
	int i=0,key,a=0;
	   FH_BZ1=1;
	for(int t=0;t<7;t++)number[t]=-1;
	while(i<7){
				if((Reuse[1]>=0&&Reuse[1]<10)||Reuse[1]==13){//��������ĵڶ�λΪ���ֻ��߸��ţ�ֱ�Ӹ�ֵ����������
					number[0]=Reuse[1];
					i++;
				}
				key = KeyScan_3X7();
			if(key>0&&key<=13){
					 if(key==11) number[i]=0;
				else number[i]=key;//�������鸳ֵ
			}
		if(number[i]==12) break;
       
		if(key==14){     //ɾ��������������
				for(int t=0;t<7;t++)number[t]=-1;
          FW_ans=0;//��λ��ʾ
					ans=0;
				  i=0;
			    a=0;
					 FH_BZ1=1;
			}
		if(FH_BZ1==-1){
					if((number[i]>=0&&number[i]<10&&a==0)||number[i]==11){//��־λa=0������������ִ��ݺ���ʾ����Ļ��
								ans=ans*10;
								ans=ans+number[i];
							if(ans>=-30&&ans<=30)  
								FW_ans=-ans;
					}		  	
					if(number[i]==10)//��־λ=1������Ϊ����ansֵ��ͨ�ŷ�����ת̨
					{
						piont=i;
						a=1;
					}
					if((piont<i)&&a==1&&number[i]>=0&&number[i]<10){
						ans+=number[i]*pow(10,piont-i);
							if(ans>=-30&&ans<=30)  
							FW_ans=-ans;
					}
					 
					if((key>0&&key<=11)||key==12){
						 i++;   
					}
		
		}else{
						if((number[i]>=0&&number[i]<10&&a==0)||number[i]==11){//��־λa=0������������ִ��ݺ���ʾ����Ļ��
									ans=ans*10;
									ans=ans+number[i];
								if(ans>=-30&&ans<=30)  
									FW_ans=ans;
						}		  	
						if(number[i]==10)//��־λ=1������Ϊ����ansֵ��ͨ�ŷ�����ת̨
						{
							piont=i;
							a=1;
						}
						if((piont<i)&&a==1&&number[i]>=0&&number[i]<10){
							ans+=number[i]*pow(10,piont-i);
								if(ans>=-30&&ans<=30)  
								FW_ans=ans;
						}
						 
						if((key>0&&key<=11)||key==12){
							 i++;   
						}
		}
		  if(number[0]==13){//��⵽���Ű���
    number[0]=-1; 
     FW_ans=0;//
     ans=0;
      i=0;
	   a=0;
    FH_BZ1=-1;
  }
			
		}
		return 	FW_ans;
}

float Read_Number1()//�Ӽ����ж�ȡ���ָ���������
{

		int number[7];//{2,3,10,2,3,12}
	int piont;
	int i=0,key,a=0;
	  FH_BZ2=1;
		for(int t=0;t<7;t++)number[t]=-1;
	while(i<7){
				key = KeyScan_3X7();
			if((key>0&&key<=11)||key==12){
				 if(key==11) number[i]=0;
				else number[i]=key;//�������鸳ֵ
			}
		if(number[i]==12) break;
		if(key==14){     //ɾ��������������
				for(int t=0;t<7;t++)number[t]=-1;
          FY_ans=0;//��λ��ʾ
					ans=0;
				  i=0;
			    a=0;
					 FH_BZ2=1;
			}
		if(FH_BZ2==-1){
					if((number[i]>=0&&number[i]<10&&a==0)||number[i]==11){//��־λa=0������������ִ��ݺ���ʾ����Ļ��
								ans=ans*10;
								ans=ans+number[i];
							if(ans>=-30&&ans<=30) 
								FY_ans=-ans;
					}		  	
					if(number[i]==10)//��־λ=1������Ϊ����ansֵ��ͨ�ŷ�����ת̨
					{
						piont=i;
						a=1;
					}
					if((piont<i)&&a==1&&number[i]>=0&&number[i]<10){
						ans+=number[i]*pow(10,piont-i);
							if(ans>=-30&&ans<=30)  
							FY_ans=-ans;
					}
					 
					if((key>0&&key<=11)||key==12){
						 i++;   
					}
		
		}else{
						if((number[i]>=0&&number[i]<10&&a==0)||number[i]==11){//��־λa=0������������ִ��ݺ���ʾ����Ļ��
									ans=ans*10;
									ans=ans+number[i];
									if(ans>=-30&&ans<=30)  FY_ans=ans;
						}		  	
						if(number[i]==10)//��־λ=1������Ϊ����ansֵ��ͨ�ŷ�����ת̨
						{
							piont=i;
							a=1;
						}
						if((piont<i)&&a==1&&number[i]>=0&&number[i]<10){
							ans+=number[i]*pow(10,piont-i);
								if(ans>=-30&&ans<=30)  FY_ans=ans;
						}
						 
						if((key>0&&key<=11)||key==12){
							 i++;   
						}
		}
		  if(number[0]==10&&number[1]==10){
    number[0]=-1; number[1]=-1;
     FY_ans=0;//
     ans=0;
      i=0;
	   a=0;
    FH_BZ2=-1;
  }
			
		}
		return 	FY_ans;
}

float Read_Number2()//�Ӽ����ж�ȡ���ָ�����λ��1
{

	int number[7]={0};//{2,3,10,2,3,12}
	int piont;
	int i=0,key,a=0;
	  FH_BZ3=1;
		for(int t=0;t<7;t++)number[t]=-1;
	while(i<7){
				key = KeyScan_3X7();
			if((key>0&&key<=11)||key==12){
			 if(key==11) number[i]=0;
				else number[i]=key;//�������鸳ֵ
			}
		if(number[i]==12) break;
       
		if(key==14){     //ɾ��������������
					for(int t=0;t<7;t++)number[t]=-1;
          LW_ans1=0;//��λ��ʾ
					ans=0;
				  i=0;
			    a=0;
					 FH_BZ3=1;
			}
		if(FH_BZ3==-1){
					if((number[i]>=0&&number[i]<10&&a==0)||number[i]==11){//��־λa=0������������ִ��ݺ���ʾ����Ļ��
								ans=ans*10;
								ans=ans+number[i];
							if(ans>=-10&&ans<=10) 
								LW_ans1=-ans;
					}		  	
					if(number[i]==10)//��־λ=1������Ϊ����ansֵ��ͨ�ŷ�����ת̨
					{
						piont=i;
						a=1;
					}
					if((piont<i)&&a==1&&number[i]>=0&&number[i]<10){
						ans+=number[i]*pow(10,piont-i);
							if(ans>=-10&&ans<=10) 
							LW_ans1=-ans;
					}
					 
					if((key>0&&key<=11)||key==12){
						 i++;   
					}
		
		}else{
						if((number[i]>=0&&number[i]<10&&a==0)||number[i]==11){//��־λa=0������������ִ��ݺ���ʾ����Ļ��
									ans=ans*10;
									ans=ans+number[i];
								if(ans>=-10&&ans<=10) 
									LW_ans1=ans;
						}		  	
						if(number[i]==10)//��־λ=1������Ϊ����ansֵ��ͨ�ŷ�����ת̨
						{
							piont=i;
							a=1;
						}
						if((piont<i)&&a==1&&number[i]>=0&&number[i]<10){
							ans+=number[i]*pow(10,piont-i);
								if(ans>=-10&&ans<=10) 
								LW_ans1=ans;
						}
						 
						if((key>0&&key<=11)||key==12){
							 i++;   
						}
		}
		  if(number[0]==10&&number[1]==10){
    number[0]=-1; number[1]=-1;
     LW_ans1=0;//
     ans=0;
      i=0;
	   a=0;
    FH_BZ3=-1;
  }
			
		}
		return 	LW_ans1;
}

float Read_Number3()//�Ӽ����ж�ȡ���ָ�����λ��2
{

		int number[7]={0};//{2,3,10,2,3,12}
	int piont;
	int i=0,key,a=0;
		  FH_BZ4=1;
		for(int t=0;t<7;t++)number[t]=-1;
	while(i<7){
				key = KeyScan_3X7();
			if((key>0&&key<=11)||key==12){
			 if(key==11) number[i]=0;
				else number[i]=key;//�������鸳ֵ
			}
		if(number[i]==12) break;
       
		if(key==14){     //ɾ��������������
					for(int t=0;t<7;t++)number[t]=-1;
          LW_ans2=0;//��λ��ʾ
					ans=0;
				  i=0;
			    a=0;
					FH_BZ4=1;
			}
		if(FH_BZ4==-1){
					if((number[i]>=0&&number[i]<10&&a==0)||number[i]==11){//��־λa=0������������ִ��ݺ���ʾ����Ļ��
								ans=ans*10;
								ans=ans+number[i];
							if(ans>=-10&&ans<=10) 
								LW_ans2=-ans;
					}		  	
					if(number[i]==10)//��־λ=1������Ϊ����ansֵ��ͨ�ŷ�����ת̨
					{
						piont=i;
						a=1;
					}
					if((piont<i)&&a==1&&number[i]>=0&&number[i]<10){
						ans+=number[i]*pow(10,piont-i);
							if(ans>=-10&&ans<=10) 
							LW_ans2=-ans;
					}
					 
					if((key>0&&key<=11)||key==12){
						 i++;   
					}
		
		}else{
						if((number[i]>=0&&number[i]<10&&a==0)||number[i]==11){//��־λa=0������������ִ��ݺ���ʾ����Ļ��
									ans=ans*10;
									ans=ans+number[i];
								if(ans>=-10&&ans<=10) 
									LW_ans2=ans;
						}		  	
						if(number[i]==10)//��־λ=1������Ϊ����ansֵ��ͨ�ŷ�����ת̨
						{
							piont=i;
							a=1;
						}
						if((piont<i)&&a==1&&number[i]>=0&&number[i]<10){
							ans+=number[i]*pow(10,piont-i);
								if(ans>=-10&&ans<=10) 
								LW_ans2=ans;
						}
						 
						if((key>0&&key<=11)||key==12){
							 i++;   
						}
		}
		  if(number[0]==10&&number[1]==10){
    number[0]=-1; number[1]=-1;
     LW_ans2=0;//
     ans=0;
      i=0;
	   a=0;
    FH_BZ4=-1;
  }
			
		}
		return 	LW_ans2;
}



void send_data()//��Ҫ����ģʽ���жϺ�ͨ�����а����İ������ñ�ģʽ
{
    	u8 check=0x00;//У��λ
	    int key_table1;
	   
			key_table1 = KeyScan_3X7();
				if(key_table1>0&&key_table1<22&&Beep==0){
						BEEP_BZ=1;
				}
				//�����ȶ�ģʽ���л�////////////////////////////////////////////////////////
				if(key_table1==21){
						mode[0]=key_table1;
						BZ=1;
				}
				if(key_table1>0&&key_table1<4&&BZ==1){
						mode[1]=key_table1;
						BZ=2;
				}
//////////////////////////////////////////////
//���²����Ǹ�ͨ���жϹ��ܼ��İ��£��������ͱ�1������
//////////////////////////////////////////				
	if(rdata1[3]!=0XB2){
					switch(state)
						{                                                                              
            case 22: 	tdata[3]=0XA6;		tdata[2]=0X01;	 	break;							                                                                                                       			                                                                                                        
            case 23: 	tdata[3]=0XA7;		tdata[2]=0X01;		break;								                                                   						
						case 24: 	tdata[3]=0XA8;		tdata[2]=0X01; 		break;						
						}
						state=0;
						switch(key_table1)
						{
						case 16: 	tdata[3]=0XA3;	  tdata[2]=0X01;  		break;						//��λ����			
						case 18: 	tdata[3]=0XA5;		tdata[2]=0X01;			break;						//	ά��
						case 19: 	tdata[3]=0XA2;  	tdata[2]=0X01;			break;					//	����
						case 17: 	tdata[3]=0XA1;	  tdata[2]=0X01;				break;					//	����
						case 20: 	tdata[3]=0XA4;		tdata[2]=0X01; 		break;							//	S20Ԥ�� 
						}
					}else{
									while(1){
									u8 key_SD=KeyScan_3X7();
									if(key_SD==19){rdata1[3]=0XB9;break;}
									}
					}
	
////////////////////////////////////////////////////////////////////////////����Ϊ��2���ڸ����ȶ����תģʽ�½��з�λ�ǵ�����

if(rdata1[3]==0XB4||rdata1[3]==0XB7){
	
			if(key_table1==15){
					Reuse[0]=key_table1;
				while(1){
					Reuse[1]=KeyScan_3X7();
					if(Reuse[1]!=0) break;
				}
					tdata[2]=0X02;
					if(Reuse[0]==15&&Reuse[1]!=15){//���������һ�ڶ�λ��ֵ
							FW_angle=Read_Number();
									ans=0;	
							coding1(FW_angle,3);
					}
			}
}	
///////////////////////////////////////////////////////////////////����Ϊ��3�ı����жϣ��ж��Ƿ��Ƿ�λ�ȶ����תģʽ�����и����ǵ�����
			

if(rdata1[3]==0XB4||rdata1[3]==0XB6){
	
	if(key_table1==15){
					Reuse[0]=15;
					Reuse[1]=KeyScan_3X7();
					tdata[2]=0X03;
		if(Reuse[0]==15&&Reuse[1]==15){
					FY_angle=Read_Number1();
					ans=0;
					coding1(FY_angle,3);
		}
	}		
}
/////////////////////////////////////////////////////////////////////




if(rdata1[3]==0XB4){
	
	if(key_table1==15){
					Reuse[0]=key_table1;
				while(1){
					Reuse[1]=KeyScan_3X7();
					if(Reuse[1]!=0) break;
				}
					
				if(Reuse[0]==15&&Reuse[1]!=15){//���������һ�ڶ�λ��ֵ
					tdata[2]=0X02;
							FW_angle=Read_Number();
									ans=0;	
							coding1(FW_angle,3);
					}
					if(Reuse[0]==15&&Reuse[1]==15){
						tdata[2]=0X03;
								FY_angle=Read_Number1();
								ans=0;
								coding1(FY_angle,3);
					}
	}		
}















/////////////////////////////////////////////////////////////	��4������λ����ģʽ�£����õ���

if(rdata1[3]==0XB3)
{
	if(key_table1==16){
	 
		tdata[2]=0X04;
		LW_angle1=Read_Number2();
	    	ans=0;
		LW_angle2=Read_Number3();
	     	ans=0;
		coding2(LW_angle1,3);
		coding2(LW_angle2,5);	
	}
	 
}
		for(int i=0;i<7;i++)
			{
					check=check^tdata[i];			
			
			}
			tdata[7]=check;

}

void uart1_send()
	{
	if(tdata[2]!=0)  HAL_UART_Transmit(&UART1_Handler,tdata,sizeof(tdata),100);//����1���ͱ���
						for(int a=2;a<7;a++)
						tdata[a]=0;	
				
	}





		for(int i=0;i<2;i++)//ÿ����һ�κ�������ù�������
						Reuse[i]=0;