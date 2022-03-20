#include "ButtonUse.h"
#include "led.h"
#include "pcf8574.h"
#include "GUI.h"
#include "stdio.h"
#include "math.h"
#include "DIALOG.h"
#include "GUI.h"
#include "stdio.h"
#include "key.h"
#include "math.h"
#include "rs422.h"
#include "adc.h"
#define Pi 3.1415926
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontsong18;
extern GUI_CONST_STORAGE GUI_FONT GUI_Fontheiti18;
extern int mode[2];
int state;
extern float FW_angle,FW_ans;
extern float FY_angle,FY_ans;
extern float LW_angle1,LW_ans1;
extern float LW_angle2,LW_ans2;
extern   float ZT1;
extern 	 float ZT2;
float num = 0,num2=0;
extern u8 rdata1[17];
extern 	float HY,ZY,SY,Height;
extern  float ans;
extern  int SD_BZ;
extern  int NA_JZ,NA_LW;
extern  int BEEP_BZ;
extern float temp;
extern  	int FH_BZ1,FH_BZ2,FH_BZ3,FH_BZ4;
//画图函数

void emwin_text(void)
{
	GUI_UC_SetEncodeUTF8();
	GUI_RECT Angle_SET = { 235,255,330,285 };//角度设置
	GUI_RECT Basic_INFO = { 195,345,280,375 };//基准信息
	GUI_RECT ZERO_INFO = { 235,525,335,555};//零位信息			  	        
	GUI_SetBkColor(GUI_BLACK);					
	GUI_SetColor(GUI_GREEN);       
	GUI_SetTextMode(GUI_TM_TRANS);
	GUI_SetFont(&GUI_Fontsong18);
	GUI_DispStringAt("角度信息", 50, 120);
	GUI_DispStringAt("角度设置", 50, 270);
	GUI_DispStringAt("基准信息", 50, 360);
	GUI_DispStringAt("转台状态", 50, 450);
	GUI_DispStringAt("零位信息", 50, 540);
	GUI_DispStringAt("方位", 265, 230);
	GUI_DispStringAt("俯仰", 475, 230);
	//判断故障位，有故障代码时指示灯变红，蜂鸣器响。
	if(rdata1[2]==0XB0||rdata1[2]==0XB1||rdata1[2]==0XB2||rdata1[2]==0XB3)
	{
	GUI_SetColor(GUI_RED);
	GUI_FillCircle(220, 455, 8);
	
	if(BEEP_BZ!=1){
				Beep=0;
						}
	
	else Beep=1;

	}
else{
	GUI_SetColor(GUI_GREEN);
	GUI_FillCircle(350, 455, 8);
			Beep=1;
			BEEP_BZ=0;
	}
	GUI_SetColor(GUI_GREEN);
	GUI_DispStringAt("故障代码", 460, 450);
	GUI_SetFont(&GUI_Font20_ASCII);
	//显示故障代码
	   switch(rdata1[2])
		{
			case 0XB0:GUI_DispHexAt(176,610,450,2);	break;	
			case 0XB1:GUI_DispHexAt(177,610,450,2);break;
			case 0XB2:GUI_DispHexAt(178,610,450,2);break;
			case 0XB3:GUI_DispHexAt(179,610,450,2);break;
		}
	//故障代码框										
		GUI_DrawRect(600,435,650,463);
	GUI_SetTextMode(GUI_TEXTMODE_NORMAL);  //????
	GUI_SetFont(&GUI_Fontheiti18); 	
		//基准信息画框
	for(int  i=0;i<3;i++)
	{
		GUI_SetColor(GUI_GREEN);
		GUI_DrawRect(Basic_INFO.x0, Basic_INFO.y0, Basic_INFO.x1, Basic_INFO.y1);
	//GUI_DispStringAt("°",Basic_INFO.x0+60,Basic_INFO.y0+13);
		GUI_DrawCircle(Basic_INFO.x0+75,Basic_INFO.y0+10,2);
		Basic_INFO.x0 += 135;
		Basic_INFO.x1 += 135;
	}	
		GUI_DrawRect(600, 345, 690, 375);
	GUI_DispStringAt("米",Basic_INFO.x0+70,Basic_INFO.y0+15)	;
	//角度信息画框
	for (int  i = 0; i < 2; i++) {
		GUI_SetColor(GUI_GREEN);
		GUI_DrawRect(Angle_SET.x0, Angle_SET.y0, Angle_SET.x1, Angle_SET.y1);
		//GUI_DispStringAt("°",Angle_SET.x0+60, Angle_SET.y0+10);
		  GUI_DrawCircle(Angle_SET.x0+80, Angle_SET.y0+10, 2);
		Angle_SET.x0 += 220;
		Angle_SET.x1 += 220;
	}
//零位信息画框
	for (int  i = 0; i < 2; i++) {
		GUI_SetColor(GUI_GREEN);
		GUI_DrawRect(ZERO_INFO.x0, ZERO_INFO.y0, ZERO_INFO.x1, ZERO_INFO.y1);
		 GUI_DrawCircle(ZERO_INFO.x0+80,ZERO_INFO.y0+10,2);
		//GUI_DispStringAt("°",ZERO_INFO.x0+60,ZERO_INFO.y0+10);
		ZERO_INFO.x0 += 220;
		ZERO_INFO.x1 += 220;
	}
}
void Round1(float num1)
{
	char ac[4];
	int x0 = 280;
	int y0 = 120;
	int r = 80;
	int i,num_1;
   num_1 = num1;
	int x,y,x1,y1;
	static const GUI_POINT aPoints[] = {{0,0},{2,0}, {2,40},{0,50},{-2,40},{-2,0}};
	GUI_POINT aMagnifiedPoints[GUI_COUNTOF(aPoints)];
 GUI_GotoXY(223 , 97);
    GUI_SetFont(&GUI_Font8x10_ASCII); //字体
    GUI_DrawCircle(x0, y0, r);//话半径为70的原
    for (i = 0; i <= 60; i++) {
        float a = (-30 + i * 4) * 3.1415926 / 180;//角度转换为弧度制
         if(i%5==0){
				 x = -r * cos(a) + x0;
         y = -r * sin(a) + y0;
         x1 = -(r - 10) * cos(a) + x0;
         y1 = -(r - 10) * sin(a) + y0;
        GUI_SetPenSize(3);
        GUI_DrawLine(x, y, x1, y1);
				 
				 
				 }else{
				  x = -r * cos(a) + x0;
         y = -r * sin(a) + y0;
         x1 = -(r - 5) * cos(a) + x0;
         y1 = -(r - 5) * sin(a) + y0;
        GUI_SetPenSize(1);
        GUI_DrawLine(x, y, x1, y1);
				 
				 
				 
				 
				 }
			



        if (i % 30 == 0) {
            x = -(r + 20) * cos(a) + x0;
            y = -(r + 5) * sin(a) + y0;
            sprintf(ac, "%d", 1*i - 30);
            GUI_SetTextAlign(GUI_TA_VCENTER);
            GUI_DispStringHCenterAt(ac, x, y);
        }
    }
    if (num > num_1) {
        num--;
        

        float a1 = num * 3.1415926 / 180;

        GUI_RotatePolygon(aMagnifiedPoints, aPoints, GUI_COUNTOF(aPoints), Pi - a1*4);
        GUI_FillPolygon(aMagnifiedPoints, GUI_COUNTOF(aPoints), x0, y0);



       
    }
		else if (num < num_1) {
        num++;


        float a1 = num * 3.1415926 / 180;

        GUI_RotatePolygon(aMagnifiedPoints, aPoints, GUI_COUNTOF(aPoints), Pi - a1 * 4);
        GUI_FillPolygon(aMagnifiedPoints, GUI_COUNTOF(aPoints), x0, y0);
    }
    else {
    
        float a1 = num * 3.1415926 / 180;

        GUI_RotatePolygon(aMagnifiedPoints, aPoints, GUI_COUNTOF(aPoints), Pi - a1 * 4);
        GUI_FillPolygon(aMagnifiedPoints, GUI_COUNTOF(aPoints), x0, y0);
    
    
    
    
    
    }

    
    }
   

