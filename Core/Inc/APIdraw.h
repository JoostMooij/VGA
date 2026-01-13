/**
 * @file APIdraw.h
 * @brief Header voor APIdraw.c
 *
 *
 * @author Thijs, Joost, Luc
 * @version 1.0
 * @date 2025-11-20
 */

#ifndef APIDRAW_H
#define APIDRAW_H

#include "APIerror.h"

#define BASE_FONT_SIZE 8

/*-------------------------------------------------------
 * Functieprototypes
 *-------------------------------------------------------*/

/**
 * @brief Teken een lijn van (x1,y1) naar (x2,y2) met opgegeven kleur en dikte.
 */
ErrorList lijn(int x1, int y1, int x2, int y2, const char *kleur, int dikte);

/**
 * @brief Teken een rechthoek op scherm met opgegeven kleur en gevuld-optie.
 */
ErrorList rechthoek(int x, int y, int w, int h, const char *kleur, int gevuld);

/**
 * @brief Teken een figuur bestaande uit 5 punten (gesloten vorm).
 */
ErrorList figuur(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, const char *kleur);

/**
 * @brief Teken een cirkel met opgegeven straal en kleur.
 */
ErrorList cirkel(int x0, int y0, int radius, const char* kleur);

/**
 * @brief Tekent een bitmap symbool, gecentreerd op (x, y).
 */
ErrorList bitMap(int nr, int x, int y);

/**
 * @brief Teken een toren bestaande uit meerdere lagen, mast en vlag.
 */
ErrorList toren(int x, int y, int grootte, const char* kleur1, const char* kleur2);

/**
 * @brief Tekent een tekststring op het scherm met gespecificeerde stijl en schaal.
 */
ErrorList tekst(int x, int y, const char *kleur_str, const char* tekst_str,
                const char* fontnaam, int fontgrootte, const char* fontstijl);

#endif /* APIDRAW_H */
