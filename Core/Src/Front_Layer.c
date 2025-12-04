/*
 * front_layer.c
 */

#include "UART.h"
#include "Front_layer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_INPUT 64
static char buffer[MAX_INPUT];


// ---------------- Buffer Utilities ----------------
static void ReadLine(char *dst)
{
    memset(dst, 0, MAX_INPUT);
    UART2_ReadString(dst, MAX_INPUT);
}

// ---------------- Pixel Handler -------------------------
// Binnen frontlayer.c

// ---------------- Pixel Handler -------------------------
static void Handle_setPixel(UserInput_t *in)
{
    // De globale 'buffer' bevat de bewerkte input (met spaties i.p.v. komma's)

    // Check of de gebruiker alles in één keer heeft getypt (d.w.z. 'buffer' is langer dan alleen "setPixel")
    if(strlen(buffer) > 8)
    {
        // Input was compleet. Doe niets.
        // Handel_UART_Input zal de ruwe string (met komma's) overnemen,
        // wat prima is als de gebruiker zelf de komma na setPixel heeft getypt.
        return;
    }

    // Input was NIET compleet (de gebruiker typte enkel 'setPixel'), start interactieve modus.
    UART2_WriteString("Voer 'X,Y,KLEUR' (bijv: 120,40,BLUE):");

    char param_buffer[MAX_INPUT];
    ReadLine(param_buffer); // Leest de parameters INCLUSIEF komma's
    UART2_WriteString("\r\n");

    // BELANGRIJK: Construeer de volledige string:
    // "setPixel" + ", " + "200, 50, RED"  -> "setPixel, 200, 50, RED"
    snprintf(in->full_command, MAX_CMD_LENGTH, "setPixel, %s", param_buffer);
}


// ---------------- HELP Handler --------------------------
void Handle_HELP(UserInput_t *in)
{
    // Bij HELP zetten we gewoon het commando "HELP" in de struct
    strcpy(in->full_command, "HELP");

    UART2_WriteString("--- Instructies ---\r\n");
    UART2_WriteString("Commando opties:\r\n");
    UART2_WriteString("setPixel  -> 'X, Y, KLEUR'\r\n");
    UART2_WriteString("HELP   -> laat instructies zien\r\n");
    UART2_WriteString("> ");
}


// ---------------- Dispatcher Tabel ----------------------
typedef void (*CommandHandler)(UserInput_t *);

typedef struct {
    const char *name;
    CommandHandler handler;
} CommandEntry;

static const CommandEntry command_table[] =
{
    { "setPixel", Handle_setPixel },
    { "HELP",     Handle_HELP  },
};

static const int command_count = sizeof(command_table) / sizeof(CommandEntry);


// ---------------- Main Input Function ---------------------
// ---------------- Main Input Function ---------------------
void Handel_UART_Input(UserInput_t *in)
{
    // Reset de struct
    memset(in->full_command, 0, sizeof(in->full_command));

    // Lees nieuwe regel
    char raw_input_buffer[MAX_INPUT]; // <-- NIEUW: Buffer om de ruwe input op te slaan
    ReadLine(raw_input_buffer);       // Lees in de ruwe buffer
    UART2_WriteString("\r\n");

    if(strlen(raw_input_buffer) == 0)
    {
        Handle_HELP(in);
        return;
    }

    // Kopieer de ruwe input naar de buffer die we gaan bewerken voor dispatching
    strncpy(buffer, raw_input_buffer, MAX_INPUT);
    buffer[MAX_INPUT - 1] = '\0';

    // Oude Komma-fix (Blijft om commando-parsing te garanderen)
    // De 'buffer' wordt nu gebruikt voor de dispatching,
    // terwijl 'raw_input_buffer' de onbewerkte string behoudt.
    for(int i = 0; i < strlen(buffer); i++)
        if(buffer[i] == ',') buffer[i] = ' '; // Verander komma's in spaties in de bewerkte buffer

    // Command uitlezen uit de BEWERKTE buffer
    char cmd_temp[12];
    sscanf(buffer, "%11s", cmd_temp);

    // Dispatcher proberen
    int found = 0;
    for(int i = 0; i < command_count; i++)
    {
        if(strcmp(cmd_temp, command_table[i].name) == 0)
        {
            // De handler (Handle_setPixel, etc.) krijgt nu de taak om de string te vullen.
            command_table[i].handler(in);

            // Als de handler niet interactief was (d.w.z. de input was compleet)
            // Kopiëren we de onbewerkte string direct naar de output struct.
            if(strcmp(in->full_command, "") == 0) { // Check als de handler de string nog niet vulde
                strncpy(in->full_command, raw_input_buffer, MAX_CMD_LENGTH - 1);
            }

            found = 1;
            break;
        }
    }

    if(!found)
    {
        UART2_WriteString("Onbekend commando.\r\n> ");
        strcpy(in->full_command, "UNKNOWN");
    }
}
