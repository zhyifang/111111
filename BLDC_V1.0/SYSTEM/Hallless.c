//############################################################
//Created on: 2019年5月18日
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//版权所有，盗版必究
//STM32电机控制开发板
//匠心科技
//网址: https://shop298362997.taobao.com/index.htm?spm=2013.1.w5002-19016181214.2.53fe2393VNXwgW
//电机控制答疑邮箱：JXKJ_2007@163.com
//############################################################
#include "Hallless.h"
#include "State_Machine.h"
#include "Tim1_PWM.h"
#include "Control stage parameters.h"
#include "Drive parameters.h"
#include "PMSM motor parameters.h"
#include "PI_Cale.h"
#ifdef HALLLESS

extern    State      StateContr;
extern    Hallless   Hallless_Three;

void ThreeHalllessPara_init(void)
{
	Hallless_Three.Poles = MOTOR_POLES;  
	Hallless_Three.speed_coeff = 16000 *60 / Hallless_Three.Poles / 2 ;  
}

unsigned char UEMF_Edge( unsigned char val)
{
	static  unsigned char oldval=0;
	
	if(oldval != val)
	{
		oldval = val;
		if(val == 0) return 0;  
		else return 1;			
	}
	return 2;  
}

unsigned char VEMF_Edge( unsigned char val)
{
	static  unsigned char oldval=0;
	
	if(oldval != val)
	{
		oldval = val;
		if(val == 0) return 0; 
		else return 1;		
	}
	return 2;  
}

unsigned char WEMF_Edge( unsigned char val)
{
	static  unsigned char oldval=0;
	
	if(oldval != val)
	{
		oldval = val;
		if(val == 0) return 0;  
		else return 1;		
	}
	return 2;  
}

void Hallless_SW(void)   //无感hall的六步换向 
{
	Hallless_Three.Queue_Status[0] = Hallless_Three.Queue_Status[0] << 1;
	Hallless_Three.Queue_Status[1] = Hallless_Three.Queue_Status[1] << 1;	
	Hallless_Three.Queue_Status[2] = Hallless_Three.Queue_Status[2] << 1;
	
	Hallless_Three.Queue_Status[0] |= HALLLESS_U_STATUS;   //GPIO_ReadInputDataBit(HALLLESS_A_GPIO_PORT,HALLLESS_A_GPIO_PIN)
	Hallless_Three.Queue_Status[1] |= HALLLESS_V_STATUS;   //GPIO_ReadInputDataBit(HALLLESS_B_GPIO_PORT,HALLLESS_B_GPIO_PIN)
	Hallless_Three.Queue_Status[2] |= HALLLESS_W_STATUS;   //GPIO_ReadInputDataBit(HALLLESS_C_GPIO_PORT,HALLLESS_C_GPIO_PIN)
	
	Hallless_Three.Filter_Math = Hallless_Three.Queue_Status[0] & FILTER_LONG;  //0xFF==1111 1111
	if(Hallless_Three.Filter_Math == FILTER_LONG)
	{
		Hallless_Three.QFilter_Status[0] = 1;
	}
	else if(Hallless_Three.Filter_Math == 0x00)
	{
		Hallless_Three.QFilter_Status[0] = 0;
	}
	else
	{
		return;
	}
	
	Hallless_Three.Filter_Math = Hallless_Three.Queue_Status[1] & FILTER_LONG;
	if(Hallless_Three.Filter_Math == FILTER_LONG)
	{
		Hallless_Three.QFilter_Status[1] = 1;
	}
	else if(Hallless_Three.Filter_Math == 0x00)
	{
		Hallless_Three.QFilter_Status[1] = 0;
	}
	else
	{
		return;
	}
	
	Hallless_Three.Filter_Math = Hallless_Three.Queue_Status[2] & FILTER_LONG;
	if(Hallless_Three.Filter_Math == FILTER_LONG)
	{
		Hallless_Three.QFilter_Status[2] = 1;
	}
	else if(Hallless_Three.Filter_Math == 0x00)
	{
		Hallless_Three.QFilter_Status[2] = 0;
	}
	else
	{
		return;
	}
	
    Hallless_Three.Hallless_State = Hallless_Three.QFilter_Status[0] 
                                   +(Hallless_Three.QFilter_Status[1]<<1) 
                                   +(Hallless_Three.QFilter_Status[2]<<2);
	
    
	Hallless_Three.Filter_Edge = UEMF_Edge(Hallless_Three.QFilter_Status[0]);
    
    if(Hallless_Three.Filter_Edge == 0)
	{
		Hallless_Three.Filter_Count = 0;
	}
	if(Hallless_Three.Filter_Edge == 2)
	{
		Hallless_Three.Filter_Count++;
	}
	if(Hallless_Three.Filter_Edge == 1)
	{
	//延迟30°换相，因为硬件上低通滤波器和软件延迟的原因，实际延迟角度为24°左右，
	//最优解可以通过示波器确定。通过PA4查看实际延迟角度
		FirstOrder_LPF_Cacl(Hallless_Three.Filter_Count , Hallless_Three.Filter_CountF ,0.1379);
		Hallless_Three.Speed_RPM = Hallless_Three.speed_coeff/Hallless_Three.Filter_CountF;
        
        
		FirstOrder_LPF_Cacl(Hallless_Three.Speed_RPM , Hallless_Three.Speed_RPMF ,0.2379);
		Hallless_Three.Filter_Delay = Hallless_Three.Filter_CountF / 6;            //时间延时。
		Hallless_Three.Filter_Count = 0;
	}

    
    
    
	

	if(Hallless_Three.Hallless_State!=Hallless_Three.OldHallless_State)
	{
		Hallless_Three.Filter_Count_All++;
	}
	//调试延迟30°函数
	//Debug_HALLLESS_OFF;
	if(Hallless_Three.Filter_Count_All >= Hallless_Three.Filter_Delay)
	{
	//调试延迟30°函数
	//Debug_HALLLESS_ON;
	     Hallless_Three.Filter_Count_All = 0;
	    if(Hallless_Three.Hallless_State!=Hallless_Three.OldHallless_State)
	    {

	        switch(Hallless_Three.Hallless_State)
	       {
		       case  0x5:     
		      {
		        MOS_Q41PWM();    //AB
		      }
		       break;
		     case  0x1:    
		     {
		        MOS_Q16PWM();    //AC
		      }
		      break;
		      case  0x3:    
		     {
		        MOS_Q63PWM();    //BC
		      }
		      break;
		     case 0x2:  
		    {
		        MOS_Q32PWM();    //BA
		    }
		      break;
		     case 0x6:   
		    {
		      MOS_Q25PWM();      //CA
		    }
		     break;
		    case 0x4:
		   {
		     MOS_Q54PWM();       //CB
		   }
		     break;
		     default:
		   {
		     Stop_Motor();
		     Hallless_Three.Speed_RPM=0;
		   }
		    break;
	     }

	    }	
	     Hallless_Three.OldHallless_State=Hallless_Three.Hallless_State ;
	}
}




