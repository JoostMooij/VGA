/******************************************************************************
 * @file    test_logicLayer.c
 * @brief   Testbestand voor het controleren van de functionaliteit van de
 *          logicLayer. Dit bestand bevat hulpfuncties om ASCII-tekst op
 *          het VGA-scherm te tonen via een 6x8 bitmap-font.
 * @author  Joost & ChatGPT
 * @version 1.0
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stm32_ub_vga_screen.h"
#include "logicLayer.h"

/* -------------------------------------------------------------------------
 * Defines
 * ------------------------------------------------------------------------- */
#define CHAR_WIDTH     6       /**< Breedte van één karakter in pixels */
#define CHAR_HEIGHT    8       /**< Hoogte van één karakter in pixels  */
#define START_X        0       /**< Startpositie X op het scherm       */
#define START_Y        0       /**< Startpositie Y op het scherm       */


/* -------------------------------------------------------------------------
 * Font data
 * Auteur: 	ChatGPT, tijdelijk alternatief voor UART output
 * 6x8 bitmap-font; subset bevat spatie, komma, cijfers en hoofdletters.
 * ------------------------------------------------------------------------- */
const uint8_t font6x8[][8] =
{
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // Space
    {0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x30}, // Comma
    {0x3E,0x63,0x73,0x7B,0x6F,0x67,0x3E,0x00}, // 0
    {0x18,0x38,0x18,0x18,0x18,0x18,0x3C,0x00}, // 1
    {0x3C,0x66,0x0C,0x18,0x30,0x66,0x7E,0x00}, // 2
    {0x3C,0x66,0x0C,0x1C,0x0C,0x66,0x3C,0x00}, // 3
    {0x0C,0x1C,0x3C,0x6C,0x7E,0x0C,0x0C,0x00}, // 4
    {0x7E,0x60,0x7C,0x06,0x06,0x66,0x3C,0x00}, // 5
    {0x1C,0x30,0x60,0x7C,0x66,0x66,0x3C,0x00}, // 6
    {0x7E,0x66,0x0C,0x18,0x18,0x18,0x18,0x00}, // 7
    {0x3C,0x66,0x66,0x3C,0x66,0x66,0x3C,0x00}, // 8
    {0x3C,0x66,0x66,0x3E,0x06,0x0C,0x38,0x00}, // 9
    {0x18,0x3C,0x66,0x7E,0x66,0x66,0x66,0x00}, // A
    {0x7C,0x66,0x66,0x7C,0x66,0x66,0x7C,0x00}, // B
    {0x3C,0x66,0x60,0x60,0x60,0x66,0x3C,0x00}, // C
    {0x78,0x6C,0x66,0x66,0x66,0x6C,0x78,0x00}, // D
    {0x7E,0x60,0x60,0x7C,0x60,0x60,0x7E,0x00}, // E
    {0x7E,0x60,0x60,0x7C,0x60,0x60,0x60,0x00}, // F
    {0x3C,0x66,0x60,0x6E,0x66,0x66,0x3C,0x00}, // G
    {0x66,0x66,0x66,0x7E,0x66,0x66,0x66,0x00}, // H
    {0x3C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00}, // I
    {0x1E,0x0C,0x0C,0x0C,0x0C,0x6C,0x38,0x00}, // J
    {0x66,0x6C,0x78,0x70,0x78,0x6C,0x66,0x00}, // K
    {0x60,0x60,0x60,0x60,0x60,0x60,0x7E,0x00}, // L
    {0x63,0x77,0x7F,0x6B,0x63,0x63,0x63,0x00}, // M
    {0x66,0x76,0x7E,0x6E,0x66,0x66,0x66,0x00}, // N
    {0x3C,0x66,0x66,0x66,0x66,0x66,0x3C,0x00}, // O
    {0x7C,0x66,0x66,0x7C,0x60,0x60,0x7E,0x00}, // P
    {0x3C,0x66,0x66,0x66,0x6E,0x3C,0x0E,0x00}, // Q
    {0x7C,0x66,0x66,0x7C,0x78,0x6C,0x66,0x00}, // R
    {0x3C,0x60,0x60,0x3C,0x06,0x06,0x7C,0x00}, // S
    {0x7E,0x5A,0x18,0x18,0x18,0x18,0x3C,0x00}, // T
    {0x66,0x66,0x66,0x66,0x66,0x66,0x3C,0x00}, // U
    {0x66,0x66,0x66,0x66,0x66,0x3C,0x18,0x00}, // V
    {0x63,0x63,0x36,0x36,0x1C,0x1C,0x08,0x00}, // W
    {0x66,0x66,0x3C,0x18,0x3C,0x66,0x66,0x00}, // X
    {0x66,0x66,0x3C,0x18,0x18,0x18,0x3C,0x00}, // Y
    {0x7E,0x06,0x0C,0x18,0x30,0x60,0x7E,0x00}  // Z
};


/* -------------------------------------------------------------------------
 * string_naar_vga()
 * ------------------------------------------------------------------------- */

#define START_X_OFFSET 10      /**< Horizontale offset in pixels */
#define START_Y_OFFSET 10      /**< Verticale offset in pixels    */


