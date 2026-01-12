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
	(void)bitMap(1, 50, 50);
	(void)bitMap(2, 70, 50);
	(void)bitMap(3, 90, 50);
	(void)bitMap(4, 110, 50);
	(void)bitMap(5, 160, 50);
	(void)bitMap(6, 220, 50);

	(void)bitMap(10, 175, 150);
	(void)bitMap(8, 60, 100);
	(void)bitMap(9, 60, 190);

  while(1)
  {

  }
}
