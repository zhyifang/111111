/******************************************************************************
 * @file     motor_math.c
 * @version  V1.00
 * $Revision: 5 $
 * $Date: 15/04/01 5:51p $
 * @brief    Template project for BLDC motor MCU
 *
 * @note
 * Copyright (C)
*****************************************************************************/

#include "motor_math.h"
//#include "IQMathLib.h"






#if MATH_ATAN_TYPE_SEL == MATH_ATAN_TYPE_CORDIC
const int16_t angle[] = {8192, 4836, 2555, 1297, 651, 326, 163, 81, 41, 20, 10, 5, 3, 1, 1};
int16_t Cordic_atan2(int16_t y, int16_t x) 
{
    int i,i32X,i32Y;
    int x_new, y_new,result;
	  
    i32X=abs(x<<15);
    i32Y=(y<<15);
	  
	  result = 0;
		for(i = 0; i < 12; i++)
		{
			if(i32Y > 0)
			{
					x_new = i32X + (i32Y >> i);
					y_new = i32Y - (i32X >> i);

					result += angle[i];
			}
			else
			{
					x_new = i32X - (i32Y >> i);
					y_new = i32Y + (i32X >> i);
							
					result -= angle[i];
			}
			i32X = x_new;
			i32Y = y_new;
		}
		
		
		if((x < 0))
		{
        result = (int16_t)(I32_DEGREE_180 - result);
		}
    return result;
//    
}

#endif

int16_t SAT0(int16_t in,int16_t sat)
{
	if(in>=sat)
	{
		return sat;
	}
	
	if(in<=0)
	{
		return 0;
	}
	return in;
}

