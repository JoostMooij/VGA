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

/** @name Errorcodes */
//@{
#define NO_ERROR   0   /**< Geen fout */
#define ERR_VGA   -1   /**< Fout bij VGA operatie */
//@}

/** @name Kleurdefinities */
typedef enum
{
    ZWART       = 0x00,
    BLAUW       = 0x03,
    LICHTBLAUW  = 0x1F,
    GROEN       = 0x1C,
    LICHTGROEN  = 0x3E,
    CYAAN       = 0x1F,
    LICHTCYAAN  = 0x3F,
    ROOD        = 0xE0,
    LICHTROOD   = 0xF8,
    MAGENTA     = 0xE3,
    LICHTMAGENTA= 0xF3,
    BRUIN       = 0xB8,
    GEEL        = 0xFC,
    GRIJS       = 0x92,
    WIT         = 0xFF,
    ROZE        = 0xF9,
    PAARS       = 0x93
} Kleur;

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
int API_io_draw_pixel(int x, int y, int color);

#endif // APIIO_H
