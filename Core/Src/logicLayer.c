/**
 * @file logicLayer.c
 * @brief Verwerkt errors en zet deze om naar tekenopdrachten.
 *
 * Dit bestand is de logicLayer. Het leest inkomende inputs van de frontLayer, splitst ze op,
 * bepaalt het type opdracht en voert de goede functie uit.
 * Daarnaast zet de logicLayer errorcodes uit de ioLayer om naar tekst en verstuurd deze naar de frontLayer.
 *
 * @author Joost Mooij
 * @date 04/12/2025
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "logicLayer.h"


/**
 * @brief Zet een foutcode om naar een tekstuele beschrijving.
 *
 * Geeft een vaste string terug die hoort bij de opgegeven foutcode.
 *
 * @param code De numerieke foutcode (0–7).
 * @return Een constante string met de foutnaam.
 */

const char* errorCodeToString(int code)
{
    switch (code)
    {
    case NO_ERROR:
        return "NO_ERROR";
    case ERROR_X1:
        return "ERROR_X1";
    case ERROR_Y1:
        return "ERROR_Y1";
    case ERROR_COLOR:
        return "ERROR_COLOR";
    case ERROR_DIJKTE_TOO_SMALL:
        return "ERROR_DIJKTE_TOO_SMALL";
    case ERROR_BREEDTE:
        return "ERROR_BREEDTE";
    case ERROR_HOOGTE:
        return "ERROR_HOOGTE";
    case ERROR_GEVULD:
        return "ERROR_GEVULD";
    case ERROR_RADIUS_TOO_SMALL:
        return "ERROR_RADIUS_TOO_SMALL";
    case ERROR_GROOTTE_TOO_SMALL:
        return "ERROR_GROOTTE_TOO_SMALL";
    case ERROR_TOREN_BUITEN_SCHERM:
        return "ERROR_TOREN_BUITEN_SCHERM";
    case ERROR_TIME_TOO_SMALL:
        return "ERROR_TIME_TOO_SMALL";
    case ERROR_bitmap_nr:
        return "ERROR_bitmap_nr";
    case ERROR_bitmap_buiten_scherm:
        return "ERROR_bitmap_buiten_scherm";
    case ERROR_tekst_buiten_scherm:
        return "ERROR_tekst_buiten_scherm";
    case ERROR_TEXT_EMPTY:
        return "ERROR_TEXT_EMPTY";
    case ERROR_INVALID_STYL:
        return "ERROR_INVALID_STYL";
    default:
        return "UNKNOWN_ERROR";
    }
}

/**
 * @brief Leest alle foutwaarden uit een ErrorList en stuurt de gevonden fouten via UART.
 *
 * De functie controleert elke error_var in de struct. Wanneer een foutcode
 * tussen 1 en 7 wordt gevonden, wordt de bijbehorende fouttekst omgezet
 * en via UART verstuurd.
 *
 * @param errors Pointer naar de ErrorList met foutcodes.
 */

void errorterugsendfunctie(const ErrorList* errors)
{
    const int* errorArray[11] = {
        &errors->error_var1,
        &errors->error_var2,
        &errors->error_var3,
        &errors->error_var4,
        &errors->error_var5,
        &errors->error_var6,
        &errors->error_var7,
        &errors->error_var8,
        &errors->error_var9,
        &errors->error_var10,
        &errors->error_var11
    };

    for (int i = 0; i < 11; i++)
    {
        int code = *errorArray[i];

        if (code != NO_ERROR)
        {
            char msg[MAX_ERROR_MESSAGE];
            snprintf(msg, MAX_ERROR_MESSAGE,
                     "Found error: %s\n",
                     errorCodeToString(code));
            UART2_WriteString(msg);
        }
    }
}



/**
 * @brief Leest een inkomende tekstregel in en verwijdert de newline.
 *
 * De functie controleert eerst of de input geldig is. Daarna wordt
 * de newline eruit gehaald en wordt het commando verwerkt.
 *
 * @param input De ontvangen string.
 * @return 0 bij succes, -1 bij een ongeldige pointer.
 */

