/*
 * front_layer.c
 * Schaalbaar commando-systeem - String Only versie
 * Stuurt commando's altijd door, ook als parameters ontbreken.
 */

#include "UART.h"
#include "Front_layer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_INPUT 128
static char buffer[MAX_INPUT];


// ---------------- Buffer Based Utilities ----------------
static void ReadLine(char *dst)
{
    memset(dst, 0, MAX_INPUT);
    UART2_ReadString(dst, MAX_INPUT);
}

static void FixCommas(char *buf)
{
    for(int i = 0; i < strlen(buf); i++)
        if(buf[i] == ',') buf[i] = ' ';
}

// ---------------- Handlers ------------------------------

// --- SETPIXEL ---
static void Handle_setPixel(UserInput_t *in)
{
    char k[20] = "";
    int x, y;

    // Poging 1: Lees X, Y, KLEUR (3 items)
    if(sscanf(buffer, "%*s %d %d %19s", &x, &y, k) == 3)
    {
        snprintf(in->full_command, MAX_CMD_LENGTH, "setPixel %d %d %s", x, y, k);
        return;
    }

    // Poging 2: Lees X, Y (2 items)
    if(sscanf(buffer, "%*s %d %d", &x, &y) == 2)
    {
        snprintf(in->full_command, MAX_CMD_LENGTH, "setPixel %d %d %s", x, y, k);
        return;
    }

    // Als de parsing faalt, stuur alleen de commandonaam.
    snprintf(in->full_command, MAX_CMD_LENGTH, "setPixel");
}

// --- LIJN ---
static void Handle_lijn(UserInput_t *in)
{
    char k[20] = "";
    int x1, y1, x2, y2, d = 0;

    // Poging 1: X1, Y1, X2, Y2, DIKTE, KLEUR (6 items)
    if(sscanf(buffer, "%*s %d %d %d %d %d %19s", &x1, &y1, &x2, &y2, &d, k) == 6)
    {
        snprintf(in->full_command, MAX_CMD_LENGTH, "lijn %d %d %d %d %d %s", x1, y1, x2, y2, d, k);
        return;
    }

    // Poging 2: X1, Y1, X2, Y2, DIKTE (5 items)
    if(sscanf(buffer, "%*s %d %d %d %d %d", &x1, &y1, &x2, &y2, &d) == 5)
    {
        snprintf(in->full_command, MAX_CMD_LENGTH, "lijn %d %d %d %d %d %s", x1, y1, x2, y2, d, k);
        return;
    }

    // Poging 3: X1, Y1, X2, Y2 (4 items)
    if(sscanf(buffer, "%*s %d %d %d %d", &x1, &y1, &x2, &y2) == 4)
    {
        snprintf(in->full_command, MAX_CMD_LENGTH, "lijn %d %d %d %d %d %s", x1, y1, x2, y2, d, k);
        return;
    }

    // Als de parsing faalt, stuur alleen de commandonaam.
    snprintf(in->full_command, MAX_CMD_LENGTH, "lijn");
}

// --- RECHTHOEK ---
static void Handle_rechthoek(UserInput_t *in)
{
    char k[20] = "";
    int x, y, w, h, gevuld = 0;

    // P1: X, Y, BR, HG, GEVULD, KLEUR (6 items)
    if(sscanf(buffer, "%*s %d %d %d %d %d %19s", &x, &y, &w, &h, &gevuld, k) == 6)
    {
        snprintf(in->full_command, MAX_CMD_LENGTH, "rechthoek %d %d %d %d %d %s", x, y, w, h, gevuld, k);
        return;
    }

    // P2: X, Y, BR, HG, GEVULD (5 items)
    if(sscanf(buffer, "%*s %d %d %d %d %d", &x, &y, &w, &h, &gevuld) == 5)
    {
        snprintf(in->full_command, MAX_CMD_LENGTH, "rechthoek %d %d %d %d %d %s", x, y, w, h, gevuld, k);
        return;
    }

    // P3: X, Y, BR, HG (4 items)
    if(sscanf(buffer, "%*s %d %d %d %d", &x, &y, &w, &h) == 4)
    {
        snprintf(in->full_command, MAX_CMD_LENGTH, "rechthoek %d %d %d %d %d %s", x, y, w, h, gevuld, k);
        return;
    }

    // Als de parsing faalt, stuur alleen de commandonaam.
    snprintf(in->full_command, MAX_CMD_LENGTH, "rechthoek");
}

