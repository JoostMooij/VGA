/**
 * @file    Front_layer.h
 * @brief   Publieke definities voor het verwerken van UART-gebaseerde commando’s.
 *
 * Dit headerbestand bevat alle data­structuren, foutcodes, constante limieten
 * en functieprototypes die nodig zijn om de Front Layer te gebruiken.
 *
 * De Front Layer leest gebruikersinput via UART, valideert deze,
 * en zet deze om naar een uniform geformatteerde commandostring.
 * De uiteindelijke, gevalideerde commandodata wordt opgeslagen in `UserInput_t`
 * en doorgegeven aan de Logic Layer.
 *
 * @author  Luc van den Engel
 * @date    2025-12-08
 * @version 1.0
 */


#ifndef FRONT_LAYER_H
#define FRONT_LAYER_H

#include "UART.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @name Error codes
 * @brief Mogelijke foutcodes die gegenereerd kunnen worden tijdens commandoparsing.
 * @{
 */
#ifdef __cplusplus
extern "C" {
#endif

/** Geen fout opgetreden. */
#define NO_ERROR               0
/** X1-parameter ontbreekt of ongeldig. */
#define ERROR_X1               1
/** Y1-parameter ontbreekt of ongeldig. */
#define ERROR_Y1               2
/** Kleurnaam of waarde is ongeldig. */
#define ERROR_COLOR            3
/** Lijndikte is kleiner dan toegestaan. */
#define ERROR_DIJKTE_TOO_SMALL 4
/** Breedte-parameter is ongeldig. */
#define ERROR_BREEDTE          5
/** Hoogte-parameter is ongeldig. */
#define ERROR_HOOGTE           6
/** Gevuld-parameter is ongeldig. */
#define ERROR_GEVULD           7
/** Radius is te klein voor het commando. */
#define ERROR_RADIUS_TOO_SMALL 8
/** Tekst overschrijdt maximale lengte. */
#define ERROR_TEKST_TE_LANG    9
/** Ongeldig bitmapnummer. */
#define ERROR_BITMAP_NR        10
/** Te weinig parameters opgegeven voor dit commando. */
#define ERROR_TE_WEINIG_PARAMS 99
/** Commando bestaat niet of wordt niet herkend. */
#define ERROR_ONBEKEND_CMD     100

/**
 * @brief Maximale lengte van de volledige commandostring.
 */
#define MAX_CMD_LENGTH 128

/**
 * @brief Maximale lengte van de rauwe UART-invoerbuffer.
 */
#define MAX_INPUT 128

/**
 * @struct UserInput_t
 * @brief Opslagstructuur voor een volledig gevalideerde en geformatteerde commandostring.
 *
 * De Front Layer vult dit veld nadat een commando succesvol is geparsed en
 * opgebouwd. Andere lagen gebruiken deze string om het commando uit te voeren.
 *
 * @var UserInput_t::full_command
 * Volledige stringrepresentatie van het geparste commando.
 */
typedef struct {
    char full_command[MAX_CMD_LENGTH];
} UserInput_t;

/**
 * @brief Leest, valideert en verwerkt een UART-invoerlijn tot een volledig commando.
 *
 * Deze functie vormt het centrale invoerpunt van de Front Layer.
 * Het leest een regel tekst via UART, verwerkt parameters, valideert het commando
 * en vult `UserInput_t` met de volledige, uniforme commandostring.
 *
 * @param[out] in Struct waarin de volledige geformatteerde commandostring wordt opgeslagen.
 */
void Handel_UART_Input(UserInput_t *in);


#ifdef __cplusplus
}
#endif

#endif // FRONT_LAYER_H
