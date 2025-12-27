/**
 * @file logicLayer.h
 * @brief .h file behorend tot logicLayer.c
 *
 *
 * @author Joost Mooij
 * @date 04/12/2025
 */

#ifndef LOGIC_LAYER_H
#define LOGIC_LAYER_H

#define MAX_INPUT 128
#define MAX_ERROR_MESSAGE 128
#define MAX_WOORD 64

#include "APIerror.h"
#include "APIdraw.h"
#include "APIio.h"
#include "UART.h"

/**
 * @brief Alle mogelijke commando’s die de logica kan verwerken.
 */
typedef enum
{
    CMD_ONBEKEND = 0,   /**< Onbekend commando */
    CMD_LIJN,           /**< Lijn tekenen */
    CMD_RECHTHOEK,      /**< Rechthoek tekenen */
    CMD_TEKST,          /**< Tekst plaatsen */
    CMD_BITMAP,         /**< Bitmap tonen */
    CMD_CLEAR,          /**< Scherm wissen */
    CMD_WACHT,          /**< Wachtopdracht */
    CMD_HERHAAL,        /**< Herhaalblok */
    CMD_CIRKEL,         /**< Cirkel tekenen */
    CMD_FIGUUR,         /**< Complex figuur tekenen */
    CMD_TOREN,          /**< Torenopdracht */
    CMD_SETPIXEL        /**< Eén pixel zetten */
} COMMANDO_TYPE;

/**
 * @brief Leest een inputregel in, verwijdert newline en verwerkt het commando.
 *
 * @param input Buffer met de binnengekomen tekst.
 * @return 0 bij succes, -1 bij fout.
 */
int string_ophalen(char input[]);

/**
 * @brief Haalt het eerste woord uit de input tot aan een komma.
 *
 * @param input De volledige invoerregel.
 * @param woord Outputbuffer voor het gevonden woord.
 */
void haal_commando(const char *input, char *woord);

/**
 * @brief Bepaalt het commando op basis van het eerste woord.
 *
 * @param woord Het woord dat het commando beschrijft.
 * @return Het bijbehorende COMMANDO_TYPE.
 */
COMMANDO_TYPE bepaal_commando(const char *woord);

/**
 * @brief Verwerkt een volledig commando en voert de juiste opdracht uit.
 *
 * @param input De volledige invoerregel.
 */
void verwerk_commando(const char *input);

/**
 * @brief Zet een commando om naar een leesbare string.
 *
 * @param cmd Het commando.
 * @return De bijbehorende tekst.
 */
const char* commando_naar_string(COMMANDO_TYPE cmd);

/**
 * @brief Stuurt een foutcode terug via UART.
 *
 * @param foutcode De foutcode.
 */
void verstuur_error(int foutcode);

/**
 * @brief Geeft een foutmeldingstring terug op basis van een foutcode.
 *
 * @param code De foutcode.
 * @return De foutomschrijving.
 */
const char* errorCodeToString(int code);

/**
 * @brief Zet alle foutvelden in een ErrorList om naar één string en verstuurt die.
 *
 * @param errors Struct met alle foutvariabelen.
 */
void errorterugsendfunctie(const ErrorList* errors);

/**
 * @brief Externe functie die tekst doorstuurt naar de VGA-laag.
 *
 * @param str De tekst die getoond moet worden.
 */
void string_naar_vga(const char *str);

#endif

