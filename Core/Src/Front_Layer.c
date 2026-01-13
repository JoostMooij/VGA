/**
 * @file front_layer.c
 * @brief Bevat de front layer handlers voor teken- en commandoverwerking via UART.
 * @details
 * Dit bestand verwerkt alle commando's die via interrupt-driven UART binnenkomen.
 * Het omvat commando's voor tekenen (lijn, rechthoek, cirkel, figuur, bitmap),
 * tekst, pixels, scherm wissen, toren tekenen, herhaling en kleurinformatie.
 * @author Luc, Joost, Thijs
 * @version 1.1
 * @date 13-01-2026
 *
 */

#include "UART.h"
#include "Front_layer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Toon alle beschikbare commando's via UART
 * @details Stuurt een lijst van commando's naar de UART monitor.
 */
static void Handle_HELP(void)
{
    UART2_WriteString("\r\n--- Beschikbare Commando's ---\r\n");
    UART2_WriteString("KLEUR\r\n");
    UART2_WriteString("lijn,x1,y1,x2,y2,kleur,dikte\r\n");
    UART2_WriteString("rechthoek,x,y,w,h,kleur,gevuld\r\n");
    UART2_WriteString("tekst,x,y,kleur,tekst,font,grootte,stijl\r\n");
    UART2_WriteString("bitmap,x,y,nr\r\n");
    UART2_WriteString("clearscherm,kleur\r\n");
    UART2_WriteString("cirkel,x,y,straal,kleur\r\n");
    UART2_WriteString("figuur,x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,kleur\r\n");
    UART2_WriteString("wacht,msecs\r\n");
    UART2_WriteString("setPixel,x,y,kleur\r\n");
}

/**
 * @brief Verwerk het setPixel commando
 * @param cmd De string met het commando, bv. "setPixel,20,20,rood"
 * @details Parseert de coördinaten en kleur en toont debug informatie via UART.
 */
static void Handle_setPixel(const char *cmd)
{
    int x = 0;
    int y = 0;
    char kleur[16] = {0};

    // Verwacht: setPixel,x,y,kleur
    if (sscanf(cmd, "setPixel,%d,%d,%15s", &x, &y, kleur) == 3)
    {
        char msg[64];
        snprintf(msg, sizeof(msg), "\r\nPixel -> x=%d y=%d kleur=%s\r\n", x, y, kleur);
        UART2_WriteString(msg);
    }
    else
    {
        UART2_WriteString("\r\nFOUT: Gebruik setPixel,x,y,kleur\r\n");
    }
}

// ================= LIJN HANDLER =========================

/**
 * @brief Verwerk het lijn commando
 * @param cmd De string met het commando, bv. "lijn,10,10,100,100,rood,2"
 * @details Parseert begin- en eindpunt, kleur en dikte en toont debug info via UART.
 */
static void Handle_Lijn(const char *cmd)
{
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    int dikte = 1;
    char kleur[16] = "zwart";

    int resultaat = sscanf(cmd, "lijn,%d,%d,%d,%d,%15[^,],%d", &x1, &y1, &x2, &y2, kleur, &dikte);

    if (resultaat == 6)
    {
        char msg[80];
        snprintf(msg, sizeof(msg), "DEBUG: Lijn van (%d,%d) naar (%d,%d) kleur=%s\r\n", x1, y1, x2, y2, kleur);
        UART2_WriteString(msg);
        // API_draw_line(x1, y1, x2, y2, kleur_naar_int(kleur), dikte);
    }
    else
    {
        UART2_WriteString("FOUT: Gebruik: lijn,x1,y1,x2,y2,kleur,dikte\r\n");
    }
}

/**
 * @brief Verwerk het rechthoek commando
 * @param cmd De string met het commando, bv. "rechthoek,10,10,50,50,blauw,1"
 * @details Parseert positie, grootte, kleur en gevuld-status en toont debug info.
 */
