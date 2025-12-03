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

static void FixCommas(char *buf)
{
    for(int i = 0; i < strlen(buf); i++)
        if(buf[i] == ',') buf[i] = ' ';
}

// ---------------- Handlers ------------------------------

// --- SETPIXEL ---
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

    UART2_WriteString("Voer 'X, Y, KLEUR' (bijv: 120, 40, ROOD): ");
    ReadLine(buffer);
    FixCommas(buffer);
    sscanf(buffer, "%d %d %15s", &in->x[0], &in->y[0], in->color_name);
}

// --- LIJN ---
// void lijn(int x, int y, int x2, int y2, int kleur, int dikte);
static void Handle_lijn(UserInput_t *in)
{
    char kleur_temp[16];
    int x1, y1, x2, y2, dikte;

    if(sscanf(buffer, "%*s %d %d %d %d %d %15s", &x1, &y1, &x2, &y2, &dikte, kleur_temp) == 6)
    {
        in->x[0] = x1; in->y[0] = y1;
        in->x[1] = x2; in->y[1] = y2;
        in->x[2] = dikte; // Opslaan in x[2]
        strcpy(in->color_name, kleur_temp);
        return;
    }

    UART2_WriteString("Voer 'X1, Y1, X2, Y2, DIKTE, KLEUR': ");
    ReadLine(buffer);
    FixCommas(buffer);
    sscanf(buffer, "%d %d %d %d %d %15s",
           &in->x[0], &in->y[0], &in->x[1], &in->y[1], &in->x[2], in->color_name);
}

// --- RECHTHOEK ---
// void rechthoek(int x_lup, int y_lup, int breedte, int hoogte, int kleur, int gevuld);
static void Handle_rechthoek(UserInput_t *in)
{
    char kleur_temp[16];
    int x, y, w, h, gevuld;

    if(sscanf(buffer, "%*s %d %d %d %d %d %15s", &x, &y, &w, &h, &gevuld, kleur_temp) == 6)
    {
        in->x[0] = x; in->y[0] = y;
        in->x[1] = w; in->y[1] = h; // Breedte/Hoogte in x[1]/y[1]
        in->x[2] = gevuld;          // Gevuld (0 of 1) in x[2]
        strcpy(in->color_name, kleur_temp);
        return;
    }

    UART2_WriteString("Voer 'X, Y, BREEDTE, HOOGTE, GEVULD(0/1), KLEUR': ");
    ReadLine(buffer);
    FixCommas(buffer);
    sscanf(buffer, "%d %d %d %d %d %15s",
           &in->x[0], &in->y[0], &in->x[1], &in->y[1], &in->x[2], in->color_name);
}

// --- TEKST ---
// void tekst(int x, int y, int kleur, const char* tekst...);
static void Handle_tekst(UserInput_t *in)
{
    char kleur_temp[16];
    char text_content[32];
    int x, y;

    // We lezen hier simpel: X, Y, KLEUR, TEKST (zonder spaties of enkele woord)
    if(sscanf(buffer, "%*s %d %d %15s %31s", &x, &y, kleur_temp, text_content) == 4)
    {
        in->x[0] = x; in->y[0] = y;
        strcpy(in->color_name, kleur_temp);
        // Let op: zorg dat UserInput_t een veld 'text_arg' heeft, anders opslaan in commando buffer
        strcpy(in->text_arg, text_content);
        return;
    }

    UART2_WriteString("Voer 'X, Y, KLEUR, TEKST': ");
    ReadLine(buffer);
    FixCommas(buffer);
    sscanf(buffer, "%d %d %15s %31s",
           &in->x[0], &in->y[0], in->color_name, in->text_arg);
}

// --- BITMAP ---
// void bitmap(int nr, int x_lup, int y_lup);
static void Handle_bitmap(UserInput_t *in)
{
    int nr, x, y;
    if(sscanf(buffer, "%*s %d %d %d", &nr, &x, &y) == 3)
    {
        in->x[0] = nr; // Nr in x[0]
        in->x[1] = x;  // X in x[1]
        in->y[1] = y;  // Y in y[1]
        return;
    }

    UART2_WriteString("Voer 'NR, X, Y': ");
    ReadLine(buffer);
    FixCommas(buffer);
    sscanf(buffer, "%d %d %d", &in->x[0], &in->x[1], &in->y[1]);
}

// --- CLEARSCHERM ---
// void clearscherm(int kleur);
static void Handle_clearscherm(UserInput_t *in)
{
    char kleur_temp[16];
    if(sscanf(buffer, "%*s %15s", kleur_temp) == 1)
    {
        strcpy(in->color_name, kleur_temp);
        return;
    }

    UART2_WriteString("Voer 'KLEUR': ");
    ReadLine(buffer);
    // Geen komma fix nodig voor 1 woord, maar voor consistentie:
    FixCommas(buffer);
    sscanf(buffer, "%15s", in->color_name);
}

// --- WACHT ---
// void wacht(int msecs);
static void Handle_wacht(UserInput_t *in)
{
    int ms;
    if(sscanf(buffer, "%*s %d", &ms) == 1)
    {
        in->x[0] = ms;
        return;
    }
    UART2_WriteString("Voer 'MILLISECONDEN': ");
    ReadLine(buffer);
    sscanf(buffer, "%d", &in->x[0]);
}

