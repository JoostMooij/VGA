/**
 * @file APIdraw.c
 * @brief Implementatie van de tekenlaag van de EE‑API‑LIB.
 *
 * Bouwt hogere tekenfuncties bovenop APIio:
 * - Gebruikt API_io_draw_pixel() als basis
 * - Implementeert algoritmes (bv. Bresenham voor lijnen)
 * - Combineert pixels tot vormen en tekst
 */

#include "APIdraw.h"
#include "APIio.h"

int API_draw_line(int x1, int y1, int x2, int y2, int color, int dikte) {
    if(x1 == x2) {
        // verticale lijn
        for(int y = y1; y <= y2; y++) {
            for(int dx = -(dikte/2); dx <= dikte/2; dx++) {
                API_io_draw_pixel(x1 + dx, y, color);
            }
        }
    } else if(y1 == y2) {
        // horizontale lijn
        for(int x = x1; x <= x2; x++) {
            for(int dy = -(dikte/2); dy <= dikte/2; dy++) {
                API_io_draw_pixel(x, y1 + dy, color);
            }
        }
    } else {
        // diagonale lijn (Bresenham + dikte)
        int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
        int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
        int err = dx + dy, e2;

        while(1) {
            // teken de "dikte" rondom de lijn
            for(int ox = -(dikte/2); ox <= dikte/2; ox++) {
                for(int oy = -(dikte/2); oy <= dikte/2; oy++) {
                    API_io_draw_pixel(x1 + ox, y1 + oy, color);
                }
            }

            if(x1 == x2 && y1 == y2) break;
            e2 = 2 * err;
            if(e2 >= dy) { err += dy; x1 += sx; }
            if(e2 <= dx) { err += dx; y1 += sy; }
        }
    }
//    return NO_ERROR;
}
