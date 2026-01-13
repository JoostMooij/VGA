/**
 * @file APIdraw.c
 * @brief de laag uit de IO Layer voor het tekenen.
 *
 *
 * Functies:
 *  - lijn(), rechthoek(), cirkel(), figuur()
 *  - toren(), tekst(), bitMap()
 *
 * @author Thijs, Joost, Luc
 * @version 1.0
 * @date 2025-11-20
 */

#include <stdlib.h>
#include <string.h>

#include "APIdraw.h"
#include "APIio.h"
#include "APIerror.h"
#include "bitMap.h"
#include "font_pearl_8x8.h"
#include "font_acorn_8x8.h"


/**
 * @brief Tekent een lijn tussen twee punten met een opgegeven kleur en dikte.
 *
 * Deze functie valideert eerst de invoerparameters. Indien er geen fouten
 * optreden, wordt een lijn getekend met behulp van het Bresenham-algoritme.
 * De lijn kan een dikte groter dan 1 pixel hebben.
 *
 * @param x1 Begin x-coördinaat
 * @param y1 Begin y-coördinaat
 * @param x2 Eind x-coördinaat
 * @param y2 Eind y-coördinaat
 * @param kleur Kleur van de lijn als string
 * @param dikte Dikte van de lijn in pixels
 *
 * @return ErrorList Struct met eventuele foutmeldingen
 */
ErrorList lijn(int x1, int y1, int x2, int y2, const char *kleur, int dikte)
{
    uint8_t color;
    ErrorList errors;
    int ik_heb_geactiveerd = 0;

    /* Zet kleur-string om naar interne kleurwaarde */
    color = kleur_omzetter(kleur);

    /* Controleer invoerparameters */
    errors = Error_handling(FUNC_lijn,
                            x1, y1, x2, y2,
                            color, dikte,
                            0, 0, 0, 0, 0);

    if (errors.error_var1 ||
        errors.error_var2 ||
        errors.error_var3 ||
        errors.error_var4 ||
        errors.error_var5 ||
        errors.error_var6)
        return errors;

    /* Commando opslaan voor herhalen (indien nodig) */
    if (herhaal_hoog == 0)
    {
        int params[] = {x1, y1, x2, y2, (int)color, dikte};

        record_command(CMD_LIJN, 6, params);
        herhaal_hoog = 1;
        ik_heb_geactiveerd = 1;
    }

    /* Variabelen voor Bresenham-algoritme */
    int dx = abs(x2 - x1);
    int dy = -abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx + dy;
    int e2;

    /* Teken de lijn pixel voor pixel */
    while (1)
    {
        /* Teken dikte rondom het huidige punt */
        for (int i = -(dikte / 2); i <= dikte / 2; i++)
        {
            for (int j = -(dikte / 2); j <= dikte / 2; j++)
            {
                (void)drawPixel(x1 + i, y1 + j, kleur);
            }
        }

        /* Stop als eindpunt bereikt is */
        if (x1 == x2 && y1 == y2)
            break;

        e2 = 2 * err;

        if (e2 >= dy)
        {
            err += dy;
            x1 += sx;
        }

        if (e2 <= dx)
        {
            err += dx;
            y1 += sy;
        }
    }

    /* Reset herhaal-status indien deze functie dit activeerde */
    if (ik_heb_geactiveerd)
        herhaal_hoog = 0;

    return errors;
}


/**
 * @brief Tekent een rechthoek op het scherm.
 *
 * Deze functie tekent een rechthoek op basis van een linker-bovenhoek,
 * breedte en hoogte. De rechthoek kan gevuld zijn of alleen een rand hebben.
 *
 * @param x X-coördinaat linker bovenhoek
 * @param y Y-coördinaat linker bovenhoek
 * @param w Breedte in pixels
 * @param h Hoogte in pixels
 * @param kleur Kleur als string
 * @param gevuld 1 = gevuld, 0 = alleen rand
 *
 * @return ErrorList Struct met eventuele fouten
 */
