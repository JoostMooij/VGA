/*
 * Front_layer.h
 *
 *  Created on: Nov 21, 2025
 *      Author: Luc
 */

#ifndef INC_FRONT_LAYER_H_
#define INC_FRONT_LAYER_H_

#define RX_BUF_SIZE 64

typedef struct {
    char command[RX_BUF_SIZE];
    int x;
    int y;
    uint8_t color;
} UserInput_t;

void Gebruiker_instructies(void);
int Handel_UART_Input(UserInput_t *input);
int variable_inputs(UserInput_t *input);
int XY_kleur_opvragen(UserInput_t *input);
uint8_t GetColorCode(const char* color_str);

#endif /* INC_FRONT_LAYER_H_ */


