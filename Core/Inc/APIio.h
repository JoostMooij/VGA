/**
 * @file APIio.h
 * @brief Header voor de I/O-laag van de EE-API-LIB.
 *
 * Deze header bevat alle prototypes, definities en kleurcodes voor de
 * basis-I/O functionaliteit:
 * - VGA-kleuren
 * - Scherm wissen
 * - Pixels tekenen
 * - Omzetting van stringkleur → kleurcode
 *
 * De implementatie bevindt zich in APIio.c.
 *
 * @author Thijs
 * @version 1.1
 * @date 2025-11-20
 */

#ifndef APIIO_H
#define APIIO_H

#include <stdint.h>

#include "APIerror.h"

/**
 * @brief Maakt een 8-bit VGA kleurwaarde op basis van R/G/B componenten.
 *
 * VGA gebruikt 3 bits rood, 3 bits groen en 2 bits blauw.
 *
 * @param R Waarde 0–7
 * @param G Waarde 0–7
 * @param B Waarde 0–3
 */
#define VGA_RGB(R,G,B)  ( ((R & 0x07) << 5) | ((G & 0x07) << 2) | (B & 0x03) )

/* -------------------------------------------------------------------------- */
/*  Typedefs                                                                  */
/* -------------------------------------------------------------------------- */

/**
 * @brief Kleurdefinities voor het VGA-scherm in 8-bit RGB.
 *
 * Deze waarden worden gebruikt door alle API-functies zoals drawPixel()
 * en clearscherm(). De namen gebruiken Nederlandse kleurnamen zodat de API
 * intuïtief blijft.
 */
typedef enum
{
    ZWART        = VGA_RGB(0,0,0),
    BLAUW        = VGA_RGB(0,0,3),
    LICHTBLAUW   = VGA_RGB(0,1,3),
    GROEN        = VGA_RGB(0,7,0),
    LICHTGROEN   = VGA_RGB(0,7,2),
    CYAAN        = VGA_RGB(0,7,3),
    LICHTCYAAN   = VGA_RGB(0,7,3),   /* Identiek aan CYAAN – aanpassen indien nodig */
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

/* -------------------------------------------------------------------------- */
/*  Functieprototypes                                                         */
/* -------------------------------------------------------------------------- */

/**
 * @brief Initialiseert de I/O-laag (VGA-scherm).
 *
 * Moet worden aangeroepen vóór de rest van de API.
 */
void API_init_io(void);

/**
 * @brief Vult het hele scherm met een bepaalde kleur.
 *
 * @param kleur Kleurnaam als string ("rood", "groen", etc.)
 * @return ErrorList Struct met foutstatus.
 */
ErrorList clearscherm(const char *kleur);

/**
 * @brief Zet een pixel naar een bepaalde kleur.
 *
 * @param x     X-coördinaat van de pixel.
 * @param y     Y-coördinaat van de pixel.
 * @param kleur Kleurnaam als string.
 * @return ErrorList Struct met foutstatus.
 */
ErrorList drawPixel(int x, int y, const char *kleur);

/**
 * @brief Zet een Nederlandse kleurnaam om naar een VGA-kleurcode.
 *
 * @param input Kleur-string ("rood", "wit", "grijs", etc.)
 * @return uint8_t kleurcode, of 0 als de kleur ongeldig is.
 */
uint8_t kleur_omzetter(const char *input);

#endif /* APIIO_H */
