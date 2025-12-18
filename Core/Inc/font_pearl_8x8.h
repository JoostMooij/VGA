/*
 * tekst.h
 *
 *  Created on: Dec 15, 2025
 *      Author: Luc
 */

#ifndef INC_FONT_PEARL_8X8_H_
#define INC_FONT_PEARL_8X8_H_

#include <stddef.h> // Voor size_t (hoewel optioneel)

// De font array is 8 bytes * 256 karakters = 2048 bytes
#define FONT_PEARL_WIDTH 8
#define FONT_PEARL_HEIGHT 8
#define FONT_PEARL_SIZE (8 * 256)

// Declareer de font data array.
// Het 'extern' keyword zorgt ervoor dat andere bestanden deze kunnen zien.
extern const unsigned char fontdata_pearl8x8[FONT_PEARL_SIZE];

#endif /* INC_FONT_PEARL_8X8_H_ */