static void Handle_Rechthoek(const char *cmd)
{
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    int gevuld = 0;
    char kleur[16] = {0};

    if (sscanf(cmd, "rechthoek,%d,%d,%d,%d,%15[^,],%d", &x, &y, &w, &h, kleur, &gevuld) == 6)
    {
        UART2_WriteString("Rechthoek getekend\r\n");
        // API_draw_rectangle(x, y, w, h, kleur_naar_int(kleur), gevuld, 0, 0);
    }
    else
    {
        UART2_WriteString("FOUT: Gebruik: rechthoek,x,y,w,h,kleur,gevuld\r\n");
    }
}

/**
 * @brief Verwerk het tekst commando
 * @param cmd Commando string: "tekst,x,y,kleur,tekst,font,grootte,stijl"
 */
static void Handle_Tekst(const char *cmd)
{
    int x = 0;
    int y = 0;
    int grootte = 0;
    int stijl = 0;
    char kleur[16] = {0};
    char tekst[64] = {0};
    char font[16] = {0};

    if (sscanf(cmd, "tekst,%d,%d,%15[^,],%63[^,],%15[^,],%d,%d",
               &x, &y, kleur, tekst, font, &grootte, &stijl) == 7)
    {
        UART2_WriteString("Tekst geplaatst\r\n");
        // API_draw_text(x, y, kleur_naar_int(kleur), tekst, font, grootte, stijl, 0);
    }
    else
    {
        UART2_WriteString("FOUT: Gebruik: tekst,x,y,kleur,tekst,font,grootte,stijl\r\n");
    }
}

/**
 * @brief Verwerk het bitmap commando
 * @param cmd Commando string: "bitmap,x,y,nr"
 */
static void Handle_Bitmap(const char *cmd)
{
    int x = 0;
    int y = 0;
    int nr = 0;

    if (sscanf(cmd, "bitmap,%d,%d,%d", &x, &y, &nr) == 3)
    {
        UART2_WriteString("Bitmap getekend\r\n");
        // API_draw_bitmap(x, y, nr);
    }
    else
    {
        UART2_WriteString("FOUT: Gebruik: bitmap,x,y,nr\r\n");
    }
}

/**
 * @brief Wis het scherm
 * @param cmd Commando string: "clearscherm,kleur"
 */
static void Handle_Clear(const char *cmd)
{
    char kleur[16] = {0};

    if (sscanf(cmd, "clearscherm,%15s", kleur) == 1)
    {
        UART2_WriteString("Scherm gewist\r\n");
        // API_clearscherm(kleur_naar_int(kleur));
    }
    else
    {
        UART2_WriteString("FOUT: Gebruik: clearscherm,kleur\r\n");
    }
}

/**
 * @brief Verwerk cirkel commando
 * @param cmd Commando string: "cirkel,x,y,straal,kleur"
 */
static void Handle_Cirkel(const char *cmd)
{
    int x = 0;
    int y = 0;
    int r = 0;
    char kleur[16] = {0};

    if (sscanf(cmd, "cirkel,%d,%d,%d,%15s", &x, &y, &r, kleur) == 4)
    {
        UART2_WriteString("Cirkel getekend\r\n");
        // API_draw_circle(x, y, r, kleur_naar_int(kleur), 0);
    }
    else
    {
        UART2_WriteString("FOUT: Gebruik: cirkel,x,y,straal,kleur\r\n");
    }
}

/**
 * @brief Wacht een aantal milliseconden
 * @param cmd Commando string: "wacht,msecs"
 */
static void Handle_Wacht(const char *cmd)
{
    int msecs = 0;

    if (sscanf(cmd, "wacht,%d", &msecs) == 1)
    {
        UART2_WriteString("Wachten...\r\n");
        // API_wait(msecs);
    }
    else
    {
        UART2_WriteString("FOUT: Gebruik: wacht,msecs\r\n");
    }
}

/**
 * @brief Verwerk figuur commando
 * @param cmd Commando string: "figuur,x1,y1,x2,y2,x3,y3,x4,y4,x5,y5,kleur"
 */
