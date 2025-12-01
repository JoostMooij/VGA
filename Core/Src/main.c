//--------------------------------------------------------------
// File     : main.c
// Doel     : Eenvoudige UART2 communicatie (Hello naar Termite)
// MCU      : STM32F4xx
//--------------------------------------------------------------

#include "main.h"
/**
>>>>>>> origin/main

@file main.c

@brief Hoofdprogramma voor UART communicatie.

@author JouwNaam
*/
#include "main.h"

#define BUFFER_SIZE 128
char buffer[BUFFER_SIZE]; // De buffer moet buiten main gedefinieerd zijn als je deze overal wilt gebruiken

int main(void)
{
    SystemInit();
    SystemCoreClockUpdate();

    UART2_Init(115200);

	test_logicLayer();
    // Initialisatie/Welkomstbericht:
    // Geef de gebruiker direct de prompt om te beginnen.
    // De 'Handel_UART_Input' zal bij lege invoer automatisch de HELP tonen.
    UART2_WriteString("> ");

    UserInput_t input;   // Struct voor de gebruikersgegevens

    while (1)
    {

        Handel_UART_Input(&input);

        if (strcmp(input.command, "setPixel") == 0)
        {
            // Formatteer de output (let op de [0] voor array-elementen!)
        	int len = snprintf(buffer, sizeof(buffer),
        	    "Verwerkt: Cmd: %s, X: %d, Y: %d, Color: %s\r\n",
        	    input.command, input.x[0], input.y[0], input.color_name); // <-- CORRECTIE HIER

            // Verwerkingsoutput printen
            UART2_WriteString(buffer);

            // Nu kun je 'input' doorgeven aan je middle layer
            // Bijvoorbeeld: MiddleLayer_ProcessCommand(&input);

            // Geef een nieuwe prompt na de verwerking
            UART2_WriteString("> ");

        }
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
