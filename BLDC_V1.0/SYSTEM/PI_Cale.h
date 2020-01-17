//############################################################
// Created on: 2019年5月18日
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//版权所有，盗版必究
//STM32电机控制开发板
//匠心科技
//网址: https://shop298362997.taobao.com/index.htm?spm=2013.1.w5002-19016181214.2.53fe2393VNXwgW
//电机控制答疑邮箱：JXKJ_2007@163.com
//############################################################
#ifndef  PI_Cale_H
#define  PI_Cale_H

#include "stm32f10x.h"
void PID_init(void);
typedef struct 
{  
				float  Ref;   		// Input: Reference input 
				float  Fdb;   		// Input: Feedback input 
				float  Err;			  // Variable: Error 
				float  Kp;			  // Parameter: Proportional gain
				float  Up;			  // Variable: Proportional output 
				float  Ui;			  // Variable: Integral output 
				float  Ud;			  // Variable: Derivative output	
				float  OutPreSat;	// Variable: Pre-saturated output
				float  OutMax;		// Parameter: Maximum output 
				float  OutMin;		// Parameter: Minimum output
				float  Out;   		// Output: PID output 
				float  SatErr;		// Variable: Saturated difference
				float  Ki;			  // Parameter: Integral gain
				float  Kc;			  // Parameter: Integral correction gain
				float  Kd; 			  // Parameter: Derivative gain 
				float  Up1;			  // History: Previous proportional output
				float  Ui_1; 
				float  OutF; 
} PIDREG_T ,*p_PIDREG_T ;;		 

#define PIDREG_T_DEFAULTS {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  // 初始化参数
#define   _IQmpy(A,B)         ((A) * (B))
#define FirstOrder_LPF_Cacl(Xn, Yn_1, a)\
										Yn_1 = (1-a)*Yn_1 + a*Xn; //Xn:in;Yn:out;a:系数
#define UP16LIMIT(var,max,min) {(var) = (var)>(max)?(max):(var) ;\
							    (var) = (var)<(min)?(min):(var) ;\
								}
#define PID_CALC(v)	\
							v.Err = v.Ref - v.Fdb; \
							v.Up= _IQmpy(v.Kp,v.Err);\
						    v.Ui= v.Ui + _IQmpy(v.Ki,v.Up);\
							UP16LIMIT(v.Ui,v.OutMax,v.OutMin);\
							v.Ud = v.Kd * (v.Up - v.Up1);\
							v.Out = v.Up + v.Ui + v.Ud;\
							UP16LIMIT(v.Out,v.OutMax,v.OutMin);\
							v.Up1 = v.Up;	

#endif
