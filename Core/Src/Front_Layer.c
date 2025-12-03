/*
 * front_layer.c
 * Schaalbaar commando-systeem
 */

#include "UART.h"
#include "Front_layer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_INPUT 64
static char buffer[MAX_INPUT];


// ---------------- Buffer Based Utilities ----------------
static void ReadLine(char *dst)
{
    memset(dst, 0, MAX_INPUT);
    UART2_ReadString(dst, MAX_INPUT);
}

// ---------------- Pixel Handler -------------------------
static void Handle_setPixel(UserInput_t *in)
{
    char kleur_temp[16];
    int x_temp, y_temp;

    // Probeer alles op dezelfde regel
    if(sscanf(buffer, "%*s %d %d %15s", &x_temp, &y_temp, kleur_temp) == 3)
    {
        in->x[0] = x_temp;
        in->y[0] = y_temp;
        strcpy(in->color_name, kleur_temp);
        return;
    }

    // Anders nieuw verzoek
    UART2_WriteString("Voer 'X, Y, KLEUR' (bijv: 120, 40, BLUE): ");
    ReadLine(buffer);
    UART2_WriteString("\r\n");

    for(int i = 0; i < strlen(buffer); i++)
        if(buffer[i] == ',') buffer[i] = ' ';

    sscanf(buffer, "%d %d %15s",
           &in->x[0], &in->y[0], in->color_name);
}


// ---------------- HELP Handler --------------------------
void Handle_HELP(UserInput_t *in)
{
    (void)in; // unused

    UART2_WriteString("--- Instructies ---\r\n");
    UART2_WriteString("Commando opties:\r\n");
    UART2_WriteString("setPixel  -> 'X, Y, KLEUR'\r\n");
    UART2_WriteString("HELP   -> laat instructies zien\r\n");
    UART2_WriteString("> ");
}


// --------------------------------------------------------
// TABLE: lijst van commands + bijbehorende handler functie
// --------------------------------------------------------
typedef void (*CommandHandler)(UserInput_t *);

typedef struct {
    const char *name;
    CommandHandler handler;
} CommandEntry;

static const CommandEntry command_table[] =
{
    { "setPixel", Handle_setPixel },
    { "HELP",  Handle_HELP  },
};

static const int command_count = sizeof(command_table) / sizeof(CommandEntry);


// ---------------- Dispatcher -----------------------------
static int DispatchCommand(const char *cmd, UserInput_t *in)
{
    for(int i = 0; i < command_count; i++)
    {
        if(strcmp(cmd, command_table[i].name) == 0)
        {
            command_table[i].handler(in);
            return 1; // success
        }
    }
    return 0; // not found
}


// ---------------- Main Input Function ---------------------
void Handel_UART_Input(UserInput_t *in)
{
    // Reset UserInput_t
    memset(in->command, 0, sizeof(in->command));
    memset(in->color_name, 0, sizeof(in->color_name));

    for(int i = 0; i < 16; i++)
    {
        in->x[i] = 0;
        in->y[i] = 0;
    }

    // Lees nieuwe regel
    ReadLine(buffer);
    UART2_WriteString("\r\n");

    if(strlen(buffer) == 0)
    {
        Handle_HELP(in);
        return;
    }

    // Komma-fix
    for(int i = 0; i < strlen(buffer); i++)
        if(buffer[i] == ',') buffer[i] = ' ';

    // Command uitlezen
    sscanf(buffer, "%11s", in->command);

    // Dispatcher proberen
    if(!DispatchCommand(in->command, in))
    {
        UART2_WriteString("Onbekend commando.\r\n> ");
    }
}
