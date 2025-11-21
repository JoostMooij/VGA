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

int main(void)
{
	SystemInit(); // System speed to 168MHz
    API_init_io(0,0);          // init VGA
    API_clearscreen(0);        // zwart scherm
    API_io_draw_pixel(100, 50, ROOD); // rode pixel op (100,50)
  while(1)
  {

  }
}
