//############################################################
// Created on: 2019年5月18日
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//版权所有，盗版必究
//STM32电机控制开发板
//匠心科技
//网址: https://shop298362997.taobao.com/index.htm?spm=2013.1.w5002-19016181214.2.53fe2393VNXwgW
//电机控制答疑邮箱：JXKJ_2007@163.com
//############################################################
#ifndef _Tim1_PWM_H
#define _Tim1_PWM_H 
#include "stm32f10x.h"

#define PWM_GPIO  GPIOB 
#define PWM1L GPIO_Pin_13
#define PWM2L GPIO_Pin_14
#define PWM3L GPIO_Pin_15

void Tim1_PWM_Init(void);
void TIM1_Gpio(void);
void Start_Motor(void);
void Stop_Motor(void);
void MOS_Q41PWM(void) ;    // 切换Q14MOS驱动
void MOS_Q16PWM(void) ;    // 切换Q16MOS驱动
void MOS_Q63PWM(void) ;    // 切换Q36MOS驱动
void MOS_Q32PWM(void) ;    // 切换Q32MOS驱动
void MOS_Q25PWM(void) ;    // 切换Q52MOS驱动
void MOS_Q54PWM(void) ;    // 切换Q54MOS驱动


void MOS_AbcPWM(void);
void MOS_ABcPWM(void);
void MOS_BacPWM(void);
void MOS_BCaPWM(void);
void MOS_CabPWM(void);
void MOS_CAbPWM(void);
#endif
