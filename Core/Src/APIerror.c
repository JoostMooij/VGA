/**
 * @file APIerror.c
 * @brief Implementatie van foutafhandeling voor de EE‑API‑LIB.
 *
 * Functies om errorcodes om te zetten naar strings of
 * te loggen via UART.
 *
 * @author Thijs
 * @version 1.0
 * @date 2025-11-20
 */

#include "APIerror.h"
#include "stm32_ub_vga_screen.h"   // nodig voor UB_VGA_SetPixel en defines

/**
 * @brief Controleer alle mogelijke fouten van een functie via switch-case.
 */
ErrorList Error_handling(FunctionID func, int waarde1, int waarde2, int waarde3, int waarde4, int waarde5, int waarde6, int waarde7, int waarde8, int waarde9, int waarde10, int waarde11)
{
    ErrorList errors = {NO_ERROR, NO_ERROR, NO_ERROR, NO_ERROR, NO_ERROR, NO_ERROR, NO_ERROR, NO_ERROR, NO_ERROR, NO_ERROR, NO_ERROR};
    switch(func)
    {
        case FUNC_clearscherm:
        {
            ErrorCode color_error = check_color(waarde1);
            if(color_error != NO_ERROR) errors.error_var1 = color_error;
            break;
        }

        case FUNC_drawPixel:
        {
            ErrorCode x_error     = check_x(waarde1);
            ErrorCode y_error     = check_y(waarde2);
            ErrorCode color_error = check_color(waarde3);
            if(x_error     != NO_ERROR) errors.error_var1 = x_error;
            if(y_error     != NO_ERROR) errors.error_var2 = y_error;
            if(color_error != NO_ERROR) errors.error_var3 = color_error;
            break;
        }

        case FUNC_lijn:
        {
            ErrorCode x1_error     = check_x(waarde1);
            ErrorCode y1_error     = check_y(waarde2);
            ErrorCode x2_error     = check_x(waarde3);
            ErrorCode y2_error     = check_y(waarde4);
            ErrorCode color_error  = check_color(waarde5);
            ErrorCode lijn_error   = check_lijn_op_scherm(waarde1, waarde2, waarde3, waarde4, waarde6);
            if(x1_error    != NO_ERROR) errors.error_var1 = x1_error;
            if(y1_error    != NO_ERROR) errors.error_var2 = y1_error;
            if(x2_error    != NO_ERROR) errors.error_var3 = x2_error;
            if(y2_error    != NO_ERROR) errors.error_var4 = y2_error;
            if(color_error != NO_ERROR) errors.error_var5 = color_error;
            if(lijn_error  != NO_ERROR) errors.error_var6 = lijn_error;
            break;
        }

        case FUNC_rechthoek:
        {
            ErrorCode x_error       = check_x(waarde1);
            ErrorCode y_error       = check_y(waarde2);
            ErrorCode breedte_error = check_breedte(waarde1, waarde3);
            ErrorCode hoogte_error  = check_hoogte(waarde2, waarde4);
            ErrorCode color_error   = check_color(waarde5);
            ErrorCode gevuld_error  = check_gevuld(waarde6);
            if(x_error       != NO_ERROR) errors.error_var1 = x_error;
            if(y_error       != NO_ERROR) errors.error_var2 = y_error;
            if(breedte_error != NO_ERROR) errors.error_var3 = breedte_error;
            if(hoogte_error  != NO_ERROR) errors.error_var4 = hoogte_error;
            if(color_error   != NO_ERROR) errors.error_var5 = color_error;
            if(gevuld_error  != NO_ERROR) errors.error_var6 = gevuld_error;
            break;
        }
        default:
            // andere functies later
            break;
    }
    return errors;
}

/**
 * @brief Controleer X-coordinaat
 */
ErrorCode check_x(int x)
{
    if(x < 0 || x >= VGA_DISPLAY_X)
    {
    	return ERROR_X1;
    }
    return NO_ERROR;
}

/**
 * @brief Controleer Y-coordinaat
 */
ErrorCode check_y(int y)
{
    if(y < 0 || y >= VGA_DISPLAY_Y)
    {
    	return ERROR_Y1;
    }
    return NO_ERROR;
}

/**
 * @brief Controleer kleurwaarde
 */
ErrorCode check_color(int color)
{
    if(color < 0 || color > 255)
    {
    	return ERROR_COLOR;
    }
    return NO_ERROR;
}

/**
 * @brief Controleer lijn op scherm inclusief dikte
 */
ErrorCode check_lijn_op_scherm(int x1, int y1, int x2, int y2, int dikte)
{
    if(dikte < 1)
	{
    	return ERROR_DIJKTE_TOO_SMALL;
	}
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
    {
    	 return ERROR_X1;
    }
    if(minY < 0 || maxY >= VGA_DISPLAY_Y)
    {
    	 return ERROR_Y1;
    }
    return NO_ERROR;
}

/**
 * @brief Controleer breedte van rechthoek
 */
ErrorCode check_breedte(int x, int breedte)
{
    if(x + breedte - 1 >= VGA_DISPLAY_X)
	{
		return ERROR_BREEDTE;
	}
    return NO_ERROR;
}

/**
 * @brief Controleer hoogte van rechthoek
 */
ErrorCode check_hoogte(int y, int hoogte)
{
    if(y + hoogte - 1 >= VGA_DISPLAY_Y)
    {
    	return ERROR_HOOGTE;
    }
    return NO_ERROR;
}

/**
 * @brief Controleer gevuld-waarde (0 of 1)
 */
ErrorCode check_gevuld(int gevuld)
{
    if(gevuld != 0 && gevuld != 1)
	{
    	return ERROR_GEVULD;
	}
    return NO_ERROR;
}
