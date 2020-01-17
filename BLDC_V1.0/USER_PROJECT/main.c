//############################################################
// Created on: 2019年5月18日
// 本程序只供学习使用，未经作者许可，不得用于其它任何用途
// 版权所有，盗版必究
// STM32电机控制开发板
// 匠心科技
// 网址: https://shop298362997.taobao.com/index.htm?spm=2013.1.w5002-19016181214.2.53fe2393VNXwgW
// 电机控制答疑邮箱：JXKJ_2007@163.com
//############################################################
#include "stm32f10x.h"
#include "GPIO_int.h"
#include "Timer.h"
#include "ADC_int.h"
#include "OLED.h"
#include "Tim1_PWM.h"
#include "PI_Cale.h"
#include "State_Machine.h"
#include "PMSM motor parameters.h"
#include "Power stage parameters.h"
#include "Control stage parameters.h"
#include "Drive parameters.h"
#include "Uart.h"
#ifdef HALL
#include "Hall.h"
#endif
#ifdef HALLLESS
#include "VvvfControl.h"
#include "Hallless.h"
#endif
PIDREG_T     pi_spd  = PIDREG_T_DEFAULTS;
PIDREG_T     pi_ICurr = PIDREG_T_DEFAULTS;
State        StateContr=State_DEFAULTS;
ADCSamp      ADCSampPare=ADCSamp_DEFAULTS;
#ifdef HALLLESS
Hallless     Hallless_Three=Hallless_DEFAULTS;
VvvF_start   VvvF_startPare=VvvF_start_DEFAULTS;
#endif
#ifdef HALL
Hall         Hall_Three=Hall_DEFAULTS;
#endif
#ifdef HALLLESS
extern       Hallless   Hallless_Three;
#endif
volatile uint16_t ADC_DualConvertedValueTab[5];  //电位器接口函数

int Current10Times;
extern u16 uiCurrent10;


int main(void)
{
	Delay(10000);	
	SysTickConfig();
	StateContr.Control_Mode = LOOP;
	PID_init();
	if(	StateContr.Control_Mode == 1)     //开环
	{
	    StateContr.Aim_Speed = 80;        //百分之（100-占空比）
//	    StateContr.Aim_Duty = 72000 / PWM_FREQ * StateContr.Aim_Speed / 100;
        StateContr.Aim_Duty = 72000 / PWM_FREQ * StateContr.Aim_Speed / 100;
	}
	if(StateContr.Control_Mode == 2 || StateContr.Control_Mode == 3)
	{
		pi_spd.Ref = 300;
	}
	Delay(10000);	
	Key_Gpio();
#ifdef HALL
	ThreeHallPara_init();
	Hall_Gpio();
#endif
#ifdef HALLLESS
	ThreeHalllessPara_init();
	Hallless_Gpio(); 
#endif	
	Delay(10000);	
	Adc_Configuration();
	Delay(10000);	
	Tim1_PWM_Init();
	Delay(10000);	
    
	Offset_CurrentReading();   //电流校正有问题
	Delay(10000);	
	OLED_Init();
	while(1)
	{
		OLED_Display();
		Key_Scanning();
	}
}

