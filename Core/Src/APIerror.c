/**
 * @file APIerror.c
 * @brief errorhandling
 *
 * Bevat functies om errorcodes van API-functies te controleren
 * en om te zetten naar een gestructureerde ErrorList.
 *
 * @author Thijs, Joost, Luc
 * @version 1.0
 * @date 2025-11-20
 */

#include "APIerror.h"
#include "stm32_ub_vga_screen.h"   // Voor UB_VGA_SetPixel en defines
#include "bitMap.h"
#include <stddef.h>
#include "APIio.h"
#include "string.h"

/**
 * @brief Controleer alle mogelijke fouten van een API-functie.
 * @param func Functie-ID om te controleren
 * @param waarde1 t/m waarde11 Waarden voor validatie (verschillend per functie)
 * @return ErrorList Struct met foutcodes per parameter
 */
ErrorList Error_handling(FunctionID func,
                         int waarde1, int waarde2, int waarde3,
                         int waarde4, int waarde5, int waarde6,
                         int waarde7, int waarde8, int waarde9,
                         int waarde10, int waarde11)
{
    ErrorList errors = {NO_ERROR, NO_ERROR, NO_ERROR, NO_ERROR, NO_ERROR, NO_ERROR,
                        NO_ERROR, NO_ERROR, NO_ERROR, NO_ERROR, NO_ERROR};

    switch(func)
    {
        case FUNC_clearscherm:
        {
            ErrorCode kleur_error = check_color(waarde1);
            if(kleur_error != NO_ERROR) errors.error_var1 = kleur_error;
            break;
        }

        case FUNC_drawPixel:
        {
            ErrorCode x_error     = check_x(waarde1);
            ErrorCode y_error     = check_y(waarde2);
            ErrorCode kleur_error = check_color(waarde3);
            if(x_error != NO_ERROR)     errors.error_var1 = x_error;
            if(y_error != NO_ERROR)     errors.error_var2 = y_error;
            if(kleur_error != NO_ERROR) errors.error_var3 = kleur_error;
            break;
        }

        case FUNC_lijn:
        {
            ErrorCode x1_error    = check_x(waarde1);
            ErrorCode y1_error    = check_y(waarde2);
            ErrorCode x2_error    = check_x(waarde3);
            ErrorCode y2_error    = check_y(waarde4);
            ErrorCode kleur_error = check_color(waarde5);
            ErrorCode lijn_error  = check_lijn_op_scherm(waarde1, waarde2, waarde3, waarde4, waarde6);
            if(x1_error != NO_ERROR)    errors.error_var1 = x1_error;
            if(y1_error != NO_ERROR)    errors.error_var2 = y1_error;
            if(x2_error != NO_ERROR)    errors.error_var3 = x2_error;
            if(y2_error != NO_ERROR)    errors.error_var4 = y2_error;
            if(kleur_error != NO_ERROR) errors.error_var5 = kleur_error;
            if(lijn_error != NO_ERROR)  errors.error_var6 = lijn_error;
            break;
        }

        case FUNC_rechthoek:
        {
            ErrorCode x_error       = check_x(waarde1);
            ErrorCode y_error       = check_y(waarde2);
            ErrorCode breedte_error = check_breedte(waarde1, waarde3);
            ErrorCode hoogte_error  = check_hoogte(waarde2, waarde4);
            ErrorCode kleur_error   = check_color(waarde5);
            ErrorCode gevuld_error  = check_gevuld(waarde6);
            if(x_error != NO_ERROR)       errors.error_var1 = x_error;
            if(y_error != NO_ERROR)       errors.error_var2 = y_error;
            if(breedte_error != NO_ERROR) errors.error_var3 = breedte_error;
            if(hoogte_error != NO_ERROR)  errors.error_var4 = hoogte_error;
            if(kleur_error != NO_ERROR)   errors.error_var5 = kleur_error;
            if(gevuld_error != NO_ERROR)  errors.error_var6 = gevuld_error;
            break;
        }

        case FUNC_figuur:
        {
            ErrorCode x1_error    = check_x(waarde1);
            ErrorCode y1_error    = check_y(waarde2);
            ErrorCode x2_error    = check_x(waarde3);
            ErrorCode y2_error    = check_y(waarde4);
            ErrorCode x3_error    = check_x(waarde5);
            ErrorCode y3_error    = check_y(waarde6);
            ErrorCode x4_error    = check_x(waarde7);
            ErrorCode y4_error    = check_y(waarde8);
            ErrorCode x5_error    = check_x(waarde9);
            ErrorCode y5_error    = check_y(waarde10);
            ErrorCode kleur_error = check_color(waarde11);
            if(x1_error != NO_ERROR)    errors.error_var1  = x1_error;
            if(y1_error != NO_ERROR)    errors.error_var2  = y1_error;
            if(x2_error != NO_ERROR)    errors.error_var3  = x2_error;
            if(y2_error != NO_ERROR)    errors.error_var4  = y2_error;
            if(x3_error != NO_ERROR)    errors.error_var5  = x3_error;
            if(y3_error != NO_ERROR)    errors.error_var6  = y3_error;
            if(x4_error != NO_ERROR)    errors.error_var7  = x4_error;
            if(y4_error != NO_ERROR)    errors.error_var8  = y4_error;
            if(x5_error != NO_ERROR)    errors.error_var9  = x5_error;
            if(y5_error != NO_ERROR)    errors.error_var10 = y5_error;
            if(kleur_error != NO_ERROR) errors.error_var11 = kleur_error;
            break;
        }

        case FUNC_cirkel:
        {
            ErrorCode x_error      = check_x(waarde1);
            ErrorCode y_error      = check_y(waarde2);
            ErrorCode radius_error = check_radius_op_scherm(waarde1, waarde2, waarde3);
            ErrorCode kleur_error  = check_color(waarde4);
            if(x_error != NO_ERROR)      errors.error_var1 = x_error;
            if(y_error != NO_ERROR)      errors.error_var2 = y_error;
            if(radius_error != NO_ERROR) errors.error_var3 = radius_error;
            if(kleur_error != NO_ERROR)  errors.error_var4 = kleur_error;
            break;
        }

        case FUNC_toren:
        {
            ErrorCode x_error       = check_x(waarde1);
            ErrorCode y_error       = check_y(waarde2);
            ErrorCode grootte_error = check_toren_op_scherm(waarde1, waarde2, waarde3);
            ErrorCode kleur1_error  = check_color(waarde4);
            ErrorCode kleur2_error  = check_color(waarde5);
            if(x_error != NO_ERROR)       errors.error_var1 = x_error;
            if(y_error != NO_ERROR)       errors.error_var2 = y_error;
            if(grootte_error != NO_ERROR) errors.error_var3 = grootte_error;
            if(kleur1_error != NO_ERROR)  errors.error_var4 = kleur1_error;
            if(kleur2_error != NO_ERROR)  errors.error_var5 = kleur2_error;
            break;
        }

        case FUNC_wacht:
        {
            ErrorCode ms_error = wacht_error(waarde1);

            if(ms_error != NO_ERROR) errors.error_var1 = ms_error;
            break;
        }


        case FUNC_herhaal:
        {
        	ErrorCode aantal_error = herhaal_aantal_error(waarde1);
        	ErrorCode hoevaak_error = herhaal_hoevaak_error(waarde2);
        	if(aantal_error != NO_ERROR) errors.error_var1 = aantal_error;
        	if(hoevaak_error != NO_ERROR) errors.error_var2 = hoevaak_error;
        	break;
        }

        case FUNC_bitmap:
        {
        	ErrorCode nr_error		= check_nr(waarde1, waarde2, waarde3);
        	ErrorCode x_error      = check_x(waarde2);
		    ErrorCode y_error      = check_y(waarde3);

		    if(nr_error		!= NO_ERROR) errors.error_var1 = nr_error;
		    if(x_error      != NO_ERROR) errors.error_var2 = x_error;
		    if(y_error      != NO_ERROR) errors.error_var3 = y_error;
		    break;
        }

        case FUNC_tekst:
        {
            ErrorCode x_error       = check_x(waarde1);
            ErrorCode y_error       = check_y(waarde2);
        	ErrorCode kleur_error = check_color(waarde3);
        	ErrorCode tekst_error =  check_tekst_op_scherm(waarde1, waarde2, (char*)waarde4, waarde6, (char*)waarde7);
        	ErrorCode empty_error = check_text_empty((char*)waarde4);
        	ErrorCode schaal_error =  check_schaal(waarde6);
        	ErrorCode fonstijl_error =  check_fontstijl((char*)waarde7);

            if(x_error != NO_ERROR) errors.error_var1 = x_error;
            if(y_error != NO_ERROR) errors.error_var2 = y_error;
            if(kleur_error != NO_ERROR) errors.error_var3 = kleur_error;
            if(kleur_error != NO_ERROR) errors.error_var3 = tekst_error;
            if(empty_error != NO_ERROR) errors.error_var5 = empty_error;
            if(schaal_error != NO_ERROR) errors.error_var5 = schaal_error;
            if(fonstijl_error != NO_ERROR) errors.error_var5 = fonstijl_error;
            break;
        }

        default:
            /**< Andere functies later */
            break;
    }

    return errors;
}

