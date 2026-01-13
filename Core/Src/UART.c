/**
 * @file uart.c
 * @brief UART functionaliteiten voor project VGA
 * @details
 * Deze module bevat functies voor:
 *  - Initialisatie van UART2
 *  - Berekening en instelling van baudrate (BRR)
 *  - Versturen van characters en strings
 *  - Interrupt  ontvangst van UART-commando's
 *
 * De module gebruikt geen HAL of SPL; registers worden direct aangesproken via stm32f4xx.h.
 * @author Luc, Joost, Thijs
 * @version 1.1
 * @date 13-01-2026
 *
 */

#include "UART.h"

volatile uint32_t uart_pclk1_debug = 0;
volatile char uart_rx_buffer[RX_BUFFER_SIZE];
volatile uint32_t uart_rx_index = 0;
volatile uint8_t uart_command_ready = 0;

/**
 * @brief  Berekent de huidige APB1-klok (PCLK1) frequentie
 * @details Leest SystemCoreClock en APB1-prescaler (PPRE1)
 *          en berekent effectieve PCLK1.
 * @return PCLK1-frequentie in Hz
 */
static uint32_t UART_GetPCLK1(void)
{
    uint32_t hclk = SystemCoreClock;
    uint32_t ppre1 = (RCC->CFGR & RCC_CFGR_PPRE1) >> 10;
    uint32_t div;

    switch (ppre1)
    {
        case 0:  div = 1; break;
        case 4:  div = 2; break;
        case 5:  div = 4; break;
        case 6:  div = 8; break;
        case 7:  div = 16; break;
        default: div = 1; break;
    }

    return hclk / div;
}

/**
 * @brief Initialiseert UART2 op de STM32F4
 * @param baudrate Baudrate voor UART2
 * @details Zet clocks aan, configureert PA2/PA3 als AF7,
 *          en activeert USART2 transmitter en receiver interrupts.
 */
void UART2_Init(uint32_t baudrate)
{
    // Enable GPIOA en USART2 clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // Configureer PA2 (TX) en PA3 (RX) als alternate function
    GPIOA->MODER &= ~((3U << (2*2)) | (3U << (3*2)));
    GPIOA->MODER |=  ((2U << (2*2)) | (2U << (3*2)));

    // Selecteer AF7 voor PA2/PA3
    GPIOA->AFR[0] &= ~((0xF << (4*2)) | (0xF << (4*3)));
    GPIOA->AFR[0] |=  ((7 << (4*2)) | (7 << (4*3)));

    // Bereken BRR
    uint32_t pclk1 = SystemCoreClock / 4; // eenvoudige schatting
    USART2->BRR = (pclk1 + baudrate/2) / baudrate;

    // Configureer USART2: RE, TE, RXNE interrupt, en USART enable
    USART2->CR1 = USART_CR1_RE | USART_CR1_TE | USART_CR1_RXNEIE | USART_CR1_UE;

    // Enable NVIC interrupt voor USART2
    NVIC_EnableIRQ(USART2_IRQn);
}

/**
 * @brief Verstuur één karakter via UART2
 * @param c Het te verzenden karakter
 * @note Wacht totdat TXE=1 voordat karakter wordt geschreven
 */
void UART2_SendChar(char c)
{
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = (uint8_t)c;
}

/**
 * @brief Verstuur een null-terminated string via UART2
 * @param str Pointer naar string
 */
void UART2_WriteString(const char *str)
{
    while (*str)
    {
        UART2_SendChar(*str++);
    }
}

/**
 * @brief Lees één karakter van UART2
 * @return Het ontvangen karakter
 */
char UART2_ReadChar(void)
{
    while (!(USART2->SR & USART_SR_RXNE));
    return USART2->DR & 0xFF;
}

/**
 * @brief Lees een string van UART2 tot '\n', '\r' of buffer vol
 * @param[out] buf Buffer voor de gelezen string
 * @param[in] maxlen Maximale lengte van buffer inclusief '\0'
 */
void UART2_ReadString(char *buf, uint32_t maxlen)
{
    uint32_t idx = 0;
    char c = 0;

    while (idx < (maxlen - 1))
    {
        c = UART2_ReadChar();
        if (c == '\r' || c == '\n') break;
        buf[idx++] = c;
    }

    buf[idx] = '\0';

    // Negeer extra '\n' na '\r'
    if (c == '\r')
    {
        char extra = UART2_ReadChar();
        if (extra != '\n') ; // niets doen
    }
}

/**
 * @brief USART2 interrupt handler
 * @details Ontvangt characters, zet uart_command_ready wanneer een commando compleet is.
 */
void USART2_IRQHandler(void)
{
    if (USART2->SR & USART_SR_RXNE)
    {
        char c = USART2->DR;

        if (c == '\r' || c == '\n')
        {
            uart_rx_buffer[uart_rx_index] = '\0';
            uart_rx_index = 0;
            uart_command_ready = 1;
        }
        else if (uart_rx_index < RX_BUFFER_SIZE - 1)
        {
            uart_rx_buffer[uart_rx_index++] = c;
        }
    }
}
