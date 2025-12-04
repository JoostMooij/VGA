/*
 * Front_layer.h
 * Definities voor het verwerken van UART commando's
 */

#ifndef FRONT_LAYER_H
#define FRONT_LAYER_H

#include "UART.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

// ********Error codes**********
#define NO_ERROR               0
#define ERROR_X1               1
#define ERROR_Y1               2
#define ERROR_COLOR            3
#define ERROR_DIJKTE_TOO_SMALL 4
#define ERROR_BREEDTE          5
#define ERROR_HOOGTE           6
#define ERROR_GEVULD           7
#define ERROR_RADIUS_TOO_SMALL 8
#define ERROR_TEKST_TE_LANG    9
#define ERROR_BITMAP_NR        10
#define ERROR_TE_WEINIG_PARAMS 99
#define ERROR_ONBEKEND_CMD     100

#define MAX_CMD_LENGTH 128
#define MAX_INPUT 128

// De structuur bevat nu enkel de volledige, gevalideerde commandostring.
typedef struct {
    char full_command[MAX_CMD_LENGTH];
} UserInput_t;

// Functie prototype
void Handel_UART_Input(UserInput_t *in);

#ifdef __cplusplus
}
#endif

#endif // FRONT_LAYER_H
