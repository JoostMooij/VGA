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

#define RX_BUFFER_SIZE 128

extern volatile char uart_rx_buffer[RX_BUFFER_SIZE];
extern volatile uint32_t uart_rx_index;
extern volatile uint8_t uart_command_ready;

// UART API
void UART2_Init(uint32_t baudrate);
void UART2_SendChar(char c);
void UART2_WriteString(const char *str);

#endif /* INC_UART_H_ */

