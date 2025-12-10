/**
 * @file APIerror.c
 * @brief Implementatie van foutafhandeling voor de EE‑API‑LIB.
 *
 * Functies om errorcodes om te zetten naar strings of
 * te loggen via UART. Wordt gebruikt door alle API‑functies
 * om invoerwaarden te valideren.
 *
 * @author Thijs
 * @version 1.0
 * @date 2025-11-20
 */

#include "APIerror.h"
#include "stm32_ub_vga_screen.h"   // nodig voor UB_VGA_SetPixel en defines
#include "bitMap.h"

/**
 * @brief Controleer alle mogelijke fouten van een API-functie.
 * @param func Functie-ID om te controleren
 * @param waarde1-waarde11 Waarden voor validatie (verschillend per functie)
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

        case FUNC_bitmap:
        {
        	ErrorCode nr_error		= check_nr(waarde1, waarde2, waarde3);
        	ErrorCode x_error      = check_x(waarde2);
			ErrorCode y_error      = check_y(waarde3);

			if(nr_error		!= NO_ERROR) errors.error_var1 = nr_error;
			if(x_error      != NO_ERROR) errors.error_var2 = x_error;
			if(y_error      != NO_ERROR) errors.error_var3 = y_error;
        }

        default:
            /**< Andere functies later */
            break;
    }

    return errors;
}

/** @brief Controleer X-coördinaat */
ErrorCode check_x(int x)
{
    return (x < 0 || x >= VGA_DISPLAY_X) ? ERROR_X1 : NO_ERROR;
}

/** @brief Controleer Y-coördinaat */
ErrorCode check_y(int y)
{
    return (y < 0 || y >= VGA_DISPLAY_Y) ? ERROR_Y1 : NO_ERROR;
}

/** @brief Controleer kleurwaarde */
ErrorCode check_color(int color)
{
    return (color < 0 || color > 255) ? ERROR_COLOR : NO_ERROR;
}

/** @brief Controleer lijn inclusief dikte binnen scherm */
ErrorCode check_lijn_op_scherm(int x1, int y1, int x2, int y2, int dikte)
{
    if(dikte < 1) return ERROR_DIJKTE_TOO_SMALL;

    int half = dikte / 2;
    int minX = x1 < x2 ? x1 : x2;
    int maxX = x1 > x2 ? x1 : x2;
    int minY = y1 < y2 ? y1 : y2;
    int maxY = y1 > y2 ? y1 : y2;
    minX -= half; maxX += half;
    minY -= half; maxY += half;

    if(minX < 0 || maxX >= VGA_DISPLAY_X) return ERROR_X1;
    if(minY < 0 || maxY >= VGA_DISPLAY_Y) return ERROR_Y1;

    return NO_ERROR;
}

/** @brief Controleer breedte van rechthoek */
ErrorCode check_breedte(int x, int breedte)
{
    return (x + breedte - 1 >= VGA_DISPLAY_X) ? ERROR_BREEDTE : NO_ERROR;
}

/** @brief Controleer hoogte van rechthoek */
ErrorCode check_hoogte(int y, int hoogte)
{
    return (y + hoogte - 1 >= VGA_DISPLAY_Y) ? ERROR_HOOGTE : NO_ERROR;
}

/** @brief Controleer gevuld-waarde (0 of 1) */
ErrorCode check_gevuld(int gevuld)
{
    return (gevuld != 0 && gevuld != 1) ? ERROR_GEVULD : NO_ERROR;
}

/** @brief Controleer radius binnen scherm */
ErrorCode check_radius_op_scherm(int x, int y, int radius)
{
    if(radius < 1) return ERROR_RADIUS_TOO_SMALL;
    if(x - radius < 0 || x + radius >= VGA_DISPLAY_X) return ERROR_X1;
    if(y - radius < 0 || y + radius >= VGA_DISPLAY_Y) return ERROR_Y1;
    return NO_ERROR;
}

/**
 * @brief Controleer de bitmap waardes
 */
ErrorCode check_nr(int nr, int x, int y)
{
    int grootte;

    if (nr < 0 || nr > 9) return ERROR_bitmap_nr;

    switch (nr)
    {
        case 1:
        case 2:
        case 3:
        case 4:
            grootte = grote_pijlen;
            break;

        case 5:
        case 6:
            grootte = grote_smiley;
            break;

        case 7:
            grootte = kat_afbeelding;
            break;

        case 8:
        case 9:
            grootte = skalet_afbeelding;
            break;

        default:
            return ERROR_bitmap_nr;
    }

    int r = grootte / 2;

    if (x + r - 1 >= VGA_DISPLAY_X || x - r < 0) return ERROR_bitmap_buiten_scherm;
    if (y + r - 1 >= VGA_DISPLAY_Y || y - r < 0) return ERROR_bitmap_buiten_scherm;

    return NO_ERROR;
}
  /**
 * @brief Controleer sec-waarde (0 of 1)
 */
ErrorCode wacht_error(int ms)
{
	if(ms < 1) return ERROR_TIME_TOO_SMALL;
	return NO_ERROR;
}
  
/** @brief Controleer grootte binnen scherm (algemeen) */
ErrorCode check_grootte_op_scherm(int x, int y, int grootte)
{
    if(grootte < 1) return ERROR_GROOTTE_TOO_SMALL;
    if(x + grootte - 1 >= VGA_DISPLAY_X) return ERROR_X1;
    if(y + grootte - 1 >= VGA_DISPLAY_Y) return ERROR_Y1;
    return NO_ERROR;
}

/** @brief Controleer toren past op scherm */
ErrorCode check_toren_op_scherm(int x, int y, int grootte)
{
    if(grootte < 1) return ERROR_GROOTTE_TOO_SMALL;

    int lagen = 6;
    int basis_radius = grootte;
    int hoogte_per_laag = grootte / 2;
    int top_radius = basis_radius / 3;
    int mast_hoogte = grootte / 4;

    int y_top_center = y - ((lagen - 1) * hoogte_per_laag);
    int y_toren_top_pixel = y_top_center - top_radius;
    int y_mast_top_pixel = y_toren_top_pixel - mast_hoogte;
    int y_bodem_pixel = y + basis_radius;

    if(x - basis_radius < 0 || x + basis_radius >= VGA_DISPLAY_X) return ERROR_TOREN_BUITEN_SCHERM;
    if(y_mast_top_pixel < 0 || y_bodem_pixel >= VGA_DISPLAY_Y) return ERROR_TOREN_BUITEN_SCHERM;

    return NO_ERROR;
}