/**
 * @brief Controleer X-coördinaat binnen scherm
 * @param x X-coördinaat
 * @return ERROR_X1 als fout, anders NO_ERROR
 */
ErrorCode check_x(int x)
{
    if(x < 0 || x >= VGA_DISPLAY_X)
        return ERROR_X1;
    return NO_ERROR;
}

/**
 * @brief Controleer Y-coördinaat binnen scherm
 * @param y Y-coördinaat
 * @return ERROR_Y1 als fout, anders NO_ERROR
 */
ErrorCode check_y(int y)
{
    if(y < 0 || y >= VGA_DISPLAY_Y)
        return ERROR_Y1;
    return NO_ERROR;
}

/**
 * @brief Controleer geldigheid van kleurwaarde
 * @param color Kleurwaarde
 * @return ERROR_COLOR als fout, anders NO_ERROR
 */
ErrorCode check_color(int color)
{
    if(color == Ongeldige_kleur)
        return ERROR_COLOR;
    if(color < 0 || color > 255)
        return ERROR_COLOR;
    return NO_ERROR;
}

/**
 * @brief Controleer lijn inclusief dikte binnen scherm
 * @param x1 X-start
 * @param y1 Y-start
 * @param x2 X-eind
 * @param y2 Y-eind
 * @param dikte Dikte van lijn
 * @return Foutcode bij ongeldig, anders NO_ERROR
 */