void SVPWM(int32_t i32_V_Alpha,int32_t i32_V_Beta,int32_t i32_PWM_Full_Scale,int16_t* pi32_PWM_A,int16_t* pi32_PWM_B,int16_t* pi32_PWM_C)	
{
  #define _IQSVPWM(x) ((int16_t)(((float)(x))*32767.0))
	//- 20161209 Maxwell modify to avoid overflow -> -//
	/*
	int16_t Va,Vb,Vc,t1,t2,temp_sv1,temp_sv2,Sector,Ta,Tb,Tc;
	*/
	//- 20161209 Maxwell modify to avoid overflow <- -//
	
	//- 20161209 Maxwell modify to avoid overflow -> -//
	int32_t Va,Vb,Vc,t1,t2,temp_sv1,temp_sv2,Sector,Ta,Tb,Tc;
	//- 20161209 Maxwell modify to avoid overflow <- -//
	
  Sector = 0;																	
  
	temp_sv1=_MultiplyQ15(C_1_OVER_2_Q15,i32_V_Beta);											
	temp_sv2=_MultiplyQ15(C_SQRT3_OVER_2_Q15,i32_V_Alpha);	/* 0.8660254 = sqrt(3)/2*/		
  
																				
/* Inverse clarke transformation */												
	Va = i32_V_Beta;																
	Vb = -temp_sv1 + temp_sv2;													
	Vc = -temp_sv1 - temp_sv2;			

	if (Va>(0)) Sector = 1;													
	if (Vb>(0)) Sector = Sector+2;											
	if (Vc>(0)) Sector = Sector+4;			
/* X,Y,Z (Va,Vb,Vc) calculations X = Va, Y = Vb, Z = Vc */						
	Va = -i32_V_Beta;																
	Vb = -temp_sv1 - temp_sv2;													
	Vc = -temp_sv1 + temp_sv2;		


/* Sector 0: this is special case for (Ualpha,Ubeta) = (0,0)*/					
																				
switch(Sector)																	
{																				
	case 0:																		
	Ta = _IQSVPWM(0.5);															
	Tb = _IQSVPWM(0.5);															
	Tc = _IQSVPWM(0.5);															
	break;																		
	case 1:   /*Sector 1: t1=Z and t2=Y (abc ---> Tb,Ta,Tc)*/					
	t1 = Vc;																	
	t2 = Vb;																	
	Tb = (_IQSVPWM(1)-t1-t2)>>1;	/* tbon = (1-t1-t2)/2	*/						
	Ta = Tb+t1;			 	/* taon = tbon+t1		*/						
	Tc = Ta+t2;			  	/* tcon = taon+t2		*/						
	break;																		
	case 2:	  /* Sector 2: t1=Y and t2=-X (abc ---> Ta,Tc,Tb)*/ 				
	t1 = Vb;																	
	t2 = -Va;																	
	Ta = (_IQSVPWM(1)-t1-t2)>>1;	/*  taon = (1-t1-t2)/2	*/						
	Tc = Ta+t1;				/*  tcon = taon+t1		*/						
	Tb = Tc+t2;				/*  tbon = tcon+t2		*/						
	break;																		
	case 3:	  /* Sector 3: t1=-Z and t2=X (abc ---> Ta,Tb,Tc)*/					
	t1 = -Vc;																	
	t2 = Va;																	
	Ta = (_IQSVPWM(1)-t1-t2)>>1;	/*  taon = (1-t1-t2)/2	*/						
	Tb = Ta+t1;				/*	tbon = taon+t1		*/						
	Tc = Tb+t2;				/*	tcon = tbon+t2		*/						
	break;																		
	case 4:	  /* Sector 4: t1=-X and t2=Z (abc ---> Tc,Tb,Ta)*/					
	t1 = -Va;																	
	t2 = Vc;																	
	Tc = (_IQSVPWM(1)-t1-t2)>>1;	/*	tcon = (1-t1-t2)/2	*/						
	Tb = Tc+t1;				/*	tbon = tcon+t1		*/						
	Ta = Tb+t2;				/*	taon = tbon+t2		*/						
	break;																		
	case 5:	  /* Sector 5: t1=X and t2=-Y (abc ---> Tb,Tc,Ta)*/					
	t1 = Va;																	
	t2 = -Vb;					/*	tbon = (1-t1-t2)/2	*/						
	Tb = (_IQSVPWM(1)-t1-t2)>>1;	/*	tcon = tbon+t1		*/						
	Tc = Tb+t1;				/*	taon = tcon+t2		*/						
	Ta = Tc+t2;																
	break;																		
	case 6:	  /* Sector 6: t1=-Y and t2=-Z (abc ---> Tc,Ta,Tb)*/				
	t1 = -Vb;																	
	t2 = -Vc;																	
	Tc = (_IQSVPWM(1)-t1-t2)>>1;	/*	tcon = (1-t1-t2)/2	*/						
	Ta = Tc+t1;				/*	taon = tcon+t1		*/						
	Tb = Ta+t2;				/*	tbon = taon+t2		*/						
	break;																		
}																				

	*pi32_PWM_A=_MultiplyQ15(Ta,i32_PWM_Full_Scale); 
	*pi32_PWM_B=_MultiplyQ15(Tb,i32_PWM_Full_Scale); 
	*pi32_PWM_C=_MultiplyQ15(Tc,i32_PWM_Full_Scale);

    *pi32_PWM_A=SAT0(*pi32_PWM_A,i32_PWM_Full_Scale);
	*pi32_PWM_B=SAT0(*pi32_PWM_B,i32_PWM_Full_Scale);
	*pi32_PWM_C=SAT0(*pi32_PWM_C,i32_PWM_Full_Scale);

	
}


