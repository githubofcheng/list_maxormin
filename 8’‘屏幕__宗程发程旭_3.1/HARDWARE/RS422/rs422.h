#ifndef _RS422_H
#define _RS422_H
#include "sys.h"
#include "stdio.h"	

u8 KeyScan_3X7(void);
void send_data();
void uart1_init(void);
void uart2_init(void);
void coding1(float num,char num1);//�����ĸ��ֽ�װһ���������ĺ�������һ��������Ҫץ�������֣��ڶ����������׸�����ı��
void coding2(float num,char num1);//�����ֽ�װһ������������һ��������Ҫת�������֣��ڶ����������׸�����ı��
float uncoding1(char num1);//����coding1�෴������ֵ�Ǹ���������ڲ�����������
float uncoding2(char num1);//����coding2�෴������ֵ�Ǹ���������ڲ�����������
float uncoding3(char num1);
void uart1_send();
#endif

