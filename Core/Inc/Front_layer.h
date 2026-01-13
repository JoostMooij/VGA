/**
 * @file Front_layer.h
 * @brief Declaraties voor front layer handlers voor UART-commandoverwerking
 * @details
 * Bevat structuren en functieprototypes voor het lezen en verwerken van
 * UART-commando's, inclusief validatie en routering naar de juiste handler.
 * Volledige commando's worden opgeslagen in een UserInput_t struct.
 * @author Luc, Joost, Thijs
 * @version 1.1
 * @date 13-01-2026
 *
 */

#ifndef FRONT_LAYER_H
#define FRONT_LAYER_H

#include <stdint.h>

#define MAX_CMD_LENGTH 128  ///< Maximale lengte van een commando string

/**
 * @brief Structuur om volledige UART-input op te slaan
 * @details Voorbeeld: "setPixel 10 20 RED"
 */
typedef struct
{
    char full_command[MAX_CMD_LENGTH]; ///< Volledige string (commando + argumenten)
} UserInput_t;

/**
 * @brief Dispatcher voor UART-invoer
 * @details Leest en valideert commando's en stuurt deze door naar de juiste handler.
 * @param in Pointer naar de UserInput_t struct met de volledige input string
 */
void Handel_UART_Input(UserInput_t *in);

#endif /* FRONT_LAYER_H */
