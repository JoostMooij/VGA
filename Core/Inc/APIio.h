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

#include <stdint.h>
#include "APIerror.h"

#define VGA_RGB(R,G,B)  ( ((R & 0x07) << 5) | ((G & 0x07) << 2) | (B & 0x03) )

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

void API_init_io(void);
ErrorList clearscherm(const char *kleur);
ErrorList drawPixel(int x, int y, const char *kleur);
uint8_t kleur_omzetter(const char *input);
#endif
