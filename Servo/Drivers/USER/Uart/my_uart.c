#include "my_uart.h"
#include <stdio.h>

int fputc(int ch, FILE *f)
{
    (void)f;
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}

int fgetc(FILE *f)
{
    uint8_t ch = 0;
    (void)f;

    while (HAL_UART_Receive(&huart1, &ch, 1, 0xFFFF) != HAL_OK) {
    }

    return ch;
}