ErrorCode check_lijn_op_scherm(int x1, int y1, int x2, int y2, int dikte)
{
    if(dikte < 1)
        return ERROR_DIJKTE_TOO_SMALL;

    int half = dikte / 2;
    int minX = x1 < x2 ? x1 : x2;
    int maxX = x1 > x2 ? x1 : x2;
    int minY = y1 < y2 ? y1 : y2;
    int maxY = y1 > y2 ? y1 : y2;

    minX -= half;
    maxX += half;
    minY -= half;
    maxY += half;

    if(minX < 0 || maxX >= VGA_DISPLAY_X)
        return ERROR_X1;
    if(minY < 0 || maxY >= VGA_DISPLAY_Y)
        return ERROR_Y1;

    return NO_ERROR;
}

/**
 * @brief Controleer breedte van rechthoek binnen scherm
 * @param x Linker X-coördinaat
 * @param breedte Breedte van rechthoek
 * @return ERROR_BREEDTE bij fout, anders NO_ERROR
 */
ErrorCode check_breedte(int x, int breedte)
{
    if(breedte < 1)
        return ERROR_BREEDTE;
    if(x < 0 || x + breedte - 1 >= VGA_DISPLAY_X)
        return ERROR_BREEDTE;
    return NO_ERROR;
}

/**
 * @brief Controleer hoogte van rechthoek binnen scherm
 * @param y Boven Y-coördinaat
 * @param hoogte Hoogte van rechthoek
 * @return ERROR_HOOGTE bij fout, anders NO_ERROR
 */
ErrorCode check_hoogte(int y, int hoogte)
{
    if(hoogte < 1)
        return ERROR_HOOGTE;
    if(y < 0 || y + hoogte - 1 >= VGA_DISPLAY_Y)
        return ERROR_HOOGTE;
    return NO_ERROR;
}

/**
 * @brief Controleer of gevuld waarde geldig is (0 of 1)
 * @param gevuld 0 = niet gevuld, 1 = gevuld
 * @return ERROR_GEVULD bij fout, anders NO_ERROR
 */
ErrorCode check_gevuld(int gevuld)
{
    if(gevuld != 0 && gevuld != 1)
        return ERROR_GEVULD;
    return NO_ERROR;
}

