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
typedef enum
{
    FUNC_clearscherm = 1,
    FUNC_drawPixel   = 2,
    FUNC_lijn        = 3,
    FUNC_rechthoek   = 4,
    FUNC_figuur      = 5,
    FUNC_cirkel      = 6,
    FUNC_toren       = 7,
    FUNC_wacht       = 8,
    FUNC_herhaal     = 9,
    FUNC_bitmap      = 10,
    FUNC_tekst       = 11,
} FunctionID;

/**
 * @brief Enum voor individuele foutcodes.
 */
typedef enum
{
    NO_ERROR                   = 0,
    ERROR_X1                   = 1,
    ERROR_Y1                   = 2,
    ERROR_COLOR                = 3,
    ERROR_DIJKTE_TOO_SMALL     = 4,
    ERROR_BREEDTE              = 5,
    ERROR_HOOGTE               = 6,
    ERROR_GEVULD               = 7,
    ERROR_RADIUS_TOO_SMALL     = 8,
    ERROR_GROOTTE_TOO_SMALL    = 9,
    ERROR_TOREN_BUITEN_SCHERM  = 10,
    ERROR_TIME_TOO_SMALL       = 11,
    AANTAL_NOT_IN_RANGE        = 12,
    HOEVAAK_NOT_IN_RANGE       = 13,
    ERROR_bitmap_nr            = 14,
    ERROR_bitmap_buiten_scherm = 15,
    ERROR_tekst_buiten_scherm  = 16,
    ERROR_TEXT_EMPTY           = 17,
    ERROR_INVALID_STYL         = 18,
} ErrorCode;

/**
 * @brief Struct om meerdere fouten tegelijk terug te geven.
 */
typedef struct
{
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
 * @brief Controleer waarden van een functie en geef fouten terug.
 * @param func Functie-ID
 * @param waarde1 t/m waarde11 Waarden voor validatie
 * @return Struct met eventuele foutcodes
 */
ErrorList Error_handling(FunctionID func,
                         int waarde1, int waarde2, int waarde3,
                         int waarde4, int waarde5, int waarde6,
                         int waarde7, int waarde8, int waarde9,
                         int waarde10, int waarde11);

/**
 * @brief Controleer X-coördinaat
 */
ErrorCode check_x(int x);

/**
 * @brief Controleer Y-coördinaat
 */
ErrorCode check_y(int y);

/**
 * @brief Controleer kleurwaarde
 */
ErrorCode check_color(int color);

/**
 * @brief Controleer of een lijn inclusief dikte binnen scherm valt
 */
ErrorCode check_lijn_op_scherm(int x1, int y1, int x2, int y2, int dikte);

/**
 * @brief Controleer breedte van rechthoek
 */
ErrorCode check_breedte(int x, int breedte);

/**
 * @brief Controleer hoogte van rechthoek
 */
ErrorCode check_hoogte(int y, int hoogte);

/**
 * @brief Controleer 'gevuld'-waarde (0 of 1)
 */
ErrorCode check_gevuld(int gevuld);

/**
 * @brief Controleer radius voor cirkel binnen scherm
 */
ErrorCode check_radius_op_scherm(int x, int y, int radius);

/**
 * @brief Controleer bitmap nummer en positie
 */
ErrorCode check_nr(int nr, int x, int y);

/**
 * @brief Controleer algemene grootte van object binnen scherm
 */
ErrorCode check_grootte_op_scherm(int x, int y, int grootte);

/**
 * @brief Controleer toren grootte en positie binnen scherm
 */
ErrorCode check_toren_op_scherm(int x, int y, int grootte);

/**
 * @brief Controleer wachttijd (ms)
 */
ErrorCode wacht_error(int ms_error);

/**
 * @brief Controleer herhaal aantal (1-20)
 */
ErrorCode herhaal_aantal_error(int aantal);

/**
 * @brief Controleer herhaal frequentie (1-40)
 */
ErrorCode herhaal_hoevaak_error(int hoevaak);

/**
 * @brief Controleer of tekst leeg is
 */
ErrorCode check_text_empty(const char* tekst_str);

/**
 * @brief Controleer schaalfactor van object of tekst
 */
ErrorCode check_schaal(int schaal_factor);

/**
 * @brief Controleer fontstijl ("vet", "cursief", "normaal")
 */
ErrorCode check_fontstijl(const char* fontstijl);

/**
 * @brief Controleer of tekst binnen scherm past
 */
ErrorCode check_tekst_op_scherm(int x, int y, const char* tekst_str, int schaal_factor, const char* fontstijl);

#endif /* APIERROR_H */
