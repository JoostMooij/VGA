/*
 * UART.h
 *
 *  Created on: Nov 21, 2025
 *      Author: beren
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "stm32f4xx.h"
#include <string.h>

volatile uint32_t uart_pclk1_debug;

static uint32_t UART_GetPCLK1(void)
{
    uint32_t hclk = SystemCoreClock;
    uint32_t ppre1 = (RCC->CFGR & RCC_CFGR_PPRE1) >> 10;
    uint32_t div;
    switch (ppre1) {
        case 0: div = 1; break;
        case 4: div = 2; break;
        case 5: div = 4; break;
        case 6: div = 8; break;
        case 7: div = 16; break;
        default: div = 1; break;
    }
    return hclk / div;
}

// --- Initialisatie van UART2 op PA2(TX)/PA3(RX) ---
void UART2_Init(uint32_t baudrate)
{
    uint32_t pclk1 = UART_GetPCLK1();
    uart_pclk1_debug = pclk1; // optioneel voor debugger

    // 1. Enable clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // Configure PA3 (TX) & PA2 (RX) as Alternate Function AF7 (USART2)
    GPIOA->MODER &= ~((3U << (2*2)) | (3U << (3*2)));  // Clear mode
    GPIOA->MODER |=  ((2U << (2*2)) | (2U << (3*2)));  // Set to AF mode

    GPIOA->AFR[0] &= ~((0xF << (4*2)) | (0xF << (4*3)));
    GPIOA->AFR[0] |=  ((7 << (4*2)) | (7 << (4*3)));

    GPIOA->OSPEEDR |= ((3U << (2*2)) | (3U << (3*2))); // high speed
    GPIOA->OTYPER  &= ~((1U<<2) | (1U<<3));            // push pull
    GPIOA->PUPDR   &= ~((3U << (2*2)) | (3U << (3*2))); // no pull

    // 3. Configure USART2 (register default)
    USART2->CR1 = 0;
    USART2->CR2 = 0;
    USART2->CR3 = 0;

    // Baudrate berekenen (oversampling 16, standaard STM32F4)
    uint32_t brr = (pclk1 + baudrate/2) / baudrate;
    USART2->BRR = brr;

    // Anabel RX en TX voor receiver en transmitter
    USART2->CR1 |= USART_CR1_RE;   // Receiver enable
    USART2->CR1 |= USART_CR1_TE;   // Transmitter enable
    USART2->CR1 |= USART_CR1_UE;   // USART enable
}

// --- Single Character versturen via UART2 ---
void UART2_SendChar(char c)
{
    while (!(USART2->SR & USART_SR_TXE));  // wachten tot TX leeg is
    USART2->DR = (uint8_t)c;
}

// --- String versturen via UART2 ---
void UART2_WriteString(const char *str)
{
    while (*str)
    {
        UART2_SendChar(*str++);
    }
}

char UART2_ReadChar(void)
{
    while (!(USART2->SR & USART_SR_RXNE));  // wacht tot er data binnen is
    return USART2->DR & 0xFF;
}

void UART2_ReadString(char *buf, uint32_t maxlen)
{
    uint32_t idx = 0;
    char c = 0;
    while (idx < (maxlen - 1))
    {
        c = UART2_ReadChar();
        if (c == '\r' || c == '\n')
            break;
        buf[idx++] = c;
    }
    buf[idx] = 0; // eind null-terminator

    // Negeer onmiddellijk volgend '\n' na '\r'
    if (c == '\r')
    {
        char extra = UART2_ReadChar();
        if (extra != '\n')
            ; // optioneel: kun je handlen/gebruiken
    }
}

#endif /* INC_UART_H_ */