void SVPWM_ForOverMod(int32_t i32_V_Alpha,int32_t i32_V_Beta,int32_t i32_PWM_Full_Scale,int16_t* pi32_PWM_A,int16_t* pi32_PWM_B,int16_t* pi32_PWM_C)	
{
  #define _IQSVPWM(x) ((int32_t)(((float)(x))*32767.0))
	//- 20161209 Maxwell modify to avoid overflow -> -//
	/*
	int16_t Va,Vb,Vc,t1,t2,temp_sv1,temp_sv2,Sector,Ta,Tb,Tc;
	*/
	//- 20161209 Maxwell modify to avoid overflow <- -//
	
	//- 20161209 Maxwell modify to avoid overflow -> -//
	int32_t Va,Vb,Vc,t1,t2,temp_sv1,temp_sv2,Sector,Ta,Tb,Tc;
	//- 20161209 Maxwell modify to avoid overflow <- -//
	
    Sector = 0;																	
  
	
	temp_sv1=_MultiplyQ15(C_1_OVER_2_Q15,i32_V_Beta);											
	temp_sv2=_MultiplyQ15(C_SQRT3_OVER_2_Q15,i32_V_Alpha);	/* 0.8660254 = sqrt(3)/2*/		
  
//	temp_sv1=_MultiplyQ15(C_1_OVER_SQRT3_Q15,i32_V_Beta);											
//	temp_sv2=i32_V_Alpha;	/* 0.8660254 = sqrt(3)/2*/		
	
																				
/* Inverse clarke transformation */												
	Va = i32_V_Beta;																
	Vb = -temp_sv1 + temp_sv2;													
	Vc = -temp_sv1 - temp_sv2;			

	if (Va>(0)) Sector = 1;													
	if (Vb>(0)) Sector = Sector+2;											
	if (Vc>(0)) Sector = Sector+4;			
/* X,Y,Z (Va,Vb,Vc) calculations X = Va, Y = Vb, Z = Vc */						
	Va = -i32_V_Beta;																
	Vb = -temp_sv1 - temp_sv2;													
	Vc = -temp_sv1 + temp_sv2;		

  Va = (Va*(int32_t)C_2_OVER_SQRT3_Q15) >> 15;
	Vb = (Vb*(int32_t)C_2_OVER_SQRT3_Q15) >> 15;
	Vc = (Vc*(int32_t)C_2_OVER_SQRT3_Q15) >> 15;

/* Sector 0: this is special case for (Ualpha,Ubeta) = (0,0)*/					
																				
switch(Sector)																	
{																				
	case 0:																		
	Ta = _IQSVPWM(0.5);															
	Tb = _IQSVPWM(0.5);															
	Tc = _IQSVPWM(0.5);															
	break;																		
	case 1:   /*Sector 1: t1=Z and t2=Y (abc ---> Tb,Ta,Tc)*/					
	t1 = Vc;																	
	t2 = Vb;																	
	Tb = (_IQSVPWM(1)-t1-t2)>>1;	/* tbon = (1-t1-t2)/2	*/						
	Ta = Tb+t1;			 	/* taon = tbon+t1		*/						
	Tc = Ta+t2;			  	/* tcon = taon+t2		*/						
	break;																		
	case 2:	  /* Sector 2: t1=Y and t2=-X (abc ---> Ta,Tc,Tb)*/ 				
	t1 = Vb;																	
	t2 = -Va;																	
	Ta = (_IQSVPWM(1)-t1-t2)>>1;	/*  taon = (1-t1-t2)/2	*/						
	Tc = Ta+t1;				/*  tcon = taon+t1		*/						
	Tb = Tc+t2;				/*  tbon = tcon+t2		*/						
	break;																		
	case 3:	  /* Sector 3: t1=-Z and t2=X (abc ---> Ta,Tb,Tc)*/					
	t1 = -Vc;																	
	t2 = Va;																	
	Ta = (_IQSVPWM(1)-t1-t2)>>1;	/*  taon = (1-t1-t2)/2	*/						
	Tb = Ta+t1;				/*	tbon = taon+t1		*/						
	Tc = Tb+t2;				/*	tcon = tbon+t2		*/						
	break;																		
	case 4:	  /* Sector 4: t1=-X and t2=Z (abc ---> Tc,Tb,Ta)*/					
	t1 = -Va;																	
	t2 = Vc;																	
	Tc = (_IQSVPWM(1)-t1-t2)>>1;	/*	tcon = (1-t1-t2)/2	*/						
	Tb = Tc+t1;				/*	tbon = tcon+t1		*/						
	Ta = Tb+t2;				/*	taon = tbon+t2		*/						
	break;																		
	case 5:	  /* Sector 5: t1=X and t2=-Y (abc ---> Tb,Tc,Ta)*/					
	t1 = Va;																	
	t2 = -Vb;					/*	tbon = (1-t1-t2)/2	*/						
	Tb = (_IQSVPWM(1)-t1-t2)>>1;	/*	tcon = tbon+t1		*/						
	Tc = Tb+t1;				/*	taon = tcon+t2		*/						
	Ta = Tc+t2;																
	break;																		
	case 6:	  /* Sector 6: t1=-Y and t2=-Z (abc ---> Tc,Ta,Tb)*/				
	t1 = -Vb;																	
	t2 = -Vc;																	
	Tc = (_IQSVPWM(1)-t1-t2)>>1;	/*	tcon = (1-t1-t2)/2	*/						
	Ta = Tc+t1;				/*	taon = tcon+t1		*/						
	Tb = Ta+t2;				/*	tbon = taon+t2		*/						
	break;																		
}																				

	*pi32_PWM_A=_MultiplyQ15(Ta,i32_PWM_Full_Scale); 
	*pi32_PWM_B=_MultiplyQ15(Tb,i32_PWM_Full_Scale); 
	*pi32_PWM_C=_MultiplyQ15(Tc,i32_PWM_Full_Scale);

  *pi32_PWM_A=SAT0(*pi32_PWM_A,i32_PWM_Full_Scale);
	*pi32_PWM_B=SAT0(*pi32_PWM_B,i32_PWM_Full_Scale);
	*pi32_PWM_C=SAT0(*pi32_PWM_C,i32_PWM_Full_Scale);

}

