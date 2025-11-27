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

    return 0;                             // Verwerking geslaagd
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

    return CMD_ONBEKEND;
}


/* -------------------------------------------------------------------------
 * Functie: verwerk_commando
 * Doel   : Verstuurd data naar juiste functie uit ioLayer
 * ------------------------------------------------------------------------- */
COMMANDO_TYPE verwerk_commando(const char *input)
{
    char woord[MAX_WOORD];
    haal_commando(input, woord);
    return bepaal_commando(woord);
}



