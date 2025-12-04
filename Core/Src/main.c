//--------------------------------------------------------------
// File     : main.c
// Doel     : Koppeling tussen UART en Logic Layer
// MCU      : STM32F4xx
//--------------------------------------------------------------

#include "main.h"

// ***************************************************************
// SIMULATIE VAN APIerror.h DEFINITIES (Nodig voor compilatie)
// ***************************************************************
// Foutcodes
#define NO_ERROR               0
#define ERROR_X1               1
#define ERROR_Y1               2
#define ERROR_COLOR            3
#define ERROR_DIJKTE_TOO_SMALL 4
#define ERROR_BREEDTE          5
#define ERROR_HOOGTE           6
#define ERROR_GEVULD           7
#define ERROR_RADIUS_TOO_SMALL 8 // Aangenomen uit check_radius_op_scherm

// Algemene Applicatie Fouten (hogere nummers)
#define ERROR_TE_WEINIG_PARAMS 99
#define ERROR_ONBEKEND_CMD     100
// Functie ID's (nodig om de juiste foutlogica te kiezen)
typedef enum { FUNC_clearscherm = 1, FUNC_drawPixel = 2, FUNC_lijn = 3, FUNC_rechthoek = 4 } FunctionID;
// ***************************************************************

// Aanname van VGA display afmetingen voor de checks
#define VGA_DISPLAY_X 640
#define VGA_DISPLAY_Y 480

extern void Return_error_code(int foutcode);

// Forward Declarations van de checks (voor de Mock)
int check_x(int x);
int check_y(int y);
int check_color(int color);
int check_lijn_op_scherm(int x1, int y1, int x2, int y2, int dikte);
// ***************************************************************

/**
 * @brief De Logic Layer Mock: voert de foutcontroles uit.
 *
 * Retourneert de eerste, hoogste foutcode (int > 0) of NO_ERROR (0).
 *
 * @param[in] command De commando-string van de Front Layer.
 * @return int De foutcode.
 */

// ERROR HANDELING SIMULATIE

//int controleer_commando_parameters(const char *command)
//{
//    char cmd[20];
//    int p[7] = {0}; // p[1..6] voor parameters
//    char s[2][20] = {"", ""}; // s[0] voor kleur
//
//    // Parse: Command + tot 6 ints + 2 strings
//    int items = sscanf(command, "%19s %d %d %d %d %d %d %19s %19s",
//                       cmd, &p[1], &p[2], &p[3], &p[4], &p[5], &p[6], s[0], s[1]);
//
//    // Convert string kleur naar int (Mock: 1 = ROOD)
//    int kleur_int = (strcmp(s[0], "ROOD") == 0 || strcmp(s[0], "rood") == 0) ? 1 : (items >= 7 ? 256 : 0);
//    // Vang het geval op dat er een niet-geparste kleur is, of een lege string, als int
//    if (items >= 7 && (p[6] == 0 && (strcmp(s[0], "") != 0)) ) kleur_int = 256;
//
//
//    if (strcmp(cmd, "setPixel") == 0)
//    {
//        // setPixel vereist minstens 2 parameters (X, Y)
//        if (items < 3) return ERROR_TE_WEINIG_PARAMS;
//
//        // Controleer op API-fouten
//        if (check_x(p[1]) != NO_ERROR) return ERROR_X1;
//        if (check_y(p[2]) != NO_ERROR) return ERROR_Y1;
//
//        // Mock: p[3] is kleur. Als string kleur niet herkend is, is kleur_int 256
//        if (check_color(kleur_int) != NO_ERROR) return ERROR_COLOR;
//    }
//    else if (strcmp(cmd, "lijn") == 0)
//    {
//        // lijn vereist minstens 4 parameters (X1, Y1, X2, Y2)
//        if (items < 5) return ERROR_TE_WEINIG_PARAMS;
//
//        // Controleer op API-fouten
//        if (check_x(p[1]) != NO_ERROR) return ERROR_X1;
//        if (check_y(p[2]) != NO_ERROR) return ERROR_Y1;
//        if (check_x(p[3]) != NO_ERROR) return ERROR_X1; // Hergebruik ERROR_X1
//        if (check_y(p[4]) != NO_ERROR) return ERROR_Y1; // Hergebruik ERROR_Y1
//
//        // Mock: dikte is p[5]. Standaard dikte 1 als p[5] == 0 (wat gebeurt bij niet geparst)
//        int dikte = (items >= 6) ? p[5] : 1;
//        if (check_lijn_op_scherm(p[1], p[2], p[3], p[4], dikte) != NO_ERROR) return ERROR_DIJKTE_TOO_SMALL;
//
//        // Mock: kleur is p[6]
//        if (check_color(kleur_int) != NO_ERROR) return ERROR_COLOR;
//    }
//    else if (strcmp(cmd, "help") == 0 || strcmp(cmd, "?") == 0)
//    {
//        return NO_ERROR;
//    }
//    else
//    {
//        // Commando bekend in Front Layer, maar niet geïmplementeerd in Mock Logic
//        if (items >= 1) return NO_ERROR;
//    }
//
//    return NO_ERROR;
//}
//
//// ----------------------------------------------------
//// MOCK IMPLEMENTATIES van check functies (uit APIerror.c)
//// ----------------------------------------------------
//
//int check_x(int x)
//{
//    if(x < 0 || x >= VGA_DISPLAY_X) return ERROR_X1;
//    return NO_ERROR;
//}
//int check_y(int y)
//{
//    if(y < 0 || y >= VGA_DISPLAY_Y) return ERROR_Y1;
//    return NO_ERROR;
//}
//int check_color(int color)
//{
//    // Simuleer een beperkte set kleuren in de mock (1=ROOD, 0=ZWART)
//    if(color < 0 || color > 255) return ERROR_COLOR;
//    return NO_ERROR;
//}
//int check_lijn_op_scherm(int x1, int y1, int x2, int y2, int dikte)
//{
//    // Eenvoudige check voor de demo
//    if(dikte < 1) return ERROR_DIJKTE_TOO_SMALL;
//    return NO_ERROR;
//}
// ... Andere check functies zouden hier moeten staan voo

int main(void)
{
    SystemInit();
    SystemCoreClockUpdate();

    UART2_Init(115200);

    UART2_WriteString("Systeem Start. Typ HELP voor commando's.\r\n> ");

    UserInput_t input;
    int logic_foutcode = NO_ERROR;

	while(1)
	{
	    // 1. Wacht op input
	    Handel_UART_Input(&input);

	    if(strlen(input.full_command) > 0)
	    {
	        // 2. Geef aan dat het commando verwerkt wordt
	        UART2_WriteString("Processing: ");
            UART2_WriteString(input.full_command);
            UART2_WriteString("...\r\n");

	        // 3. Logic Layer voert controle uit
	        //logic_foutcode = controleer_commando_parameters(input.full_command);

	        // 4. Stuur de integer foutcode naar de Front Layer voor weergave en prompt
	        Return_error_code(logic_foutcode);
	    }
	}
}
