#include "fft_user.h"
#include "FFTInc.h"
#include "mic_adc.h"

#include <math.h>

struct compx s[10240];

static float tem1[MAX_FFT_N];
static uint16_t peak[(MAX_FFT_N / 2) / (7 / 2 - 1)];
static float fft_value[MAX_FFT_N / 2];

void PowerPhaseRadians_f32(uint16_t fft_points, float32_t threshold)
{
    uint32_t i;

    for (i = 0; i < fft_points; i++) {
        float32_t lX = s[i].real;
        float32_t lY = s[i].imag;
        float32_t phase = atan2f(lY, lX);
        float32_t mag = 0.0f;

        arm_sqrt_f32((float32_t)(lX * lX + lY * lY), &mag);
        s[i].real = (threshold > mag) ? 0.0f : (phase * 180.0f / 3.1415926f);
    }
}

static void find_peaks_max(float *src,
                           uint32_t src_len,
                           uint32_t distance,
                           uint16_t *ind_max,
                           uint32_t max_len,
                           uint32_t *ind_max_len)
{
    uint32_t i, j;
    int32_t diff;

    tem1[0] = 0;
    ind_max[0] = 0;

    for (j = 1, i = 2 + distance; i < src_len - distance; i++) {
        diff = (int32_t)src[i] - (int32_t)src[i - 1];
        if (diff > 0) {
            tem1[i - 1] = 1;
        } else if (diff < 0) {
            tem1[i - 1] = -1;
        } else {
            tem1[i - 1] = 0;
        }

        diff = (int32_t)tem1[i - 1] - (int32_t)tem1[i - 2];
        if ((diff < 0) && (j < max_len)) {
            if ((uint32_t)ind_max[j - 1] + distance < i) {
                ind_max[j++] = (uint16_t)(i - 1);
            }
        }
    }

    *ind_max_len = j;
}

#define PEAK_SIZE 7
#define HALF_PEAK_SIZE (PEAK_SIZE / 2 - 1)
#define MAX_PEAK (MAX_FFT_N / 2 / HALF_PEAK_SIZE)
#define PEAK_THS 1000000

int cfft_f32_mag(void)
{
    uint32_t i, j, peak_nb;

#if (MAX_FFT_N != 8192) && (MAX_FFT_N != 16384)
    InitTableFFT(MAX_FFT_N);
#endif

    for (i = 0; i < MAX_FFT_N; i++) {
        s[i].real = (float32_t)adc_value_buffer[i];
        s[i].imag = 0.0f;
    }

    cfft(s, MAX_FFT_N);

    for (i = 0; i < 80; i++) {
        fft_value[i] = 0.0f;
    }

    for (i = 80; i < MAX_FFT_N / 2; i++) {
        arm_sqrt_f32((float32_t)(s[i].real * s[i].real + s[i].imag * s[i].imag), &fft_value[i]);
    }

    find_peaks_max(fft_value, MAX_FFT_N / 2, HALF_PEAK_SIZE, peak, MAX_PEAK, &peak_nb);

    for (i = j = 0; i < peak_nb; i++) {
        if (fft_value[peak[i]] > PEAK_THS) {
            peak[j++] = peak[i];
        }
    }
    peak_nb = j;

    if (peak_nb > 0U) {
        if ((peak[0] > 90U) && (peak[0] < 140U)) {
            return (int)peak[0];
        }
    }

    return 0;
}

void cfft_f32_phase(void)
{
    uint32_t i;

#if (MAX_FFT_N != 8192) && (MAX_FFT_N != 16384)
    InitTableFFT(MAX_FFT_N);
#endif

    for (i = 0; i < MAX_FFT_N; i++) {
        s[i].real = 1.0f + cosf(2.0f * 3.1415926f * 500.0f * (float)i / (float)MAX_FFT_N + 3.1415926f / 3.0f);
        s[i].imag = 0.0f;
    }

    cfft(s, MAX_FFT_N);
    PowerPhaseRadians_f32(MAX_FFT_N, 0.5f);
}
