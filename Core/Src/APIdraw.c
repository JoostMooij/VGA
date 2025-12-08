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
#include "bitMap.h"

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

ErrorList figuur(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, const char *kleur)
{
    uint8_t color = kleur_omzetter(kleur);
    ErrorList errors = Error_handling(FUNC_figuur, x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, color);
    if(errors.error_var1 || errors.error_var2 || errors.error_var3 || errors.error_var4 || errors.error_var5 || errors.error_var6 || errors.error_var7 || errors.error_var8 || errors.error_var9 || errors.error_var10 || errors.error_var11)
    {
        return errors;
    }
    // 5 punten → 5 lijnen vormen een gesloten vorm
    lijn(x1, y1, x2, y2, kleur, 1);
    lijn(x2, y2, x3, y3, kleur, 1);
    lijn(x3, y3, x4, y4, kleur, 1);
    lijn(x4, y4, x5, y5, kleur, 1);
    lijn(x5, y5, x1, y1, kleur, 1);
    return errors;
}

ErrorList cirkel(int x0, int y0, int radius, const char* kleur)
{
    uint8_t color = kleur_omzetter(kleur);
    ErrorList errors = Error_handling(FUNC_cirkel, x0, y0, radius, color, 0,0,0,0,0,0,0);
    if(errors.error_var1 || errors.error_var2 || errors.error_var3 || errors.error_var4)
    {
        return errors;
    }
    int x = radius;
    int y = 0;
    int err = 0;
    while (x >= y)
    {
        drawPixel(x0 + x, y0 + y, kleur);
        drawPixel(x0 + y, y0 + x, kleur);
        drawPixel(x0 - y, y0 + x, kleur);
        drawPixel(x0 - x, y0 + y, kleur);
        drawPixel(x0 - x, y0 - y, kleur);
        drawPixel(x0 - y, y0 - x, kleur);
        drawPixel(x0 + y, y0 - x, kleur);
        drawPixel(x0 + x, y0 - y, kleur);
        y += 1;
        if(err <= 0)
        {
            err += 2*y + 1;
        }
        if(err > 0)
        {
            x -= 1;
            err -= 2*x + 1;
        }
    }
    return errors;
}

/**
 * @brief Tekent een bitmap symbool, gecentreerd op (x, y).
 *
 * @param nr Het nummer van het symbool (1 = pijl omhoog).
 * @param x X-coordinaat van het midden van het symbool.
 * @param y Y-coordinaat van het midden van het symbool.
 * @return ErrorList Struct met eventuele fouten (0 = geen fouten)
 */
// ... (bestaande functies lijn, rechthoek, figuur, cirkel) ...

/**
 * @brief Tekent een bitmap symbool, gecentreerd op (x, y).
 *
 * @param nr Het nummer van het symbool (1 = pijl omhoog, 2 = pijl omlaag, etc.).
 * @param x X-coordinaat van het midden van het symbool.
 * @param y Y-coordinaat van het midden van het symbool.
 * @return ErrorList Struct met eventuele fouten (0 = geen fouten)
 */
ErrorList bitMap(int nr, int x, int y)
{
    ErrorList errors = {0};
    const Bitmap *bmp_ptr = NULL;

    switch (nr)
    {
        case 1: bmp_ptr = &arrow_up;    break;
        case 2: bmp_ptr = &arrow_down;  break;
        case 3: bmp_ptr = &arrow_left;  break;
        case 4: bmp_ptr = &arrow_right; break;
        case 5: bmp_ptr = &yes_cat_thumbs_up; break;
        default: return errors;
    }

    if (bmp_ptr == NULL || bmp_ptr->data == NULL)
        return errors;

    int x0 = x - (bmp_ptr->width / 2);
    int y0 = y - (bmp_ptr->height / 2);

    drawBitmap(x0, y0, bmp_ptr, 0); // 8-bit data bevat al de kleurcodes

    return errors;
}

