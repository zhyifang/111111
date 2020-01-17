#ifndef _Hall_H
#define _Hall_H

#include "stm32f10x.h"

unsigned char UHALL_Edge( unsigned char val);
void Hall_SW(void);
void ThreeHallPara_init(void);

typedef struct {
	    uint8_t       HallUVW[3]; // 读取三个霍尔的对应状态
	    uint8_t       Hall_State; //当前霍尔状态
	    uint8_t       OldHall_State; // 历史霍尔状态
			int32_t       step_angle_error;  //步进角度误差
	    int32_t       step_angle ;  //步进角度 
			int32_t       step_angleFitter ; //步进角度 滤波值
	    uint16_t      Speed_count;   //速度计数值
	    uint16_t      Speed_countFitter; //速度计数滤波值
      uint16_t      Speed_count_old;  //速度计数历史值
			uint32_t      speed_coeff;    //速度系数
	    uint8_t       Poles;    //电机极对数
	    uint8_t       Move_State;       //电机旋转状态
      uint16_t      Speed_RPM;     //电机旋转速度 
      uint16_t      Speed_RPMF;	   //电机旋转滤波速度
			uint16_t      Filter_Edge;	
			uint16_t      Filter_Count;
			uint16_t    	Filter_CountF;
			uint16_t      Queue_Status[3];
			uint16_t      Filter_Math;
			uint16_t      QFilter_Status[3];
	   } Hall;
		 
#define  Hall_DEFAULTS {0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0} // 参数初始化		 

#endif
