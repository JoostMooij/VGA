/**
 * @file APIdraw.c
 * @brief Implementatie van de tekenlaag van de EE‑API‑LIB.
 *
 * Bouwt hogere tekenfuncties bovenop APIio:
 * - Gebruikt API_io_draw_pixel() als basis
 * - Implementeert algoritmes (bv. Bresenham voor lijnen)
 * - Combineert pixels tot vormen en tekst
 *
 * @author Thijs
 * @version 1.0
 * @date 2025-11-20
 */

#include "APIdraw.h"
#include "APIio.h"
#include "APIerror.h"
#include <stdlib.h>

/**
 * @brief Teken een lijn van (x1,y1) naar (x2,y2) met opgegeven kleur en dikte.
 *
 * @param x1 Start X-coordinaat
 * @param y1 Start Y-coordinaat
 * @param x2 Eind X-coordinaat
 * @param y2 Eind Y-coordinaat
 * @param kleur Kleur als string
 * @param dikte Dikte van de lijn in pixels
 * @return ErrorList Struct met eventuele fouten (0 = geen fouten)
 *
 * @note Gebruikt Bresenham-algoritme en voegt dikte toe door extra pixels.
 */
ErrorList lijn(int x1, int y1, int x2, int y2, const char *kleur, int dikte)
{
    uint8_t color = kleur_omzetter(kleur);

    ErrorList errors = Error_handling(FUNC_lijn, x1, y1, x2, y2, color, dikte, 0,0,0,0,0);
    if (errors.error_var1 || errors.error_var2 || errors.error_var3 || errors.error_var4 || errors.error_var5 || errors.error_var6)
    {
        return errors;
    }
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;
    while (1)
    {
        // teken de dikte rondom de lijn
        for (int ox = -(dikte/2); ox <= dikte/2; ox++)
        {
            for (int oy = -(dikte/2); oy <= dikte/2; oy++)
            {
                (void)drawPixel(x1 + ox, y1 + oy, kleur);
            }
        }
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
    return errors;
}

/**
 * @brief Teken een rechthoek op scherm met opgegeven kleur en gevuld-optie.
 *
 * @param x Linker bovenhoek X
 * @param y Linker bovenhoek Y
 * @param w Breedte van de rechthoek
 * @param h Hoogte van de rechthoek
 * @param kleur Kleur als string
 * @param gevuld 1 = gevuld, 0 = alleen rand
 * @return ErrorList Struct met eventuele fouten (0 = geen fouten)
 *
 * @note Bij gevulde rechthoek worden alle pixels binnen het gebied gezet,
 *       anders enkel de rand.
 */
ErrorList rechthoek(int x, int y, int w, int h, const char *kleur, int gevuld)
{
    uint8_t color = kleur_omzetter(kleur);
    ErrorList errors = Error_handling(FUNC_rechthoek, x, y, w, h, color, gevuld, 0,0,0,0,0);
    if (errors.error_var1 || errors.error_var2 || errors.error_var3 || errors.error_var4 || errors.error_var5 || errors.error_var6)
    {
        return errors;
    }
    int x2 = x + w - 1;
    int y2 = y + h - 1;
    if (gevuld)
    {
        for (unsigned int j = y; j <= y2; j++)
        {
            for (unsigned int i = x; i <= x2; i++)
            {
                drawPixel(i, j, kleur);
            }
        }
    }
    else
    {
        for (unsigned int i = x; i <= x2; i++)
        {
        	drawPixel(i, y,  kleur);
        }
        for (unsigned int i = x; i <= x2; i++)
        {
        	drawPixel(i, y2, kleur);
        }
        for (unsigned int j = y; j <= y2; j++)
        {
        	drawPixel(x,  j, kleur);
        }
        for (unsigned int j = y; j <= y2; j++)
        {
        	drawPixel(x2, j, kleur);
        }
    }
    return errors;
}
