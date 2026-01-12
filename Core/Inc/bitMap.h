/*
 * bitMap.h
 *
 *  Created on: Dec 8, 2025
 *      Author: beren
 */

#ifndef BITMAP_H_
#define BITMAP_H_

#include <stdint.h>

#define VGA_DISPLAY_X   320
#define VGA_DISPLAY_Y   240

#define grote_pijlen 16
#define grote_smiley 32
#define kat_afbeelding 120
#define skalet_afbeelding 90
#define KIP_afbeelding 90


// --- Struct Definitie ---
// Aangepast om kleurdiepte te ondersteunen (1-bit voor pijlen, 4-bit voor kleuren)
typedef struct {
    uint8_t width;
    uint8_t height;
    const uint8_t *data;
    uint8_t color_depth; // 1 (monochroom) of 4 (16 kleuren)
} Bitmap;

// --- Externe Declaraties van Bitmaps ---
// 1-bit bitmaps (Pijlen)
extern const Bitmap arrow_up;
extern const Bitmap arrow_down;
extern const Bitmap arrow_left;
extern const Bitmap arrow_right;
extern const Bitmap smily_blij;
extern const Bitmap smily_boos;


// 4-bit bitmap (Kleurafbeelding)
extern const Bitmap yes_cat_thumbs_up; // NIEUW
extern const Bitmap skeleton_banging_on_shield_meme_frame_00;
extern const Bitmap skeleton_banging_on_shield_meme_frame_06;
extern const Bitmap KIP;

// --- Functie Prototypes ---
/**
 * @brief Tekent een bitmap op het scherm, ondersteunt 1-bit en 4-bit.
 *
 * @param x0 Linkerbovenhoek X-coordinaat
 * @param y0 Linkerbovenhoek Y-coordinaat
 * @param bmp Pointer naar de Bitmap struct
 * @param kleur Kleur om de "1" bits mee te tekenen (alleen gebruikt voor 1-bit bitmaps)
 */
void drawBitmap(int x0, int y0, const Bitmap *bmp, uint8_t fg_vga_code);

#endif /* BITMAP_H_ */
