//############################################################
// Created on: 2019年5月18日
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//版权所有，盗版必究
//STM32电机控制开发板
//匠心科技
//网址: https://shop298362997.taobao.com/index.htm?spm=2013.1.w5002-19016181214.2.53fe2393VNXwgW
//电机控制答疑邮箱：JXKJ_2007@163.com
//############################################################
#ifndef _TIMER_H
#define _TIMER_H 

#include "stm32f10x.h"

void SysTickConfig(void);
void TIM2_Init(void);
void TIM3_Init(void);
void TIM3_IRQHandler(void);

	
#endif
