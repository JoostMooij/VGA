/*
 * Front_Layer.c
 *
 *  Created on: Nov 21, 2025
 *      Author: Luc
 */
#include "UART.h"
#include "Front_layer.h"
#include "stm32_ub_vga_screen.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void Gebruiker_instructies(void)
{
	UART2_WriteString("\n");
	UART2_WriteString("Hallo Termite!\r\n");
	UART2_WriteString("Typ 'help' voor alle commandos.\r\n");
}

int Handel_UART_Input(UserInput_t *input)
{
    char rx_buf[RX_BUF_SIZE];
    UART2_ReadString(rx_buf, RX_BUF_SIZE);

    // Reset velden elke keer nieuw commando binnenkomt
    memset(input, 0, sizeof(UserInput_t));

    strncpy(input->command, rx_buf, RX_BUF_SIZE - 1);
    input->command[RX_BUF_SIZE - 1] = 0;

    if (strcmp(rx_buf, "ping") == 0)
    {
        UART2_WriteString("pong!\r\n");
    }
    else if (strcmp(rx_buf, "status") == 0)
    {
        UART2_WriteString("System OK!\r\n");
    }
    else if (strcmp(rx_buf, "help") == 0)
    {
        UART2_WriteString("Commands: ping, status, help, Set_pixel\r\n");
    }
    else if (strcmp(rx_buf, "Set_pixel") == 0)
    {
        UART2_WriteString("Het commando Set_pixel wordt uitgevoerd\r\n");
        variable_inputs(input);

        // Hier kun je dan wel de velden printen of doorgeven
        char buffer[128];
        sprintf(buffer, "Cmd: %s, X: %d, Y: %d, Color: 0x%02X\r\n",
                input->command, input->x, input->y, input->color);
        UART2_WriteString(buffer);
    }
    else if (strcmp(rx_buf, "") == 0)
    {
        UART2_WriteString("Stuur een commando:\r\n");
    }
    else
    {
        UART2_WriteString("Onbekend commando: ");
        UART2_WriteString(rx_buf);
        UART2_WriteString("\r\n");
    }
}

uint8_t GetColorCode(const char* color_str)
{
    if (strcmp(color_str, "black") == 0)   return VGA_COL_BLACK;
    if (strcmp(color_str, "blue") == 0)    return VGA_COL_BLUE;
    if (strcmp(color_str, "green") == 0)   return VGA_COL_GREEN;
    if (strcmp(color_str, "red") == 0)     return VGA_COL_RED;
    if (strcmp(color_str, "white") == 0)   return VGA_COL_WHITE;
    if (strcmp(color_str, "cyan") == 0)    return VGA_COL_CYAN;
    if (strcmp(color_str, "magenta") == 0) return VGA_COL_MAGENTA;
    if (strcmp(color_str, "yellow") == 0)  return VGA_COL_YELLOW;

    return VGA_COL_BLACK; // standaard fallback
}

int variable_inputs(UserInput_t *input)
{
    if (strcmp(input->command, "Set_pixel") != 0)
    {
        UART2_WriteString("Onbekend commando!\r\n");
        return -1;
    }

    XY_kleur_opvragen(input);

    UART2_WriteString("Ingevoerde waarden:\r\n");

    UART2_WriteString("Cmd: ");
    UART2_WriteString(input->command);
    UART2_WriteString("\r\nX: ");

    char buffer[12];
    sprintf(buffer, "%d", input->x);
    UART2_WriteString(buffer);
    UART2_WriteString("\r\nY: ");

    sprintf(buffer, "%d", input->y);
    UART2_WriteString(buffer);
    UART2_WriteString("\r\nKleur (hex): ");

    sprintf(buffer, "0x%02X", input->color);
    UART2_WriteString(buffer);
    UART2_WriteString("\r\n");

    return 0;
}

int XY_kleur_opvragen(UserInput_t *input)
{
    int step = 1;
    char temp_buf[RX_BUF_SIZE];

    while(step <= 3)
    {
        switch(step)
        {
            case 1:
                UART2_WriteString("Geef X-coordinaat (0-319) of 'back' om terug te gaan: ");
                UART2_ReadString(temp_buf, RX_BUF_SIZE);
                if (strcmp(temp_buf, "back") == 0)
                {
                    UART2_WriteString("Geen vorige stap, blijf bij X.\r\n");
                    break;
                }
                int x_val = atoi(temp_buf);
                if (x_val < 0 || x_val >= VGA_DISPLAY_X)
                {
                    UART2_WriteString("Ongeldige X waarde!\r\n");
                    break;
                }
                input->x = x_val;
                step++;
                break;

            case 2:
                UART2_WriteString("Geef Y-coordinaat (0-239) of 'back' om terug te gaan: ");
                UART2_ReadString(temp_buf, RX_BUF_SIZE);
                if (strcmp(temp_buf, "back") == 0)
                {
                    step--;
                    break;
                }
                int y_val = atoi(temp_buf);
                if (y_val < 0 || y_val >= VGA_DISPLAY_Y)
                {
                    UART2_WriteString("Ongeldige Y waarde!\r\n");
                    break;
                }
                input->y = y_val;
                step++;
                break;

            case 3:
                UART2_WriteString("Geef kleur (black, blue, green, red, white, cyan, magenta, yellow) of 'back' om terug te gaan: ");
                UART2_ReadString(temp_buf, RX_BUF_SIZE);
                if (strcmp(temp_buf, "back") == 0)
                {
                    step--;
                    break;
                }
                uint8_t kleur_code = GetColorCode(temp_buf);
                if (kleur_code == VGA_COL_BLACK && strcmp(temp_buf, "black") != 0)
                {
                    UART2_WriteString("Ongeldige kleur!\r\n");
                    break;
                }
                input->color = kleur_code;
                step++;
                break;
        }
    }
    return 0;
}


