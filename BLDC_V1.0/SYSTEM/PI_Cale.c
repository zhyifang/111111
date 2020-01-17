//############################################################
// Created on: 2019年5月18日
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//版权所有，盗版必究
//STM32电机控制开发板
//匠心科技
//网址: https://shop298362997.taobao.com/index.htm?spm=2013.1.w5002-19016181214.2.53fe2393VNXwgW
//电机控制答疑邮箱：JXKJ_2007@163.com
//############################################################
#include "PI_Cale.h"
#include "Drive parameters.h"

extern    PIDREG_T       pi_spd;
extern    PIDREG_T       pi_ICurr;

void PID_init(void)
{
  pi_spd.Kp = Speed_Kp;    
  pi_spd.Ki = Speed_Ki;  
	pi_spd.Kc = Speed_Kc;
  pi_spd.OutMax =4500;
#ifdef HALLLESS	
  pi_spd.OutMin =500;
#endif	
#ifdef HALL
  pi_spd.OutMin =500;
#endif	
	pi_spd.Ref = 0;

  pi_ICurr.Kp = Current_Kp;    
  pi_ICurr.Ki = Current_Ki;  
	pi_ICurr.Kc = Current_Kc;
  pi_ICurr.OutMax =4500; 
  pi_ICurr.OutMin =300;
	pi_ICurr.Ref = 0;	
	
 }