void OverModand5SVPWM(int32_t i32_V_Alpha,int32_t i32_V_Beta,int32_t i32_PWM_Full_Scale,int16_t* pi32_PWM_A,int16_t* pi32_PWM_B,int16_t* pi32_PWM_C)	
{
	int32_t Va,Vb,Vc,t1,t2,temp_sv1,temp_sv2,Sector,Ta,Tb,Tc;
	
  Sector = 0;																	
 
	temp_sv1=_MultiplyQ15(C_1_OVER_2_Q15,i32_V_Beta);											
	temp_sv2=_MultiplyQ15(C_SQRT3_OVER_2_Q15,i32_V_Alpha);	/* 0.8660254 = sqrt(3)/2*/		
  
//	temp_sv1=_MultiplyQ15(C_1_OVER_SQRT3_Q15,i32_V_Beta);											
//	temp_sv2=i32_V_Alpha;	/* 0.8660254 = sqrt(3)/2*/		
	
																				
/* Inverse clarke transformation */												
	Va = i32_V_Beta;																
	Vb = -temp_sv1 + temp_sv2;													
	Vc = -temp_sv1 - temp_sv2;			

	if (Va>(0)) Sector = 1;													
	if (Vb>(0)) Sector = Sector+2;											
	if (Vc>(0)) Sector = Sector+4;			
	
	
	
/* X,Y,Z (Va,Vb,Vc) calculations X = Va, Y = Vb, Z = Vc */						
	Va = -i32_V_Beta;																
	Vb = -temp_sv1 - temp_sv2;													
	Vc = -temp_sv1 + temp_sv2;		

  Va = (Va*(int32_t)C_2_OVER_SQRT3_Q15) >> 15;
	Vb = (Vb*(int32_t)C_2_OVER_SQRT3_Q15) >> 15;
	Vc = (Vc*(int32_t)C_2_OVER_SQRT3_Q15) >> 15;

/* Sector 0: this is special case for (Ualpha,Ubeta) = (0,0)*/					
//	printf("Sector=%d\n",Sector);																			
switch(Sector)																	
{																				
	case 0:																		
	Ta = _IQSVPWM(0.5);															
	Tb = _IQSVPWM(0.5);															
	Tc = _IQSVPWM(0.5);															
	break;																		
	case 1:   /*Sector 1: t1=Z and t2=Y (abc ---> Tb,Ta,Tc)*/					
	t1 = Vc;																	
	t2 = Vb;																	
	Tb = (_IQSVPWM(1)-t1-t2)>>1;	/* tbon = (1-t1-t2)/2	*/						
	Ta = Tb+t1;			 	/* taon = tbon+t1		*/						
	Tc = Ta+t2;			  	/* tcon = taon+t2		*/						
	
	Tb = Tb - Tc;
	Ta = Ta - Tc;
	Tc = 0;
	break;																		
	case 2:	  /* Sector 2: t1=Y and t2=-X (abc ---> Ta,Tc,Tb)*/ 				
	t1 = Vb;																	
	t2 = -Va;																	
	Ta = (_IQSVPWM(1)-t1-t2)>>1;	/*  taon = (1-t1-t2)/2	*/						
	Tc = Ta+t1;				/*  tcon = taon+t1		*/						
	Tb = Tc+t2;				/*  tbon = tcon+t2		*/			
	
  Ta = Ta - Tb;
	Tc = Tc - Tb;
	Tb = 0;
	break;																		
	case 3:	  /* Sector 3: t1=-Z and t2=X (abc ---> Ta,Tb,Tc)*/					
	t1 = -Vc;																	
	t2 = Va;																	
	Ta = (_IQSVPWM(1)-t1-t2)>>1;	/*  taon = (1-t1-t2)/2	*/						
	Tb = Ta+t1;				/*	tbon = taon+t1		*/						
	Tc = Tb+t2;				/*	tcon = tbon+t2		*/						
	
		
  Ta = Ta - Tc;
	Tb = Tb - Tc;
	Tc = 0;
	break;																		
	case 4:	  /* Sector 4: t1=-X and t2=Z (abc ---> Tc,Tb,Ta)*/					
	t1 = -Va;																	
	t2 = Vc;																	
	Tc = (_IQSVPWM(1)-t1-t2)>>1;	/*	tcon = (1-t1-t2)/2	*/						
	Tb = Tc+t1;				/*	tbon = tcon+t1		*/						
	Ta = Tb+t2;				/*	taon = tbon+t2		*/				

  Tc = Tc - Ta;
	Tb = Tb - Ta;
	Ta = 0;	
	break;																		
	case 5:	  /* Sector 5: t1=X and t2=-Y (abc ---> Tb,Tc,Ta)*/					
	t1 = Va;																	
	t2 = -Vb;					/*	tbon = (1-t1-t2)/2	*/						
	Tb = (_IQSVPWM(1)-t1-t2)>>1;	/*	tcon = tbon+t1		*/						
	Tc = Tb+t1;				/*	taon = tcon+t2		*/						
	Ta = Tc+t2;				

  Tb = Tb - Ta;
	Tc = Tc - Ta;
	Ta = 0;		
	break;																		
	case 6:	  /* Sector 6: t1=-Y and t2=-Z (abc ---> Tc,Ta,Tb)*/				
	t1 = -Vb;																	
	t2 = -Vc;																	
	Tc = (_IQSVPWM(1)-t1-t2)>>1;	/*	tcon = (1-t1-t2)/2	*/						
	Ta = Tc+t1;				/*	taon = tcon+t1		*/						
	Tb = Ta+t2;				/*	tbon = taon+t2		*/			

  Tc = Tc - Tb;
	Ta = Ta - Tb;
	Tb = 0;		
	break;																		
}																				



	*pi32_PWM_A=_MultiplyQ15(Ta,i32_PWM_Full_Scale); 
	*pi32_PWM_B=_MultiplyQ15(Tb,i32_PWM_Full_Scale); 
	*pi32_PWM_C=_MultiplyQ15(Tc,i32_PWM_Full_Scale);
  *pi32_PWM_A=SAT0(*pi32_PWM_A,i32_PWM_Full_Scale);
  *pi32_PWM_B=SAT0(*pi32_PWM_B,i32_PWM_Full_Scale);
  *pi32_PWM_C=SAT0(*pi32_PWM_C,i32_PWM_Full_Scale);
}


