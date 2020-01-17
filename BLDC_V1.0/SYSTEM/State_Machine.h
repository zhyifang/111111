//############################################################
// Created on: 2019年5月18日
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//版权所有，盗版必究
//STM32电机控制开发板
//匠心科技
//网址: https://shop298362997.taobao.com/index.htm?spm=2013.1.w5002-19016181214.2.53fe2393VNXwgW
//电机控制答疑邮箱：JXKJ_2007@163.com
//############################################################
#ifndef _STATE_MACHINE_H
#define _STATE_MACHINE_H

#include "stm32f10x.h"
 
typedef struct {
	    uint8_t    Control_Mode;   // 控制模式  
	    uint8_t    TripFlagDMC;    // 过流 保护标志
	    uint8_t    drive_car;      // 开始驱动电机状态
	    uint8_t    olddrive_car;   // 历史开始驱动电机状态
	    uint8_t    clear_PWMtripz; // 清除保护标志
	    uint8_t    Run_mode;       // 正反转运行状态
	    uint16_t   Qiehuan_count;  // 切换状态的计数值
    
	    uint8_t    Start_order;    // 启动PWM，启动电机
    
	    uint16_t   Duty;           // 切换状态的计数值
	    uint16_t   Speed_Count;    // 速度环计时
	    uint16_t   Current_Count;  // 电流环计时	
		uint16_t   Aim_Speed;
		uint32_t   Aim_Duty;
        uint8_t    Zero_Check_Flag; //零电检测ok
	   	 }State; 

#define  State_DEFAULTS  {0,0,0,0,0,0,0,1,0,0,0,0,0,0} // 初始化参数

void Key_Scanning(void);
void Key_Gpio(void);
void OLED_Display(void);
#endif
