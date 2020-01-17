/******************************************************************************
 * @file     motor_math.h
 * @version  V1.00
 * $Revision: 5 $
 * $Date: 17/21/03 5:51p $
 * @brief    
 *
 * @note

*****************************************************************************/
#ifndef __MOTOR_MATH_H__
#define __MOTOR_MATH_H__

#include "math.h"
#include "stdint.h"
/****************************************************************************//**
 * @brief      Select the method of calculation of sine and cosine 
 *
 * @select     MATH_SINE_TABLE_TYPE_SEL
 *              the sine function with theta accuracy is 10 bit fixed.  
 *             User can select the different sine table according to dedicated 
 *             application and resource.
 *             - \ref MATH_SINE_TABLE_HALF_PERIOD 
 *                    The table size is half with half period sine table.
 *                    The table size is 1024 Byte, that means, 1KB.
 *             - \ref MATH_SINE_TABLE_QUARTER_PERIOD 
 *                    The table size is quarter full period sine table.
 *                    The table size is 512 Byte, that means, 0.5KB.
 *******************************************************************************/
#define MATH_SINE_TABLE_HALF_PERIOD    (0)
#define MATH_SINE_TABLE_QUARTER_PERIOD (1)
#define MATH_SINE_TABLE_TYPE_SEL       MATH_SINE_TABLE_QUARTER_PERIOD

/****************************************************************************//**
 * @brief      Select the method of calculation atan2(y,x) 
 *
 * @select     MATH_ATAN_TYPE_SEL
 *               The atan2 calculation method (Default is MATH_ATAN_TYPE_TABLE)
 *             - \ref MATH_ATAN_TYPE_TABLE
 *                    It cost table size of table of atan but speed is fast. 
 *                    The table size is 512 byte.
 *             - \ref MATH_ATAN_TYPE_CORDIC 
 *                    The table size is small but takes more CPU cycles to compute.
 *                    The method suits on the application that code size critical.
 *                    The table size is 30 byte.
 *******************************************************************************/
#define MATH_ATAN_TYPE_TABLE   (0)
#define MATH_ATAN_TYPE_CORDIC  (1)
#define MATH_ATAN_TYPE_SEL     MATH_ATAN_TYPE_TABLE


//-  Define Region   -//
#if MATH_SINE_TABLE_TYPE_SEL   == MATH_SINE_TABLE_HALF_PERIOD
  #define C_SINE_TABLE_SIZE 513 
#elif MATH_SINE_TABLE_TYPE_SEL == MATH_SINE_TABLE_QUARTER_PERIOD
  #define C_SINE_TABLE_SIZE (257+1) 
#endif

#define ShiftQ15ThetaToQ

#define I32_DEGREE_360			((int32_t)(-32767))
#define I32_DEGREE_180			((int32_t)32767)
#define I32_DEGREE_120			((int32_t)(21845))
#define I32_DEGREE_90			((int32_t)(I32_DEGREE_180/2)) //16383
#define I32_DEGREE_90_2			((int32_t)(32768/2))          //16384

#define I32_DEGREE_60			((int32_t)(10922))
#define I32_DEGREE_30			((int32_t)(I32_DEGREE_180/3))

#define I32_2_PI		    	((int32_t)(32767*2))
#define I32_PI  		    	((int32_t)(32767))
#define I32_PI_Q10              ((uint32_t)((3.14159265358979)*1024))

#define DegreeToPi(x)           ((int32_t)((x)*32767/180))


#define C_1_OVER_SQRT3_Q15      18918
#define C_SQRT3_Q15  			56755
#define C_1_OVER_2_Q15          16383
#define C_SQRT3_OVER_2_Q15      28377 
#define C_2_OVER_SQRT3_Q15      37836 
#define C_1_OVER_3_Q15 			10922
#define C_2_OVER_3_Q15 			21844
#define _MultiplyQ15(x,y)       ((((int32_t)(x))*((int32_t)(y)))>>15)

/****************************************************************************//**
 * @brief      Right shift with rounding
 * @input      Z: input value
 * @input      N: shift bits 
 * @return     result
 *
 *******************************************************************************/
#define RSN(Z, N) (((Z)>>(N)) + (((Z) & ((0x1)<<((N)-1))) != 0))   //