// Must with SVPWM_ForOverMod
void OverModulationConstrain2(int32_t i32V_AlphaIn,int32_t i32V_BetaIn,int16_t* pi16V_AlphaOut,int16_t* pi16V_BetaOut)
{
	  int16_t i16VectorTheta;
	  int16_t theta_1,theta_2;
	  int32_t i32LengthofHex;
    	uint32_t u32VAMP;
	  int32_t i32AlphaOut,i32BetaOut;
	
	  i16VectorTheta = Motor_atan2(i32V_BetaIn/2,i32V_AlphaIn/2); // Avoid atan2 input exceeds 32767
	
	  // Calculate the boundary of hexagonal
	  if(i16VectorTheta >=0 && i16VectorTheta<I32_DEGREE_60)
		{
			theta_1 = 0; theta_2 = I32_DEGREE_60;
		}
		else if(i16VectorTheta >=I32_DEGREE_60 && i16VectorTheta<(I32_DEGREE_60*2))
		{
			theta_1 = I32_DEGREE_60; theta_2 = (I32_DEGREE_60*2);
		}
		else if(i16VectorTheta >=(I32_DEGREE_60*2) && i16VectorTheta<I32_DEGREE_180)
		{
			theta_1 = (I32_DEGREE_60*2); theta_2 = I32_DEGREE_180;
		}
		else if(i16VectorTheta <=0 && i16VectorTheta>(-I32_DEGREE_60))
		{
			theta_1 = 0; theta_2 = -I32_DEGREE_60;
		}
		else if(i16VectorTheta <= -I32_DEGREE_60 && i16VectorTheta> -(I32_DEGREE_60*2))
		{
			theta_1 = -I32_DEGREE_60; theta_2 = -(I32_DEGREE_60*2);
		}
		else if(i16VectorTheta <=(I32_DEGREE_60*2) && i16VectorTheta>= -I32_DEGREE_180)
		{
			theta_1 = -(I32_DEGREE_60*2); theta_2 = -I32_DEGREE_180;
		}
		
		//i32LengthofHex = ((((1<<30)-1)/(Cosine(i16VectorTheta -(theta_1+theta_2)/2 )))*(int32_t)C_SQRT3_OVER_2_Q15)>>15;
		i32LengthofHex = ((929887695)/(Cosine(i16VectorTheta -(theta_1+theta_2)/2 )));
		
		
		u32VAMP = fast_sqrt((uint32_t)(i32V_AlphaIn*i32V_AlphaIn) 
	                  + (uint32_t)(i32V_BetaIn*i32V_BetaIn)); 
	
	  if (u32VAMP>(i32LengthofHex))  // 0.866*VDC
	  {
			i32AlphaOut = (i32LengthofHex*Cosine(i16VectorTheta))>>15;
		  i32BetaOut = (i32LengthofHex*Sine(i16VectorTheta))>>15;
	  }
		else
		{
			i32AlphaOut = i32V_AlphaIn;
			i32BetaOut  = i32V_BetaIn;
		}
		
		if(i32AlphaOut>=32767) i32AlphaOut =32767;
		if(i32AlphaOut<=-32767) i32AlphaOut =-32767;
		if(i32BetaOut>=32767) i32BetaOut =32767;
		if(i32BetaOut<=-32767) i32BetaOut =-32767;
		*pi16V_AlphaOut = i32AlphaOut;
		*pi16V_BetaOut  = i32BetaOut;
}


