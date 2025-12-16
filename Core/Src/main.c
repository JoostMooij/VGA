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

	(void)bitMap(7, 175, 150);
	(void)bitMap(8, 60, 100);
	(void)bitMap(9, 60, 190);
	(void)drawPixel(100, 100, "rood");
	(void)lijn(10, 10, 10, 150, "rood", 10);
	(void)rechthoek(80, 80, 60, 40, "geel", 0);
	(void)figuur(10,10,60,20,30,30,90,40,150,50,"zwart");
	(void)wacht(100);
	(void)cirkel(90, 90, 50, "blauw");
	(void)toren(180, 150, 45, "groen", "geel");
  while(1)
  {
		(void)bitMap(8, 60, 100);
		(void)wacht(1000);
		(void)bitMap(9, 60, 100);
		(void)wacht(160);
		(void)bitMap(8, 60, 100);
		(void)wacht(160);
		(void)bitMap(9, 60, 100);
		(void)wacht(160);
		(void)bitMap(8, 60, 100);
		(void)wacht(160);
		(void)bitMap(9, 60, 100);
		(void)wacht(160);
		(void)bitMap(8, 60, 100);
		(void)wacht(160);
		(void)bitMap(9, 60, 100);
		(void)wacht(160);

		(void)bitMap(8, 60, 100);
		(void)wacht(1000);
		(void)bitMap(9, 60, 100);
		(void)wacht(90);
		(void)bitMap(8, 60, 100);
		(void)wacht(90);
		(void)bitMap(9, 60, 100);
		(void)wacht(90);
		(void)bitMap(8, 60, 100);
		(void)wacht(90);
		(void)bitMap(9, 60, 100);
		(void)wacht(90);
		(void)bitMap(8, 60, 100);
		(void)wacht(90);
		(void)bitMap(9, 60, 100);
		(void)wacht(90);

		(void)bitMap(8, 60, 100);
		(void)wacht(1000);
		(void)bitMap(9, 60, 100);
		(void)wacht(40);
		(void)bitMap(8, 60, 100);
		(void)wacht(40);
		(void)bitMap(9, 60, 100);
		(void)wacht(40);
		(void)bitMap(8, 60, 100);
		(void)wacht(40);
		(void)bitMap(9, 60, 100);
		(void)wacht(40);
		(void)bitMap(8, 60, 100);
		(void)wacht(40);
		(void)bitMap(9, 60, 100);
		(void)wacht(40);
  }
}
