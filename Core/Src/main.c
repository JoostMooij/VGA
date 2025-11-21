//--------------------------------------------------------------
// File     : main.c
// Doel     : Eenvoudige UART2 communicatie (Hello naar Termite)
// MCU      : STM32F4xx
//--------------------------------------------------------------

#include "stm32f4xx.h"

volatile uint32_t pclk1;

void UART2_Init(void);
void UART2_WriteString(const char *str);

static uint32_t get_pclk1(void)
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


void UART2_Init(void)
{
    uint32_t baudrate = 115200;                // gewenste baudrate
    uint32_t pclk1 = get_pclk1();              // actuele PCLK1 ophalen

    // --- 1. Enable clocks ---
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;       // GPIOA clock aan
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;      // USART2 clock aan

    // --- 2. Configure PA2 (TX) & PA3 (RX) as Alternate Function 7 ---
    GPIOA->MODER &= ~((3U << (2*2)) | (3U << (3*2))); // clear PA2/PA3
    GPIOA->MODER |=  ((2U << (2*2)) | (2U << (3*2))); // set AF mode

    GPIOA->AFR[0] &= ~((0xF << (4*2)) | (0xF << (4*3)));
    GPIOA->AFR[0] |=  ((7 << (4*2)) | (7 << (4*3)));

    GPIOA->OSPEEDR |= ((3U << (2*2)) | (3U << (3*2))); // high speed
    GPIOA->OTYPER  &= ~((1U<<2) | (1U<<3));            // push pull
    GPIOA->PUPDR   &= ~((3U << (2*2)) | (3U << (3*2))); // no pull

    // --- 3. Configure USART2 ---
    USART2->CR1 = 0;          // disable alles eerst
    USART2->CR2 = 0;
    USART2->CR3 = 0;

    /* Baudrate berekening:
       BRR = PCLK1 / Baudrate
       bij oversampling 16: BRR = round(PCLK1 / baudrate)
       bij PCLK1 = 31.5 MHz en baudrate 115200:
       BRR = 31500000 / 115200 ≈ 273.5 → 0x112
    */
    uint32_t brr = (pclk1 + baudrate / 2) / baudrate;
    USART2->BRR = brr;        // correcte berekening met actuele PCLK1!

    // Enable TX + USART
    USART2->CR1 |= USART_CR1_TE;   // transmitter enable
    USART2->CR1 |= USART_CR1_UE;   // USART enable
}


/* Send 1 char */
void UART2_SendChar(char c)
{
    while (!(USART2->SR & USART_SR_TXE));  // wachten tot TX leeg is
    USART2->DR = c;
}

/* Send string */
void UART2_WriteString(const char *str)
{
    while (*str)
    {
        UART2_SendChar(*str++);
    }
}

int main(void)
{
    SystemInit();    // Zorg dat PLL/HSE goed staat
    SystemCoreClockUpdate(); // <--- toegevoegde regel

    UART2_Init();

    pclk1 = get_pclk1();

    UART2_WriteString("UART werkt! Hallo Termite!\r\n");

    while (1)
    {
        UART2_WriteString("Hello Termite!\r\n");
        for (volatile int i = 0; i < 1000000; i++); // simpele delay
    }
}

//int main(void)
//{
////	SystemInit(); // System speed to 168MHz
////
////	UB_VGA_Screen_Init(); // Init VGA-Screen
////
////	UB_VGA_FillScreen(VGA_COL_BLUE);
////
////  while(1)
////  {
////
////  }
//}