ErrorList rechthoek(int x, int y, int w, int h, const char *kleur, int gevuld)
{
    uint8_t color;
    ErrorList errors;
    int ik_heb_geactiveerd = 0;

    /* Zet kleur om naar interne waarde */
    color = kleur_omzetter(kleur);

    /* Controleer invoer */
    errors = Error_handling(FUNC_rechthoek,
                            x, y, w, h,
                            color, gevuld,
                            0, 0, 0, 0, 0);

    if (errors.error_var1 ||
        errors.error_var2 ||
        errors.error_var3 ||
        errors.error_var4 ||
        errors.error_var5 ||
        errors.error_var6)
        return errors;

    /* Commando opslaan indien nodig */
    if (herhaal_hoog == 0)
    {
        int params[] = {x, y, w, h, (int)color, gevuld};

        record_command(CMD_RECHTHOEK, 6, params);
        herhaal_hoog = 1;
        ik_heb_geactiveerd = 1;
    }

    /* Bereken rechter-onderhoek */
    int x2 = x + w - 1;
    int y2 = y + h - 1;

    /* Teken rechthoek */
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
            drawPixel(i, y, kleur);

        for (unsigned int i = x; i <= x2; i++)
            drawPixel(i, y2, kleur);

        for (unsigned int j = y; j <= y2; j++)
            drawPixel(x, j, kleur);

        for (unsigned int j = y; j <= y2; j++)
            drawPixel(x2, j, kleur);
    }

    /* Herhaalstatus resetten */
    if (ik_heb_geactiveerd)
        herhaal_hoog = 0;

    return errors;
}


/**
 * @brief Tekent een gesloten figuur met vijf punten.
 *
 * Deze functie verbindt vijf opgegeven punten met lijnen
 * en sluit de figuur door het laatste punt met het eerste te verbinden.
 *
 * @param x1 X-coördinaat punt 1
 * @param y1 Y-coördinaat punt 1
 * @param x2 X-coördinaat punt 2
 * @param y2 Y-coördinaat punt 2
 * @param x3 X-coördinaat punt 3
 * @param y3 Y-coördinaat punt 3
 * @param x4 X-coördinaat punt 4
 * @param y4 Y-coördinaat punt 4
 * @param x5 X-coördinaat punt 5
 * @param y5 Y-coördinaat punt 5
 * @param kleur Kleur als string
 *
 * @return ErrorList Struct met eventuele fouten
 */
ErrorList figuur(int x1, int y1,
                 int x2, int y2,
                 int x3, int y3,
                 int x4, int y4,
                 int x5, int y5,
                 const char *kleur)
{
    uint8_t color;
    ErrorList errors;
    int ik_heb_geactiveerd = 0;

    /* Zet kleur om */
    color = kleur_omzetter(kleur);

    /* Controleer invoer */
    errors = Error_handling(FUNC_figuur,
                            x1, y1, x2, y2,
                            x3, y3, x4, y4,
                            x5, y5, color);

    if (errors.error_var1 || errors.error_var2)
        return errors;

    /* Commando opslaan indien nodig */
    if (herhaal_hoog == 0)
    {
        int params[] = {x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, (int)color};

        record_command(CMD_FIGUUR, 11, params);
        herhaal_hoog = 1;
        ik_heb_geactiveerd = 1;
    }

    /* Verbind alle punten met lijnen */
    lijn(x1, y1, x2, y2, kleur, 1);
    lijn(x2, y2, x3, y3, kleur, 1);
    lijn(x3, y3, x4, y4, kleur, 1);
    lijn(x4, y4, x5, y5, kleur, 1);
    lijn(x5, y5, x1, y1, kleur, 1);

    /* Herhaalstatus resetten */
    if (ik_heb_geactiveerd)
        herhaal_hoog = 0;

    return errors;
}

/**
 * @brief Tekent een cirkel op het scherm.
 *
 * Deze functie tekent een cirkel met een opgegeven straal
 * vanuit een middelpunt, met behulp van het Bresenham-algoritme.
 *
 * @param x0 X-coördinaat middelpunt
 * @param y0 Y-coördinaat middelpunt
 * @param radius Straal in pixels
 * @param kleur Kleur als string
 *
 * @return ErrorList Struct met eventuele fouten
 */
