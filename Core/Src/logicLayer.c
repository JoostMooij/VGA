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

/* -------------------------------------------------------------------------
 * Defines
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * Prototypes
 * ------------------------------------------------------------------------- */

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
int string_ophalen(char input[]);

/* -------------------------------------------------------------------------
 * string_ophalen()
 * ------------------------------------------------------------------------- */
int string_ophalen(char input[])
{
    if (input == NULL) return -1;         // Ongeldige pointercontrole

    input[strcspn(input, "\n")] = '\0';   // Verwijder newline uit input

    string_naar_vga(input);               // Geef string grafisch weer op VGA

    return 0;                             // Verwerking geslaagd
}
