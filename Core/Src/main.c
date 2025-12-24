//--------------------------------------------------------------
// File     : main.c
// Doel     : Eenvoudige UART2 communicatie (Hello naar Termite)
// MCU      : STM32F4xx
//--------------------------------------------------------------

/**

@file main.c

@brief Hoofdprogramma voor UART communicatie.

@author JouwNaam
*/
#include "main.h"
#include <math.h>
#include "APIio.h"
#include "APIerror.h"
#include "APIdraw.h"

int main(void)
{
	SystemInit(); // System speed to 168MHz
	API_init_io();          // init VGA
	SysTick_Init();
	UART2_Init(115200);

	(void)clearscherm("wit");
	(void)wacht(2000);
	(void)lijn(10, 10, 10, 150, "rood", 10);
	(void)wacht(2000);
	(void)rechthoek(20,20,40,80,"geel", 1);
	(void)wacht(2000);
	(void)figuur(10,10,20,20,60,50,80,100,110,114,"groen");
	(void)wacht(2000);
	(void)cirkel(70,50,25,"blauw");
	(void)wacht(2000);
	(void)toren(100,100,20,"groen","geel");
	(void)wacht(2000);
	(void)clearscherm("wit");
	(void)wacht(2000);
	(void)herhaal(14, 2);
	(void)clearscherm("rood");

  while(1)
  {

  }
}























//#define BUFFER_SIZE 128
//char buffer[BUFFER_SIZE]; // De buffer moet buiten main gedefinieerd zijn als je deze overal wilt gebruiken
//
//int main(void)
//{
//    SystemInit();
//    SystemCoreClockUpdate();
//
//    UART2_Init(115200);
//
//    // Initialisatie/Welkomstbericht:
//    // Geef de gebruiker direct de prompt om te beginnen.
//    // De 'Handel_UART_Input' zal bij lege invoer automatisch de HELP tonen.
//    UART2_WriteString("> ");
//
//    UserInput_t input;   // Struct voor de gebruikersgegevens
//
//    while (1)
//    {
//
//        Handel_UART_Input(&input);
//
//        if (strcmp(input.command, "setPixel") == 0)
//        {
//            // Formatteer de output (let op de [0] voor array-elementen!)
//        	int len = snprintf(buffer, sizeof(buffer),
//        	    "Verwerkt: Cmd: %s, X: %d, Y: %d, Color: %s\r\n",
//        	    input.command, input.x[0], input.y[0], input.color_name); // <-- CORRECTIE HIER
//
//            // Verwerkingsoutput printen
//            UART2_WriteString(buffer);
//
//            // Nu kun je 'input' doorgeven aan je middle layer
//            // Bijvoorbeeld: MiddleLayer_ProcessCommand(&input);
//
//            // Geef een nieuwe prompt na de verwerking
//            UART2_WriteString("> ");
//
//        }
//      }
//  }
