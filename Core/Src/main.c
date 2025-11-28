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
	SystemInit(); // System speed to 168MHz
	API_init_io();          // init VGA
	(void)clearscherm("wit");
	(void)drawPixel(100, 100, "rood");
	(void)lijn(10, 10, 10, 150, "rood", 10);

  while(1)
  {

  }
}
