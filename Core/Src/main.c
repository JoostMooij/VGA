/**
 * @file main.c
 * @brief Hoofdprogramma voor demo met UART interface
 *
 * Dit bestand bevat de main-loop van het project.
 * Het initieert systeemconfiguratie, UART-communicatie en de grafische interface.
 * Vervolgens leest het continu commando’s van de gebruiker via UART, verwerkt deze
 * met de API, toont debuginformatie en roept de juiste functies aan.
 *
 * Functionaliteiten:
 *  - Initialisatie van systeemklok en UART2 op 115200 baud
 *  - Initialisatie van API I/O en leeg scherm
 *  - Continu uitlezen van UART-commando’s
 *  - Loggen van ontvangen commando’s en lengte
 *  - Verwerking van commando’s via Handel_UART_Input()
 *  - Tekstverwerking via string_ophalen()
 *
 * @author Joost, Luc, Thijs
 * @version 1.0
 * @date 2025-12-08
 *
 * @note
 *  - BUFFER_SIZE definieert de grootte van de globale buffer voor UART-invoer
 *  - UserInput_t struct wordt gebruikt voor tijdelijke opslag van de volledige commando string
 *  - UART2_WriteString wordt gebruikt voor debug- en gebruikersinterface
 */

#include "main.h"
#define BUFFER_SIZE 64
char buffer[BUFFER_SIZE];

int main(void)
{
    SystemInit();
    SystemCoreClockUpdate();
    UART2_Init(115200);
    (void)API_init_io();
    (void)clearscherm("wit");
    UART2_WriteString("> ");
    static UserInput_t input;
    while (1)
    {
    	if (uart_command_ready)
    	{
    	    memset(&input, 0, sizeof(UserInput_t));
    	    strncpy(input.full_command, (const char *)uart_rx_buffer, MAX_CMD_LENGTH - 1);

    	    UART2_WriteString("\r\n[RX]: \"");
    	    UART2_WriteString(input.full_command);
    	    UART2_WriteString("\"\r\n");

    	    char dbg[64];
    	    snprintf(dbg, sizeof(dbg), "[LEN]: %d\r\n", (int)strlen(input.full_command));
    	    UART2_WriteString("Dit is de string die naar string_ophalen gaat: ");
    	    UART2_WriteString(input.full_command);  // print de string
    	    UART2_WriteString("\r\n");
    	    UART2_WriteString(dbg);

    	    Handel_UART_Input(&input);

    	    string_ophalen(input.full_command);

    	    UART2_WriteString("> ");

    	    uart_command_ready = 0;
    	    uart_rx_index = 0;
    	}
    }
}

