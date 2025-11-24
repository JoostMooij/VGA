//--------------------------------------------------------------
// File     : main.c
// Doel     : Eenvoudige UART2 communicatie (Hello naar Termite)
// MCU      : STM32F4xx
//--------------------------------------------------------------

#include "UART.h"
#include "Front_layer.h"

char buffer[128];

int main(void)
{
    SystemInit();
    SystemCoreClockUpdate();

    UART2_Init(115200);

    Gebruiker_instructies();

    UserInput_t input;   // Struct voor de gebruikersgegevens

    while (1)
    {
        // Vul 'input' met gegevens via de front layer
        Handel_UART_Input(&input);

        sprintf(buffer, "Cmd: %s, X: %d, Y: %d, Color: 0x%02X\r\n",
                input.command, input.x, input.y, input.color);

        UART2_WriteString(buffer);

        // Nu kun je 'input' doorgeven aan je middle layer of verwerkingslaag
        // Bijvoorbeeld: MiddleLayer_ProcessCommand(&input);
        // Die zorgt voor verdere verwerking (pixel tekenen etc.)
    }
}


//int main(void)
//{
//	SystemInit(); // System speed to 168MHz
//
//	UB_VGA_Screen_Init(); // Init VGA-Screen
//
//	UB_VGA_FillScreen(VGA_COL_BLUE);
//
//  while(1)
//  {
//
//  }
//}
