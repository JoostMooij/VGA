/*
 * Front_layer.h
 * Definities voor het verwerken van UART commando's
 */

#ifndef FRONT_LAYER_H
#define FRONT_LAYER_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_CMD_LENGTH 128

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
