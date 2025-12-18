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

// ---------------- HELP Handler --------------------------
static void Handle_HELP(void)
{
    UART2_WriteString("\r\n--- Beschikbare Commando's ---\r\n");
    UART2_WriteString("KLEUR\r\n");
    UART2_WriteString("lijn,x1,y1,x2,y2,kleur,dikte\r\n");
    UART2_WriteString("rechthoek,x,y,w,h,kleur,gevuld\r\n");
    UART2_WriteString("tekst,x,y,kleur,tekst,font,grootte,stijl\r\n");
    UART2_WriteString("bitmap,x,y,nr\r\n");
    UART2_WriteString("clearscreen,kleur\r\n");
    UART2_WriteString("cirkel,x,y,straal,kleur\r\n");
    UART2_WriteString("figuur,x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,kleur\r\n");
    UART2_WriteString("wacht,msecs\r\n");
    UART2_WriteString("setPixel,x,y,kleur\r\n");
}

// ---------------- COMMAND Handlers ----------------------

// Deze ontbrak in de vorige versie:
static void Handle_setPixel(const char *cmd)
{
    int x, y;
    char kleur[16];

    // Verwacht: setPixel,20,20,rood
    if (sscanf(cmd, "setPixel,%d,%d,%15s", &x, &y, kleur) == 3)
    {
        // Debug output
        char msg[64];
        snprintf(msg, sizeof(msg), "\r\nPixel -> x=%d y=%d kleur=%s\r\n", x, y, kleur);
        UART2_WriteString(msg);

        // TODO: Aanroep naar API
        // API_draw_pixel(x, y, kleur_naar_int(kleur));
    }
    else
    {
        UART2_WriteString("\r\nFout: Gebruik setPixel,x,y,kleur\r\n");
    }
}

static void Handle_Lijn(const char *cmd) {
    // 1. Initialiseer variabelen op veilige waarden (0)
    int x1 = 0, y1 = 0, x2 = 0, y2 = 0, dikte = 1;
    char kleur[16] = "zwart";

    // 2. Parse de string
    int resultaat = sscanf(cmd, "lijn,%d,%d,%d,%d,%15[^,],%d", &x1, &y1, &x2, &y2, kleur, &dikte);

    // 3. Controleer of ALLE 6 de waarden gevonden zijn
    if (resultaat == 6) {
        // DEBUG: Stuur terug wat we gaan tekenen
        char msg[80];
        snprintf(msg, sizeof(msg), "DEBUG: Lijn van (%d,%d) naar (%d,%d) kleur=%s\r\n", x1, y1, x2, y2, kleur);
        UART2_WriteString(msg);
    }
    else {
        // Foutmelding als parsen mislukt
        UART2_WriteString("FOUT: Input onjuist. Gebruik: lijn,x1,y1,x2,y2,kleur,dikte\r\n");
    }
}

static void Handle_Rechthoek(const char *cmd) {
    int x, y, w, h, gevuld;
    char kleur[16];
    if (sscanf(cmd, "rechthoek,%d,%d,%d,%d,%15[^,],%d", &x, &y, &w, &h, kleur, &gevuld) == 6) {
        UART2_WriteString("Rechthoek getekend\r\n");
        // API_draw_rectangle(x, y, w, h, kleur_naar_int(kleur), gevuld, 0, 0);
    }
}

static void Handle_Tekst(const char *cmd) {
    int x, y, grootte, stijl;
    char kleur[16], tekst[64], font[16];
    // %[^-] leest alles tot de volgende komma (dus spaties toegestaan in tekst)
    if (sscanf(cmd, "tekst,%d,%d,%15[^,],%63[^,],%15[^,],%d,%d", &x, &y, kleur, tekst, font, &grootte, &stijl) == 7) {
        UART2_WriteString("Tekst geplaatst\r\n");
        // API_draw_text(x, y, kleur_naar_int(kleur), tekst, font, grootte, stijl, 0);
    }
}

static void Handle_Bitmap(const char *cmd) {
    int x, y, nr;
    if (sscanf(cmd, "bitmap,%d,%d,%d", &x, &y, &nr) == 3) {
        UART2_WriteString("Bitmap getekend\r\n");
        // API_draw_bitmap(x, y, nr);
    }
}

static void Handle_Clear(const char *cmd) {
    char kleur[16];
    if (sscanf(cmd, "clearscreen,%15s", kleur) == 1) {
        UART2_WriteString("Scherm gewist\r\n");
        // API_clearscreen(kleur_naar_int(kleur));
    }
}