// --- TEKST ---
static void Handle_tekst(UserInput_t *in)
{
    char k[20] = "", t[64] = "";
    int x, y;

    // P1: X, Y, KLEUR, TEKST (4 items)
    if(sscanf(buffer, "%*s %d %d %19s %63s", &x, &y, k, t) == 4)
    {
        snprintf(in->full_command, MAX_CMD_LENGTH, "tekst %d %d %s %s", x, y, k, t);
        return;
    }

    // P2: X, Y, KLEUR (3 items) - Tekst 't' is leeg
    if(sscanf(buffer, "%*s %d %d %19s", &x, &y, k) == 3)
    {
        snprintf(in->full_command, MAX_CMD_LENGTH, "tekst %d %d %s %s", x, y, k, t);
        return;
    }

    // P3: X, Y (2 items) - Kleur en Tekst zijn leeg
    if(sscanf(buffer, "%*s %d %d", &x, &y) == 2)
    {
        snprintf(in->full_command, MAX_CMD_LENGTH, "tekst %d %d %s %s", x, y, k, t);
        return;
    }

    // Als de parsing faalt, stuur alleen de commandonaam.
    snprintf(in->full_command, MAX_CMD_LENGTH, "tekst");
}

// --- BITMAP ---
static void Handle_bitmap(UserInput_t *in)
{
    int nr, x, y;
    if(sscanf(buffer, "%*s %d %d %d", &nr, &x, &y) == 3)
    {
        snprintf(in->full_command, MAX_CMD_LENGTH, "bitmap %d %d %d", nr, x, y);
        return;
    }
    // Als de parsing faalt, stuur alleen de commandonaam.
    snprintf(in->full_command, MAX_CMD_LENGTH, "bitmap");
}

// --- CLEARSCHERM ---
static void Handle_clearscherm(UserInput_t *in)
{
    char k[20] = "";

    // P1: KLEUR (1 item)
    if(sscanf(buffer, "%*s %19s", k) == 1)
    {
        snprintf(in->full_command, MAX_CMD_LENGTH, "clearscherm %s", k);
        return;
    }

    // P2: Geen kleur, stuur alleen de commandonaam.
    snprintf(in->full_command, MAX_CMD_LENGTH, "clearscherm %s", k);
}

// --- WACHT ---
static void Handle_wacht(UserInput_t *in)
{
    int ms;
    if(sscanf(buffer, "%*s %d", &ms) == 1)
    {
        snprintf(in->full_command, MAX_CMD_LENGTH, "wacht %d", ms);
        return;
    }
    // Als de parsing faalt, stuur alleen de commandonaam.
    snprintf(in->full_command, MAX_CMD_LENGTH, "wacht");
}

// --- HERHAAL ---
static void Handle_herhaal(UserInput_t *in)
{
    int a, h;
    if(sscanf(buffer, "%*s %d %d", &a, &h) == 2)
    {
        snprintf(in->full_command, MAX_CMD_LENGTH, "herhaal %d %d", a, h);
        return;
    }
    // Als de parsing faalt, stuur alleen de commandonaam.
    snprintf(in->full_command, MAX_CMD_LENGTH, "herhaal");
}

// --- CIRKEL ---
static void Handle_cirkel(UserInput_t *in)
{
    char k[20] = "";
    int x, y, r;

    // P1: X, Y, RADIUS, KLEUR (4 items)
    if(sscanf(buffer, "%*s %d %d %d %19s", &x, &y, &r, k) == 4)
    {
        snprintf(in->full_command, MAX_CMD_LENGTH, "cirkel %d %d %d %s", x, y, r, k);
        return;
    }

    // P2: X, Y, RADIUS (3 items)
    if(sscanf(buffer, "%*s %d %d %d", &x, &y, &r) == 3)
    {
        snprintf(in->full_command, MAX_CMD_LENGTH, "cirkel %d %d %d %s", x, y, r, k);
        return;
    }
    // Als de parsing faalt, stuur alleen de commandonaam.
    snprintf(in->full_command, MAX_CMD_LENGTH, "cirkel");
}

// --- FIGUUR ---
static void Handle_figuur(UserInput_t *in)
{
    char k[20];
    int x1,y1, x2,y2, x3,y3, x4,y4, x5,y5;

    // Scan op 11 items (10 coordinaten + 1 kleur)
    if(sscanf(buffer, "%*s %d %d %d %d %d %d %d %d %d %d %19s",
              &x1,&y1, &x2,&y2, &x3,&y3, &x4,&y4, &x5,&y5, k) == 11)
    {
        snprintf(in->full_command, MAX_CMD_LENGTH,
                 "figuur %d %d %d %d %d %d %d %d %d %d %s", x1,y1, x2,y2, x3,y3, x4,y4, x5,y5, k);
        return;
    }
    // Als de parsing faalt, stuur alleen de commandonaam.
    snprintf(in->full_command, MAX_CMD_LENGTH, "figuur");
}

