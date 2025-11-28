/**
 * @file APIdraw.c
 * @brief Implementatie van de tekenlaag van de EE‑API‑LIB.
 *
 * Bouwt hogere tekenfuncties bovenop APIio:
 * - Gebruikt API_io_draw_pixel() als basis
 * - Implementeert algoritmes (bv. Bresenham voor lijnen)
 * - Combineert pixels tot vormen en tekst
 */

#include "APIdraw.h"
#include "APIio.h"
#include "APIerror.h"

ErrorList lijn(int x1, int y1, int x2, int y2, const char *kleur, int dikte)
{
	uint8_t color =  kleur_omzetter(kleur);
	ErrorList errors = Error_handling(FUNC_lijn, x1, y1, x2, y2, color, dikte,
								  0,0,0,0,0);
    if(errors.error_var1 || errors.error_var2 || errors.error_var3 || errors.error_var4|| errors.error_var5|| errors.error_var6)
    {
        return errors;
    }
	// diagonale lijn (Bresenham + dikte)
	int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
	int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
	int err = dx + dy, e2;

	while(1)
	{
		// teken de "dikte" rondom de lijn
		for(int ox = -(dikte/2); ox <= dikte/2; ox++)
		{
			for(int oy = -(dikte/2); oy <= dikte/2; oy++)
			{
				(void)drawPixel(x1 + ox, y1 + oy, kleur);
			}
		}
		if(x1 == x2 && y1 == y2) break;
		e2 = 2 * err;
		if(e2 >= dy) { err += dy; x1 += sx; }
		if(e2 <= dx) { err += dx; y1 += sy; }
	}
    return errors;
}
