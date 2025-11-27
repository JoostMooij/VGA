/**
 * @file ioLayer.h
 * @brief Headerfile voor een grafische library voor STM32
 * @author Joost
 * @version 1.0
 * @date 2025-11-20
 * @details Bevat alle prototypes en typedefs voor tekenen, kleuren, timers en eenvoudige animaties.
 */

#ifndef ioLayer_H
#define ioLayer_H

#include <stdint.h>

/**
 * @brief Beschikbare kleuren
 */
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

/**
 * @brief Wis het volledige scherm
 * @param kleur Achtergrondkleur
 */
int clearscherm(int kleur);

/**
 * @brief Teken een lijn tussen twee punten
 */
int lijn(int x0, int y0, int x1, int y1, int kleur, int dikte);

/**
 * @brief Teken een rechthoek
 */
int rechthoek(int x_lup, int y_lup, int breedte, int hoogte, int kleur, int gevuld);

/**
 * @brief Teken een cirkel
 */
int cirkel(int xc, int yc, int radius, int kleur);

/**
 * @brief Teken een vijfhoekig figuur
 */
int figuur(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, int kleur);

/**
 * @brief Teken een toren (rechthoek met cirkel)
 */
int toren(int x, int y, int grootte, int kleur1, int kleur2);

/**
 * @brief Wacht een aantal milliseconden
 */
int wacht(int msecs);

/**
 * @brief Herhaal een functie meerdere keren
 */
int herhaal(int aantal, int hoevaak);

/**
 * @brief Plaats tekst op het scherm
 */
int tekst(int x, int y, int kleur, const char* tekst, const char* fontnaam, int fontgrootte, const char* fontstijl);

/**
 * @brief Teken een bitmap op het scherm
 */
int bitmap(int nr, int x_lup, int y_lup);

/**
 * @brief Teken een bitmap op het scherm
 */
int setPixel(int x, int y, int kleur);

#endif
