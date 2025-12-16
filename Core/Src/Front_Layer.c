/*
 * front_layer.c
 *
 * Aangepast voor interrupt-driven UART.
 */

#include "UART.h"
#include "Front_layer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_INPUT 64

// ---------------- HELP Handler --------------------------
static void Handle_HELP(void)
{
    UART2_WriteString("\r\n--- Instructies ---\r\n");
    UART2_WriteString("Commando's:\r\n");
    UART2_WriteString("setPixel,x,y,kleur   (bv: setPixel,20,20,rood)\r\n");
    UART2_WriteString("HELP                 laat dit menu zien\r\n");
}

// ---------------- SETPIXEL Handler ----------------------
static void Handle_setPixel(const char *cmd)
{
    int x, y;
    char kleur[16];

    // Verwacht exact: setPixel,20,20,rood
    int parsed = sscanf(cmd,
                         "setPixel,%d,%d,%15s",
                         &x, &y, kleur);

    if (parsed != 3)
    {
        UART2_WriteString("\r\nGebruik: setPixel,20,20,rood\r\n");
        return;
    }

    // Debug output
    char msg[64];
    snprintf(msg, sizeof(msg),
             "\r\nPixel -> x=%d y=%d kleur=%s\r\n",
             x, y, kleur);
    UART2_WriteString(msg);

    // TODO: stuur door naar je pixel API
    // API_setPixel(x, y, kleur);

    // Als je string_ophalen() wilt gebruiken:
    string_ophalen(cmd);  // stuurt de volledige originele string door
}

// ---------------- MAIN DISPATCH FUNCTION ----------------
void Handel_UART_Input(UserInput_t *in)
{
    if (strlen(in->full_command) == 0)
        return;

    // strip newline/carriage return
    char *p = in->full_command;
    while(*p)
    {
        if(*p == '\r' || *p == '\n') *p = 0;
        p++;
    }

    // HELP command
    if (strncmp(in->full_command, "HELP", 4) == 0)
    {
        Handle_HELP();
    }
    // setPixel command gevolgd door parameters
    else if (strncmp(in->full_command, "setPixel", 8) == 0)
    {
        Handle_setPixel(in->full_command);
    }
    else
    {
        UART2_WriteString("\r\nOnbekend commando\r\n");
    }
}
