#include "sys.h"

/******** ����״̬�궨�� ************/
#define		RAW1_STATE		PBin(12)
#define		RAW2_STATE		PBin(13)
#define		RAW3_STATE		PBin(14)
/******** ����״̬�궨�� ************/
#define		COL1_STATE		PBin(5)
#define		COL2_STATE  	PBin(6)
#define		COL3_STATE		PBin(7)
#define		COL4_STATE		PBin(8)
#define		COL5_STATE		PBin(9)
#define		COL6_STATE		PBin(10)
#define		COL7_STATE		PBin(11)
/********************* ��ֵ�ֲ�************************* 
                 S1 	  S2	   S3		S4
                 S5		  S6	   S7	    S8
                 S9		  S10      S11		S12
                 S13	  S14	   S15	    S16   
*******************************************************
********************���󰴼�����***********************
                 PC6      PC7      PC8      PC9
        PB6-------|--------|--------|--------|----
        PB7 ------|--------|--------|--------|----
        PB8-------|--------|--------|--------|----
        PB9 ------|--------|--------|--------|----
*******************************************************/
/* -- ����״̬�궨�� -- */
#define 	KEY3X7_OPEN			1
#define 	KEY3X7_CLOSE		0
/* -----------------������غ��� --------------------- */
void KEY3X7_ON(void);   // �����������
void KEY3X7_OFF(void);	// �رվ������
void KEY3X7_RAW_SET(void);  				// ������λ
void KEY3X7_RAW_RESET(void); 				// ���߸�λ
void KEY3X7_RAW_READ_Init(void);  	        // ������Ϊ����ģʽ
void KEY3X7_COL_SET(void);					// ������λ
void KEY3X7_COL_RESET(void);				// ���߸�λ
void KEY3X7_COL_READ_Init(void);		    // ������Ϊ����ģʽ
u8 KeyScan_3X7(void); 	// ����ɨ�躯��



