/**
 * @file uart.h
 * @brief Header voor UART2-driver (register-level) voor STM32F4
 * @details
 * Bevat prototypes voor UART2-initialisatie, verzenden van characters en strings.
 * Implementatie staat in uart.c.
 * Inclusief globale variabelen voor bufferbeheer en commando-detectie.
 * @author Luc, Joost, Thijs
 * @version 1.1
 * @date 13-01-2026
 *
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "stm32f4xx.h"

#define RX_BUFFER_SIZE 128  ///< Maximale grootte van de RX-buffer

// ================= EXTERNE VARIABELEN ==================
extern volatile char uart_rx_buffer[RX_BUFFER_SIZE];  ///< Buffer voor ontvangen UART-characters
extern volatile uint32_t uart_rx_index;               ///< Huidige index in RX-buffer
extern volatile uint8_t uart_command_ready;           ///< Flag: 1 als een commando compleet ontvangen is

// ================= UART API ==========================
/**
 * @brief Initialiseert UART2 met opgegeven baudrate
 * @param baudrate Baudrate voor UART2
 */
void UART2_Init(uint32_t baudrate);

/**
 * @brief Verstuur één karakter via UART2
 * @param c Het te versturen karakter
 */
void UART2_SendChar(char c);

/**
 * @brief Verstuur een null-terminated string via UART2
 * @param str Pointer naar string
 */
void UART2_WriteString(const char *str);

#endif /* INC_UART_H_ */