void Round2(float num1)
{
	int x0 = 500;
	int y0 = 120;
	int r = 80;
	char ac[4];
	int i,num_1;
    num_1 = num1;
	int x,y,x1,y1;
	static const GUI_POINT aPoints[] = {
	 {0,0},{2,0}, {2,40},{0,50},{-2,40},{-2,0}
	};
	GUI_POINT aMagnifiedPoints[GUI_COUNTOF(aPoints)];
	  GUI_GotoXY(223, 97);
    GUI_SetFont(&GUI_Font8x10_ASCII); //字体
    GUI_DrawCircle(x0, y0, r);//话半径为70的原
    for (i = 0; i <= 60; i++) {
        float a = (-30 + i * 4) * 3.1415926 / 180;//角度转换为弧度制
			if(i%5==0){
		   	x = -r * cos(a) + x0;
        y = -r * sin(a) + y0;
         x1 = -(r - 10) * cos(a) + x0;
        y1 = -(r - 10) * sin(a) + y0;
        GUI_SetPenSize(3);
        GUI_DrawLine(x, y, x1, y1);
			}else{
			x = -r * cos(a) + x0;
        y = -r * sin(a) + y0;
         x1 = -(r - 5) * cos(a) + x0;
      y1 = -(r - 5) * sin(a) + y0;
        GUI_SetPenSize(1);
        GUI_DrawLine(x, y, x1, y1);
			
			
			
			
			}
        



        if (i % 30 == 0) {
            x = -(r + 20) * cos(a) + x0;
            y = -(r + 5) * sin(a) + y0;
            sprintf(ac, "%d", 1 * i - 30);
            GUI_SetTextAlign(GUI_TA_VCENTER);
            GUI_DispStringHCenterAt(ac, x, y);
        }
    }

    if (num2 > num_1) {
        num2 = num2 - 1;


        float a1 = num2 * 3.1415926 / 180;

        GUI_RotatePolygon(aMagnifiedPoints, aPoints, GUI_COUNTOF(aPoints), Pi - a1 * 4);
        GUI_FillPolygon(aMagnifiedPoints, GUI_COUNTOF(aPoints), x0, y0);
    }
    else if (num2 < num_1) {
        num2 = num2 + 1;
        float a1 = num2 * 3.1415926 / 180;

        GUI_RotatePolygon(aMagnifiedPoints, aPoints, GUI_COUNTOF(aPoints), Pi - a1 * 4);
        GUI_FillPolygon(aMagnifiedPoints, GUI_COUNTOF(aPoints), x0, y0);
    }
    else {
        float a1 = num2 * 3.1415926 / 180;

        GUI_RotatePolygon(aMagnifiedPoints, aPoints, GUI_COUNTOF(aPoints), Pi - a1 * 4);
        GUI_FillPolygon(aMagnifiedPoints, GUI_COUNTOF(aPoints), x0, y0);
    }

	
	
	//////////////////////////////////////////////////


}

