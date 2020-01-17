#ifndef _VvvfControl_H
#define _VvvfControl_H

#include "stm32f10x.h"

typedef struct {
	uint16_t      Freq_T_Ref;      //开环根据固定霍尔状态启动频率
	uint16_t      Voilage_Ref;     //开环给定参考电压
	uint16_t      VvvF_huanxCount; //换相计数
	uint16_t      VvvF_Count;      //VF计算
	uint16_t      VvvF_state;      //VF换相状态
	uint16_t      VvvF_num[6];     //VF霍尔换相顺序
	uint16_t      OldVF_State;     //上一时刻VF换相状态 
	uint16_t      initial_state;   //VF初始状态定位
	uint16_t      Freq_T_Ref_Count;//换相计数
	uint16_t      Count;           //换相计数
	  }VvvF_start;

#define  VvvF_start_DEFAULTS   {0,0,0,0,0,0,0,0,0,0} // 初始化参数

void Delay_MS(u32 nCount_temp);
void Delay_US(u32 nCount_temp);




void OpenLoop_SW(u8 ucHallTemp);

void Change_Voltage(void);

void Open_loop(void);
void Open_12step_loop(void);
void Open_loop1(void);
#endif