ErrorList cirkel(int x0, int y0, int radius, const char *kleur)
{
    uint8_t color;
    ErrorList errors;
    int ik_heb_geactiveerd = 0;

    /* Zet kleur om */
    color = kleur_omzetter(kleur);

    /* Controleer invoer */
    errors = Error_handling(FUNC_cirkel,
                            x0, y0, radius,
                            color,
                            0, 0, 0, 0, 0, 0, 0);

    if (errors.error_var1 ||
        errors.error_var2 ||
        errors.error_var3 ||
        errors.error_var4)
        return errors;

    /* Commando opslaan indien nodig */
    if (herhaal_hoog == 0)
    {
        int params[] = {x0, y0, radius, (int)color};

        record_command(CMD_CIRKEL, 4, params);
        herhaal_hoog = 1;
        ik_heb_geactiveerd = 1;
    }

    /* Bresenham-cirkel algoritme */
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

        y++;

        if (err <= 0)
            err += 2 * y + 1;

        if (err > 0)
        {
            x--;
            err -= 2 * x + 1;
        }
    }

    /* Herhaalstatus resetten */
    if (ik_heb_geactiveerd)
        herhaal_hoog = 0;

    return errors;
}


/**
 * @brief Tekent een toren met meerdere lagen, een mast en een vlag.
 *
 * Deze functie bouwt een toren op uit cirkelvormige lagen.
 * Bovenop de toren wordt een mast en een vlag getekend.
 *
 * @param x X-coördinaat van het midden van de torenbasis
 * @param y Y-coördinaat van de torenbasis
 * @param grootte Basisgrootte van de toren
 * @param kleur1 Kleur van de torenlagen
 * @param kleur2 Kleur van mast en vlag
 *
 * @return ErrorList Struct met eventuele fouten
 */
ErrorList toren(int x, int y, int grootte, const char *kleur1, const char *kleur2)
{
    uint8_t color1;
    uint8_t color2;
    ErrorList errors;
    int ik_heb_geactiveerd = 0;

    /* Zet kleuren om */
    color1 = kleur_omzetter(kleur1);
    color2 = kleur_omzetter(kleur2);

    /* Controleer invoer */
    errors = Error_handling(FUNC_toren,
                            x, y, grootte,
                            color1, color2,
                            0, 0, 0, 0, 0, 0);

    if (errors.error_var1 ||
        errors.error_var2 ||
        errors.error_var3 ||
        errors.error_var4 ||
        errors.error_var5)
        return errors;

    /* Commando opslaan indien nodig */
    if (herhaal_hoog == 0)
    {
        int params[] = {x, y, grootte, (int)color1, (int)color2};

        record_command(CMD_TOREN, 5, params);
        herhaal_hoog = 1;
        ik_heb_geactiveerd = 1;
    }

    /* Instellingen voor de toren */
    int lagen = 6;
    int basis_radius = grootte;
    int hoogte_per_laag = grootte / 2;
    int top_radius = basis_radius / 3;

    /* Teken alle lagen */
    for (int i = 0; i < lagen; i++)
    {
        int radius;
        int y_midden;

        if (i < lagen - 1)
            radius = basis_radius - (i * (basis_radius - top_radius) / (lagen - 1));
        else
            radius = top_radius;

        y_midden = y - (i * hoogte_per_laag);

        /* Teken buiten- en binnencirkel */
        (void)cirkel(x, y_midden, radius, kleur1);
        (void)cirkel(x, y_midden, radius - 2, kleur1);

        /* Verbind lagen met verticale lijnen */
        if (i > 0)
        {
            int y_start = y_midden + (hoogte_per_laag / 2);
            int y_eind  = y_midden - (hoogte_per_laag / 2);

            (void)lijn(x - radius + 1, y_start,
                       x - radius + 1, y_eind, kleur1, 1);

            (void)lijn(x + radius - 1, y_start,
                       x + radius - 1, y_eind, kleur1, 1);
        }
    }

    /* Teken mast */
    int y_mast_top = y - ((lagen - 1) * hoogte_per_laag) - top_radius;
    int mast_hoogte = grootte / 4;

    (void)rechthoek(x, y_mast_top - mast_hoogte, 1, mast_hoogte, kleur2, 1);

    /* Teken vlag */
    (void)figuur(x, y_mast_top - mast_hoogte,
                 x + 5, y_mast_top - mast_hoogte - 2,
                 x + 5, y_mast_top - mast_hoogte - 7,
                 x,     y_mast_top - mast_hoogte - 5,
                 x,     y_mast_top - mast_hoogte,
                 kleur2);

    /* Herhaalstatus resetten */
    if (ik_heb_geactiveerd)
        herhaal_hoog = 0;

    return errors;
}