uint32_t fast_sqrt(uint32_t val)
{
  int r = 0;
  int shift,x;
  for(shift=0; shift<32; shift+=2)
  { 
		x=0x40000000l >> shift; 
   if(x + r <= val)
   { 
		val -= x + r;
		r    = (r >> 1) | x;
   } 
	 else
   { 
		 r = r >> 1;
   }
  }
        //if(r < val) ++r;
   return r;
};

float Q_rsqrt( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;
 
	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed
 
	return y;
}


int Quick_Mod(int x,int y)
{
	y=(y<0)?(-y):(y);
	while(!(x<y && x>=0))  
	{
		if(x>=y) x=x-y;
		if(x<0) x=x+y;
	}
	return x;
	
}
#define FAST_MATH_Q15_SHIFT 6
int16_t Sine2(int16_t i16_Theta)
{
  int32_t sinVal;                                  /* Temporary variables for input, output */
  int32_t index;                                   /* Index variables */
  int16_t a, b;                                    /* Four nearest output values */
  int16_t fract;                                   /* Temporary values for fractional values */
	
	if(abs(i16_Theta)>16384)
		i16_Theta=32768-i16_Theta;	
	
	if(i16_Theta>=0)
	{  
		index = (i16_Theta)>>FAST_MATH_Q15_SHIFT;
		a     = sinTable[index];
		b     = sinTable[index + 1];
		fract = (i16_Theta - (index << FAST_MATH_Q15_SHIFT));
	}
	else
	{
		index = (-i16_Theta)>>FAST_MATH_Q15_SHIFT;
		a = -sinTable[index];
		b = -sinTable[index + 1];
		fract = (-i16_Theta - (index << FAST_MATH_Q15_SHIFT));
	}
			/* Calculation of fractional value */
		sinVal  = ((1 << FAST_MATH_Q15_SHIFT) - fract) * a;
		sinVal = (sinVal + fract * b) >> FAST_MATH_Q15_SHIFT;		
	
	return (int16_t)sinVal;
}

