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
    UB_VGA_Screen_Init();
    UART2_Init(115200);

    // Initialisatie/Welkomstbericht:
    // Geef de gebruiker direct de prompt om te beginnen.
    // De 'Handel_UART_Input' zal bij lege invoer automatisch de HELP tonen.
    UART2_WriteString("> ");
    Handle_HELP();

    UserInput_t input;   // Struct voor de gebruikersgegevens

    while(1)
	{
		// 1. Wacht op input en verwerk deze tot een complete string
		Handel_UART_Input(&input);

		// 2. Check wat er in de struct staat (Debugging)
		// Nu bevat input.full_command zoiets als "setPixel 100 50 BLUE"
		if(strlen(input.full_command) > 0)
		{
			char msg[100];
			sprintf(msg, "Main ontving string: '%s'\r\n> ", input.full_command);
			UART2_WriteString(msg);
			string_ophalen(input.full_command);


			// Hier zou je deze string doorsturen naar je Logic Layer:
			// API_ParseAndExecute(input.full_command);
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
