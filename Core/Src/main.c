//--------------------------------------------------------------
// File     : main.c
// Doel     : Eenvoudige UART2 communicatie (Hello naar Termite)
// MCU      : STM32F4xx
//--------------------------------------------------------------

#include "UART.h"

#define RX_BUF_SIZE 64

int main(void)
{
    SystemInit();
    SystemCoreClockUpdate();

    UART2_Init(115200);

    char rx_buf[RX_BUF_SIZE]; // declareer buffer

    UART2_WriteString("Stuur een bericht en druk Enter:\r\n");

    while (1)
    {
        UART2_ReadString(rx_buf, RX_BUF_SIZE); // wacht en ontvang string
        UART2_WriteString("Ontvangen: ");
        UART2_WriteString(rx_buf);
        UART2_WriteString("\r\n");
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