int32_t count_temp=0;
void Get_Hall_EMC_state(void)
{
 
    
    
    Hallless_Three.Queue_Status[0] = Hallless_Three.Queue_Status[0] << 1;
	Hallless_Three.Queue_Status[1] = Hallless_Three.Queue_Status[1] << 1;	
	Hallless_Three.Queue_Status[2] = Hallless_Three.Queue_Status[2] << 1;
	
	Hallless_Three.Queue_Status[0] |= HALLLESS_U_STATUS;   //GPIO_ReadInputDataBit(HALLLESS_A_GPIO_PORT,HALLLESS_A_GPIO_PIN)
	Hallless_Three.Queue_Status[1] |= HALLLESS_V_STATUS;   //GPIO_ReadInputDataBit(HALLLESS_B_GPIO_PORT,HALLLESS_B_GPIO_PIN)
	Hallless_Three.Queue_Status[2] |= HALLLESS_W_STATUS;   //GPIO_ReadInputDataBit(HALLLESS_C_GPIO_PORT,HALLLESS_C_GPIO_PIN)
	
	Hallless_Three.Filter_Math = Hallless_Three.Queue_Status[0] & FILTER_LONG;  //0xFF==1111 1111
	if(Hallless_Three.Filter_Math == FILTER_LONG)                               //0xFF==1111 1111
	{
		Hallless_Three.QFilter_Status[0] = 1;
	}
	else if(Hallless_Three.Filter_Math == 0x00)
	{
		Hallless_Three.QFilter_Status[0] = 0;
	}
	else
	{
		return;
	}
	
	Hallless_Three.Filter_Math = Hallless_Three.Queue_Status[1] & FILTER_LONG;
	if(Hallless_Three.Filter_Math == FILTER_LONG)//0xFF==1111 1111
	{
		Hallless_Three.QFilter_Status[1] = 1;
	}
	else if(Hallless_Three.Filter_Math == 0x00)
	{
		Hallless_Three.QFilter_Status[1] = 0;
	}
	else
	{
		return;
	}
	
	Hallless_Three.Filter_Math = Hallless_Three.Queue_Status[2] & FILTER_LONG;
	if(Hallless_Three.Filter_Math == FILTER_LONG)
	{
		Hallless_Three.QFilter_Status[2] = 1;
	}
	else if(Hallless_Three.Filter_Math == 0x00)
	{
		Hallless_Three.QFilter_Status[2] = 0;
	}
	else
	{
		return;
	}	
    
   Hallless_Three.Hallless_State =  (Hallless_Three.QFilter_Status[0]<<2)    
                                   +(Hallless_Three.QFilter_Status[1]<<1) 
                                   +(Hallless_Three.QFilter_Status[2]);   //ABC=100, 110 ,010 ,011 , 001 ,101(462315)
    
    
    
    
    //判断hall状态，
    if( Hallless_Three.OldHallless_State==Hallless_Three.Hallless_State)
    {
        count_temp++;
        
    }
    else
    {
        Hallless_Three.step_angle=count_temp;
        count_temp=0;
    }
    
    
   //状态切换 
     switch(Hallless_Three.Hallless_State)
	 {
         case  0x5:   
//         case  0x5:        
         {
		        MOS_Q41PWM();    //AB
         }   
         break;
		    
       case  0x4: 
//         case  0x1:      
         {
		        MOS_Q16PWM();    //AC
         } 
         break;
         case  0x6:  
//         case  0x3:    
		    
         {
		        MOS_Q63PWM();    //BC
		    
         }
		     
         break;
         case  0x2: 
//         case 0x2:  
		  
         {
		   
           
             MOS_Q32PWM();    //BA
		   
         }
         
		   
         break;
         case  0x3: 
//         case 0x6:   
		   
         {
		      MOS_Q25PWM();      //CA
		   
         }
		   
         break;
         case  0x1:   
//         case 0x4:
		   
         {
		     MOS_Q54PWM();       //CB
		  
         }
		   
         break;
		   
         default:
		   
         {
		     Stop_Motor();
		     Hallless_Three.Speed_RPM=0;
		  
         } 
         break;
	     
     }
    
    
    
    
   Hallless_Three.OldHallless_State=Hallless_Three.Hallless_State ;

}

#endif

