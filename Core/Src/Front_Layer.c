/**
 * @file    Front_layer.c
 * @brief   Implementatie van de Front Layer voor commandoparsing en inputafhandeling.
 *
 * Deze module leest ruwe UART-invoer, verwerkt parameters, voert commandoherkenning uit
 * en bouwt een volledige commandostring op die wordt doorgegeven aan de Logic Layer.
 * Ondersteunt o.a. setPixel, lijn, rechthoek, bitmap, tekst, cirkel, toren, herhaal
 * en aanvullende diagnostische commando’s zoals HELP.
 *
 * Functionaliteit:
 * - UART-regel lezen en normaliseren (komma → spatie).
 * - Extractie van commandonaam en parameters.
 * - Dispatcher-tabel voor het selecteren van de juiste handler.
 * - Opbouw van een uniforme commandostring in `UserInput_t`.
 * - Afhandeling van foutcodes en genereren van bijbehorende meldingen.
 *
 * @author  Luc van den Engel
 * @date    2025-12-08
 * @version 1.0
 */

#include "Front_layer.h"

typedef void (*CommandHandler)(UserInput_t *);

typedef struct {
    const char *name;
    CommandHandler handler;
} CommandEntry;

static char buffer[MAX_INPUT];

// ---------------- Buffer Based Utilities ----------------
/**
 * @brief Leest een volledige regel tekst via UART2 in een buffer.
 *
 * Initialiseert de buffer met nullen en leest vervolgens maximaal MAX_INPUT
 * tekens via UART2_ReadString().
 *
 * @param[out] dst Doelbuffer waarin de ingelezen tekst wordt geplaatst.
 */
static void ReadLine(char *dst)
{
    memset(dst, 0, MAX_INPUT);
    UART2_ReadString(dst, MAX_INPUT);
}


/**
 * @brief Vervangt alle komma's in de opgegeven buffer door spaties.
 *
 * Dit maakt het mogelijk om commando's met komma-gescheiden parameters
 * eenvoudig te parsen met sscanf().
 *
 * @param[in,out] buf Buffer waarvan de komma's gewijzigd worden.
 */
static void FixCommas(char *buf)
{
    for(int i = 0; i < strlen(buf); i++)
        if(buf[i] == ',') buf[i] = ' ';
}

// ---------------- Handlers ------------------------------
/**
 * @brief Verwerkt het setPixel-commando en zet de volledige commandostring in de UserInput-struct.
 *
 * Probeert parameters X, Y en optioneel kleur uit de globale buffer te lezen.
 * Als de parsing mislukt wordt alleen "setPixel" ingevuld.
 *
 * @param[out] in Struct waarin de uiteindelijke commandostring wordt opgeslagen.
 */
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

/**
 * @brief Verwerkt het lijn-commando en reconstrueert de volledige commandostring.
 *
 * Ondersteunt meerdere parameterformaten: met/zonder dikte en kleur.
 * Bij mislukte parsing wordt alleen het commando "lijn" opgeslagen.
 *
 * @param[out] in UserInput-struct voor de uitvoer.
 */
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

/**
 * @brief Verwerkt het rechthoek-commando en vult de volledige commandostring.
 *
 * Ondersteunt varianten met breedte, hoogte, gevuld-flag en kleur.
 * Fallback is alleen het woord "rechthoek".
 *
 * @param[out] in Struct waarin de commandostring wordt geplaatst.
 */
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

/**
 * @brief Verwerkt het tekst-commando en bouwt de volledige commandostring op.
 *
 * Ondersteunt: X, Y, kleur en tekst. Niet opgegeven velden blijven leeg.
 * Bij parse-fout wordt alleen "tekst" gebruikt.
 *
 * @param[out] in Doelstruct voor de geformatteerde uitvoerstring.
 */
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

/**
 * @brief Verwerkt het bitmap-commando (nummer, X, Y).
 *
 * Maakt een complete commandostring of gebruikt alleen "bitmap" als parsing mislukt.
 *
 * @param[out] in Struct waarin de uitvoerstring wordt opgeslagen.
 */
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

/**
 * @brief Verwerkt het clearscherm-commando en zet optioneel de kleur.
 *
 * Als er geen kleur wordt opgegeven wordt een lege kleurstring meegenomen.
 *
 * @param[out] in Struct waarin het geformatteerde commando wordt geplaatst.
 */
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

/**
 * @brief Verwerkt het wacht-commando dat een aantal milliseconden bevat.
 *
 * @param[out] in Struct waarin het uiteindelijke commando wordt geschreven.
 */
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

