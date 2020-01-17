//############################################################
// Created on: 2019年5月18日
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//版权所有，盗版必究
//STM32电机控制开发板
//匠心科技
//网址: https://shop298362997.taobao.com/index.htm?spm=2013.1.w5002-19016181214.2.53fe2393VNXwgW
//电机控制答疑邮箱：JXKJ_2007@163.com
//############################################################
#include "ADC_int.h"
#include "Hall.h"
#include "Control stage parameters.h"
#include "State_Machine.h"
#include "Hallless.h"
#include "Drive parameters.h"

uint32_t  ADC_ConvertedValue[5]={0,0,0,0,0};//初始化AD采样值

extern volatile uint16_t ADC_DualConvertedValueTab[5];    //调用时重新取值

extern    State       StateContr;
extern    ADCSamp     ADCSampPare;

#ifdef HALL
extern    Hall        Hall_Three;
#endif

#ifdef HALLLESS
extern    Hallless   Hallless_Three;
#endif
void Adc_Configuration(void)
{
	ADC_InitTypeDef       ADC_InitStructure;	
//	DMA_InitTypeDef       DMA_InitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1|RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
//	RCC_ADCCLKConfig(RCC_PCLK2_Div6);  
	
	GPIO_InitStructure.GPIO_Pin = PHASE_U_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(PHASE_U_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PHASE_V_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(PHASE_V_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PHASE_W_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(PHASE_W_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = BusVolt_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(BusVolt_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = POT_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(POT_GPIO_PORT, &GPIO_InitStructure);
	
	ADC_StructInit(&ADC_InitStructure);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  ////触发沿为下降沿触发
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
//	ADC_InitStructure.ADC_NbrOfChannel = 4;     
	ADC_Init(ADC1, &ADC_InitStructure);

//	ADC_RegularChannelConfig(ADC1, PHASE_U_CURR_CHANNEL, 1, ADC_SampleTime_1Cycles5);
//	ADC_RegularChannelConfig(ADC1, PHASE_V_CURR_CHANNEL, 2, ADC_SampleTime_1Cycles5);
//	ADC_RegularChannelConfig(ADC1, PHASE_W_CURR_CHANNEL, 3, ADC_SampleTime_1Cycles5);
//    
//  ADC_RegularChannelConfig(ADC1, BusVolt_CHANNEL, 4, ADC_SampleTime_1Cycles5); 
//	ADC_RegularChannelConfig(ADC1, POT_CHANNEL, 4, ADC_SampleTime_1Cycles5); 
    
    ADC_ExternalTrigInjectedConvConfig(ADC1,ADC_ExternalTrigInjecConv_T1_TRGO);//注入组触发源为TIM1的触发
	ADC_InjectedSequencerLengthConfig(ADC1,4);//注入组长度
	ADC_InjectedChannelConfig(ADC1, BusVolt_CHANNEL, 1, ADC_SampleTime_7Cycles5);//注入组采样序列2,VBUS

    ADC_InjectedChannelConfig(ADC1, PHASE_U_CURR_CHANNEL, 2, ADC_SampleTime_7Cycles5);//注入组采样序列2,VBUS
	ADC_InjectedChannelConfig(ADC1, PHASE_V_CURR_CHANNEL, 3, ADC_SampleTime_7Cycles5);//注入组采样序列3,TEMP
    ADC_InjectedChannelConfig(ADC1, PHASE_W_CURR_CHANNEL, 4, ADC_SampleTime_7Cycles5);
    
    ADC_Cmd(ADC1, ENABLE);
//	ADC_DMACmd(ADC1, ENABLE);
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
    
    while(ADC_GetCalibrationStatus(ADC1));
    ADC_ExternalTrigInjectedConvCmd(ADC1,ENABLE);
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE); 

	
	/*-------DMA配置,用于传输AD采样值---------*/
//	DMA_DeInit(DMA1_Channel1);
//	DMA_InitStructure.DMA_PeripheralBaseAddr = (unsigned int) &(ADC1->DR);
//	DMA_InitStructure.DMA_MemoryBaseAddr = (unsigned int)&ADC_DualConvertedValueTab;	 
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;

//	DMA_InitStructure.DMA_BufferSize =4;
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;

//	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
//	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
//	
//	DMA_Cmd(DMA1_Channel1, ENABLE);
    
}

void Offset_CurrentReading(void)  
{

	static uint8_t i=0;
    uint32_t sum_U=0;
	uint32_t sum_V=0;
	uint32_t sum_W=0;
  /* ADC Channel used for current reading are read  in order to get zero currents ADC values*/
    for(i=0;i<8;i++)
	{
        while(!(ADC_GetFlagStatus(ADC1,ADC_FLAG_JEOC))){};
            sum_U += ADC1->JDR2;
			sum_V += ADC1->JDR3;
			sum_W += ADC1->JDR4;
        ADC_ClearFlag(ADC1, ADC_FLAG_JEOC);
	}
    ADCSampPare.OffsetPhaseU_Curr = sum_U /8;
    ADCSampPare.OffsetPhaseV_Curr = sum_V /8;
	ADCSampPare.OffsetPhaseW_Curr = sum_W /8;
}
#ifdef HALL
void HALL_ADC_Sample(void) // 放在PWM中断进来后，采样时间为1us左右,(7cycles)  保证在PWM中断后进来采样后为在PWM中间采集相电流
{ 
    ADCSampPare.PhaseU_Curr  =    (ADC_DualConvertedValueTab[0]-ADCSampPare.OffsetPhaseU_Curr)<<1;	
	ADCSampPare.PhaseV_Curr  =    (ADC_DualConvertedValueTab[1]-ADCSampPare.OffsetPhaseV_Curr)<<1;
	ADCSampPare.PhaseW_Curr  =    (ADC_DualConvertedValueTab[2]-ADCSampPare.OffsetPhaseW_Curr)<<1;   
    ADCSampPare.BUS_Voltage  =     ADC_DualConvertedValueTab[3];	
	
//    
//    if(Hall_Three.Hall_State == 0x05)
//	{
//	   ADCSampPare.BUS_Curr = ADCSampPare.PhaseV_Curr;
//       
//	}
//	else if(Hall_Three.Hall_State == 0x04)
//	{
//	   ADCSampPare.BUS_Curr = ADCSampPare.PhaseW_Curr;
//        
//	}
//	else if(Hall_Three.Hall_State == 0x06)
//	{
//	   ADCSampPare.BUS_Curr =  ADCSampPare.PhaseW_Curr;
//        
//	}
//	else if(Hall_Three.Hall_State == 0x02)
//	{
//	   ADCSampPare.BUS_Curr = ADCSampPare.PhaseU_Curr + ADCSampPare.PhaseV_Curr;
//        
//	}
//	else if(Hall_Three.Hall_State == )
//	{
//	   ADCSampPare.BUS_Curr = ADCSampPare.PhaseU_Curr + ADCSampPare.PhaseV_Curr;
//        
//	}
//	else if(Hall_Three.Hall_State == )
//	{
//	   ADCSampPare.BUS_Curr = ADCSampPare.PhaseW_Curr + ADCSampPare.PhaseV_Curr;
//        
//	}
}
#endif

#ifdef HALLLESS

void HALLLESS_ADC_Sample(u8 temp) // 放在PWM中断进来后，采样时间为1us左右,(7cycles)  保证在PWM中断后进来采样后为在PWM中间采集相电流
{    
    ADCSampPare.PhaseU_Curr  =    (ADC1->JDR2-ADCSampPare.OffsetPhaseU_Curr);	 //683
	ADCSampPare.PhaseV_Curr  =    (ADC1->JDR3-ADCSampPare.OffsetPhaseV_Curr);     //683
	ADCSampPare.PhaseW_Curr  =    (ADC1->JDR4-ADCSampPare.OffsetPhaseW_Curr);      //683
    ADCSampPare.BUS_Voltage  =    ADC1->JDR1;	
	
    
    if(temp == 0x05)
	{
	   ADCSampPare.BUS_Curr = ADCSampPare.PhaseU_Curr;
	}
	else if(temp == 0x04)
	{
	   ADCSampPare.BUS_Curr = ADCSampPare.PhaseW_Curr;
	}
	else if(temp == 0x06)
	{
	   ADCSampPare.BUS_Curr = ADCSampPare.PhaseW_Curr;
	}
	else if(temp == 0x02)
	{
	   ADCSampPare.BUS_Curr = ADCSampPare.PhaseU_Curr ;
	}
	else if(temp == 0x03)
	{
	   ADCSampPare.BUS_Curr = ADCSampPare.PhaseU_Curr ;
	}
	else if(temp == 0x01)
	{
	   ADCSampPare.BUS_Curr = ADCSampPare.PhaseV_Curr;
	}
}


u16 Caculate_CurrentZeroCalibration(char times)
{
	u16 CurrentSum = 0;
//char i;

	return (CurrentSum/times);
}

//u16 Caculate_10TimesCurrent(u16 ADC_Value)
//{

//   return ((((ADC_Value*396)>>12)-198));  //原创程序放大10倍  
//}


u16 Caculate_10TimesVbus(u16 ADC_Value)
{
    return (((ADC_Value)*33*21)>>12);
}


#endif