// --- HERHAAL ---
// void herhaal(int aantal, int hoevaak); (Interpretatie: loop parameters)
static void Handle_herhaal(UserInput_t *in)
{
    int a, b;
    if(sscanf(buffer, "%*s %d %d", &a, &b) == 2)
    {
        in->x[0] = a;
        in->x[1] = b;
        return;
    }
    UART2_WriteString("Voer 'AANTAL, HOEVAAK': ");
    ReadLine(buffer);
    FixCommas(buffer);
    sscanf(buffer, "%d %d", &in->x[0], &in->x[1]);
}

// --- CIRKEL ---
// void cirkel(int x, int y, int radius, int kleur);
static void Handle_cirkel(UserInput_t *in)
{
    char kleur_temp[16];
    int x, y, r;

    if(sscanf(buffer, "%*s %d %d %d %15s", &x, &y, &r, kleur_temp) == 4)
    {
        in->x[0] = x; in->y[0] = y;
        in->x[1] = r; // Radius in x[1]
        strcpy(in->color_name, kleur_temp);
        return;
    }

    UART2_WriteString("Voer 'X, Y, RADIUS, KLEUR': ");
    ReadLine(buffer);
    FixCommas(buffer);
    sscanf(buffer, "%d %d %d %15s",
           &in->x[0], &in->y[0], &in->x[1], in->color_name);
}

// --- FIGUUR ---
// void figuur(x1,y1, x2,y2, x3,y3, x4,y4, x5,y5, kleur);
static void Handle_figuur(UserInput_t *in)
{
    char k[16];
    int x[5], y[5];

    // Scan 5 coordinaten paren + kleur
    if(sscanf(buffer, "%*s %d %d %d %d %d %d %d %d %d %d %15s",
       &x[0], &y[0], &x[1], &y[1], &x[2], &y[2], &x[3], &y[3], &x[4], &y[4], k) == 11)
    {
        for(int i=0; i<5; i++) { in->x[i] = x[i]; in->y[i] = y[i]; }
        strcpy(in->color_name, k);
        return;
    }

    UART2_WriteString("Voer 'X1,Y1, X2,Y2, X3,Y3, X4,Y4, X5,Y5, KLEUR': ");
    ReadLine(buffer);
    FixCommas(buffer);
    sscanf(buffer, "%d %d %d %d %d %d %d %d %d %d %15s",
       &in->x[0], &in->y[0], &in->x[1], &in->y[1], &in->x[2], &in->y[2],
       &in->x[3], &in->y[3], &in->x[4], &in->y[4], in->color_name);
}

// --- TOREN ---
// void toren(int x, int y, int grootte, int kleur1, int kleur2);
static void Handle_toren(UserInput_t *in)
{
    char k1[16], k2[16];
    int x, y, gr;

    if(sscanf(buffer, "%*s %d %d %d %15s %15s", &x, &y, &gr, k1, k2) == 5)
    {
        in->x[0] = x; in->y[0] = y;
        in->x[1] = gr;
        strcpy(in->color_name, k1);
        // Tweede kleur slaan we op in text_arg (of maak een 2e kleur veld in struct)
        strcpy(in->text_arg, k2);
        return;
    }

    UART2_WriteString("Voer 'X, Y, GROOTTE, KLEUR1, KLEUR2': ");
    ReadLine(buffer);
    FixCommas(buffer);
    sscanf(buffer, "%d %d %d %15s %15s",
           &in->x[0], &in->y[0], &in->x[1], in->color_name, in->text_arg);
}


// ---------------- HELP Handler --------------------------
void Handle_HELP(UserInput_t *in)
{
    (void)in;

    UART2_WriteString("--- Beschikbare Commando's ---\r\n");
    // Korte woorden (< 8 letters) krijgen 2 tabs om bij 16 te komen
    // Lange woorden (> 8 letters) krijgen 1 tab om bij 16 te komen

    UART2_WriteString("setPixel\t\tX,Y,Kleur\r\n");               // 8 letters -> sprong naar 16
    UART2_WriteString("lijn\t\tX1,Y1,X2,Y2,Dikte,Kleur\r\n"); // 4 letters -> sprong naar 8 -> sprong naar 16
    UART2_WriteString("rechthoek\tX,Y,Br,Hg,Gevuld,Kleur\r\n"); // 9 letters -> sprong naar 16
    UART2_WriteString("tekst\t\tX,Y,Kleur,Tekst\r\n");          // 5 letters -> sprong naar 8 -> sprong naar 16
    UART2_WriteString("bitmap\t\tNr,X,Y\r\n");                    // 6 letters -> ...
    UART2_WriteString("clearscherm\tKleur\r\n");                    // 11 letters -> sprong naar 16
    UART2_WriteString("cirkel\t\tX,Y,Radius,Kleur\r\n");
    UART2_WriteString("figuur\t\tX1,Y1... X5,Y5,Kleur\r\n");
    UART2_WriteString("toren\t\tX,Y,Grootte,Kleur1,Kleur2\r\n");
    UART2_WriteString("wacht\t\tMsecs\r\n");
    UART2_WriteString("herhaal\t\tAantal,Hoevaak\r\n");
    UART2_WriteString("------------------------------\r\n");
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
    // OPLOSSING: Reset de HELE struct in één keer naar 0.
    // Dit zorgt dat command, x, y, color_name EN text_arg allemaal leeg zijn.
    memset(in, 0, sizeof(UserInput_t));

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
        UART2_WriteString("Onbekend commando. Typ HELP.\r\n> ");
    }
}
