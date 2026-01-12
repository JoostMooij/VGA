/**
 ******************************************************************************
 * @file           : uart.h
 * @brief          : Header voor de UART2 driver (register-level)
 * @author         : <jouw naam>
 * @date           : 2025
 *
 * @details
 * Dit headerbestand bevat de prototypes van de UART2-drivers.
 * De implementatie staat in uart.c.
 *
 ******************************************************************************
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "stm32f4xx.h"

// Optioneel: externe debug variable declaratie, maar géén definitie!
// extern volatile uint32_t uart_pclk1_debug;

void UART2_Init(uint32_t baudrate);
void UART2_SendChar(char c);
void UART2_WriteString(const char *str);
char UART2_ReadChar(void);
void UART2_ReadString(char *buf, uint32_t maxlen);

#endif /* INC_UART_H_ */
