//--------------------------------------------------------------
// File     : main.c
// Datum    : 30.03.2016
// Version  : 1.0
// Autor    : UB
// mods by	: J.F. van der Bent
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.x
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Function : VGA_core DMA LIB 320x240, 8bit color
//--------------------------------------------------------------

#include "main.h"
#include <math.h>
#include "APIio.h"
#include "APIerror.h"
#include "APIdraw.h"

int main(void)
{
	SystemInit();
	API_init_io();
	(void)clearscherm("rood");
	(void)drawPixel(100, 100, "blauw");
	(void)lijn(10, 10, 10, 150, "groen", 10);
	(void)rechthoek(80, 80, 60, 40, "paars", 0);
  while(1)
  {
  }
}
