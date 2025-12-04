/**
 * @file APIerror.h
 * @brief Centrale definitie van foutcodes voor de EE‑API‑LIB.
 *
 * Bevat defines voor alle errorcodes, struct voor meerdere fouten tegelijk,
 * en prototypes voor validatiefuncties en foutafhandeling.
 *
 * @author Thijs
 * @version 1.0
 * @date 2025-11-20
 */

#ifndef APIERROR_H
#define APIERROR_H

/**
 * @brief Functies die gecontroleerd worden.
 */
typedef enum {
    FUNC_clearscherm = 1,
    FUNC_drawPixel   = 2,
    FUNC_lijn        = 3,
    FUNC_rechthoek   = 4,
    FUNC_figuur		 = 5,     // ← toevoegen
    // later uitbreiden met andere functies
} FunctionID;

/**
 * @brief Enum voor individuele foutcodes.
 */
typedef enum {
    NO_ERROR               = 0,
    ERROR_X1               = 1,
    ERROR_Y1               = 2,
    ERROR_COLOR            = 3,
    ERROR_DIJKTE_TOO_SMALL = 4,
    ERROR_BREEDTE          = 5,
    ERROR_HOOGTE           = 6,
    ERROR_GEVULD           = 7,
} ErrorCode;

/**
 * @brief Struct om meerdere fouten tegelijk terug te geven.
 */
typedef struct {
    int error_var1;
    int error_var2;
    int error_var3;
    int error_var4;
    int error_var5;
    int error_var6;
    int error_var7;
    int error_var8;
    int error_var9;
    int error_var10;
    int error_var11;
} ErrorList;

/**
 * @brief Controleer de X-coordinaat.
 */
ErrorCode check_x(int x);

/**
 * @brief Controleer de Y-coordinaat.
 */
ErrorCode check_y(int y);

/**
 * @brief Controleer de kleurwaarde.
 */
ErrorCode check_color(int color);

/**
 * @brief Controleer of een lijn binnen het scherm valt inclusief dikte.
 */
ErrorCode check_lijn_op_scherm(int x1, int y1, int x2, int y2, int dikte);

/**
 * @brief Controleer breedte van een rechthoek.
 */
ErrorCode check_breedte(int x, int breedte);

/**
 * @brief Controleer hoogte van een rechthoek.
 */
ErrorCode check_hoogte(int y, int hoogte);

/**
 * @brief Controleer de 'gevuld'-waarde (0 of 1).
 */
ErrorCode check_gevuld(int gevuld);

/**
 * @brief Controleer waarden van een functie en geef fouten terug.
 */
ErrorList Error_handling(FunctionID func, int waarde1, int waarde2, int waarde3, int waarde4, int waarde5, int waarde6, int waarde7, int waarde8, int waarde9, int waarde10, int waarde11);

#endif
