/**
 * @file APIio.h
 * @brief Header voor de I/O-laag
 *
 * De implementatie bevindt zich in APIio.c.
 *
 * @author Thijs, Joost, Luc
 * @version 1.1
 * @date 2025-11-20
 */

#ifndef APIIO_H
#define APIIO_H

#include <stdint.h>
#include "APIerror.h"
#include "stm32f4xx.h"
#include "APIdraw.h"
#include "stm32_ub_vga_screen.h"

/**
 * @brief Alle mogelijke commando’s die de logica kan verwerken.
 */
typedef enum
{
    CMD_ONBEKEND    = 0,  /**< Onbekend commando */
    CMD_LIJN        = 1,  /**< Lijn tekenen */
    CMD_RECHTHOEK   = 2,  /**< Rechthoek tekenen */
    CMD_TEKST       = 3,  /**< Tekst plaatsen */
    CMD_BITMAP      = 4,  /**< Bitmap tonen */
    CMD_CLEAR       = 5,  /**< Scherm wissen */
    CMD_WACHT       = 6,  /**< Wachtopdracht */
    CMD_HERHAAL     = 7,  /**< Herhaalblok */
    CMD_CIRKEL      = 8,  /**< Cirkel tekenen */
    CMD_FIGUUR      = 9,  /**< Complex figuur tekenen */
    CMD_TOREN       = 10, /**< Torenopdracht */
    CMD_SETPIXEL    = 11, /**< Eén pixel zetten */
} COMMANDO_TYPE;

/*-------------------------------------------------------
 * VGA kleurmacro
 *-------------------------------------------------------*/
/**
 * @brief Maakt een 8-bit VGA kleurwaarde op basis van R/G/B componenten.
 *
 * VGA gebruikt 3 bits rood, 3 bits groen en 2 bits blauw.
 *
 * @param R Waarde 0–7
 * @param G Waarde 0–7
 * @param B Waarde 0–3
 */
#define VGA_RGB(R,G,B)  ( ((R & 0x07) << 5) | ((G & 0x07) << 2) | (B & 0x03) )

/*-------------------------------------------------------
 * Herhalings- en bufferdefinities
 *-------------------------------------------------------*/
#define MAX_COMMAND_HISTORY_SIZE 25
#define MAX_COMMAND_BUFFER_SIZE 245
#define MAX_TEKST_HISTORY 4

/*-------------------------------------------------------
 * Globale variabelen (extern)
 *-------------------------------------------------------*/
extern uint16_t command_buffer[MAX_COMMAND_BUFFER_SIZE];
extern uint16_t command_buffer_index;
extern volatile uint32_t ms_tick_counter;
extern int herhaal_hoog;

/*-------------------------------------------------------
 * VGA kleuren (Nederlandse namen)
 *-------------------------------------------------------*/
typedef enum
{
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
    PAARS        = VGA_RGB(5,0,3),
    Ongeldige_kleur
} Kleur;

/*-------------------------------------------------------
 * Functieprototypes
 *-------------------------------------------------------*/

/** Initialiseert de I/O-laag (VGA-scherm) */
void API_init_io(void);

/** Vult het hele scherm met een bepaalde kleur */
ErrorList clearscherm(const char *kleur);

/** Zet een pixel naar een bepaalde kleur (string) */
ErrorList drawPixel(int x, int y, const char *kleur);

/** Zet een pixel naar een bepaalde kleur (numeriek) */
ErrorList setPixel(int x, int y, int kleur);

/** Zet een Nederlandse kleurnaam om naar een VGA-kleurcode */
uint8_t kleur_omzetter(const char *input);

/** Configureert de SysTick timer voor 1 ms ticks */
void SysTick_Init(void);

/** Vertraging (delay) op basis van SysTick */
ErrorList wacht(int ms);

/** Voert de laatste 'aantal' commando's opnieuw uit, 'hoevaak' keer */
ErrorList herhaal(int aantal, int hoevaak);

/** Slaat een commando ID en parameters op in de lineaire buffer */
void record_command(COMMANDO_TYPE type, int param_count, const int params[]);

/** Geeft de totale omvang van een commando inclusief parameters */
int get_command_size(COMMANDO_TYPE type);

/** Converteert een VGA-kleurcode terug naar string */
const char* get_color_string_from_code(int code);

#endif /* APIIO_H */
