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
        case FUNC_SET_PIXEL:
            if(check_x1(waarde1) != NO_ERROR) errors.error_var1 = 1;
            if(check_y1(waarde2) != NO_ERROR) errors.error_var2 = 1;
            if(check_color(waarde3) != NO_ERROR) errors.error_var3 = 1;
            break;

//        case FUNC_DRAW_LINE:
//            if(check_x1(waarde1) != NO_ERROR || check_x1(x2) != NO_ERROR) errors.error_var1 = 1;
//            if(check_y1(waarde2) != NO_ERROR || check_y1(y2) != NO_ERROR) errors.error_var2 = 1;
//            if(check_color(waarde3) != NO_ERROR) errors.error_var3 = 1;
//            break;

        default:
            // andere functies later
            break;
    }

    return errors; // alles in één keer terug
}

// Functies om individuele waarden te checken
ErrorCode check_x1(int x)
{
    if(x < 0 || x >= VGA_DISPLAY_X) return ERROR_X1;
    return NO_ERROR;
}

ErrorCode check_y1(int y)
{
    if(y < 0 || y >= VGA_DISPLAY_Y) return ERROR_Y1;
    return NO_ERROR;
}

ErrorCode check_color(int color)
{
    if(color < ZWART || color > PAARS) return ERROR_COLOR;
    return NO_ERROR;
}
