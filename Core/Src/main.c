//--------------------------------------------------------------
// File     : main.c
// Doel     : Koppeling tussen UART en Logic Layer
// MCU      : STM32F4xx
//--------------------------------------------------------------

#include "main.h"
#include "Front_layer.h" // Zorg dat deze included is!
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
		// 2. Wacht op input van de gebruiker (blokkerende functie)
		Handel_UART_Input(&input);

		// 3. Check of er een geldig commando is gevonden
		if(strlen(input.command) > 0)
		{
			// We bouwen een string die alle belangrijke velden laat zien.
			// Ik print hier x[0] t/m x[2] en y[0] t/m y[2] zodat je ook lijnen en figuren kunt checken.

			sprintf(msg, "Check: Cmd='%s' | X=[%d, %d, %d] | Y=[%d, %d, %d] | Kleur='%s' | Extra='%s'\r\n",
					input.command,
					input.x[0], input.x[1], input.x[2],  // Eerste 3 X waardes (ook dikte/radius etc)
					input.y[0], input.y[1], input.y[2],  // Eerste 3 Y waardes
					input.color_name,                    // De kleur
					input.text_arg);                     // Tekst of 2e kleur

			// Stuur de geformatteerde tekst naar Termite
			UART2_WriteString(msg);

			// Hier zou je Logic_Execute(&input) aanroepen.
		}
	}
}