/**
 * @brief Tekent een bitmap symbool op het scherm.
 *
 * De bitmap wordt getekend met (x,y) als linker-bovenhoek.
 * Afhankelijk van het symbool kan transparantie worden gebruikt.
 *
 * @param nr Nummer van het symbool
 * @param x X-coördinaat linker-bovenhoek
 * @param y Y-coördinaat linker-bovenhoek
 *
 * @return ErrorList Struct met eventuele fouten
 */
ErrorList bitMap(int nr, int x, int y)
{
    ErrorList errors;
    int ik_heb_geactiveerd = 0;
    const Bitmap *bmp_ptr = NULL;
    uint8_t use_transparency = 0;

    /* Controleer invoer */
    errors = Error_handling(FUNC_bitmap,
                            nr, x, y,
                            0, 0, 0, 0, 0, 0, 0, 0);

    if (errors.error_var1 ||
        errors.error_var2 ||
        errors.error_var3)
        return errors;

    /* Commando opslaan indien nodig */
    if (herhaal_hoog == 0)
    {
        int params[] = {nr, x, y};

        record_command(CMD_BITMAP, 3, params);
        herhaal_hoog = 1;
        ik_heb_geactiveerd = 1;
    }

    /* Kies bitmap */
    switch (nr)
    {
        case 1:  bmp_ptr = &arrow_up;    use_transparency = 1; break;
        case 2:  bmp_ptr = &arrow_down;  use_transparency = 1; break;
        case 3:  bmp_ptr = &arrow_left;  use_transparency = 1; break;
        case 4:  bmp_ptr = &arrow_right; use_transparency = 1; break;
        case 5:  bmp_ptr = &smily_blij;  use_transparency = 1; break;
        case 6:  bmp_ptr = &smily_boos;  use_transparency = 1; break;
        case 7:  bmp_ptr = &yes_cat_thumbs_up; use_transparency = 0; break;
        case 8:  bmp_ptr = &skeleton_banging_on_shield_meme_frame_00; use_transparency = 0; break;
        case 9:  bmp_ptr = &skeleton_banging_on_shield_meme_frame_06; use_transparency = 0; break;
        case 10: bmp_ptr = &KIP; use_transparency = 0; break;
        default:
            return errors;
    }

    if (bmp_ptr == NULL || bmp_ptr->data == NULL)
        return errors;

    /* Teken bitmap */
    drawBitmap(x, y, bmp_ptr, use_transparency);

    /* Herhaalstatus resetten */
    if (ik_heb_geactiveerd)
        herhaal_hoog = 0;

    return errors;
}


/**
 * @brief Tekent een tekststring op het scherm met opgegeven stijl en schaal.
 *
 * Deze functie gebruikt het Pearl 8x8 of Acorn 8x8 bitmap font.
 * Tekst wordt automatisch naar de volgende regel gewrapt indien nodig.
 *
 * @param x Start X-coördinaat
 * @param y Start Y-coördinaat
 * @param kleur_str Kleur als string ("zwart", "wit", etc.)
 * @param tekst_str Tekst die getekend moet worden
 * @param fontnaam Naam van het font ("pearl" of "acorn")
 * @param schaal_factor Schaalfactor (1 = 8x8, 2 = 16x16, etc.)
 * @param fontstijl Stijl van het font ("normaal", "vet", "cursief")
 *
 * @return ErrorList Struct met eventuele fouten (0 = geen fouten)
 */