/**
 *  @brief  Calculate the absolute value of x
 *  @param[in]  x Integer value
 *  @return  The absolute value of x
 */
#define abs(x)                    (((x)<0)? (-(x)):(x))
/*	Math table */
extern const int16_t sinTable[C_SINE_TABLE_SIZE];
extern const int16_t atanTable[257];	

const int16_t sinTable[C_SINE_TABLE_SIZE]={
     0,   201,   402,   603,   804,  1005,  1206,  1407,  1608,  1809,  2009,  2210,  2410,  2611,  2811,  3012,  //16
  3212,  3412,  3612,  3811,  4011,  4210,  4410,  4609,  4808,  5007,  5205,  5404,  5602,  5800,  5998,  6195,  //32
  6393,  6590,  6786,  6983,  7179,  7375,  7571,  7767,  7962,  8157,  8351,  8545,  8739,  8933,  9126,  9319,  //48
  9512,  9704,  9896, 10087, 10278, 10469, 10659, 10849, 11039, 11228, 11417, 11605, 11793, 11980, 12167, 12353,  //64
 12539, 12725, 12910, 13094, 13279, 13462, 13645, 13828, 14010, 14191, 14372, 14553, 14732, 14912, 15090, 15269,  //80
 15446, 15623, 15800, 15976, 16151, 16325, 16499, 16673, 16846, 17018, 17189, 17360, 17530, 17700, 17869, 18037,  //96
 18204, 18371, 18537, 18703, 18868, 19032, 19195, 19357, 19519, 19680, 19841, 20000, 20159, 20317, 20475, 20631,  //112
 20787, 20942, 21096, 21250, 21403, 21554, 21705, 21856, 22005, 22154, 22301, 22448, 22594, 22739, 22884, 23027,  //128
 23170, 23311, 23452, 23592, 23731, 23870, 24007, 24143, 24279, 24413, 24547, 24680, 24811, 24942, 25072, 25201,  //144
 25329, 25456, 25582, 25708, 25832, 25955, 26077, 26198, 26319, 26438, 26556, 26674, 26790, 26905, 27019, 27133,  //160
 27245, 27356, 27466, 27575, 27683, 27790, 27896, 28001, 28105, 28208, 28310, 28411, 28510, 28609, 28706, 28803,  //176
 28898, 28992, 29085, 29177, 29268, 29358, 29447, 29534, 29621, 29706, 29791, 29874, 29956, 30037, 30117, 30195,  //192
 30273, 30349, 30424, 30498, 30571, 30643, 30714, 30783, 30852, 30919, 30985, 31050, 31113, 31176, 31237, 31297,  //208
 31356, 31414, 31470, 31526, 31580, 31633, 31685, 31736, 31785, 31833, 31880, 31926, 31971, 32014, 32057, 32098,  //224
 32137, 32176, 32213, 32250, 32285, 32318, 32351, 32382, 32412, 32441, 32469, 32495, 32521, 32545, 32567, 32589,  //240
 32609, 32628, 32646, 32663, 32678, 32692, 32705, 32717, 32728, 32737, 32745, 32752, 32757, 32761, 32765, 32766,  //256

#if MATH_SINE_TABLE_TYPE_SEL == MATH_SINE_TABLE_QUARTER_PERIOD
 32767, 32767};  //Prevent overflow
