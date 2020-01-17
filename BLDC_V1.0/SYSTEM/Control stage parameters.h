//############################################################
// Created on: 2019年5月18日
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//版权所有，盗版必究
//STM32电机控制开发板
//匠心科技
//网址: https://shop298362997.taobao.com/index.htm?spm=2013.1.w5002-19016181214.2.53fe2393VNXwgW
//电机控制答疑邮箱：JXKJ_2007@163.com
//############################################################
#ifndef __CONTROL_STAGE_PARAMETERS_H
#define __CONTROL_STAGE_PARAMETERS_H


#define PHASE_U_CURR_CHANNEL            ADC_Channel_6
#define PHASE_U_GPIO_PORT               GPIOA
#define PHASE_U_GPIO_PIN                GPIO_Pin_6

#define PHASE_V_CURR_CHANNEL            ADC_Channel_7
#define PHASE_V_GPIO_PORT               GPIOA
#define PHASE_V_GPIO_PIN                GPIO_Pin_7

#define PHASE_W_CURR_CHANNEL            ADC_Channel_14
#define PHASE_W_GPIO_PORT               GPIOC
#define PHASE_W_GPIO_PIN                GPIO_Pin_4 

#define BusVolt_CHANNEL                 ADC_Channel_11
#define BusVolt_GPIO_PORT   		    GPIOC
#define BusVolt_GPIO_PIN     		    GPIO_Pin_1

#define POT_CHANNEL                     ADC_Channel_8
#define POT_GPIO_PORT        		    GPIOB
#define POT_GPIO_PIN         		    GPIO_Pin_0

#define PHASE_UH_GPIO_PORT              GPIOA                                                                            
#define PHASE_UH_GPIO_PIN               GPIO_Pin_8 

#define PHASE_VH_GPIO_PORT              GPIOA                                                                                                                                                    
#define PHASE_VH_GPIO_PIN               GPIO_Pin_9

#define PHASE_WH_GPIO_PORT              GPIOA                                                                         
#define PHASE_WH_GPIO_PIN               GPIO_Pin_10

#define PHASE_UL_GPIO_PORT              GPIOB                                                                        
#define PHASE_UL_GPIO_PIN               GPIO_Pin_13

#define PHASE_VL_GPIO_PORT              GPIOB                                                                
#define PHASE_VL_GPIO_PIN               GPIO_Pin_14

#define PHASE_WL_GPIO_PORT              GPIOB                                                        
#define PHASE_WL_GPIO_PIN               GPIO_Pin_15

#define HALL_A_GPIO_PORT                GPIOC                                                                        
#define HALL_A_GPIO_PIN                 GPIO_Pin_6

#define HALL_B_GPIO_PORT                GPIOC                                                                
#define HALL_B_GPIO_PIN                 GPIO_Pin_7

#define HALL_C_GPIO_PORT                GPIOC                                                        
#define HALL_C_GPIO_PIN                 GPIO_Pin_8

#define HALLLESS_A_GPIO_PORT            GPIOC                                                                        
#define HALLLESS_A_GPIO_PIN             GPIO_Pin_10

#define HALLLESS_B_GPIO_PORT            GPIOC                                                                
#define HALLLESS_B_GPIO_PIN             GPIO_Pin_11

#define HALLLESS_C_GPIO_PORT            GPIOC                                                        
#define HALLLESS_C_GPIO_PIN             GPIO_Pin_12

#define HALLLESS_U_STATUS               GPIO_ReadInputDataBit(HALLLESS_A_GPIO_PORT,HALLLESS_A_GPIO_PIN)
#define HALLLESS_V_STATUS              	GPIO_ReadInputDataBit(HALLLESS_B_GPIO_PORT,HALLLESS_B_GPIO_PIN)
#define HALLLESS_W_STATUS               GPIO_ReadInputDataBit(HALLLESS_C_GPIO_PORT,HALLLESS_C_GPIO_PIN)

#define HALL_U_STATUS                   GPIO_ReadInputDataBit(HALL_A_GPIO_PORT,HALL_A_GPIO_PIN)
#define HALL_V_STATUS                   GPIO_ReadInputDataBit(HALL_B_GPIO_PORT,HALL_B_GPIO_PIN)
#define HALL_W_STATUS                   GPIO_ReadInputDataBit(HALL_C_GPIO_PORT,HALL_C_GPIO_PIN)

#define RUN_GPIO_PORT            		GPIOC                                                                        
#define RUN_GPIO_PIN             		GPIO_Pin_5

#define STOP_GPIO_PORT            		GPIOB                                                                
#define STOP_GPIO_PIN             		GPIO_Pin_1

#define UP_GPIO_PORT            		GPIOB                                                        
#define UP_GPIO_PIN             		GPIO_Pin_10

#define DOWN_GPIO_PORT            		GPIOB                                                                
#define DOWN_GPIO_PIN           		GPIO_Pin_11

#define DIR_GPIO_PORT           		GPIOB                                                        
#define DIR_GPIO_PIN             		GPIO_Pin_12

#define RUN_STATUS                      GPIO_ReadInputDataBit(RUN_GPIO_PORT,RUN_GPIO_PIN)
#define STOP_STATUS                     GPIO_ReadInputDataBit(STOP_GPIO_PORT,STOP_GPIO_PIN)
#define UP_STATUS                       GPIO_ReadInputDataBit(UP_GPIO_PORT,UP_GPIO_PIN)
#define DOWN_STATUS                     GPIO_ReadInputDataBit(DOWN_GPIO_PORT,DOWN_GPIO_PIN)
#define DIR_STATUS                      GPIO_ReadInputDataBit(DIR_GPIO_PORT,DIR_GPIO_PIN)

#define Debug_HALLLESS_PORT           	GPIOA                                                        
#define Debug_HALLLESS_PIN             	GPIO_Pin_4
#define Debug_HALLLESS_ON 		        GPIO_SetBits(GPIOA, GPIO_Pin_4 )  	  
#define Debug_HALLLESS_OFF 	            GPIO_ResetBits(GPIOA, GPIO_Pin_4 )

#endif
