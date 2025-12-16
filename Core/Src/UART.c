/**
 ******************************************************************************
 * @file           : uart.c
 * @brief          : Low-level UART2 driver (register-level)
 * @author         : Luc van den Engel
 * @date           : 2025
 *
 * @details
 * Deze module bevat alle functies voor:
 *  - Initialiseren van UART2
 *  - Configure van GPIOA (PA2 = TX, PA3 = RX)
 *  - Berekenen en instellen van Baudrate (BRR)
 *  - Versturen van characters en strings
 *
 * De module gebruikt geen HAL of SPL. Alle registers worden direct
 * aangesproken via stm32f4xx.h.
 *
 ******************************************************************************
 */

#include "UART.h"

volatile uint32_t uart_pclk1_debug = 0;
volatile char uart_rx_buffer[RX_BUFFER_SIZE];
volatile uint32_t uart_rx_index = 0;
volatile uint8_t uart_command_ready = 0;

/**
 * @brief  Geeft de huidige APB1-klok (PCLK1) terug.
 *
 * @details Leest SystemCoreClock en het APB1-prescaler veld (PPRE1)
 * om de effectieve PCLK1-frequentie te berekenen.
 *
 * @return PCLK1-frequentie in Hz.
 */

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

/**
 * @brief Initialiseert UART2 op de STM32F4.
 *
 * Zet alle benodigde clocks aan, configureert PA2/PA3 als AF7,
 * en activeert de USART2 transmitter.
 *
 * De baudrate wordt niet hier ingesteld, maar in uart2_set_baud().
 */

void UART2_Init(uint32_t baudrate)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    GPIOA->MODER &= ~((3U << (2*2)) | (3U << (3*2)));
    GPIOA->MODER |=  ((2U << (2*2)) | (2U << (3*2)));
    GPIOA->AFR[0] &= ~((0xF << (4*2)) | (0xF << (4*3)));
    GPIOA->AFR[0] |=  ((7 << (4*2)) | (7 << (4*3)));

    uint32_t pclk1 = SystemCoreClock / 4;
    USART2->BRR = (pclk1 + baudrate/2) / baudrate;

    USART2->CR1 =
          USART_CR1_RE
        | USART_CR1_TE
        | USART_CR1_RXNEIE
        | USART_CR1_UE;

    NVIC_EnableIRQ(USART2_IRQn);
}


/**
 * @brief  Verstuurt één karakter via UART2.
 *
 * @param  c  Het te verzenden karakter.
 *
 * @note   Wacht totdat de TX-buffer leeg is voordat het karakter wordt geschreven.
 */

void UART2_SendChar(char c)
{
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = (uint8_t)c;
}

/**
 * @brief Verstuurd een null-terminated string via UART2.
 *
 * @param str Pointer naar string in RAM/Flash.
 */

void UART2_WriteString(const char *str)
{
    while (*str)
    {
        UART2_SendChar(*str++);
    }
}

/**
 * @brief Leest één karakter van UART2.
 * @return Het gelezen karakter.
 */

char UART2_ReadChar(void)
{
    while (!(USART2->SR & USART_SR_RXNE));
    return USART2->DR & 0xFF;
}

/**
 * @brief Leest een string van UART2 tot '\n', '\r' of buffer vol.
 * @param[out] buf Buffer voor de gelezen string.
 * @param[in] maxlen Maximale lengte van de buffer inclusief '\0'.
 */

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
