/**
 * @file APIio.h
 * @brief Header voor de I/O-laag van de EE-API-LIB.
 *
 * Bevat prototypes en defines voor basis I/O-functies:
 * - Pixel zetten
 * - Scherm wissen
 * - Errorcodes en kleurdefines
 *
 * @author Thijs
 * @version 1.0
 * @date 2025-11-20
 */

#ifndef APIIO_H
#define APIIO_H

#include <stdint.h>  // voor uint8_t etc.

/* Forward declaratie van ErrorList */
typedef struct ErrorList ErrorList1;

// Macro om R,G,B los in te voeren (R=0..7, G=0..7, B=0..3)
#define VGA_RGB(R,G,B)  ( ((R & 0x07) << 5) | ((G & 0x07) << 2) | (B & 0x03) )

// Voorbeeld van hoe je het kunt gebruiken
typedef enum {
    ZWART        = VGA_RGB(0,0,0),
    BLAUW        = VGA_RGB(0,0,3),
    LICHTBLAUW   = VGA_RGB(0,1,3),
    GROEN        = VGA_RGB(0,7,0),
    LICHTGROEN   = VGA_RGB(0,7,2),
    CYAAN        = VGA_RGB(0,7,3),
    LICHTCYAAN   = VGA_RGB(0,7,3),
    ROOD         = VGA_RGB(7,0,0),
    LICHTROOD    = VGA_RGB(7,0,1),
    MAGENTA      = VGA_RGB(7,0,3),
    LICHTMAGENTA = VGA_RGB(7,0,2),
    BRUIN        = VGA_RGB(5,3,0),
    GEEL         = VGA_RGB(7,7,0),
    GRIJS        = VGA_RGB(4,4,2),
    WIT          = VGA_RGB(7,7,3),
    ROZE         = VGA_RGB(7,3,3),
    PAARS        = VGA_RGB(5,0,3)
} Kleur;

//// Struct om meerdere fouten tegelijk terug te geven
//typedef struct {
//    int error1_var1;
//    int error1_var2;
//    int error1_var3;
//    int error1_var4;
//    int error1_var5;
//    int error1_var6;
//    int error1_var7;
//    int error1_var8;
//    int error1_var9;
//    int error1_var10;
//    int error1_var11;
//} ErrorList1;

/** @brief Initialiseert de I/O hardware
 *
 * Roept de driver init-functies aan voor VGA en eventuele andere devices.
 *
 * @param dev1 ID van device 1 (optioneel)
 * @param dev2 ID van device 2 (optioneel)
 * @return NO_ERROR bij succes
 */
int API_init_io(int dev1, int dev2);

/** @brief Leegt het scherm met een bepaalde kleur
 *
 * @param color Kleur om het scherm mee te vullen (enum Kleur)
 * @return NO_ERROR bij succes
 */
int API_clearscreen(int color);

/** @brief Zet één pixel op het VGA-scherm
 *
 * Controleert eerst of de coördinaten binnen het scherm vallen.
 *
 * @param x X-coördinaat (0 .. VGA_DISPLAY_X-1)
 * @param y Y-coördinaat (0 .. VGA_DISPLAY_Y-1)
 * @param color Kleur (enum Kleur)
 * @return NO_ERROR bij succes, ERR_VGA bij fout
 */
ErrorList1 API_io_draw_pixel_checked(int x, int y, int color);

#endif // APIIO_H
