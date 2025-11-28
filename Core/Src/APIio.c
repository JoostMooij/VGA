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
#include "stm32_ub_vga_screen.h"
#include <stdint.h>
#include <string.h>

void API_init_io(void)
{
    UB_VGA_Screen_Init();
}

ErrorList clearscherm(const char *kleur)
{
	uint8_t color =  kleur_omzetter(kleur);
	ErrorList errors = Error_handling(FUNC_clearscherm, color,
									  0,0,0,0,0,0,0,0,0,0);
    if(errors.error_var1)
    {
        return errors;
    }
    UB_VGA_FillScreen((uint8_t)color);
    return errors;
}

ErrorList drawPixel(int x, int y, const char *kleur)
{
	uint8_t color =  kleur_omzetter(kleur);
    ErrorList errors = Error_handling(FUNC_drawPixel, x, y, color,
                                      0,0,0,0,0,0,0,0);
    if(errors.error_var1 || errors.error_var2 || errors.error_var3)
    {
        return errors;
    }
    UB_VGA_SetPixel(x, y, (uint8_t)color);
    return errors;
}

uint8_t kleur_omzetter(const char *input)
{
    if (strcmp(input, "zwart") == 0)        return ZWART;
    if (strcmp(input, "blauw") == 0)        return BLAUW;
    if (strcmp(input, "lichtblauw") == 0)   return LICHTBLAUW;
    if (strcmp(input, "groen") == 0)        return GROEN;
    if (strcmp(input, "lichtgroen") == 0)   return LICHTGROEN;
    if (strcmp(input, "cyaan") == 0)        return CYAAN;
    if (strcmp(input, "lichtcyaan") == 0)   return LICHTCYAAN;
    if (strcmp(input, "rood") == 0)         return ROOD;
    if (strcmp(input, "lichtrood") == 0)    return LICHTROOD;
    if (strcmp(input, "magenta") == 0)      return MAGENTA;
    if (strcmp(input, "lichtmagenta") == 0) return LICHTMAGENTA;
    if (strcmp(input, "bruin") == 0)        return BRUIN;
    if (strcmp(input, "geel") == 0)         return GEEL;
    if (strcmp(input, "grijs") == 0)        return GRIJS;
    if (strcmp(input, "wit") == 0)          return WIT;
    if (strcmp(input, "roze") == 0)         return ROZE;
    if (strcmp(input, "paars") == 0)        return PAARS;
    return 0; // ongeldig
}