int string_ophalen(char input[])
{
    if (input == NULL) return -1;

    input[strcspn(input, "\n")] = '\0';
    verwerk_commando(input);
    return 0;
}

/**
 * @brief Haalt het eerste woord uit een commando, tot aan de eerste komma.
 *
 * De functie kopieert tekens uit de input naar het woord-buffer totdat
 * een komma of het einde van de string is bereikt.
 *
 * @param input De volledige invoertekst.
 * @param woord Buffer waarin het eerste woord wordt opgeslagen.
 */

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

/**
 * @brief Bepaalt het type commando aan de hand van het eerste woord.
 *
 * Vergelijkt het woord met bekende commando’s en geeft het juiste
 * COMMANDO_TYPE terug. Als het woord niet herkend wordt, volgt CMD_ONBEKEND.
 *
 * @param woord Het eerste woord uit het invoercommando.
 * @return Het bijbehorende COMMANDO_TYPE.
 */

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

/**
 * @brief Verwerkt een volledig tekstcommando en voert de juiste opdracht uit.
 *
 * De functie splitst de input op komma’s, bepaalt het commando,
 * roept de passende tekenfunctie aan en controleert op fouten.
 * Als er fouten zijn, worden deze teruggestuurd via UART.
 *
 * @param input De volledige binnengekomen opdrachtregel.
 */

void verwerk_commando(const char *input)
{
    char buff[MAX_INPUT];
    char *delen[20];
    int aantal = 0;
    ErrorList errors;

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
        	errors = lijn(atoi(delen[1]), atoi(delen[2]), atoi(delen[3]), atoi(delen[4]), delen[5], atoi(delen[6]));

            break;

        case CMD_RECHTHOEK:
        	errors = rechthoek(atoi(delen[1]), atoi(delen[2]), atoi(delen[3]), atoi(delen[4]), delen[5], atoi(delen[6]));
            break;

        case CMD_TEKST:
        	errors = tekst(atoi(delen[1]), atoi(delen[2]), delen[3], delen[4], delen[5], atoi(delen[6]), delen[7]);
            break;

        case CMD_BITMAP:
        	errors = bitMap(atoi(delen[1]), atoi(delen[2]), atoi(delen[3]));
            break;

        case CMD_CLEAR:
        	errors = clearscherm(delen[1]);
            break;

        case CMD_WACHT:
        	errors = wacht(atoi(delen[1]));
            break;

        case CMD_HERHAAL:
//        	errors = herhaal(atoi(delen[1]), atoi(delen[2]));
            break;

        case CMD_CIRKEL:
        	errors = cirkel(atoi(delen[1]), atoi(delen[2]), atoi(delen[3]), delen[4]);
            break;

        case CMD_FIGUUR:
        	errors = figuur(atoi(delen[1]), atoi(delen[2]), atoi(delen[3]), atoi(delen[4]), atoi(delen[5]), atoi(delen[6]), atoi(delen[7]), atoi(delen[8]), atoi(delen[9]), atoi(delen[10]), delen[11]);
            break;

        case CMD_TOREN:
        	errors = toren(atoi(delen[1]), atoi(delen[2]), atoi(delen[3]), delen[4], delen[5]);
            break;

        case CMD_SETPIXEL:
        	errors = drawPixel(atoi(delen[1]), atoi(delen[2]), delen[3]);
        	//errors = setPixel(atoi(delen[1]), atoi(delen[2]),  atoi(delen[3]));
			break;


        case CMD_ONBEKEND:
        default:
            /* onbekend commando, doe niets */
            break;
    }
    if(errors.error_var1 || errors.error_var2 || errors.error_var3 ||errors.error_var4 || errors.error_var5 || errors.error_var6 || errors.error_var7 || errors.error_var8 || errors.error_var9 || errors.error_var10 || errors.error_var11)
    {
    	errorterugsendfunctie(&errors);
    }
}
