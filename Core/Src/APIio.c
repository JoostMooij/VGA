/**
 * @file APIio.c
 * @brief Implementatie van de I/O-laag van de EE-API-LIB.
 *
 * Wrapt hardware drivers (UB_VGA, UART) naar nette API-functies.
 * Functies geven altijd een foutcode terug.
 * Dit bestand vormt de basisbouwstenen voor APIdraw.
 *
 * @author Thijs
 * @version 1.0
 * @date 2025-11-20
 */

#include "APIio.h"
#include "APIerror.h"
#include "stm32_ub_vga_screen.h"   // nodig voor UB_VGA_SetPixel en defines
#include <stdint.h>

/**
 * @brief Initialiseert de I/O hardware.
 *
 * Roept de driver init-functie aan voor VGA en eventuele andere devices.
 *
 * @param dev1 ID van device 1 (optioneel)
 * @param dev2 ID van device 2 (optioneel)
 * @return NO_ERROR bij succes
 */
int API_init_io(int dev1, int dev2)
{
    UB_VGA_Screen_Init();
    return NO_ERROR;
}

/**
 * @brief Leegt het scherm met een bepaalde kleur.
 *
 * @param color 8-bit kleurwaarde om het scherm mee te vullen
 * @return NO_ERROR bij succes
 */
int API_clearscreen(int color)
{
    UB_VGA_FillScreen((uint8_t)color);
    return NO_ERROR;
}

/**
 * @brief Zet één pixel op het VGA-scherm met foutafhandeling.
 *
 * @param x X-coördinaat
 * @param y Y-coördinaat
 * @param color Kleur
 * @return ErrorList met alle fouten tegelijk (0 = geen fout, 1 = fout)
 */
ErrorList API_io_draw_pixel(int x, int y, int color)
{
    ErrorList errors = Error_handling(FUNC_SET_PIXEL, x, y, color,
                                      0,0,0,0,0,0,0,0);

    if(errors.error_var1 || errors.error_var2 || errors.error_var3)
    {
        return errors;
    }

    UB_VGA_SetPixel(x, y, (uint8_t)color);
    return errors;
}

