/**
 * @file APIdraw.h
 * @brief Header voor de tekenlaag van de EE‑API‑LIB.
 *
 * Bevat prototypes voor hogere tekenfuncties bovenop APIio:
 * - Lijn tekenen
 * - Rechthoek tekenen
 * - Bitmap tekenen (later)
 * - Tekst tekenen (later)
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
 */
ErrorList rechthoek(int x, int y, int w, int h, const char *kleur, int gevuld);

ErrorList figuur(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, const char *kleur);

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

#endif
