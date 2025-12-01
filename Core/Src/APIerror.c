/**
 * @file APIerror.c
 * @brief Implementatie van foutafhandeling voor de EE‑API‑LIB.
 *
 * Functies om errorcodes om te zetten naar strings of
 * te loggen via UART.
 */

#include "APIerror.h"
#include "stm32_ub_vga_screen.h"   // nodig voor UB_VGA_SetPixel en defines

// Controleer alle mogelijke fouten van een functie via switch-case
ErrorList Error_handling(FunctionID func, int waarde1, int waarde2, int waarde3, int waarde4, int waarde5, int waarde6, int waarde7, int waarde8, int waarde9, int waarde10, int waarde11)
{
    ErrorList errors = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // begin met geen fouten

    switch(func) {
		case FUNC_clearscherm:
			if(check_color(waarde1) != NO_ERROR) errors.error_var1 = 1;
			break;

        case FUNC_drawPixel:
            if(check_x(waarde1) != NO_ERROR) errors.error_var1 = 1;
            if(check_y(waarde2) != NO_ERROR) errors.error_var2 = 1;
            if(check_color(waarde3) != NO_ERROR) errors.error_var3 = 1;
            break;

        case FUNC_lijn:
            if(check_x(waarde1) != NO_ERROR) errors.error_var1 = 1;
            if(check_y(waarde2) != NO_ERROR) errors.error_var2 = 1;
            if(check_x(waarde3) != NO_ERROR) errors.error_var3 = 1;
            if(check_y(waarde4) != NO_ERROR) errors.error_var4 = 1;
            if(check_color(waarde5) != NO_ERROR) errors.error_var5 = 1;
            if(check_lijn_op_scherm(waarde1, waarde2, waarde3, waarde4, waarde6) != NO_ERROR) errors.error_var6 = 1;
            break;

        case FUNC_rechthoek:
            if(check_x(waarde1) != NO_ERROR) errors.error_var1 = 1;          // x
            if(check_y(waarde2) != NO_ERROR) errors.error_var2 = 1;          // y
            if(check_breedte(waarde1, waarde3) != NO_ERROR) errors.error_var3 = 1; // breedte
            if(check_hoogte(waarde2, waarde4) != NO_ERROR) errors.error_var4 = 1;  // hoogte
            if(check_color(waarde5) != NO_ERROR) errors.error_var5 = 1;      // kleur
            if(check_gevuld(waarde6) != NO_ERROR) errors.error_var6 = 1;     // gevuld
            break;
        default:
            // andere functies later
            break;
    }

    return errors; // alles in één keer terug
}

ErrorCode check_x(int x)
{
    if(x < 0 || x >= VGA_DISPLAY_X) return ERROR_X1;
    return NO_ERROR;
}

ErrorCode check_y(int y)
{
    if(y < 0 || y >= VGA_DISPLAY_Y) return ERROR_Y1;
    return NO_ERROR;
}

ErrorCode check_color(int color)
{
    if(color < 0 || color > 255) return ERROR_COLOR;
    return NO_ERROR;
}

ErrorCode check_lijn_op_scherm(int x1, int y1, int x2, int y2, int dikte)
{
    if(dikte < 1) return ERROR_DIJKTE_TOO_SMALL;

    int half = dikte / 2;

    // Bereken de min/max x en y van de lijn inclusief dikte
    int minX = x1 < x2 ? x1 : x2;
    int maxX = x1 > x2 ? x1 : x2;
    int minY = y1 < y2 ? y1 : y2;
    int maxY = y1 > y2 ? y1 : y2;

    minX -= half;
    maxX += half;
    minY -= half;
    maxY += half;

    // Check of alles binnen het scherm valt
    if(minX < 0 || maxX >= VGA_DISPLAY_X) return ERROR_X1;
    if(minY < 0 || maxY >= VGA_DISPLAY_Y) return ERROR_Y1;

    return NO_ERROR;
}

ErrorCode check_breedte(int x, int breedte)
{
    if(x + breedte - 1 >= VGA_DISPLAY_X) return ERROR_BREEDTE;
    return NO_ERROR;
}

ErrorCode check_hoogte(int y, int hoogte)
{
    if(y + hoogte - 1 >= VGA_DISPLAY_Y) return ERROR_HOOGTE;
    return NO_ERROR;
}

ErrorCode check_gevuld(int gevuld)
{
    if(gevuld != 0 && gevuld != 1) return ERROR_GEVULD;
    return NO_ERROR;
}

