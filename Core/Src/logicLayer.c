/******************************************************************************
 *  Bestand   : logic.c
 *  Functie   : Logic-laag. Ontvangt opdrachten als string, bepaalt welk
 *              commando afgehandeld moet worden en roept de juiste
 *              uitvoerfunctis aan.
 *  Auteur    : Joost
 *  Versie    : 1.0
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logicLayer.h"
#include "test_ioLayer.h"

/**
 * @brief   Verwerkt een binnenkomende opdrachtstring.
 *
 * Controleert de ontvangen inputstring, verwijdert een eventuele newline,
 * en stuurt de string door naar de VGA-uitvoerfunctie.
 *
 * @param[in,out] input
 *      De binnenkomende opdrachtstring. Deze mag geen NULL zijn.
 *      De functie past de string aan door een newline ('\n') te vervangen
 *      door een afsluitende nulterminator ('\0').
 *
 * @return
 *      0  : Succesvol verwerkt.
 *     -1 : Ongeldige pointer (NULL).
 *
 * @note
 *  - De functie roept string_naar_vga() aan voor visuele uitvoer.
 *  - Geheugengebruik: geen extra allocaties; werkt direct op `input`.
 *  - Verwachte executietijd: O(n) afhankelijk van stringlengte.
 */
int string_ophalen(char input[])
{
    if (input == NULL) return -1;         // Ongeldige pointercontrole

    input[strcspn(input, "\n")] = '\0';   // Verwijder newline uit input
    verwerk_commando(input);
    return 0;                            // Verwerking geslaagd
}

/* -------------------------------------------------------------------------
 * Functie: haal_commando
 * Doel   : Haal het woord voor de eerste komma uit de inputstring
 * ------------------------------------------------------------------------- */
void haal_commando(const char *input, char *woord)
{
    int i = 0;

    while (input[i] != '\0' && input[i] != ',' && i < MAX_WOORD - 1)
    {
        woord[i] = input[i];
        i++;
    }

    woord[i] = '\0';
}


/* -------------------------------------------------------------------------
 * Functie: bepaal_commando
 * Doel   : Zet een commando-woord om naar enum waarde
 * ------------------------------------------------------------------------- */
COMMANDO_TYPE bepaal_commando(const char *woord)
{
    if (strcmp(woord, "lijn") == 0)         return CMD_LIJN;
    if (strcmp(woord, "rechthoek") == 0)    return CMD_RECHTHOEK;
    if (strcmp(woord, "tekst") == 0)        return CMD_TEKST;
    if (strcmp(woord, "bitmap") == 0)       return CMD_BITMAP;
    if (strcmp(woord, "clearscherm") == 0)  return CMD_CLEAR;
    if (strcmp(woord, "wacht") == 0)        return CMD_WACHT;
    if (strcmp(woord, "herhaal") == 0)      return CMD_HERHAAL;
    if (strcmp(woord, "cirkel") == 0)       return CMD_CIRKEL;
    if (strcmp(woord, "figuur") == 0)       return CMD_FIGUUR;
    if (strcmp(woord, "toren") == 0)        return CMD_TOREN;
    if (strcmp(woord, "setPixel") == 0)     return CMD_SETPIXEL;

    return CMD_ONBEKEND;
}


int kleur_omzetter(const char *input)
{
    if (strcmp(input, "zwart") == 0)        return ZWART;
    if (strcmp(input, "blauw") == 0)        return BLAUW;
    if (strcmp(input, "lichtblauw") == 0)   return LICHTBLAUW;
    if (strcmp(input, "groen") == 0)        return GROEN;
    if (strcmp(input, "lichtgroen") == 0)   return LICHTGROEN;
    if (strcmp(input, "cyaan") == 0)        return CYAAN;
    if (strcmp(input, "lichtcyaan") == 0)   return LICHTCYAAN;
    if (strcmp(input, "rood") == 0)         return ROOD;
    if (strcmp(input, "lichtrood") == 0)    return LICHTROOD;
    if (strcmp(input, "magenta") == 0)      return MAGENTA;
    if (strcmp(input, "lichtmagenta") == 0) return LICHTMAGENTA;
    if (strcmp(input, "bruin") == 0)        return BRUIN;
    if (strcmp(input, "geel") == 0)         return GEEL;
    if (strcmp(input, "grijs") == 0)        return GRIJS;
    if (strcmp(input, "wit") == 0)          return WIT;
    if (strcmp(input, "roze") == 0)         return ROZE;
    if (strcmp(input, "paars") == 0)        return PAARS;

    return -1;
}

/* -------------------------------------------------------------------------
 * Functie: verwerk_commando
 * Doel   : Verstuurd data naar juiste functie uit ioLayer
 * ------------------------------------------------------------------------- */
void verwerk_commando(const char *input)
{
    char buff[MAX_INPUT];
    char *delen[20];
    int aantal = 0;

    strncpy(buff, input, MAX_INPUT);
    buff[MAX_INPUT - 1] = '\0';

    char *ptr = strtok(buff, ",");
    while (ptr != NULL && aantal < 20)
    {
        delen[aantal++] = ptr;
        ptr = strtok(NULL, ",");
    }

    if (aantal == 0)
    {
        return;
    }

    char woord[MAX_WOORD];
    haal_commando(input, woord);

    COMMANDO_TYPE type = bepaal_commando(woord);

    switch (type)
    {
        case CMD_LIJN:
            lijn(atoi(delen[1]), atoi(delen[2]), atoi(delen[3]), atoi(delen[4]), kleur_omzetter(delen[5]), atoi(delen[6]));

            break;

        case CMD_RECHTHOEK:
            rechthoek(atoi(delen[1]), atoi(delen[2]), atoi(delen[3]), atoi(delen[4]), kleur_omzetter(delen[5]), atoi(delen[6]));
            break;

        case CMD_TEKST:
            tekst(atoi(delen[1]), atoi(delen[2]), kleur_omzetter(delen[3]), delen[4], delen[5], atoi(delen[6]), delen[7]);
            break;

        case CMD_BITMAP:
            bitmap(atoi(delen[1]), atoi(delen[2]), atoi(delen[3]));
            break;

        case CMD_CLEAR:
            clearscherm(kleur_omzetter(delen[1]));
            break;

        case CMD_WACHT:
            wacht(atoi(delen[1]));
            break;

        case CMD_HERHAAL:
            herhaal(atoi(delen[1]), atoi(delen[2]));
            break;

        case CMD_CIRKEL:
            cirkel(atoi(delen[1]), atoi(delen[2]), atoi(delen[3]), kleur_omzetter(delen[4]));
            break;
git
        case CMD_FIGUUR:
            figuur(atoi(delen[1]), atoi(delen[2]), atoi(delen[3]), atoi(delen[4]), atoi(delen[5]), atoi(delen[6]), atoi(delen[7]), atoi(delen[8]), atoi(delen[9]), atoi(delen[10]), kleur_omzetter(delen[11]));
            break;

        case CMD_TOREN:
            toren(atoi(delen[1]), atoi(delen[2]), atoi(delen[3]), kleur_omzetter(delen[4]), kleur_omzetter(delen[5]));
            break;

        case CMD_SETPIXEL:
			setPixel(atoi(delen[1]), atoi(delen[2]), kleur_omzetter(delen[3]));
			break;


        case CMD_ONBEKEND:
        default:
            /* onbekend commando, doe niets */
            break;
    }
}