static void Handle_Cirkel(const char *cmd) {
    int x, y, r;
    char kleur[16];
    if (sscanf(cmd, "cirkel,%d,%d,%d,%15s", &x, &y, &r, kleur) == 4) {
        UART2_WriteString("Cirkel getekend\r\n");
        // API_draw_circle(x, y, r, kleur_naar_int(kleur), 0);
    }
}

static void Handle_Wacht(const char *cmd) {
    int msecs;
    if (sscanf(cmd, "wacht,%d", &msecs) == 1) {
        UART2_WriteString("Wachten...\r\n");
        // API_wait(msecs);
    }
}

static void Handle_Figuur(const char *cmd) {
    int x1, y1, x2, y2, x3, y3, x4, y4, x5, y5;
    char kleur[16];
    if (sscanf(cmd, "figuur,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%15s",
               &x1, &y1, &x2, &y2, &x3, &y3, &x4, &y4, &x5, &y5, kleur) == 11) {
        UART2_WriteString("Figuur getekend\r\n");
        // API_draw_figure(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, kleur_naar_int(kleur), 0);
    }
}

static void Handle_Toren(const char *cmd) {
    int x, y, grootte;
    char kl1[20], kl2[20];
    if (sscanf(cmd, "toren,%d,%d,%d,%19[^,],%19s", &x, &y, &grootte, kl1, kl2) == 5) {
        UART2_WriteString("Toren tekenen...\r\n");
        toren(x, y, grootte, kleur_omzetter(kl1), kleur_omzetter(kl2));
    } else UART2_WriteString("Err: toren\r\n");
}

static void Handle_Herhaal(const char *cmd) {
     // TODO: Implementeer logica voor herhalen indien nodig
     UART2_WriteString("Herhaal commando nog niet actief\r\n");
}

static void Handle_KLEUR_INFO(void){
    UART2_WriteString("\r\n--- Beschikbare Kleuren ---\r\n");
    UART2_WriteString("1. zwart\r\n");
	UART2_WriteString("2. blauw\r\n");
	UART2_WriteString("3. lichtblauw\r\n");
	UART2_WriteString("4. groen\r\n");
	UART2_WriteString("5. lichtgroen\r\n");
    UART2_WriteString("6. cyaan\r\n");
    UART2_WriteString("7. lichtcyaan\r\n");
    UART2_WriteString("8. rood\r\n");
    UART2_WriteString("9. lichtrood\r\n");
    UART2_WriteString("10. magenta\r\n");
    UART2_WriteString("11. lichtmagenta\r\n");
    UART2_WriteString("12. bruin\r\n");
    UART2_WriteString("13. geel\r\n");
    UART2_WriteString("14. grijs\r\n");
    UART2_WriteString("15. wit\r\n");
    UART2_WriteString("16. roze\r\n");
    UART2_WriteString("17. paars\r\n");
}
// ---------------- MAIN DISPATCH FUNCTION ----------------

void Handel_UART_Input(UserInput_t *in)
{
    if (strlen(in->full_command) == 0) return;

    // Opschonen (newline verwijderen)
    char *p = in->full_command;
    while(*p) { if(*p == '\r' || *p == '\n') *p = 0; p++; }

    // Dispatcher (routering)
    if      (strncmp(in->full_command, "HELP", 4) == 0)         Handle_HELP();
    else if (strncmp(in->full_command, "KLEUR", 5) == 0)        Handle_KLEUR_INFO();

    else if (strncmp(in->full_command, "lijn", 4) == 0)         Handle_Lijn(in->full_command);
    else if (strncmp(in->full_command, "rechthoek", 9) == 0)    Handle_Rechthoek(in->full_command);
    else if (strncmp(in->full_command, "tekst", 5) == 0)        Handle_Tekst(in->full_command);
    else if (strncmp(in->full_command, "bitmap", 6) == 0)       Handle_Bitmap(in->full_command);
    else if (strncmp(in->full_command, "clearscreen", 11) == 0) Handle_Clear(in->full_command);
    else if (strncmp(in->full_command, "cirkel", 6) == 0)       Handle_Cirkel(in->full_command);
    else if (strncmp(in->full_command, "figuur", 6) == 0)       Handle_Figuur(in->full_command);
    else if (strncmp(in->full_command, "toren", 5) == 0)        Handle_Toren(in->full_command);
    else if (strncmp(in->full_command, "wacht", 5) == 0)        Handle_Wacht(in->full_command);
    else if (strncmp(in->full_command, "herhaal", 7) == 0)      Handle_Herhaal(in->full_command);
    else if (strncmp(in->full_command, "setPixel", 8) == 0)     Handle_setPixel(in->full_command);

    else {
        UART2_WriteString("\r\nOnbekend commando. Typ HELP.\r\n");
    }
}
