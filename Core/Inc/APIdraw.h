/**
 * @file APIdraw.h
 * @brief Header voor de tekenlaag van de EE‑API‑LIB.
 *
 * Bevat prototypes voor hogere tekenfuncties:
 * - Lijn tekenen
 * - Rechthoek tekenen
 * - Bitmap tekenen
 * - Tekst tekenen
 */

#ifndef APIDRAW_H
#define APIDRAW_H

#include "APIerror.h"

ErrorList lijn(int x1, int y1, int x2, int y2, const char *kleur, int dikte);
ErrorList rechthoek(int x, int y, int w, int h, const char *kleur, int gevuld);
#endif
