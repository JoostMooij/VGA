/*
 * Front_layer.h
 *
 * Created on: Nov 21, 2025
 * Author: Luc
 */

/*
 * Front_layer.h
 */

#ifndef FRONT_LAYER_H
#define FRONT_LAYER_H

#include <stdint.h>

#define MAX_CMD_LENGTH 128

/**
 * @brief Structuur om de input als volledige string op te slaan.
 * Bijvoorbeeld: "setPixel 10 20 RED"
 */
typedef struct {
    char full_command[MAX_CMD_LENGTH]; ///< De volledige string (commando + args)
} UserInput_t;


/**
 * @brief Leest UART invoer, valideert commando's en vult de struct met een complete string.
 * @param in Pointer naar de UserInput_t struct.
 */
void Handel_UART_Input(UserInput_t *in);

#endif /* FRONT_LAYER_H */