void Show_number(void)
{
		
				GUI_SetFont(&GUI_Font20_ASCII);
//						GUI_GotoXY(50,50);
//						GUI_DispFloatMin(temp,3);
if(NA_JZ){
				GUI_GotoXY(205, 360);	//基准信息四个框显示数字，转台未发送值是默认为零
				GUI_DispFloatMin(HY,2);
				GUI_GotoXY(340, 360);
				GUI_DispFloatMin(ZY,2);
				GUI_GotoXY(475, 360);
				GUI_DispFloatMin(SY,2);
				GUI_GotoXY(610, 360);
				GUI_DispFloatMin(Height,2);
	}
	else
	{
		GUI_DispStringAt("NA",220, 360);
		GUI_DispStringAt("NA",355, 360);
		GUI_DispStringAt("NA",490, 360);
		GUI_DispStringAt("NA",625, 360);
	}
	///////////////////////////////////////////////////////////////////////////////////
	
	if(NA_LW){		
		if(FH_BZ3==-1){
			GUI_DispCharAt('-',242,529);
			GUI_GotoXY(250, 540);	// 零位修正角度1设置	
			GUI_DispFloatMin(-LW_ans1,3);
		}
		else{
			GUI_GotoXY(250, 540);	// 零位修正角度1设置	
			GUI_DispFloatMin(LW_ans1,3);
		}
		
		
		if(FH_BZ4==-1)		
		{
			GUI_DispCharAt('-',462,529);
			GUI_GotoXY(470, 540);	// 零位修正角度2设置	
			GUI_DispFloatMin(-LW_ans2,3);
		}
		else{
			GUI_GotoXY(470, 540);	// 零位修正角度2设置	
			GUI_DispFloatMin(LW_ans2,3);
		}
		
	}
	else{
																		// 零位修正角度1设置

				GUI_DispStringAt("NA",270, 540);
				                         	// 零位修正角度2设置

				GUI_DispStringAt("NA",490, 540);
	}
	///////////////////////////////////////////////////////////////////////////////////

	
	
	if(rdata1[3]!=0XB1){//归零功能
		if(FH_BZ1==-1){
			GUI_DispCharAt('-',242,259);
		  GUI_GotoXY(250, 270);   //方位 角度设置
			//	GUI_DispFloatFix(FW_ans, 6, 3);
		
				GUI_DispFloatMin(-FW_ans,3);		
		
		}else{
		    GUI_GotoXY(250, 270);   //方位 角度设置
			//	GUI_DispFloatFix(FW_ans, 6, 3);
		
				GUI_DispFloatMin(FW_ans,3);
		
		
		
		}
			if(FH_BZ2==-1){
			GUI_DispCharAt('-',462,259);
			GUI_GotoXY(470, 270);	// 俯仰 角度设置
			GUI_DispFloatMin(-FY_ans,3);
			
			}		
			else{
				GUI_GotoXY(470, 270);	// 俯仰 角度设置
			GUI_DispFloatMin(FY_ans,3);
			}
						
	}
	else
	{ 
	   	GUI_GotoXY(250, 270);   //方位 角度设置
				                             //GUI_DispFloatFix(0, 6, 3);
					GUI_DispFloatMin(0,2);	
				GUI_GotoXY(470, 270);	// 俯仰 角度设置
		                             //		GUI_DispFloatFix(0, 6, 3);
			GUI_DispFloatMin(0,2);
				GUI_GotoXY(250, 540);	// 零位修正角度1设置
			                              	//GUI_DispFloatFix(0, 5, 2);
				GUI_DispFloatMin(0,2);
			
				GUI_GotoXY(470, 540);	// 零位修正角度2设置
		                             	//	GUI_DispFloatFix(0, 5, 2);
				GUI_DispFloatMin(0,2);
	}
	///////////////////////////////////////////////////////////////////////////////////
				GUI_GotoXY(255, 170);     //转台1角度显示
				GUI_DispFloatMin(ZT1,3);
				   GUI_DrawCircle(315, 160, 2);    //转台方位的°号
				GUI_GotoXY(475, 170);     //转台2角度显示
				GUI_DispFloatMin(ZT2,3);
	       GUI_DrawCircle(535, 160, 2);    //转台俯仰的°号                      //GUI_DispFloatFix(ZT2, 6, 3);

}
void Show_WorkMode(void)//显示工作模式
{
				GUI_SetFont(&GUI_Fontsong18);
			if(mode[0]==21&&mode[1]==0){
				GUI_DispStringAt("1.方位稳定",650, 70);
				GUI_DispStringAt("2.俯仰稳定",650, 120);
				GUI_DispStringAt("3.转台稳定",650, 170);	
				   rdata1[3]=0;//显示菜单时，清除状态
			}	
			if(mode[0]==21&&mode[1]==1){
				//GUI_DispStringAt("1.方位稳定",650, 30);
						state=22;
						mode[0]=0;
						mode[1]=0;
       }	
				if(mode[0]==21&&mode[1]==2){
				//GUI_DispStringAt("2.俯仰稳定",650, 80);
						state=23;
						mode[0]=0;
						mode[1]=0;
			}	
				if(mode[0]==21&&mode[1]==3){
				//GUI_DispStringAt("3.转台稳定",650, 130);
						state=24;
						mode[0]=0;
						mode[1]=0;
			}				
		
		
////////////////////////////////////////////////////////////		
		 
	switch(rdata1[3]){
		case 0XB1: 	GUI_DispStringAt("归零    ",650, 120);break;
		case 0XB2: 	GUI_DispStringAt("锁定    ",650, 120);break; 
		case 0XB3: 	GUI_DispStringAt("零位修正",650, 120);break;
		case 0XB4: 	GUI_DispStringAt("调转    ",650, 120);break;
		case 0XB5: 	GUI_DispStringAt("维修    ",650, 120);break;
		case 0XB6: 	GUI_DispStringAt("1.方位稳定",650, 70);break;
		case 0XB7: 	GUI_DispStringAt("2.俯仰稳定",650, 120);break;
		case 0XB8: 	GUI_DispStringAt("3.转台稳定",650, 170);break;
										}
}
////////////////////////////////////////////////////////////////////////

void MainTask(void) {
	GUI_Init();
GUI_SetOrientation(GUI_MIRROR_X);
	GUI_MEMDEV_Handle hMem = GUI_MEMDEV_Create(0, 0, 800, 600);
	GUI_AA_EnableHiRes();
	GUI_AA_SetFactor(1);
	while (1) {
		  GUI_MEMDEV_Select(hMem); //选择内存设备
		 //uart1_send();
			GUI_Delay(5);
			GUI_Clear();
			emwin_text();
			Round1(ZT1);
			Round2(ZT2);
			Show_number();
			Show_WorkMode();
		pingmujiare();
			GUI_MEMDEV_Select(0); // 恢复LCD设备
       GUI_MEMDEV_CopyToLCDAt(hMem, 0, 0);
	}
}