/**
 * @brief Verwerkt het herhaal-commando (aantal en hoe vaak).
 *
 * @param[out] in Struct waarin de geformatteerde commandostring terechtkomt.
 */
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

/**
 * @brief Verwerkt het cirkel-commando met X, Y, radius en optionele kleur.
 *
 * @param[out] in Struct waar de volledige commandostring wordt opgesteld.
 */
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

/**
 * @brief Verwerkt het figuur-commando met vijf punten en een kleur.
 *
 * Vereist 11 items (10 coördinaten + kleur). Bij een fout blijft alleen "figuur" staan.
 *
 * @param[out] in Struct waarin de uitvoerstring wordt geschreven.
 */
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

/**
 * @brief Verwerkt het toren-commando en ondersteunt meerdere parameterformaten.
 *
 * Mogelijke parameters: X, Y, grootte, kleur1, kleur2.
 *
 * @param[out] in Struct met de geformatteerde uitvoerstring.
 */
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

/**
 * @brief Toont alle beschikbare commando’s via UART en print een nieuwe prompt.
 *
 * @param[in] in Niet gebruikt.
 */
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

/**
 * @brief Zoekt het gevraagde commando in de dispatch-tabel en voert de juiste handler uit.
 *
 * @param[in] cmd Het eerste woord uit de gebruikersinput (commando).
 * @param[out] in Struct die gevuld wordt met de uiteindelijke commandostring.
 *
 * @return 1 als het commando gevonden is, 0 als het onbekend is.
 */
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

/**
 * @brief Converteert een foutcode naar een leesbare foutmelding en stuurt deze via UART.
 *
 * @param[in] foutcode Integer foutcode afkomstig uit de Logic Layer.
 */
void Return_error_code(int foutcode)
{
    char err_msg[150];
    const char *error_description = "";

    if (foutcode == NO_ERROR)
    {
        UART2_WriteString("OK\r\n> ");
        return;
    }

    // 1. Bepaal de beschrijving op basis van de foutcode
    switch (foutcode)
    {
        case ERROR_X1:
            error_description = "Ongeldige X-coördinaat (buiten scherm of te groot/klein).";
            break;
        case ERROR_Y1:
            error_description = "Ongeldige Y-coördinaat (buiten scherm of te groot/klein).";
            break;
        case ERROR_COLOR:
            error_description = "Ongeldige of onbekende kleur opgegeven (moet 0..255 zijn, of een bekende string).";
            break;
        case ERROR_DIJKTE_TOO_SMALL:
            error_description = "Lijndikte is te klein (<1) of valt buiten het scherm.";
            break;
        case ERROR_BREEDTE:
            error_description = "Rechthoek breedte is te groot (loopt buiten het scherm).";
            break;
        case ERROR_HOOGTE:
            error_description = "Rechthoek hoogte is te groot (loopt buiten het scherm).";
            break;
        case ERROR_GEVULD:
            error_description = "'Gevuld' parameter is ongeldig (moet 0 of 1 zijn).";
            break;
        case ERROR_RADIUS_TOO_SMALL:
            error_description = "Cirkel radius is te klein (<1) of valt buiten het scherm.";
            break;
        case ERROR_TEKST_TE_LANG:
            error_description = "Tekst is te lang voor de buffer of het scherm.";
            break;
        case ERROR_BITMAP_NR:
            error_description = "Ongeldig BitMap nummer (onbekende bitmap).";
            break;
        case ERROR_TE_WEINIG_PARAMS:
            error_description = "Te weinig essentiële parameters opgegeven voor dit commando.";
            break;
        case ERROR_ONBEKEND_CMD:
            error_description = "Onbekend commando. Typ HELP voor de lijst.";
            break;
        default:
            error_description = "Onbekende foutcode (niet gedefinieerd).";
            break;
    }

    // 2. Formatteer en print de gewenste foutmelding
    // Formaat: "ERROR {waarde int}\r\n{tekst die aangeeft wat de error dus is}\r\n> "
    snprintf(err_msg, sizeof(err_msg),
             "ERROR %d\r\n%s\r\n> ",
             foutcode, error_description);

    UART2_WriteString(err_msg);
}

/**
 * @brief Centrale invoerfunctie voor UART-gebruikersinput.
 *
 * Leest een regel tekst, normaliseert komma’s, haalt het commando eruit,
 * en roept de juiste handler aan. Onbekende commando’s worden gemeld.
 *
 * @param[out] in Struct waarin het uiteindelijke geformatteerde commando wordt opgeslagen.
 */
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
