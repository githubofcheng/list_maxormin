#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/5
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//��ʼ��PB1Ϊ���.��ʹ��ʱ��	    
//LED IO��ʼ��
void Beep_Init(void)
{
	    GPIO_InitTypeDef GPIO_Initure;
			__HAL_RCC_GPIOA_CLK_ENABLE();  
			 GPIO_Initure.Pin=GPIO_PIN_12|GPIO_PIN_0;
			GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  
			GPIO_Initure.Pull=GPIO_PULLUP;          
			GPIO_Initure.Speed=GPIO_SPEED_HIGH;     
			HAL_GPIO_Init(GPIOA,&GPIO_Initure);
			PAout(12)=1;
}