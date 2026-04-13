#include "buzzer.h"
#include "tim.h"
#include "usart.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

unsigned char beat = 0;
unsigned char note = 0;
unsigned int time = 0;
unsigned int beatTime = 0;
unsigned int soundTime = 0;

unsigned char enable = 1;
unsigned char tmrflag = 0;
unsigned char Buzzer_play_Enable = 0;

double countA = 0;
unsigned char *pNote = NULL;
unsigned char *pBeat = NULL;
unsigned char Note_1 = 0;
unsigned char _pNote[1] = {0};
unsigned char _pBeat[1] = {0};

/* C major: middle, low, high */
static unsigned int NoteFrequ[] = {
    262, 294, 330, 350, 393, 441, 495,
    131, 147, 165, 175, 196, 221, 248,
    525, 589, 661, 700, 786, 882, 990,
};

static unsigned char TwoTigerNote[] = {
    1, 2, 3, 1, 1, 2, 3, 1, 3, 4, 5, 3, 4, 5,
    5, 6, 5, 4, 3, 1, 5, 6, 5, 4, 3, 1, 2, 5, 1, 2, 5, 1,
};

static unsigned char TwoTigerBeat[] = {
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 8, 4, 4, 8,
    3, 1, 3, 1, 4, 4, 3, 1, 3, 1, 4, 4, 4, 4, 8, 4, 4, 8,
};

static unsigned char testNote[] = {
    1, 2, 3, 4, 5, 6, 7,
    7, 6, 5, 4, 3, 2, 1,
};

static unsigned char testBeat[] = {
    4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4,
};

static void PlayTwoTiger(void)
{
    if (Buzzer_play_Enable > 0U) {
        if (tmrflag == 1U) {
            tmrflag = 0U;

            if (time == 0U) {
                note = pNote[beat] - 1U;
                countA = 200000000.0 / 200.0;
                countA = countA / (double)(NoteFrequ[note] * 2U);
                __HAL_TIM_SET_AUTORELOAD(&htim1, (uint16_t)countA - 1U);

                beatTime = (pBeat[beat] * NoteFrequ[note]) >> 2;
                soundTime = beatTime - (beatTime >> 2);
                enable = 1U;
                time++;
            } else {
                if (time >= beatTime) {
                    time = 0U;
                    beat++;
                } else {
                    time++;
                    if (time == soundTime) {
                        enable = 0U;
                    }
                }
            }

            if (beat == Note_1) {
                beat = 0U;
                if (Buzzer_play_Enable > 0U) {
                    Buzzer_play_Enable--;
                }
            }

            if (Buzzer_play_Enable == 0U) {
                BUZZER1(0);
            }
        }
    }
}

void Play_Note(unsigned char _Note, unsigned char _Beat, unsigned char play_count)
{
    if ((_Note >= 1U) && (_Note <= 21U)) {
        _pNote[0] = _Note;
        _pBeat[0] = _Beat;
        pNote = &_pNote[0];
        pBeat = &_pBeat[0];
        Note_1 = 1U;
    } else if (_Note == 101U) {
        Note_1 = (unsigned char)sizeof(TwoTigerNote);
        pNote = TwoTigerNote;
        pBeat = TwoTigerBeat;
    } else if (_Note == 200U) {
        Note_1 = (unsigned char)sizeof(testNote);
        pNote = testNote;
        pBeat = testBeat;
    } else {
        return;
    }

    /* play_count represents real repeat count (1 = play once). */
    Buzzer_play_Enable = (play_count == 0U) ? 1U : play_count;
    beat = 0U;
    time = 0U;
    enable = 1U;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&htim1)) {
        if (Buzzer_play_Enable != 0U) {
            tmrflag = 1U;
            if (enable) {
                BUZZER1TOGGLE;
            } else {
                BUZZER1(0);
            }
            PlayTwoTiger();
        }
    }
}

uint8_t UART4_Rx_Buffer = 0U;

static char HMI_TxBuff[128];

void hmi_printf(char *fmt, ...)
{
    unsigned int length;
    va_list ap;

    va_start(ap, fmt);
    vsnprintf(HMI_TxBuff, sizeof(HMI_TxBuff), fmt, ap);
    va_end(ap);

    length = (unsigned int)strlen(HMI_TxBuff);
    HAL_UART_Transmit(&huart4, (uint8_t *)HMI_TxBuff, (uint16_t)length, 0xFFFF);
}

extern volatile uint8_t runing_state;

static uint8_t rx_frame[5];
static uint8_t rx_len = 0U;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    uint8_t byte;
    uint32_t tick;
    static uint32_t rx_tick = 0U;

    tick = HAL_GetTick();
    if ((tick - rx_tick) > 10U) {
        rx_len = 0U;
    }
    rx_tick = tick;

    if (huart == &huart4) {
        byte = UART4_Rx_Buffer;

        if (rx_len < 5U) {
            rx_frame[rx_len++] = byte;
        } else {
            memmove(&rx_frame[0], &rx_frame[1], 4U);
            rx_frame[4] = byte;
        }

        if ((rx_len >= 5U) &&
            (rx_frame[0] == 0x7EU) &&
            (rx_frame[3] == 0xFFU) &&
            (rx_frame[4] == 0xFFU)) {
            runing_state = rx_frame[1];
            rx_len = 0U;
        }

        HAL_UART_Receive_IT(&huart4, &UART4_Rx_Buffer, UART4_MACRO);
    }
}