static void Handle_Figuur(const char *cmd)
{
    int x1=0, y1=0, x2=0, y2=0, x3=0, y3=0, x4=0, y4=0, x5=0, y5=0;
    char kleur[16] = {0};

    if (sscanf(cmd, "figuur,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%15s",
               &x1,&y1,&x2,&y2,&x3,&y3,&x4,&y4,&x5,&y5,kleur) == 11)
    {
        UART2_WriteString("Figuur getekend\r\n");
        // API_draw_figure(x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, kleur_naar_int(kleur), 0);
    }
    else
    {
        UART2_WriteString("FOUT: Gebruik: figuur,x1,y1,...,x5,y5,kleur\r\n");
    }
}

/**
 * @brief Verwerk toren commando
 * @param cmd Commando string: "toren,x,y,grootte,kleur1,kleur2"
 */
static void Handle_Toren(const char *cmd)
{
    int x = 0;
    int y = 0;
    int grootte = 0;
    char kl1[20] = {0};
    char kl2[20] = {0};

    if (sscanf(cmd, "toren,%d,%d,%d,%19[^,],%19s", &x,&y,&grootte,kl1,kl2) == 5)
    {
        UART2_WriteString("Toren tekenen...\r\n");
        // toren(x, y, grootte, kleur_omzetter(kl1), kleur_omzetter(kl2));
    }
    else
    {
        UART2_WriteString("FOUT: Gebruik: toren,x,y,grootte,kleur1,kleur2\r\n");
    }
}

/**
 * @brief Verwerk herhaal commando
 * @param cmd Commando string: "herhaal,aantal,hoevaak"
 */
static void Handle_Herhaal(const char *cmd)
{
    int aantal = 0;
    int hoevaak = 0;

    if (sscanf(cmd, "herhaal,%d,%d", &aantal,&hoevaak) == 2)
    {
        UART2_WriteString("Herhaal...\r\n");
        // herhaal(aantal,hoevaak);
    }
    else
    {
        UART2_WriteString("FOUT: Gebruik: herhaal,aantal,hoevaak\r\n");
    }
}

/**
 * @brief Toon beschikbare kleuren
 */
static void Handle_KLEUR_INFO(void)
{
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

/**
 * @brief Dispatcher die binnenkomende UART input naar juiste handler stuurt
 * @param in Struct met volledige input string (UserInput_t)
 */
void Handel_UART_Input(UserInput_t *in)
{
    if (strlen(in->full_command) == 0) return;

    // Opschonen: verwijder newline characters
    char *p = in->full_command;
    while (*p)
    {
        if (*p == '\r' || *p == '\n') *p = 0;
        p++;
    }

    // Dispatcher
    if      (strncmp(in->full_command, "HELP", 4) == 0)        Handle_HELP();
    else if (strncmp(in->full_command, "KLEUR", 5) == 0)       Handle_KLEUR_INFO();
    else if (strncmp(in->full_command, "lijn", 4) == 0)        Handle_Lijn(in->full_command);
    else if (strncmp(in->full_command, "rechthoek", 9) == 0)   Handle_Rechthoek(in->full_command);
    else if (strncmp(in->full_command, "tekst", 5) == 0)       Handle_Tekst(in->full_command);
    else if (strncmp(in->full_command, "bitmap", 6) == 0)      Handle_Bitmap(in->full_command);
    else if (strncmp(in->full_command, "clearscherm", 11) == 0) Handle_Clear(in->full_command);
    else if (strncmp(in->full_command, "cirkel", 6) == 0)      Handle_Cirkel(in->full_command);
    else if (strncmp(in->full_command, "figuur", 6) == 0)      Handle_Figuur(in->full_command);
    else if (strncmp(in->full_command, "toren", 5) == 0)       Handle_Toren(in->full_command);
    else if (strncmp(in->full_command, "wacht", 5) == 0)       Handle_Wacht(in->full_command);
    else if (strncmp(in->full_command, "herhaal", 7) == 0)     Handle_Herhaal(in->full_command);
    else if (strncmp(in->full_command, "setPixel", 8) == 0)    Handle_setPixel(in->full_command);
    else
    {
        UART2_WriteString("\r\nOnbekend commando. Typ HELP.\r\n");
    }
}
