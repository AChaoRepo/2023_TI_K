#ifndef __FFT_USER_H__
#define __FFT_USER_H__
#include "FFTInc.h"		

extern struct  compx s[10240];


void PowerPhaseRadians_f32(uint16_t _usFFTPoints, float32_t _uiCmpValue);
int cfft_f32_mag(void);
void cfft_f32_phase(void);
#endif /* __USART_H__ */