#else
 32767, 32766, 32765, 32761, 32757, 32752, 32745, 32737, 32728, 32717, 32705, 32692, 32678, 32663, 32646, 32628,  //272
 32609, 32589, 32567, 32545, 32521, 32495, 32469, 32441, 32412, 32382, 32351, 32318, 32285, 32250, 32213, 32176,  //288
 32137, 32098, 32057, 32014, 31971, 31926, 31880, 31833, 31785, 31736, 31685, 31633, 31580, 31526, 31470, 31414,  //304
 31356, 31297, 31237, 31176, 31113, 31050, 30985, 30919, 30852, 30783, 30714, 30643, 30571, 30498, 30424, 30349,  //320
 30273, 30195, 30117, 30037, 29956, 29874, 29791, 29706, 29621, 29534, 29447, 29358, 29268, 29177, 29085, 28992,  //336
 28898, 28803, 28706, 28609, 28510, 28411, 28310, 28208, 28105, 28001, 27896, 27790, 27683, 27575, 27466, 27356,  //352
 27245, 27133, 27019, 26905, 26790, 26674, 26556, 26438, 26319, 26198, 26077, 25955, 25832, 25708, 25582, 25456,  //368
 25329, 25201, 25072, 24942, 24811, 24680, 24547, 24413, 24279, 24143, 24007, 23870, 23731, 23592, 23452, 23311,  //384
 23170, 23027, 22884, 22739, 22594, 22448, 22301, 22154, 22005, 21856, 21705, 21554, 21403, 21250, 21096, 20942,  //400
 20787, 20631, 20475, 20317, 20159, 20000, 19841, 19680, 19519, 19357, 19195, 19032, 18868, 18703, 18537, 18371,  //416
 18204, 18037, 17869, 17700, 17530, 17360, 17189, 17018, 16846, 16673, 16499, 16325, 16151, 15976, 15800, 15623,  //432
 15446, 15269, 15090, 14912, 14732, 14553, 14372, 14191, 14010, 13828, 13645, 13462, 13279, 13094, 12910, 12725,  //448
 12539, 12353, 12167, 11980, 11793, 11605, 11417, 11228, 11039, 10849, 10659, 10469, 10278, 10087,  9896,  9704,  //464
  9512,  9319,  9126,  8933,  8739,  8545,  8351,  8157,  7962,  7767,  7571,  7375,  7179,  6983,  6786,  6590,  //480
  6393,  6195,  5998,  5800,  5602,  5404,  5205,  5007,  4808,  4609,  4410,  4210,  4011,  3811,  3612,  3412,  //496
  3212,  3012,  2811,  2611,  2410,  2210,  2009,  1809,  1608,  1407,  1206,  1005,   804,   603,   402,   201,  //512
#if MATH_SINE_TABLE_TYPE_SEL == MATH_SINE_TABLE_HALF_PERIOD
 0}; 
#endif
#endif





#if MATH_ATAN_TYPE_SEL == MATH_ATAN_TYPE_TABLE
const int16_t atanTable[257] = 
{
	    0,    20,    41,    61,    81,   102,   122,   143,   163,   183,   204,   224,   244,   265,   285,   305,
	  326,   346,   366,   386,   407,   427,   447,   467,   487,   508,   528,   548,   568,   588,   608,   628,
	  649,   669,   689,   709,   729,   749,   769,   788,   808,   828,   848,   868,   888,   907,   927,   947,
	  967,   986,  1006,  1026,  1045,  1065,  1084,  1104,  1123,  1143,  1162,  1181,  1201,  1220,  1239,  1258,
	 1278,  1297,  1316,  1335,  1354,  1373,  1392,  1411,  1430,  1449,  1468,  1486,  1505,  1524,  1542,  1561,
	 1580,  1598,  1617,  1635,  1654,  1672,  1690,  1708,  1727,  1745,  1763,  1781,  1799,  1817,  1835,  1853,
	 1871,  1889,  1907,  1924,  1942,  1960,  1977,  1995,  2012,  2030,  2047,  2065,  2082,  2099,  2117,  2134,
	 2151,  2168,  2185,  2202,  2219,  2236,  2253,  2269,  2286,  2303,  2319,  2336,  2352,  2369,  2385,  2402,
	 2418,  2434,  2451,  2467,  2483,  2499,  2515,  2531,  2547,  2563,  2578,  2594,  2610,  2626,  2641,  2657,
	 2672,  2688,  2703,  2718,  2734,  2749,  2764,  2779,  2794,  2809,  2824,  2839,  2854,  2869,  2884,  2899,
	 2913,  2928,  2942,  2957,  2971,  2986,  3000,  3014,  3029,  3043,  3057,  3071,  3085,  3099,  3113,  3127,
	 3141,  3155,  3169,  3182,  3196,  3210,  3223,  3237,  3250,  3264,  3277,  3290,  3303,  3317,  3330,  3343,
	 3356,  3369,  3382,  3395,  3408,  3421,  3433,  3446,  3459,  3471,  3484,  3496,  3509,  3521,  3534,  3546,
	 3558,  3571,  3583,  3595,  3607,  3619,  3631,  3643,  3655,  3667,  3679,  3691,  3702,  3714,  3726,  3737,
	 3749,  3760,  3772,  3783,  3795,  3806,  3817,  3829,  3840,  3851,  3862,  3873,  3884,  3895,  3906,  3917,
	 3928,  3939,  3949,  3960,  3971,  3982,  3992,  4003,  4013,  4024,  4034,  4045,  4055,  4065,  4076,  4086, 
	 4096,
};	
int16_t Table_atan2(int16_t y, int16_t x)  /* 0~32767 0~2*pi */
{
    int32_t tmp;
	  int16_t result;
    
    if(x)
    {
        tmp = ((int32_t)y * 256) / x;
        
        if((tmp < 0) && (tmp >= -256))
        {
            tmp = -tmp;
            result = atanTable[tmp]*2;
        }
        else if(tmp < -256)
        {
            tmp = (-256 * 256)/tmp;
            result = I32_DEGREE_90 - atanTable[tmp]*2;
        }
        else if(tmp > 256)
        {
            tmp = (256 * 256) / tmp;
            result = I32_DEGREE_90 - atanTable[tmp]*2;
        }
        else if((tmp > 0) && (tmp <= 256))
        {
            result = atanTable[tmp]*2;
        }
        else
        {
            result = 0;
        }
    }
    else
    {
        result = I32_DEGREE_90;
    }
    
    if((x < 0) && (y >= 0))
        result = (int16_t)(I32_DEGREE_180 - result);
    else if((x < 0) && (y < 0))
        result = (int16_t)(I32_DEGREE_180 + result);
    else if((x >= 0) && (y < 0))
        result = (int16_t)( - result);

    return result;
}

