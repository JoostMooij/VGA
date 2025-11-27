/*
 * Front_layer.h
 *
 * Created on: Nov 21, 2025
 * Author: Luc
 */

#ifndef FRONT_LAYER_H
#define FRONT_LAYER_H

#include <stdint.h>

/**
 * @brief Structuur om de geparste invoer van de gebruiker op te slaan.
 * * Dit omvat het commando en de bijbehorende parameters zoals
 * coördinaten en kleurnaam.
 */
typedef struct {
    char command[12];     ///< Het ingevoerde commando (max. 11 karakters + NULL)
    int x[16];            ///< X-coördinaten array
    int y[16];            ///< Y-coördinaten array
    char color_name[16];  ///< Naam van de kleur (max. 15 karakters + NULL)
} UserInput_t;


/**
 * @brief Hoofdfunctie voor het afhandelen van UART-invoer.
 * * Leest een regel van de UART, parseert het commando, en roept
 * de juiste commandohandler aan. De geparste gegevens worden opgeslagen in 'in'.
 * * @param in Pointer naar een UserInput_t structuur waarin de geparste
 * gebruikersinvoer wordt opgeslagen.
 */
void Handel_UART_Input(UserInput_t *in);

#endif /* FRONT_LAYER_H */
