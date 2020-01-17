//############################################################
// Created on: 2019年5月18日
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//版权所有，盗版必究
//STM32电机控制开发板
//匠心科技
//网址: https://shop298362997.taobao.com/index.htm?spm=2013.1.w5002-19016181214.2.53fe2393VNXwgW
//电机控制答疑邮箱：JXKJ_2007@163.com
//############################################################
#ifndef ADC_int_H
#define ADC_int_H

#include "stm32f10x.h"

void Adc_Configuration(void);
void Offset_CurrentReading(void);
void HALL_ADC_Sample(void);

void HALLLESS_ADC_Sample(u8 temp);
typedef struct {
	     int32_t   BUS_CurrF ;         // 母线电流 DC Bus  Current
		 int32_t   BUS_Curr ;          // 母线电流 DC Bus  Current
	     int32_t   PhaseW_Curr ;       // W相电流 Phase U Current
	     int32_t   PhaseU_Curr;        // U相电流 Phase U Current
	     int32_t   PhaseV_Curr;        // V相电流Phase V Current
	     int32_t   BUS_Voltage ;       //母线电压DC Bus  Voltage	     
	     int32_t   RP_speed_Voltage ;   // 电位器电压 RP1_Voltage
	     int32_t   OffsetPhaseU_Curr;   // U相电流偏执值  Phase U Current
	     int32_t   OffsetPhaseV_Curr;   // V相电流偏执值  Phase V Current
		 int32_t   OffsetPhaseW_Curr ;  // W相电流偏执值  Phase V Current
	     int32_t   Coeff_filterK1;      // 一阶低通滤波器系数1
		 int32_t   Coeff_filterK2;    // 一阶低通滤波器系数2
       }ADCSamp;

#define  ADCSamp_DEFAULTS  {0,0,0,0,0,0,0,0,0,1000,24}   // 初始化参数

u16 Caculate_10TimesCurrent(u16 ADC_Value);
u16 Caculate_10TimesVbus(u16 ADC_Value);


#endif
