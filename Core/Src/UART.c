/*
 * UART.c
 *
 *  Created on: Nov 21, 2025
 *      Author: Luc
 */

#include "UART.h"

// Debug variable (optioneel voor debugging)
volatile uint32_t uart_pclk1_debug = 0;

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

void UART2_Init(uint32_t baudrate)
{
    uint32_t pclk1 = UART_GetPCLK1();
    uart_pclk1_debug = pclk1; // optioneel voor debugger

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // Configure PA3 (TX) & PA2 (RX) as Alternate Function AF7 (USART2)
    GPIOA->MODER &= ~((3U << (2*2)) | (3U << (3*2)));
    GPIOA->MODER |=  ((2U << (2*2)) | (2U << (3*2)));
    GPIOA->AFR[0] &= ~((0xF << (4*2)) | (0xF << (4*3)));
    GPIOA->AFR[0] |=  ((7 << (4*2)) | (7 << (4*3)));

    GPIOA->OSPEEDR |= ((3U << (2*2)) | (3U << (3*2)));
    GPIOA->OTYPER  &= ~((1U<<2) | (1U<<3));
    GPIOA->PUPDR   &= ~((3U << (2*2)) | (3U << (3*2)));

    USART2->CR1 = 0;
    USART2->CR2 = 0;
    USART2->CR3 = 0;

    uint32_t brr = (pclk1 + baudrate/2) / baudrate;
    USART2->BRR = brr;

    USART2->CR1 |= USART_CR1_RE;   // Receiver enable
    USART2->CR1 |= USART_CR1_TE;   // Transmitter enable
    USART2->CR1 |= USART_CR1_UE;   // USART enable
}

void UART2_SendChar(char c)
{
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = (uint8_t)c;
}

void UART2_WriteString(const char *str)
{
    while (*str)
    {
        UART2_SendChar(*str++);
    }
}

char UART2_ReadChar(void)
{
    while (!(USART2->SR & USART_SR_RXNE));
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
    buf[idx] = 0;
    // Negeer onmiddellijk volgend '\n' na '\r'
    if (c == '\r')
    {
        char extra = UART2_ReadChar();
        if (extra != '\n')
            ; // niks doen
    }
}