/**
 * @brief   Render een ASCII-string op het VGA-scherm met behulp van font6x8.
 * Auteur: 	ChatGPT, tijdelijk alternatief voor UART output
 * @details De functie leest de opgegeven string karakter voor karakter, zoekt
 *          het corresponderende 6x8 bitmap-patroon op in font6x8 en tekent dit
 *          pixel voor pixel op het VGA-scherm. Wanneer de rechterrand van het
 *          scherm wordt bereikt, gaat de functie automatisch naar de volgende
 *          regel. Onbekende karakters worden genegeerd.
 *
 * @param   str Pointer naar een null-terminated ASCII-string.
 *
 * @note    Alleen spatie, komma, cijfers en A-Z worden ondersteund door dit
 *          tijdelijke font. Kleine letters worden omgezet naar hoofdletters.
 */
void string_naar_vga(const char *str)
{
    if (str == NULL) return;

    uint16_t x_off = START_X + START_X_OFFSET;
    uint16_t y_off = START_Y + START_Y_OFFSET;

    while (*str)
    {
        char c = *str;
        uint8_t index = 0xFF;

        /* Karakter → font-index mapping */
        if (c == ' ')                     index = 0;
        else if (c == ',')                index = 1;
        else if (c >= '0' && c <= '9')    index = (c - '0') + 2;
        else if (c >= 'A' && c <= 'Z')    index = (c - 'A') + 12;
        else if (c >= 'a' && c <= 'z')    index = (c - 'a') + 12;

        if (index != 0xFF)
        {
            for (uint8_t yp = 0; yp < CHAR_HEIGHT; yp++)
            {
                uint8_t row = font6x8[index][yp];
                for (uint8_t xp = 0; xp < CHAR_WIDTH; xp++)
                {
                    if (row & (1 << (CHAR_WIDTH - 1 - xp)))
                        UB_VGA_SetPixel(x_off + xp, y_off + yp, VGA_COL_WHITE);
                    else
                        UB_VGA_SetPixel(x_off + xp, y_off + yp, VGA_COL_BLACK);
                }
            }
        }

        /* Volgende karakterpositie */
        x_off += CHAR_WIDTH;

        if (x_off + CHAR_WIDTH >= VGA_DISPLAY_X)
        {
            x_off = START_X + START_X_OFFSET;
            y_off += CHAR_HEIGHT;

            if (y_off + CHAR_HEIGHT >= VGA_DISPLAY_Y)
                y_off = START_Y + START_Y_OFFSET;
        }

        str++;
    }
}


/**
 * @brief   Converteer een COMMANDO_TYPE-enum naar een bijbehorende tekststring.
 * Auteur: 	ChatGPT, testen van omzetten naar ENUM
 * @details De functie biedt een eenvoudige mapping van elk commando in de
 *          enumeratie COMMANDO_TYPE naar een constant ASCII-label dat kan
 *          worden weergegeven op het VGA-scherm of gebruikt in debugging.
 *
 * @param   cmd Het COMMANDO_TYPE-commando dat moet worden omgezet.
 *
 * @return  Constante pointer naar de corresponderende ASCII-string.
 */
const char* commando_naar_string(COMMANDO_TYPE cmd)
{
    switch(cmd) {
        case CMD_LIJN:       return "CMD_LIJN";
        case CMD_RECHTHOEK:  return "CMD_RECHTHOEK";
        case CMD_TEKST:      return "CMD_TEKST";
        case CMD_BITMAP:     return "CMD_BITMAP";
        case CMD_CLEAR:      return "CMD_CLEAR";
        case CMD_WACHT:      return "CMD_WACHT";
        case CMD_HERHAAL:    return "CMD_HERHAAL";
        case CMD_CIRKEL:     return "CMD_CIRKEL";
        case CMD_FIGUUR:     return "CMD_FIGUUR";
        case CMD_TOREN:      return "CMD_TOREN";
        default:             return "CMD_ONBEKEND";
    }
}


/**
 * @brief   Testfunctie voor de logicLayer.
 *
 * @details Deze functie vormt een eenvoudige demonstratie van de werking van
 *          de logicLayer. Een vooraf ingestelde teststring wordt:
 *          1) opgeschoond via string_ophalen(),
 *          2) geanalyseerd met verwerk_commando(),
 *          3) omgezet naar tekst via commando_naar_string(),
 *          4) weergegeven op het VGA-scherm via string_naar_vga().
 *
 * @note    Dit is uitsluitend een testfunctie; in de uiteindelijke applicatie
 *          zal de input afkomstig zijn van UART of een andere interface.
 */
void test_logicLayer(void)
{
    char testInput[MAX_INPUT] = "lijn,1,1,100,100,rood,4";

    // Stap 1: input opschonen
    string_ophalen(testInput);

    // Stap 2: commando eruit halen
    COMMANDO_TYPE cmd = verwerk_commando(testInput);

    // Stap 3: enum → string
    const char *cmdStr = commando_naar_string(cmd);

    // Stap 4: toon op VGA
    string_naar_vga(cmdStr);
}
