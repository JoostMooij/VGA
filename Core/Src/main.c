//--------------------------------------------------------------
// File     : main.c
// Doel     : Koppeling tussen UART en Logic Layer
// MCU      : STM32F4xx
//--------------------------------------------------------------

#include "main.h"
#include "Front_layer.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    SystemInit();
    SystemCoreClockUpdate();

    // Init UART (snelheid moet matchen met Termite, bijv. 115200)
    UART2_Init(115200);

    // Init VGA (als je dat al wilt aanzetten)
    // UB_VGA_Screen_Init();
    // UB_VGA_FillScreen(VGA_COL_BLACK);

    UART2_WriteString("Systeem Start...\r\n");
    //Handle_HELP();

    UserInput_t input; // De struct waar de data in komt
	char msg[256];     // Een buffer die groot genoeg is voor de debug tekst

	while(1)
	    {
	        // 2. Wacht op input en laat front_layer de string bouwen
	        Handel_UART_Input(&input);

	        // 3. Check of er iets in de string staat
	        if(strlen(input.full_command) > 0)
	        {
	            // Omdat front_layer de string al netjes heeft gemaakt (bijv: "lijn 10 10 200 200 5 ROOD"),
	            // kunnen we die direct printen of doorsturen.

	            UART2_WriteString("Main stuurt naar Logic: ");
	            UART2_WriteString(input.full_command);
	            UART2_WriteString("\r\n");

	            // Hier roep je straks je Logic Layer aan met de string:
	            // Logic_ParseAndExecute(input.full_command);
	        }
	    }
}