#endif


#if MATH_SINE_TABLE_TYPE_SEL == MATH_SINE_TABLE_HALF_PERIOD
__STATIC_INLINE int16_t Sine(int16_t i16_Theta)
{
	//int16_t i16ThetaQ10;
	//i16ThetaQ10 = (i16_Theta>>6);//Q15->Q10
	if(i16_Theta>=0)
	{
	  return sinTable[(i16_Theta)>>6];
	}
	else
	{
		return -sinTable[(-i16_Theta)>>6];
	}
	
}
#elif MATH_SINE_TABLE_TYPE_SEL == MATH_SINE_TABLE_QUARTER_PERIOD
 int16_t Sine(int16_t i16_Theta)
{

  /* OKªº */ 
	//printf("i16_Theta0=%d\n",i16_Theta);

	if((i16_Theta > I32_DEGREE_90))   //16383
	{
		
		i16_Theta = (I32_DEGREE_180 - i16_Theta) + 64;	//32767	
	}
	else if((i16_Theta < (-16384)))
	{
		
		i16_Theta = (I32_DEGREE_180 - i16_Theta) - 62;
  	//i16_Theta = i16_Theta - 62;
	}
	
	if(i16_Theta>=0)
	{  
	  return sinTable[(i16_Theta)>>6];		
	}
	else
	{
		return -sinTable[(-i16_Theta)>>6];
	}

}
#endif


 int16_t Cosine(int16_t i16_Theta)
{

	/* OKªº */
	i16_Theta = i16_Theta + I32_DEGREE_90;  //16383
	if((i16_Theta > I32_DEGREE_90) || (i16_Theta < (-16384)))
	{
		i16_Theta = I32_DEGREE_180 - i16_Theta;
		
	}
	
	if(i16_Theta>=0)
	{
	  return sinTable[(i16_Theta>>6)];
	}
	else
	{
		return -sinTable[(-i16_Theta)>>6];
	}
	
	
	
}



 void Clarke_Cal(int16_t i16_U,int16_t i16_V,int16_t i16_W,int16_t *pi16_Alpha,int16_t *pi16_Beta)
{

	*pi16_Alpha =_MultiplyQ15(i16_U,C_2_OVER_3_Q15)-_MultiplyQ15(i16_V,C_1_OVER_3_Q15)-_MultiplyQ15(i16_W,C_1_OVER_3_Q15);
	*pi16_Beta  =  ((i16_V - i16_W) * C_1_OVER_SQRT3_Q15) >> 15;  								 
}		
 void  InvPark_Cal(int16_t i16_Vd,int16_t i16_Vq,int16_t i16_Theta,int16_t *pi16_Alpha,int16_t *pi16_Beta)
{
	int16_t i16_Cosine,i16_Sine;
	i16_Cosine=Cosine(i16_Theta);
	i16_Sine=Sine(i16_Theta);
	//printf("sine=%d,cos=%d\n",i32_Sine,i32_Cosine);
	*pi16_Alpha = (i16_Cosine* i16_Vd  - i16_Sine * i16_Vq) >> 15;	
	*pi16_Beta  = (i16_Cosine * i16_Vq + i16_Sine * i16_Vd) >> 15;	
}


 void  Park_Cal(int16_t i16_Alpha,int16_t i16_Beta,int16_t i16_Theta,int16_t *pi16_Id,int16_t *pi16_Iq)
{
	int32_t i32_Cosine,i32_Sine;
	i32_Cosine=Cosine(i16_Theta);
	i32_Sine=Sine(i16_Theta);
	
	*pi16_Id  = (int16_t) ((i32_Cosine * i16_Alpha + i32_Sine * i16_Beta) >> 15);
	*pi16_Iq  = (int16_t) ((i32_Cosine * i16_Beta - i32_Sine * i16_Alpha) >> 15);	 
}		