/**
 * @brief Controleer radius van cirkel binnen scherm
 * @param x X-coördinaat middelpunt
 * @param y Y-coördinaat middelpunt
 * @param radius Radius van cirkel
 * @return Foutcode bij ongeldig, anders NO_ERROR
 */
ErrorCode check_radius_op_scherm(int x, int y, int radius)
{
    if(radius < 1)
        return ERROR_RADIUS_TOO_SMALL;
    if(x - radius < 0 || x + radius >= VGA_DISPLAY_X)
        return ERROR_X1;
    if(y - radius < 0 || y + radius >= VGA_DISPLAY_Y)
        return ERROR_Y1;
    return NO_ERROR;
}

/**
 * @brief Controleer bitmap nummer en positie binnen scherm
 * @param nr Bitmap nummer
 * @param x Linker X-coördinaat
 * @param y Boven Y-coördinaat
 * @return Foutcode bij ongeldig, anders NO_ERROR
 */
ErrorCode check_nr(int nr, int x, int y)
{
    int grootte;

    if(nr < 0 || nr > 10)
        return ERROR_bitmap_nr;

    switch(nr)
    {
        case 1: case 2: case 3: case 4:
            grootte = grote_pijlen;
            break;
        case 5: case 6:
            grootte = grote_smiley;
            break;
        case 7:
            grootte = kat_afbeelding;
            break;
        case 8: case 9:
            grootte = skalet_afbeelding;
            break;
        case 10:
            grootte = KIP_afbeelding;
            break;
        default:
            return ERROR_bitmap_nr;
    }

    if(x < 0 || x + grootte - 1 >= VGA_DISPLAY_X)
        return ERROR_bitmap_buiten_scherm;
    if(y < 0 || y + grootte - 1 >= VGA_DISPLAY_Y)
        return ERROR_bitmap_buiten_scherm;

    return NO_ERROR;
}

/**
 * @brief Controleer waarde van wacht-functie (minimaal 1 ms)
 * @param ms Wachttijd in milliseconden
 * @return ERROR_TIME_TOO_SMALL bij fout, anders NO_ERROR
 */
ErrorCode wacht_error(int ms)
{
    if(ms < 1)
        return ERROR_TIME_TOO_SMALL;
    return NO_ERROR;
}

/**
 * @brief Controleer of herhaal aantal geldig is (1-20)
 * @param aantal Aantal herhalingen
 * @return AANTAL_NOT_IN_RANGE bij fout, anders NO_ERROR
 */
ErrorCode herhaal_aantal_error(int aantal)
{
    if(aantal < 1 || aantal > 20)
        return AANTAL_NOT_IN_RANGE;
    return NO_ERROR;
}

/**
 * @brief Controleer hoe vaak herhalen geldig is (1-40)
 * @param hoevaak Hoe vaak herhalen
 * @return HOEVAAK_NOT_IN_RANGE bij fout, anders NO_ERROR
 */
ErrorCode herhaal_hoevaak_error(int hoevaak)
{
    if(hoevaak < 1 || hoevaak > 40)
        return HOEVAAK_NOT_IN_RANGE;
    return NO_ERROR;
}

/**
 * @brief Controleer algemene grootte binnen scherm
 * @param x Linker X-coördinaat
 * @param y Boven Y-coördinaat
 * @param grootte Grootte van object
 * @return Foutcode bij buiten scherm, anders NO_ERROR
 */
ErrorCode check_grootte_op_scherm(int x, int y, int grootte)
{
    if(grootte < 1)
        return ERROR_GROOTTE_TOO_SMALL;
    if(x + grootte - 1 >= VGA_DISPLAY_X)
        return ERROR_X1;
    if(y + grootte - 1 >= VGA_DISPLAY_Y)
        return ERROR_Y1;
    return NO_ERROR;
}

/**
 * @brief Controleer of toren past op scherm
 * @param x X-coördinaat middelpunt basis
 * @param y Y-coördinaat basis
 * @param grootte Basisgrootte toren
 * @return ERROR_TOREN_BUITEN_SCHERM bij fout, anders NO_ERROR
 */
