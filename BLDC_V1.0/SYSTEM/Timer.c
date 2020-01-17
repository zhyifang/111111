//############################################################
// Created on: 2019年5月18日
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//版权所有，盗版必究
//STM32电机控制开发板
//匠心科技
//网址: https://shop298362997.taobao.com/index.htm?spm=2013.1.w5002-19016181214.2.53fe2393VNXwgW
//电机控制答疑邮箱：JXKJ_2007@163.com
//############################################################
#include "Timer.h"

void SysTickConfig(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
  if (SysTick_Config(SystemCoreClock /100))   
  {	
    while (1);
  }
  NVIC_SetPriority(SysTick_IRQn, 0x0);
}


void TIM2_Init(void)
{

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_DeInit(TIM2);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
    /*-------转速计算 定时器配置---------*/ 
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;   //重载值65535
	TIM_TimeBaseStructure.TIM_Prescaler = 71;    //47 分频系数48分频，1us计数一次，最大可计65.535ms
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM2->CNT = 0;//计数值清零
	TIM_Cmd(TIM2, ENABLE); 
    
}

void TIM3_Init(void)
{
 
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef         NVIC_InitStructure;
    TIM_DeInit(TIM3);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  
    /*-------用于换相 定时器配置---------*/
	TIM_TimeBaseStructure.TIM_Period = 0;         //初始化重载值设为0 			
	TIM_TimeBaseStructure.TIM_Prescaler = 71;     //分频系数48分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
    /*-------用于换相 定时器中断配置---------*/ 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//优先级配置
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM3,DISABLE);                                   //先失能定时器
}


void TIM3_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//清除中断标志
        
        
        
        
    }
}
