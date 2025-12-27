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
#include "font_pearl_8x8.h"
#include "font_acorn_8x8.h"

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
 * @note Gebruikt Bresenham-algoritme en voegt dikte toe door extra pixels rondom de lijn.
 */
ErrorList lijn(int x1, int y1, int x2, int y2, const char *kleur, int dikte)
{
	uint8_t color = kleur_omzetter(kleur);
    ErrorList errors = Error_handling(FUNC_lijn, x1, y1, x2, y2, color, dikte, 0,0,0,0,0);

    if(errors.error_var1 || errors.error_var2 || errors.error_var3 || errors.error_var4 || errors.error_var5 || errors.error_var6)
        return errors;

    if (herhaal_hoog == 0)
    {
		int params[] = {x1, y1, x2, y2, color, dikte};
		record_command(CMD_LIJN, 6, params);
    }

    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while(1)
    {
        for(int ox = -(dikte/2); ox <= dikte/2; ox++)
            for(int oy = -(dikte/2); oy <= dikte/2; oy++)
                (void)drawPixel(x1 + ox, y1 + oy, kleur);

        if(x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if(e2 >= dy) { err += dy; x1 += sx; }
        if(e2 <= dx) { err += dx; y1 += sy; }
    }
    return errors;
}

/**
 * @brief Teken een rechthoek op scherm met opgegeven kleur en gevuld-optie.
 *
 * @param x Linker bovenhoek X-coordinaat
 * @param y Linker bovenhoek Y-coordinaat
 * @param w Breedte van de rechthoek in pixels
 * @param h Hoogte van de rechthoek in pixels
 * @param kleur Kleur als string
 * @param gevuld 1 = gevuld, 0 = alleen rand
 * @return ErrorList Struct met eventuele fouten (0 = geen fouten)
 *
 * @note Bij gevulde rechthoek worden alle pixels binnen het gebied gezet,
 *       anders wordt alleen de rand getekend.
 */
ErrorList rechthoek(int x, int y, int w, int h, const char *kleur, int gevuld)
{
    uint8_t color = kleur_omzetter(kleur);
    ErrorList errors = Error_handling(FUNC_rechthoek, x, y, w, h, color, gevuld, 0,0,0,0,0);

    if(errors.error_var1 || errors.error_var2 || errors.error_var3 || errors.error_var4 || errors.error_var5 || errors.error_var6)
        return errors;

    if (herhaal_hoog == 0)
    {
		int params[] = {x, y, w, h, color, gevuld};
		record_command(CMD_RECHTHOEK, 6, params);
    }

    int x2 = x + w - 1;
    int y2 = y + h - 1;

    if(gevuld)
    {
        for(unsigned int j = y; j <= y2; j++)
            for(unsigned int i = x; i <= x2; i++)
                drawPixel(i, j, kleur);
    }
    else
    {
        for(unsigned int i = x; i <= x2; i++) drawPixel(i, y, kleur);
        for(unsigned int i = x; i <= x2; i++) drawPixel(i, y2, kleur);
        for(unsigned int j = y; j <= y2; j++) drawPixel(x, j, kleur);
        for(unsigned int j = y; j <= y2; j++) drawPixel(x2, j, kleur);
    }
    return errors;
}

/**
 * @brief Teken een gesloten figuur bestaande uit 5 punten.
 *
 * @param x1 X-coordinaat punt 1
 * @param y1 Y-coordinaat punt 1
 * @param x2 X-coordinaat punt 2
 * @param y2 Y-coordinaat punt 2
 * @param x3 X-coordinaat punt 3
 * @param y3 Y-coordinaat punt 3
 * @param x4 X-coordinaat punt 4
 * @param y4 Y-coordinaat punt 4
 * @param x5 X-coordinaat punt 5
 * @param y5 Y-coordinaat punt 5
 * @param kleur Kleur als string
 * @return ErrorList Struct met eventuele fouten (0 = geen fouten)
 *
 * @note Verbindt de punten met lijnen en vormt zo een gesloten vijfhoek.
 */
ErrorList figuur(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, const char *kleur)
{
    uint8_t color = kleur_omzetter(kleur);
    ErrorList errors = Error_handling(FUNC_figuur, x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, color);

    if(errors.error_var1 || errors.error_var2 || errors.error_var3 || errors.error_var4 || errors.error_var5 ||
       errors.error_var6 || errors.error_var7 || errors.error_var8 || errors.error_var9 || errors.error_var10 ||
       errors.error_var11)
        return errors;

    if (herhaal_hoog == 0)
    {
		int params[] = {x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, color};
		record_command(CMD_FIGUUR, 11, params);
    }

    lijn(x1, y1, x2, y2, kleur, 1);
    lijn(x2, y2, x3, y3, kleur, 1);
    lijn(x3, y3, x4, y4, kleur, 1);
    lijn(x4, y4, x5, y5, kleur, 1);
    lijn(x5, y5, x1, y1, kleur, 1);

    return errors;
}

/**
 * @brief Teken een cirkel met opgegeven straal en kleur.
 *
 * @param x0 X-coordinaat van het midden
 * @param y0 Y-coordinaat van het midden
 * @param radius Straal van de cirkel in pixels
 * @param kleur Kleur als string
 * @return ErrorList Struct met eventuele fouten (0 = geen fouten)
 *
 * @note Gebruikt het Bresenham-algoritme voor cirkels.
 */
ErrorList cirkel(int x0, int y0, int radius, const char* kleur)
{
    uint8_t color = kleur_omzetter(kleur);
    ErrorList errors = Error_handling(FUNC_cirkel, x0, y0, radius, color, 0,0,0,0,0,0,0);

    if(errors.error_var1 || errors.error_var2 || errors.error_var3 || errors.error_var4)
        return errors;

    if (herhaal_hoog == 0)
    {
		int params[] = {x0, y0, radius, color};
		record_command(CMD_CIRKEL, 4, params);
    }

    int x = radius;
    int y = 0;
    int err = 0;

    while(x >= y)
    {
        drawPixel(x0 + x, y0 + y, kleur);
        drawPixel(x0 + y, y0 + x, kleur);
        drawPixel(x0 - y, y0 + x, kleur);
        drawPixel(x0 - x, y0 + y, kleur);
        drawPixel(x0 - x, y0 - y, kleur);
        drawPixel(x0 - y, y0 - x, kleur);
        drawPixel(x0 + y, y0 - x, kleur);
        drawPixel(x0 + x, y0 - y, kleur);
        y++;
        if(err <= 0) err += 2*y + 1;
        if(err > 0) { x--; err -= 2*x + 1; }
    }

    return errors;
}

/**
 * @brief Teken een toren bestaande uit meerdere lagen, mast en vlag.
 *
 * @param x X-coordinaat van het midden van de torenbasis
 * @param y Y-coordinaat van de torenbasis
 * @param grootte Basisgrootte van de toren
 * @param kleur1 Kleur van de torenlagen
 * @param kleur2 Kleur van mast en vlag
 * @return ErrorList Struct met eventuele fouten (0 = geen fouten)
 *
 * @note Bouwt de toren laag voor laag op, tekent vervolgens mast en vlag.
 */

/////////////////////////////////
//
//  Code copied from: chatGPT
//
/////////////////////////////////
ErrorList toren(int x, int y, int grootte, const char* kleur1, const char* kleur2)
{
    uint8_t c1 = kleur_omzetter(kleur1);
    uint8_t c2 = kleur_omzetter(kleur2);

    // Controleer of de waarden geldig zijn
    ErrorList errors = Error_handling(FUNC_toren, x, y, grootte, c1, c2, 0, 0, 0, 0, 0, 0);
    if(errors.error_var1 || errors.error_var2 || errors.error_var3 || errors.error_var4 || errors.error_var5)
        return errors;

    if (herhaal_hoog == 0)
    {
		int params[] = {x, y, grootte, c1, c2};
		record_command(CMD_TOREN, 5, params);
    }

    int lagen = 6;                    // aantal torenlagen
    int basis_radius = grootte;       // basisradius van de toren
    int hoogte_per_laag = grootte / 2;// hoogte van elke laag
    int top_radius = basis_radius / 3;// radius van de bovenste laag

    // Bouw de lagen van de toren
    for(int l = 0; l < lagen; l++)
    {
        int r = (l < lagen - 1)
                ? basis_radius - (l * (basis_radius - top_radius) / (lagen - 1))
                : top_radius;

        int y_center = y - (l * hoogte_per_laag);

        (void)cirkel(x, y_center, r, kleur1);       // buitenste cirkel
        (void)cirkel(x, y_center, r - 2, kleur1);   // binnenste cirkel voor diepte

        // Kolommen tekenen voor lagen boven de basis
        if(l > 0)
        {
            int kolom_hoogte = hoogte_per_laag;
            int y_start_kolom = y_center + (kolom_hoogte / 2);
            int y_eind_kolom = y_center - (kolom_hoogte / 2);

            (void)lijn(x - r + 1, y_start_kolom, x - r + 1, y_eind_kolom, kleur1, 1);
            (void)lijn(x + r - 1, y_start_kolom, x + r - 1, y_eind_kolom, kleur1, 1);
        }
    }

    // --- MAST EN VLAG ---
    int y_top_center = y - ((lagen - 1) * hoogte_per_laag);
    int y_toren_top_pixel = y_top_center - top_radius;
    int mast_hoogte = grootte / 4;
    int y_mast_top_pixel = y_toren_top_pixel - mast_hoogte;

    // Mast tekenen
    (void)rechthoek(x, y_mast_top_pixel, 1, mast_hoogte, kleur2, 1);

    // Vlag tekenen
    (void)figuur(
        x,              y_mast_top_pixel,
        x + 5,          y_mast_top_pixel - 2,
        x + 5,          y_mast_top_pixel - 7,
        x,              y_mast_top_pixel - 5,
        x,              y_mast_top_pixel,
        kleur2
    );

    return errors;
}

/////////////////////////////////
//
//  end copied code
//
/////////////////////////////////

/**
 * @brief Tekent een bitmap symbool, gecentreerd op (x, y).
 *
 * @param nr Het nummer van het symbool (1 = pijl omhoog, 2 = pijl omlaag, etc.).
 * @brief Tekent een bitmap symbool met (x,y) als linkerbovenhoek.
 * @return ErrorList Struct met eventuele fouten (0 = geen fouten)
 */
ErrorList bitMap(int nr, int x, int y)
{
    ErrorList errors = Error_handling(FUNC_bitmap, nr, x, y, 0, 0, 0, 0, 0, 0, 0, 0);
    if(errors.error_var1 || errors.error_var2 || errors.error_var3)
        return errors;

    const Bitmap *bmp_ptr = NULL;
    uint8_t use_transparency = 0;

    switch (nr)
    {
        case 1: bmp_ptr = &arrow_up;    use_transparency = 1; break;
        case 2: bmp_ptr = &arrow_down;  use_transparency = 1; break;
        case 3: bmp_ptr = &arrow_left;  use_transparency = 1; break;
        case 4: bmp_ptr = &arrow_right; use_transparency = 1; break;
        case 5: bmp_ptr = &smily_blij;  use_transparency = 1; break;
        case 6: bmp_ptr = &smily_boos;  use_transparency = 1; break;
        case 7: bmp_ptr = &yes_cat_thumbs_up; use_transparency = 0; break;
        case 8: bmp_ptr = &skeleton_banging_on_shield_meme_frame_00; use_transparency = 0; break;
        case 9: bmp_ptr = &skeleton_banging_on_shield_meme_frame_06; use_transparency = 0; break;
        default: return errors;
    }

    if (bmp_ptr == NULL || bmp_ptr->data == NULL)
        return errors;

    /* (x,y) is nu linkerbovenhoek */
    drawBitmap(x, y, bmp_ptr, use_transparency);

    return errors;
}

/**
 * @brief Tekent een tekststring op het scherm met gespecificeerde stijl en schaal,
 * gebruikmakend van het Pearl 8x8 bitmap font.
 * * @param x De start X-coördinaat.
 * @param y De start Y-coördinaat.
 * @param kleur_str De kleur als string (bv. "zwart"), welke direct wordt doorgegeven aan drawPixel.
 * @param tekst_str De tekst die getekend moet worden.
 * @param fontnaam De naam van het font (moet "pearl" zijn voor deze implementatie).
 * @param fontgrootte De schaalfactor (1 is 8x8, 2 is 16x16, etc.).
 * @param fontstijl De stijl ("normaal" of "vet").
 * @return ErrorList Retourneert 0 bij succes.
 */
ErrorList tekst(int x, int y, const char *kleur_str, const char* tekst_str, const char* fontnaam, int schaal_factor, const char* fontstijl)
{
    // --- 1. Error Handling ---

    // Zet de kleur-string om naar een integer voor de validatie
    // (Zorg dat kleur_omzetter beschikbaar is via een header of extern)
    int kleur_val = kleur_omzetter(kleur_str);

    // Roep de centrale error handler aan.
    // We casten de pointers naar int omdat de interface van Error_handling dat vereist.
    ErrorList errors = Error_handling(FUNC_tekst,
                                     x, y, kleur_val,
                                     (int)tekst_str, (int)fontnaam, schaal_factor, (int)fontstijl,
                                     0, 0, 0, 0);

    // Controleer of er fouten zijn gevonden in de variabelen die we hebben meegegeven
    if (errors.error_var1 != NO_ERROR || // X-fout
        errors.error_var2 != NO_ERROR || // Y-fout
        errors.error_var3 != NO_ERROR || // Kleur-fout
        errors.error_var4 != NO_ERROR)   // Tekst/Breedte/Stijl-fout (uit check_tekst_op_scherm)
    {
        // Optioneel: stuur hier een UART bericht met de specifieke error
        return errors;
    }

    // --- 2. Voorbereiding voor tekenen ---

    const unsigned char *active_font_data;
    if (strcmp(fontnaam, "pearl") == 0) {
        active_font_data = fontdata_pearl8x8;
    } else if (strcmp(fontnaam, "acorn") == 0) {
        active_font_data = fontdata_acorn8x8;
    } else {
        active_font_data = fontdata_pearl8x8;
    }

    int is_vet    = (strcmp(fontstijl, "vet") == 0);
    int is_cursief = (strcmp(fontstijl, "cursief") == 0);
    int current_char_index = 0;

    // --- 3. Hoofdloop: Karakter voor Karakter ---
    while (tekst_str[current_char_index] != '\0') {

        unsigned char karakter = (unsigned char)tekst_str[current_char_index];
        int bitmap_index = karakter * BASE_FONT_SIZE;

        for (int rij = 0; rij < BASE_FONT_SIZE; rij++) {
            unsigned char rij_data = active_font_data[bitmap_index + rij];

            int offset_schuin = 0;
            if (is_cursief) {
                offset_schuin = (rij / 3) * schaal_factor;
            }

            for (int bit = 0; bit < BASE_FONT_SIZE; bit++) {
                if ((rij_data >> (7 - bit)) & 1) {
                    for (int s_y = 0; s_y < schaal_factor; s_y++) {
                        for (int s_x = 0; s_x < schaal_factor; s_x++) {

                            int pixel_x = x + (bit * schaal_factor) + s_x + offset_schuin;
                            int pixel_y = y + (rij * schaal_factor) + s_y;

                            // Teken de basis pixel
                            (void)drawPixel(pixel_x, pixel_y, kleur_str);

                            if (is_vet) {
                                // Teken de extra pixel voor vetgedrukt
                                (void)drawPixel(pixel_x + schaal_factor, pixel_y, kleur_str);
                            }
                        }
                    }
                }
            }
        }

        // --- 4. Bereken positie voor volgend karakter ---
        int char_width = BASE_FONT_SIZE * schaal_factor;

        if (is_vet) {
            char_width += schaal_factor;
        }

        if (is_cursief) {
            char_width += (7 / 3) * schaal_factor;
        }

        char_width += schaal_factor; // Spatie tussen karakters

        x += char_width;
        current_char_index++;
    }
    return errors;
}
