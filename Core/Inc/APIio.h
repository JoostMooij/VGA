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

/**
 * @name VGA kleurmacro
 * @{
 */

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

/** @} */

/**
 * @name Herhalings- en bufferdefinities
 * @{
 */
#define MAX_COMMAND_HISTORY_SIZE 25
#define MAX_COMMAND_BUFFER_SIZE 245
#define MAX_TEKST_HISTORY 4
/** @} */

/**
 * @name Globale variabelen
 * @{
 */

/**
 * @brief Buffer waarin commando’s lineair worden opgeslagen.
 */
extern uint16_t command_buffer[MAX_COMMAND_BUFFER_SIZE];

/**
 * @brief Index van het huidige commando in de buffer.
 */
extern uint16_t command_buffer_index;

/**
 * @brief Milliseconde-teller, opgehoogd door SysTick interrupt.
 */
extern volatile uint32_t ms_tick_counter;

/**
 * @brief Huidige diepte van geneste herhaalcommando’s.
 */
extern int herhaal_hoog;

/** @} */

/**
 * @brief VGA kleuren met Nederlandse benamingen.
 */
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

/**
 * @name Functieprototypes
 * @{
 */

/**
 * @brief Initialiseert de I/O-laag.
 *
 * Initialiseert het VGA-scherm en bijbehorende hardware.
 */
void API_init_io(void);

/**
 * @brief Vult het volledige scherm met een kleur.
 *
 * @param kleur Nederlandse kleurnaam
 * @return ErrorList Foutcodestructuur
 */
ErrorList clearscherm(const char *kleur);

/**
 * @brief Zet één pixel met een kleur opgegeven als string.
 *
 * @param x X-coördinaat
 * @param y Y-coördinaat
 * @param kleur Nederlandse kleurnaam
 * @return ErrorList Foutcodestructuur
 */
ErrorList drawPixel(int x, int y, const char *kleur);

/**
 * @brief Zet één pixel met een numerieke VGA-kleurcode.
 *
 * @param x X-coördinaat
 * @param y Y-coördinaat
 * @param kleur VGA-kleurcode
 * @return ErrorList Foutcodestructuur
 */
ErrorList setPixel(int x, int y, int kleur);

/**
 * @brief Zet een kleurnaam om naar een VGA-kleurcode.
 *
 * @param input Nederlandse kleurnaam
 * @return uint8_t VGA-kleurcode
 */
uint8_t kleur_omzetter(const char *input);

/**
 * @brief Initialiseert de SysTick timer voor 1 ms interrupts.
 */
void SysTick_Init(void);

/**
 * @brief Blokkerende vertraging in milliseconden.
 *
 * @param ms Aantal milliseconden
 * @return ErrorList Foutcodestructuur
 */
ErrorList wacht(int ms);

/**
 * @brief Herhaalt eerder uitgevoerde commando’s.
 *
 * @param aantal Aantal commando’s
 * @param hoevaak Aantal herhalingen
 * @return ErrorList Foutcodestructuur
 */
ErrorList herhaal(int aantal, int hoevaak);

/**
 * @brief Slaat een commando en parameters op in de buffer.
 *
 * @param type Commando-type
 * @param param_count Aantal parameters
 * @param params Parameterlijst
 */
void record_command(COMMANDO_TYPE type, int param_count, const int params[]);

/**
 * @brief Bepaalt de buffer-grootte van een commando.
 *
 * @param type Commando-type
 * @return int Totale grootte
 */
int get_command_size(COMMANDO_TYPE type);

/**
 * @brief Zet een VGA-kleurcode om naar een kleurnaam.
 *
 * @param code VGA-kleurcode
 * @return const char* Kleurnaam
 */
const char* get_color_string_from_code(int code);

/**
 * @brief Reserveert tijdelijke opslag voor tekst.
 *
 * @param tekst Tekststring
 * @return int Index van tekstslot, of -1 bij fout
 */
int reserveer_tekst_slot(const char* tekst);

/** @} */

#endif /* APIIO_H */