// --- TOREN ---
static void Handle_toren(UserInput_t *in)
{
    char k1[20] = "", k2[20] = "";
    int x, y, g;

    // P1: X, Y, GROOTTE, KLEUR1, KLEUR2 (5 items)
    if(sscanf(buffer, "%*s %d %d %d %19s %19s", &x, &y, &g, k1, k2) == 5)
    {
        snprintf(in->full_command, MAX_CMD_LENGTH, "toren %d %d %d %s %s", x, y, g, k1, k2);
        return;
    }

    // P2: X, Y, GROOTTE, KLEUR1 (4 items) - Kleur2 is leeg
    if(sscanf(buffer, "%*s %d %d %d %19s", &x, &y, &g, k1) == 4)
    {
        snprintf(in->full_command, MAX_CMD_LENGTH, "toren %d %d %d %s %s", x, y, g, k1, k2);
        return;
    }

    // P3: X, Y, GROOTTE (3 items) - Kleur1 en Kleur2 zijn leeg
    if(sscanf(buffer, "%*s %d %d %d", &x, &y, &g) == 3)
    {
        snprintf(in->full_command, MAX_CMD_LENGTH, "toren %d %d %d %s %s", x, y, g, k1, k2);
        return;
    }
    // Als de parsing faalt, stuur alleen de commandonaam.
    snprintf(in->full_command, MAX_CMD_LENGTH, "toren");
}


// ---------------- HELP Handler --------------------------
void Handle_HELP(UserInput_t *in)
{
    (void)in;

    UART2_WriteString("--- Beschikbare Commando's ---\r\n");
    UART2_WriteString("setPixel\t\tX,Y[,Kleur]\r\n");
    UART2_WriteString("lijn\t\tX1,Y1,X2,Y2[,Dikte][,Kleur]\r\n");
    UART2_WriteString("rechthoek\tX,Y,Br,Hg[,Gevuld][,Kleur]\r\n");
    UART2_WriteString("tekst\t\tX,Y[,Kleur][,Tekst]\r\n");
    UART2_WriteString("bitmap\t\tNr,X,Y\r\n");
    UART2_WriteString("clearscherm\t[Kleur]\r\n");
    UART2_WriteString("cirkel\t\tX,Y,Radius[,Kleur]\r\n");
    UART2_WriteString("figuur\t\tX1,Y1... X5,Y5,Kleur\r\n");
    UART2_WriteString("toren\t\tX,Y,Grootte[,Kleur1][,Kleur2]\r\n");
    UART2_WriteString("wacht\t\tMsecs\r\n");
    UART2_WriteString("herhaal\t\tAantal,Hoevaak\r\n");
    UART2_WriteString("------------------------------\r\n");
    UART2_WriteString("> ");
}

// --------------------------------------------------------
// TABLE: Dispatcher Table
// --------------------------------------------------------
typedef void (*CommandHandler)(UserInput_t *);

typedef struct {
    const char *name;
    CommandHandler handler;
} CommandEntry;

static const CommandEntry command_table[] =
{
    { "setPixel",    Handle_setPixel },
    { "lijn",        Handle_lijn },
    { "rechthoek",   Handle_rechthoek },
    { "tekst",       Handle_tekst },
    { "bitmap",      Handle_bitmap },
    { "clearscherm", Handle_clearscherm },
    { "wacht",       Handle_wacht },
    { "herhaal",     Handle_herhaal },
    { "cirkel",      Handle_cirkel },
    { "figuur",      Handle_figuur },
    { "toren",       Handle_toren },
    { "help",        Handle_HELP },
    { "HELP",        Handle_HELP },
    { "?",           Handle_HELP },
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
    // Maak de struct leeg
    memset(in, 0, sizeof(UserInput_t));

    // Lees input
    ReadLine(buffer);
    UART2_WriteString("\r\n");

    // FIX: Als de input leeg is (alleen enter), doe niks en print nieuwe prompt
    if(strlen(buffer) == 0)
    {
        UART2_WriteString("> ");
        return;
    }

    // Komma fix: setPixel,200,200,red wordt setPixel 200 200 red
    FixCommas(buffer);

    // Lees enkel het eerste woord om te kijken welke handler nodig is
    char command_only[32];
    sscanf(buffer, "%31s", command_only);

    if(!DispatchCommand(command_only, in))
    {
        // Onbekend commando
        UART2_WriteString("Onbekend commando. Typ HELP.\r\n> ");
    }
    // GEEN extra foutafhandeling hier! De commandostring in 'in->full_command' wordt
    // nu altijd doorgegeven aan de volgende laag als het commando bekend is.
}
