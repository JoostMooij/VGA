/**
 * @file APIdraw.h
 * @brief Header voor de tekenlaag van de EE‑API‑LIB.
 *
 * Bevat prototypes voor hogere tekenfuncties bovenop APIio:
 * - Lijn tekenen
 * - Rechthoek tekenen
 * - Figuur tekenen
 * - Cirkel tekenen
 * - Toren tekenen
 *
 * @author Thijs
 * @version 1.0
 * @date 2025-11-20
 */

#ifndef APIDRAW_H
#define APIDRAW_H

#include "APIerror.h"

/**
 * @brief Teken een lijn van (x1,y1) naar (x2,y2) met opgegeven kleur en dikte.
 *
 * @param x1 Start X-coordinaat
 * @param y1 Start Y-coordinaat
 * @param x2 Eind X-coordinaat
 * @param y2 Eind Y-coordinaat
 * @param kleur Kleur als string
 * @param dikte Dikte van de lijn in pixels
 * @return ErrorList Struct met eventuele fouten (0 = geen fouten)
 *
 * @note Gebruikt Bresenham-algoritme en voegt dikte toe door extra pixels.
 */
ErrorList lijn(int x1, int y1, int x2, int y2, const char *kleur, int dikte);

/**
 * @brief Teken een rechthoek op scherm met opgegeven kleur en gevuld-optie.
 *
 * @param x Linker bovenhoek X
 * @param y Linker bovenhoek Y
 * @param w Breedte van de rechthoek
 * @param h Hoogte van de rechthoek
 * @param kleur Kleur als string
 * @param gevuld 1 = gevuld, 0 = alleen rand
 * @return ErrorList Struct met eventuele fouten (0 = geen fouten)
 *
 * @note Bij gevulde rechthoek worden alle pixels binnen het gebied gezet, anders enkel de rand.
 */
ErrorList rechthoek(int x, int y, int w, int h, const char *kleur, int gevuld);

/**
 * @brief Teken een figuur bestaande uit 5 punten (gesloten vorm).
 *
 * @param x1 X-coordinaat punt 1
 * @param y1 Y-coordinaat punt 1
 * @param x2 X-coordinaat punt 2
 * @param y2 Y-coordinaat punt 2
 * @param x3 X-coordinaat punt 3
 * @param y3 Y-coordinaat punt 3
 * @param x4 X-coordinaat punt 4
 * @param y4 Y-coordinaat punt 4
 * @param x5 X-coordinaat punt 5
 * @param y5 Y-coordinaat punt 5
 * @param kleur Kleur van de lijnen als string
 * @return ErrorList Struct met eventuele fouten (0 = geen fouten)
 *
 * @note Verbindt de punten met lijnen om een gesloten vorm te maken.
 */
ErrorList figuur(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, const char *kleur);

/**
 * @brief Teken een cirkel met opgegeven straal en kleur.
 *
 * @param x0 X-coordinaat van het midden
 * @param y0 Y-coordinaat van het midden
 * @param radius Straal van de cirkel
 * @param kleur Kleur als string
 * @return ErrorList Struct met eventuele fouten (0 = geen fouten)
 *
 * @note Gebruikt midpoint/circle algoritme voor rasterweergave.
 */
ErrorList cirkel(int x0, int y0, int radius, const char* kleur);

/**
 * @brief Tekent een bitmap symbool, gecentreerd op (x, y).
 *
 * @param nr Het nummer van het symbool (1 = pijl omhoog, 2 = pijl omlaag, etc.).
 * @param x X-coordinaat van het midden van het symbool.
 * @param y Y-coordinaat van het midden van het symbool.
 * @return ErrorList Struct met eventuele fouten (0 = geen fouten)
 */
ErrorList bitMap(int nr, int x, int y);

 /*
 * @brief Teken een toren bestaande uit meerdere lagen, mast en vlag.
 *
 * @param x X-coordinaat van het midden van de torenbasis
 * @param y Y-coordinaat van de torenbasis
 * @param grootte Basisgrootte van de toren
 * @param kleur1 Kleur van de torenlagen
 * @param kleur2 Kleur van mast en vlag
 * @return ErrorList Struct met eventuele fouten (0 = geen fouten)
 *
 * @note Bouwt de toren laag voor laag op, tekent vervolgens mast en vlag.
 */
ErrorList toren(int x, int y, int grootte, const char* kleur1, const char* kleur2);

#endif
