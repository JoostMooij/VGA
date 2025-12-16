/*
 * font_acorn_8x8.h
 *
 *  Created on: Dec 16, 2025
 *      Author: beren
 */

#ifndef FONT_ACORN_8X8_H
#define FONT_ACORN_8X8_H

// Definitie van de Acorn font afmetingen
#define FONT_ACORN_WIDTH 8
#define FONT_ACORN_HEIGHT 8
#define FONT_ACORN_SIZE (8 * 256) // 8 bytes per karakter * 256 karakters

// Declareer de font data array.
extern const unsigned char fontdata_acorn8x8[FONT_ACORN_SIZE];

#endif // FONT_ACORN_8X8_H
