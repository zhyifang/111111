//############################################################
// Created on: 2019年5月18日
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//版权所有，盗版必究
//STM32电机控制开发板
//匠心科技
//网址: https://shop298362997.taobao.com/index.htm?spm=2013.1.w5002-19016181214.2.53fe2393VNXwgW
//电机控制答疑邮箱：JXKJ_2007@163.com
//############################################################
#include "State_Machine.h"
#include "OLED.h"
#include "PI_Cale.h"
#include "Drive parameters.h"
#include "ADC_int.h"
#include "Tim1_PWM.h"
#include "Control stage parameters.h"
#ifdef HALLLESS
#include "Hallless.h"
#include "VvvfControl.h"
#endif
#ifdef HALL
#include "Hall.h"
#endif
#ifdef HALLLESS
extern    Hallless     Hallless_Three;
extern    VvvF_start   VvvF_startPare;
#endif
#ifdef HALL
extern    Hall         Hall_Three;
#endif
extern    ADCSamp      ADCSampPare;
extern    State        StateContr;
extern    PIDREG_T     pi_spd;
extern    PIDREG_T     pi_ICurr;
extern   u16 Vbus10Times;
extern   u16 Current10Times;
extern u16 uiCurrent10;
void Key_Gpio(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = RUN_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(RUN_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = STOP_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(STOP_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = UP_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(UP_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DOWN_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(DOWN_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = DIR_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(DIR_GPIO_PORT, &GPIO_InitStructure);
}
		
void Key_Scanning(void)
{
	static uint16_t Count = 0;
	
	if(RUN_STATUS == 0&& (Count == 0))
	{
		Start_Motor();
		StateContr.drive_car = 1;
		if(	StateContr.Control_Mode == 1)
		{
		  StateContr.Aim_Speed = 80;
		  StateContr.Aim_Duty = 72000 / PWM_FREQ * StateContr.Aim_Speed / 100;
		}
        
        
		if(StateContr.Control_Mode == 2 || StateContr.Control_Mode == 3)
		{
		  pi_spd.Ref = 300;
		}
		Count = 50;
	}
	
	if(STOP_STATUS == 0&& (Count == 0))
	{
		StateContr.Duty = 0;
//		TIM_SetCompare1(TIM1, 0);
//		TIM_SetCompare2(TIM1, 0);
//		TIM_SetCompare1(TIM1, 0);
		Stop_Motor();
		StateContr.Aim_Speed = 0;
		StateContr.Aim_Duty = 0;
		StateContr.drive_car = 0;
		StateContr.Start_order = 1;
		StateContr.Current_Count = 0;
		StateContr.Speed_Count = 0;
		#ifdef HALLLESS
		VvvF_startPare.Count = 0;
		Hallless_Three.Filter_Count = 0;
		Hallless_Three.Filter_CountF = 0;
		Hallless_Three.Speed_RPM = 0;
		Hallless_Three.Speed_RPMF = 0;
		#endif
		#ifdef HALL
		Hall_Three.Filter_Count = 0;
		Hall_Three.Filter_CountF = 0;
		Hall_Three.Speed_RPM = 0;
		Hall_Three.Speed_RPMF = 0;
		#endif
		pi_spd.Err = 0;
		pi_spd.Fdb = 0;
		pi_spd.Out =0;
		pi_spd.OutF =0;
		pi_spd.OutPreSat =0;
		pi_spd.Ref = 0;
		pi_spd.SatErr = 0;
		pi_spd.Ud = 0;
		pi_spd.Ui = 0;
		pi_spd.Ui_1 = 0;
		pi_spd.Up = 0;
		pi_spd.Up1 = 0;
		
		pi_ICurr.Err = 0;
		pi_ICurr.Fdb = 0;
		pi_ICurr.Out =0;
		pi_ICurr.OutF =0;
		pi_ICurr.OutPreSat =0;
		pi_ICurr.Ref = 0;
		pi_ICurr.SatErr = 0;
		pi_ICurr.Ud = 0;
		pi_ICurr.Ui = 0;
		pi_ICurr.Ui_1 = 0;
		pi_ICurr.Up = 0;
		pi_ICurr.Up1 = 0;

		Count = 50;
	}
	
	if(UP_STATUS == 0&& (Count == 0))
	{
		if(	StateContr.Control_Mode == 2 || StateContr.Control_Mode == 3)
		{
		pi_spd.Ref = pi_spd.Ref + 10;
		}
		if(	StateContr.Control_Mode == 1 )
		{
		StateContr.Aim_Speed = StateContr.Aim_Speed + 1;
		StateContr.Aim_Duty = 4500 * StateContr.Aim_Speed / 100;
		}
		if(StateContr.Aim_Speed > 80)
		{
		StateContr.Aim_Speed = 80;
		}
		if(pi_spd.Ref > 2500)
		{
		pi_spd.Ref = 2500;
		}
		
		Count = 50;
	}
	
	if(DOWN_STATUS == 0&& (Count == 0))
	{
		if(	StateContr.Control_Mode == 2 || StateContr.Control_Mode == 3)
		{
		pi_spd.Ref = pi_spd.Ref - 10;
		}
		if(	StateContr.Control_Mode == 1 )
		{
		StateContr.Aim_Speed = StateContr.Aim_Speed - 1;
		StateContr.Aim_Duty = 4500 * StateContr.Aim_Speed / 100;
		}
		if(StateContr.Aim_Speed < 10)
		{
		StateContr.Aim_Speed = 10;
		}
		if(pi_spd.Ref < 300)
		{
		pi_spd.Ref = 300;
		}
		
		Count = 50;
	}
	
	
    
    if(DIR_STATUS == 0&& (Count == 0))
	{
	
		Count = 50;
	}
	
	if (Count > 0)
	{
		
		Count--;
	}	
}

void OLED_Display(void)
{
	//第一行
	OLED_ShowCHinese(32,0,46,1);    //OLED_ShowCHinese(32,0,46,1);
	OLED_ShowCHinese(48,0,47,1);    //OLED_ShowCHinese(48,0,47,1);
	OLED_ShowCHinese(64,0,48,1);    //OLED_ShowCHinese(64,0,48,1); 
	OLED_ShowCHinese(80,0,49,1);    //OLED_ShowCHinese(80,0,49,1);    
   //第二行
	if(StateContr.Control_Mode == 2||StateContr.Control_Mode == 3)
	{	
	
        OLED_ShowCHinese(0,2,12,1);
        OLED_ShowCHinese(16,2,13,1); 
        OLED_ShowCHinese(32,2,14,1);
        OLED_ShowCHinese(48,2,15,1); 
        OLED_ShowString(80,2," ",1);
        OLED_ShowNum(90,2,pi_spd.Ref,4,16);
	}

	//第三行
	if(StateContr.Control_Mode == 2||StateContr.Control_Mode == 3)
	{
	OLED_ShowCHinese(0,4,16,1);   
	OLED_ShowCHinese(16,4,17,1); 
	OLED_ShowCHinese(32,4,14,1);
	OLED_ShowCHinese(48,4,15,1); 
	
	OLED_ShowNum(90,4,pi_spd.Fdb,4,16);
	OLED_ShowString(80,4," ",1);
	}
	
	//第四行
	#ifdef HALLLESS
	if(	StateContr.Control_Mode == 1)
	{
	OLED_ShowCHinese(0,2,75,1);   
	OLED_ShowCHinese(16,2,78,1); 
	OLED_ShowCHinese(32,2,77,1);
    OLED_ShowString(64,2," ",1);	 
	OLED_ShowNum(90,2,Vbus10Times,4,16);   //数字
	        
	OLED_ShowCHinese(0,4,75,1);   
	OLED_ShowCHinese(16,4,76,1); 
	OLED_ShowCHinese(32,4,77,1);
    OLED_ShowString(64,4," ",1);	 
	OLED_ShowNum(90,4,uiCurrent10,4,16); //
        
        
    OLED_ShowCHinese(0,6,12,1);
	OLED_ShowCHinese(16,6,13,1); 
	OLED_ShowCHinese(32,6,14,1);
	OLED_ShowCHinese(48,6,15,1); 
	
	OLED_ShowString(80,6," ",1);
	OLED_ShowNum(90,6,pi_spd.Ref,4,16);
//	OLED_ShowCHinese(8,6,60,1);     //无感方波双开环  间16
//	OLED_ShowCHinese(24,6,61,1);    //
//	OLED_ShowCHinese(40,6,62,1);    //
//	OLED_ShowCHinese(56,6,63,1);    //
//	OLED_ShowCHinese(72,6,64,1);    //
//	OLED_ShowCHinese(88,6,65,1);    //
//	OLED_ShowCHinese(104,6,66,1);   //
	}
	if(	StateContr.Control_Mode == 2)
	{	
	OLED_ShowCHinese(8,6,60,1);   
	OLED_ShowCHinese(24,6,61,1); 
	OLED_ShowCHinese(40,6,62,1);
	OLED_ShowCHinese(56,6,63,1); 
	OLED_ShowCHinese(72,6,70,1);
	OLED_ShowCHinese(88,6,71,1);
	OLED_ShowCHinese(104,6,72,1);
	}
	if(	StateContr.Control_Mode == 3)
	{
	OLED_ShowCHinese(8,6,60,1);   
	OLED_ShowCHinese(24,6,61,1); 
	OLED_ShowCHinese(40,6,62,1);
	OLED_ShowCHinese(56,6,63,1); 
	OLED_ShowCHinese(72,6,73,1);
	OLED_ShowCHinese(88,6,71,1);
	OLED_ShowCHinese(104,6,72,1);
	}
	#endif
	#ifdef HALL
	if(	StateContr.Control_Mode == 1)
	{
	OLED_ShowCHinese(0,3,67,1);   
	OLED_ShowCHinese(16,3,68,1); 
	OLED_ShowCHinese(32,3,69,1);
	OLED_ShowNum(90,3,StateContr.Aim_Speed,4,16);
	OLED_ShowString(80,3," ",1);	
		
	OLED_ShowCHinese(8,6,74,1);   
	OLED_ShowCHinese(24,6,61,1); 
	OLED_ShowCHinese(40,6,62,1);
	OLED_ShowCHinese(56,6,63,1); 
	OLED_ShowCHinese(72,6,64,1);
	OLED_ShowCHinese(88,6,65,1);
	OLED_ShowCHinese(104,6,66,1);
	}
	if(	StateContr.Control_Mode == 2)
	{	
	OLED_ShowCHinese(8,6,74,1);   
	OLED_ShowCHinese(24,6,61,1); 
	OLED_ShowCHinese(40,6,62,1);
	OLED_ShowCHinese(56,6,63,1); 
	OLED_ShowCHinese(72,6,70,1);
	OLED_ShowCHinese(88,6,71,1);
	OLED_ShowCHinese(104,6,72,1);
	}
	if(	StateContr.Control_Mode == 3)
	{
	OLED_ShowCHinese(8,6,74,1);   
	OLED_ShowCHinese(24,6,61,1); 
	OLED_ShowCHinese(40,6,62,1);
	OLED_ShowCHinese(56,6,63,1); 
	OLED_ShowCHinese(72,6,73,1);
	OLED_ShowCHinese(88,6,71,1);
	OLED_ShowCHinese(104,6,72,1);
	}
	#endif
}



