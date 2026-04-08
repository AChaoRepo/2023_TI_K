#include "fft_user.h"
#include "arm_const_structs.h"
#include "FFTInc.h"	
#include "main.h"
#include "stdio.h"
#include "EventRecorder.h" 
 #include "mic_adc.h"
struct  compx s[10240];

////复数求出幅值
//float Absolute_abs_Value_(complex a)
//{
//   	float i,j;
//   	float value;
//   	i=a.real*a.real;
//   	j=a.img*a.img;
//   	value=i+j;
//   	return value;
//}

/*
*********************************************************************************************************
*	函 数 名: PowerPhaseRadians_f32
*	功能说明: 求相位
*	形    参：_usFFTPoints  复数个数，每个复数是两个float32_t数值
*             _uiCmpValue  比较值，需要求出相位的数值
*	返 回 值: 无
*********************************************************************************************************
*/
void PowerPhaseRadians_f32(uint16_t _usFFTPoints, float32_t _uiCmpValue)		
{
	float32_t lX, lY;
	uint32_t i;
	float32_t phase;
	float32_t mag;
	
	
	for (i=0; i <_usFFTPoints; i++)
	{
		lX= s[i].real; 	  /* 实部 */
		lY= s[i].imag;    /* 虚部 */ 
		
 		phase = atan2f(lY, lX);    		  				 /* atan2求解的结果范围是(-pi, pi], 弧度制 */
		arm_sqrt_f32((float32_t)(lX*lX+ lY*lY), &mag);   /* 求模 */
		
		if(_uiCmpValue > mag)
		{
			s[i].real = 0;			
		}
		else
		{
			s[i].real= phase* 180.0f/3.1415926f;   /* 将求解的结果由弧度转换为角度 */
		}
	}
}

float tem1[MAX_FFT_N];
void find_peaks_max(float *src, uint32_t src_lenth, uint32_t distance, uint16_t *ind_max, uint32_t max_len, uint32_t *ind_max_len)
{
    uint32_t i, j;
    int32_t diff;

    tem1[0] = ind_max[0] = 0;

    for (j = 1, i = 2 + distance; i < src_lenth - distance; i++)
    {
        diff = src[i] - src[i - 1];
        if (diff > 0)
            tem1[i - 1] = 1;
        else if (diff < 0) 
            tem1[i - 1] = -1;
        else                
            tem1[i - 1] = 0;

        diff = tem1[i - 1] - tem1[i - 2];
        if (diff < 0 && j < max_len)
        {          
            if (ind_max[j - 1] + distance < i)  
            {                
                ind_max[j++] = i - 1;
            }                    
        }
    }
    *ind_max_len = j;
}

#define PEAK_SIZE             7 
#define HALF_PEAK_SIZE       (PEAK_SIZE / 2 - 1) 
#define MAX_PEAK             (MAX_FFT_N / 2 / HALF_PEAK_SIZE)
#define PEAK_THS             1000000
static uint16_t peak[MAX_PEAK];
float fft_value[MAX_FFT_N / 2];
/*
*********************************************************************************************************
*	函 数 名: cfft_f32_mag
*	功能说明: 计算幅频
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
 int cfft_f32_mag(void)
{
	uint32_t i, j, peak_nb;
	float32_t Max_mag=0;
	uint32_t Max_mag_index=0; 
	/* 计算一批sin，cos系数 */
#if (MAX_FFT_N != 8192) && (MAX_FFT_N != 16384)
	InitTableFFT(MAX_FFT_N);
#endif
	
	for(i=0; i<MAX_FFT_N; i++)
	{
		/* 波形是由直流分量，500Hz正弦波组成，波形采样率MAX_FFT_N，初始相位60° */
		//s[i].real = 1 + cos(2*3.1415926f*500*i/MAX_FFT_N + 3.1415926f/3);		
		//s[i].imag = 0;
		 s[i].real =  adc_value_buffer[i];//1 + cos(2*3.1415926f*500*i/MAX_FFT_N + 3.1415926f/3);		
		 s[i].imag = 0;
		
	}
	
	/* MAX_FFT_N点快速FFT */ 
	cfft(s, MAX_FFT_N);
	
	for (i = 0; i < 80; i++)
	{
		fft_value[i] = 0;
	}
	/* 计算幅频 */ 
	for(i=80; i<MAX_FFT_N/2; i++)
	{
		arm_sqrt_f32((float32_t)(s[i].real *s[i].real+ s[i].imag*s[i].imag ), &fft_value[i]); 			
		//printf("%f\n",s[i].real);
	}

	find_peaks_max(fft_value, MAX_FFT_N / 2, HALF_PEAK_SIZE, peak, MAX_PEAK, &peak_nb); 
	for (i = j = 0; i < peak_nb; i++)
	{
			if (fft_value[peak[i]] > PEAK_THS)
			{
					peak[j++] = peak[i];
			}
			if(Max_mag<fft_value[peak[i]])
		  {
			    Max_mag=fft_value[peak[i]];
			    Max_mag_index=peak[i];
		  }
	}
	peak_nb = j;
//	for (i = 0; i < peak_nb; i++)
//	{
//			printf("index %d, %f\n",peak[i],fft_value[peak[i]]);
//	}
//	printf("Max_mag=%f,Max_mag_index=%d \n",fft_value[Max_mag_index],Max_mag_index);

  if (peak_nb)
	{
		if (peak[0] > 90 && peak[0] < 140)
		{
			return peak[0];
		}
	}

	return 0;
}


/*
*********************************************************************************************************
*	函 数 名: cfft_f32_phase
*	功能说明: 计算相频
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
 void cfft_f32_phase(void)
{
	uint32_t i;
	

	/* 计算一批sin，cos系数 */
#if (MAX_FFT_N != 8192) && (MAX_FFT_N != 16384)
	InitTableFFT(MAX_FFT_N);
#endif
	
	for(i=0; i<MAX_FFT_N; i++)
	{
		/* 波形是由直流分量，500Hz正弦波组成，波形采样率MAX_FFT_N，初始相位60° */
		s[i].real = 1 + cos(2*3.1415926f*500*i/MAX_FFT_N + 3.1415926f/3);		
		s[i].imag = 0;
	}
	
	/* MAX_FFT_N点快速FFT */ 
	cfft(s, MAX_FFT_N);
	
	/* 求相频 */
	PowerPhaseRadians_f32(MAX_FFT_N, 0.5f);
	
	/* 串口打印求解相频 */
	for(i=0; i<MAX_FFT_N; i++)
	{
		//printf("%f\r\n", s[i].real);
	}
	
}