ErrorList tekst(int x, int y, const char *kleur_str, const char *tekst_str,
                const char *fontnaam, int schaal_factor, const char *fontstijl)
{
    int color;
    ErrorList errors;
    int ik_heb_geactiveerd = 0;

    /* Zet kleur om */
    color = kleur_omzetter(kleur_str);

    /* Controleer invoer */
    errors = Error_handling(FUNC_tekst,
                            x, y, color,
                            (int)tekst_str, (int)fontnaam, schaal_factor, (int)fontstijl,
                            0, 0, 0, 0);

    if (errors.error_var1 != NO_ERROR ||
        errors.error_var2 != NO_ERROR ||
        errors.error_var3 != NO_ERROR ||
        errors.error_var4 != NO_ERROR ||
        errors.error_var5 != NO_ERROR ||
        errors.error_var6 != NO_ERROR ||
        errors.error_var7 != NO_ERROR)
    {
        return errors;
    }

    /* Commando opslaan indien nodig */
    if (herhaal_hoog == 0)
    {
        int tekst_index = reserveer_tekst_slot(tekst_str);

        /* Zet fontnaam om naar ID: pearl=0, acorn=1 */
        int font_id = (strcmp(fontnaam, "acorn") == 0) ? 1 : 0;

        /* Zet stijl om naar ID: normaal=0, vet=1, cursief=2 */
        int stijl_id = 0;
        if (strcmp(fontstijl, "vet") == 0)
            stijl_id = 1;
        else if (strcmp(fontstijl, "cursief") == 0)
            stijl_id = 2;

        /* Sla parameters op voor herhalen */
        int params[] = {x, y, color, tekst_index, font_id, schaal_factor, stijl_id};
        record_command(CMD_TEKST, 7, params);

        herhaal_hoog = 1;
        ik_heb_geactiveerd = 1;
    }

    /* Selecteer fontdata */
    const unsigned char *active_font_data = fontdata_pearl8x8;
    if (strcmp(fontnaam, "acorn") == 0)
        active_font_data = fontdata_acorn8x8;

    int is_vet    = (strcmp(fontstijl, "vet") == 0);
    int is_cursief = (strcmp(fontstijl, "cursief") == 0);
    int current_char_index = 0;

    /* Loop door alle karakters */
    while (tekst_str[current_char_index] != '\0')
    {
        unsigned char karakter = (unsigned char)tekst_str[current_char_index];

        /* Zorg dat karakter binnen ASCII printable range is */
        if (karakter < 0 || karakter > 126)
            karakter = 0;

        int bitmap_index = karakter * BASE_FONT_SIZE;

        /* Bereken breedte van het karakter */
        int char_width = BASE_FONT_SIZE * schaal_factor;
        if (is_vet)
            char_width += schaal_factor;
        if (is_cursief)
            char_width += (int)((7.0 / 3.0) * schaal_factor + 0.5);
        char_width += schaal_factor;

        /* Wrap naar volgende regel indien nodig */
        if (x + char_width > VGA_DISPLAY_X)
        {
            x = 0;
            y += BASE_FONT_SIZE * schaal_factor + schaal_factor;
        }

        /* Teken bitmap van karakter */
        for (int rij = 0; rij < BASE_FONT_SIZE; rij++)
        {
            unsigned char rij_data = active_font_data[bitmap_index + rij];
            int offset_schuin = 0;

            if (is_cursief)
                offset_schuin = (int)((rij / 3.0) * schaal_factor + 0.5);

            for (int bit = 0; bit < BASE_FONT_SIZE; bit++)
            {
                if ((rij_data >> (7 - bit)) & 1)
                {
                    for (int s_y = 0; s_y < schaal_factor; s_y++)
                    {
                        for (int s_x = 0; s_x < schaal_factor; s_x++)
                        {
                            int pixel_x = x + (bit * schaal_factor) + s_x + offset_schuin;
                            int pixel_y = y + (rij * schaal_factor) + s_y;

                            drawPixel(pixel_x, pixel_y, kleur_str);

                            if (is_vet)
                                drawPixel(pixel_x + schaal_factor, pixel_y, kleur_str);
                        }
                    }
                }
            }
        }

        x += char_width;
        current_char_index++;
    }

    /* Herhaalstatus resetten */
    if (ik_heb_geactiveerd)
        herhaal_hoog = 0;

    return errors;
}