ErrorCode check_toren_op_scherm(int x, int y, int grootte)
{
    if(grootte < 1)
        return ERROR_GROOTTE_TOO_SMALL;

    int lagen = 6;
    int basis_radius = grootte;
    int hoogte_per_laag = grootte / 2;
    int top_radius = basis_radius / 3;
    int mast_hoogte = grootte / 4;

    int y_top_center = y - ((lagen - 1) * hoogte_per_laag);
    int y_toren_top_pixel = y_top_center - top_radius;
    int y_mast_top_pixel = y_toren_top_pixel - mast_hoogte;
    int y_bodem_pixel = y + basis_radius;

    if(x - basis_radius < 0 || x + basis_radius >= VGA_DISPLAY_X)
        return ERROR_TOREN_BUITEN_SCHERM;
    if(y_mast_top_pixel < 0 || y_bodem_pixel >= VGA_DISPLAY_Y)
        return ERROR_TOREN_BUITEN_SCHERM;

    return NO_ERROR;
}

/**
 * @brief Controleer of tekst niet leeg is
 * @param tekst_str Pointer naar tekst
 * @return ERROR_TEXT_EMPTY bij fout, anders NO_ERROR
 */
ErrorCode check_text_empty(const char* tekst_str)
{
    if(tekst_str == NULL || tekst_str[0] == '\0')
        return ERROR_TEXT_EMPTY;
    return NO_ERROR;
}

/**
 * @brief Controleer of schaalfactor geldig is
 * @param schaal_factor Schaalfactor van tekst/object
 * @return ERROR_GROOTTE_TOO_SMALL bij fout, anders NO_ERROR
 */
ErrorCode check_schaal(int schaal_factor)
{
    if(schaal_factor < 1)
        return ERROR_GROOTTE_TOO_SMALL;
    return NO_ERROR;
}

/**
 * @brief Controleer of fontstijl geldig is
 * @param fontstijl String met fontstijl ("vet", "cursief", "normaal")
 * @return ERROR_INVALID_STYL bij fout, anders NO_ERROR
 */
ErrorCode check_fontstijl(const char* fontstijl)
{
    int is_vet = (strcmp(fontstijl, "vet") == 0);
    int is_cursief = (strcmp(fontstijl, "cursief") == 0);
    int is_normaal = (strcmp(fontstijl, "normaal") == 0);

    if(!is_vet && !is_cursief && !is_normaal)
        return ERROR_INVALID_STYL;
    return NO_ERROR;
}

/**
 * @brief Controleer of tekst binnen scherm past
 * @param x Linker X-coördinaat
 * @param y Boven Y-coördinaat
 * @param tekst_str Pointer naar tekst
 * @param schaal_factor Schaalfactor van tekst
 * @param fontstijl Fontstijl ("vet", "cursief", "normaal")
 * @return Foutcode bij overschrijding scherm of invalid font, anders NO_ERROR
 */
ErrorCode check_tekst_op_scherm(int x, int y, const char* tekst_str, int schaal_factor, const char* fontstijl)
{
    if(tekst_str == NULL)
        return ERROR_TEXT_EMPTY;
    if(schaal_factor < 1)
        return ERROR_GROOTTE_TOO_SMALL;

    int is_vet = (strcmp(fontstijl, "vet") == 0);
    int is_cursief = (strcmp(fontstijl, "cursief") == 0);
    int is_normaal = (strcmp(fontstijl, "normaal") == 0);
    if(!is_vet && !is_cursief && !is_normaal)
        return ERROR_INVALID_STYL;

    int current_x = x;
    int current_y = y;
    int lengte = strlen(tekst_str);

    for(int i = 0; i < lengte; i++)
    {
        unsigned char karakter = (unsigned char)tekst_str[i];
        if(karakter < 32 || karakter > 126)
            karakter = 32; // spatie voor niet-printables

        // Bereken breedte van karakter
        int char_width = BASE_FONT_SIZE * schaal_factor;
        if(is_vet) char_width += schaal_factor;
        if(is_cursief) char_width += (int)((7.0 / 3.0) * schaal_factor + 0.5);
        char_width += schaal_factor;

        // Wrap naar nieuwe regel als we rechtsrand bereiken
        if(current_x + char_width > VGA_DISPLAY_X)
        {
            current_x = 0;
            current_y += BASE_FONT_SIZE * schaal_factor + schaal_factor;
        }

        // Check of we niet onder de Y-rand komen
        if(current_y + BASE_FONT_SIZE * schaal_factor > VGA_DISPLAY_Y)
            return ERROR_HOOGTE; // Tekst past niet op scherm

        current_x += char_width;
    }

    return NO_ERROR;
}
