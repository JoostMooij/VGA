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
	(void)clearscherm("wit");
	(void)tekst(1, 1, "zwart", "abcdefghijklmnopqrstuvwxyz pearl", "pearl", 1, "normaal");
	(void)tekst(1, 10, "zwart", "ABCDEFGHIJKLMNOPQRSTUVWXYZ pearl", "pearl", 1, "vet");
	(void)tekst(1, 20, "zwart", "?!@#$%^&*)_+-={]:';<>,./ pearl", "pearl", 1, "cursief");

	(void)tekst(1, 30, "zwart", "abcdefghijklmnopqrstuvwxyz acorn", "acorn", 1, "normaal");
	(void)tekst(1, 40, "zwart", "ABCDEFGHIJKLMNOPQRSTUVWXYZ acorn", "acorn", 1, "vet");
	(void)tekst(1, 50, "zwart", "?!@#$%^&*)_+-={]:';<>,./ acorn", "acorn", 1, "cursief");

	(void)tekst(1, 60, "zwart", "abcdefghijklmnopqrstuvwxyz pearl", "pearl", 2, "normaal");
	(void)tekst(1, 75, "zwart", "?!@#$%^&*)_+-={]:';<>,./ pearl", "pearl", 2, "normaal");
	(void)tekst(1, 90, "zwart", "abcdefghijklmnopqrstuvwxyz pearl", "acorn", 2, "normaal");
	(void)tekst(1, 105, "zwart", "?!@#$%^&*)_+-={]:';<>,./ acorn", "acorn", 2, "normaal");

//	(void)tekst(5, 5, "zwart", "Riegiel is", "pearl", 3, "normaal");
//	(void)tekst(5, 35, "zwart", "een top", "pearl", 3, "cursief");
//	(void)tekst(5, 65, "zwart", "docent", "pearl", 3, "vet");
	(void)bitMap(7, 100, 180);

  while(1)
  {

  }
}