/**
 * @brief Fast approximation to the trigonometric cosine function for Q15 data.
 * @param[in] x Scaled input value in radians.
 * @return  cos(x).
 *
 * The Q15 input value is in the range [0 +0.9999] and is mapped to a radian
 * value in the range [0 2*pi).
 */

int16_t Cosine2(int16_t i16_Theta)
{
  int32_t cosinVal;                                /* Temporary variables for input, output */
  int32_t index;                                   /* Index variables */
  int16_t a, b;                                    /* Four nearest output values */
  int16_t fract;                                   /* Temporary values for fractional values */

	i16_Theta = i16_Theta + I32_DEGREE_90_2;
	
	if(abs(i16_Theta)>16384)
		i16_Theta=32768-i16_Theta;	
	
	if(i16_Theta>=0)
	{  
		index = (i16_Theta)>>FAST_MATH_Q15_SHIFT;
		a     = sinTable[index];
		b     = sinTable[index + 1];
		fract = (i16_Theta - (index << FAST_MATH_Q15_SHIFT));
	}
	else
	{
		index = (-i16_Theta)>>FAST_MATH_Q15_SHIFT;
		a = -sinTable[index];
		b = -sinTable[index + 1];
		fract = (-i16_Theta - (index << FAST_MATH_Q15_SHIFT));
	}
			/* Calculation of fractional value */
		cosinVal  = ((1 << FAST_MATH_Q15_SHIFT) - fract) * a;
		cosinVal  = (cosinVal + fract * b) >> FAST_MATH_Q15_SHIFT;		
	
	return (int16_t)cosinVal;
}


/*** (C) COPYRIGHT 2017 Spintrol Technology Corp. ***/

