#ifndef _Hallless_H
#define _Hallless_H

#include "stm32f10x.h"


unsigned char UEMF_Edge( unsigned char val);
unsigned char VEMF_Edge( unsigned char val);
unsigned char WEMF_Edge( unsigned char val);
typedef struct {
	    uint8_t       HalllessUVW[3];       //读取三个反电动势的对应状态
	    uint8_t       Hallless_State;       //当前反电动势状态 
	    uint8_t       OldHallless_State;    //历史反电动势状态
		int32_t       step_angle_error;     //步进角度误差
	    int32_t       step_angle ;          //步进角度 
		int32_t       step_angleFitter ;    //步进角度 滤波值
	    uint16_t      Speed_count;          //速度计数值
	    uint16_t      Speed_countFitter;    //速度计数滤波值
        uint16_t      Speed_count_old;      //速度计数历史值
		uint32_t      speed_coeff;          //速度系数
	    uint8_t       Poles;                //电机极对数
	    uint8_t       Move_State;           //电机旋转状态
        uint16_t      Speed_RPM;              //电机旋转速度 
        uint16_t      Speed_RPMF;	            //电机旋转滤波速度
			uint16_t      Queue_Status[3];	    //滤波
			uint16_t      Filter_Count;	        //滤波计数
			uint16_t      Filter_CountF;	    //滤波计数
			uint16_t      Filter_Math;	        //滤波
			uint16_t      QFilter_Status[3];	//滤波
			uint16_t      Filter_Edge;	        //滤波
			uint16_t      Filter_Delay;	        //滤波
			uint16_t      Filter_Count_All;	    //滤波计数
	   } Hallless;

#define  Hallless_DEFAULTS {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}  // 初始化参数
void Get_Hall_EMC_state(void);
void ThreeHalllessPara_init(void);
void Hallless_SW(void);
#endif