int16_t Table_atan2(int16_t y, int16_t x);
int16_t Cordic_atan2(int16_t y,int16_t x);

 int16_t Motor_atan2(int16_t y, int16_t x)
{
	#if MATH_ATAN_TYPE_SEL == MATH_ATAN_TYPE_TABLE
	  return Table_atan2(y,x);
	#elif MATH_ATAN_TYPE_SEL == MATH_ATAN_TYPE_CORDIC
	  return Cordic_atan2(y,x);
	#endif
				 
}		


int16_t Cosine2(int16_t i16_Theta);
int16_t Sine2(int16_t i16_Theta);


 void  InvPark_Cal32(int16_t i16Vd,int16_t i16Vq,int16_t i16_Theta,int32_t *pi32_Alpha,int32_t *pi32_Beta)
{
	int16_t i16_Cosine,i16_Sine;
	i16_Cosine=Cosine2(i16_Theta);
	i16_Sine=Sine2(i16_Theta);
//	printf("sine=%d,cos=%d\n",i16_Sine,i16_Cosine);
	if((abs(i16Vd) + (abs(i16Vq)) > 32767))
	{
		i16Vd = i16Vd >> 1;
		i16Vq = i16Vq >> 1;
		*pi32_Alpha = (i16_Cosine* i16Vd - i16_Sine * i16Vq) >> 14;	
		*pi32_Beta  = (i16_Cosine *i16Vq + i16_Sine * i16Vd) >> 14;	
	}
	else
	{
		*pi32_Alpha = (i16_Cosine* i16Vd - i16_Sine * i16Vq) >> 15;	
		*pi32_Beta  = (i16_Cosine *i16Vq + i16_Sine * i16Vd) >> 15;	
//		printf("Vd %x, Vq%x\n",i16Vd, i16Vq);
//		printf("%x",  i16_Cosine* i16Vd);
//		printf("%x",  i16_Sine * i16Vq);
//		printf("%x",  i16_Cosine *i16Vq);
//		printf("%x\n",i16_Sine * i16Vd);
	}

}


uint32_t fast_sqrt( uint32_t i32Input );
float Q_rsqrt( float number );
int Quick_Mod(int x,int y);

void SVPWM(int32_t i32_V_Alpha,int32_t i32_V_Beta,int32_t i32_PWM_Full_Scale,int16_t* pi32_PWM_A,int16_t* pi32_PWM_B,int16_t* pi32_PWM_C);
void OverModulationConstrain2(int32_t i32V_AlphaIn,int32_t i32V_BetaIn,int16_t* pi16V_AlphaOut,int16_t* pi16V_BetaOut);
void SVPWM_ForOverMod(int32_t i32_V_Alpha,int32_t i32_V_Beta,int32_t i32_PWM_Full_Scale,int16_t* pi32_PWM_A,int16_t* pi32_PWM_B,int16_t* pi32_PWM_C);

#endif //__MOTOR_MATH_H__
/*** (C) COPYRIGHT  ***/
