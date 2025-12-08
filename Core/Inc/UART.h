/**
 ******************************************************************************
 * @file           : uart.h
 * @brief          : Publieke interface voor de UART2-driver (register-level).
 *
 * Deze header definieert alle benodigde functieprototypes voor de UART2-driver.
 * De implementatie maakt gebruik van directe registermanipulatie van de
 * STM32F4-serie en bevindt zich in uart.c.
 *
 * Functionaliteit:
 * - Initialiseren van UART2 met een opgegeven baudrate.
 * - Versturen van individuele karakters en volledige strings.
 * - Inlezen van losse karakters of volledige tekstregels.
 *
 * @author         : Joost Mooij
 * @date           : 2025-12-08
 * @version        : 1.0
 ******************************************************************************
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "stm32f4xx.h"

/**
 * @brief Initialiseert UART2 met de opgegeven baudrate.
 *
 * Configureert GPIO, kloklijnen en UART2-registers zodat data verzonden en
 * ontvangen kan worden via register-level aansturing.
 *
 * @param baudrate Gewenste baudrate, bijvoorbeeld 9600 of 115200.
 */
void UART2_Init(uint32_t baudrate);

/**
 * @brief Verstuurt één karakter via UART2.
 *
 * Wacht indien nodig tot de zender gereed is en plaatst daarna het karakter
 * in het dataregister.
 *
 * @param c Het te versturen karakter.
 */
void UART2_SendChar(char c);

/**
 * @brief Verstuurt een null-terminated string via UART2.
 *
 * Itereert door de volledige string en verstuurt elk karakter afzonderlijk.
 *
 * @param str Pointer naar de te versturen string.
 */
void UART2_WriteString(const char *str);

/**
 * @brief Leest één karakter in via UART2.
 *
 * Wacht tot er een nieuw karakter beschikbaar is in het ontvangregister.
 *
 * @return Het ingelezen karakter.
 */
char UART2_ReadChar(void);

/**
 * @brief Leest een string in totdat ENTER wordt ontvangen of maxlen bereikt is.
 *
 * De functie stopt bij `\r` of `\n`, en sluit de buffer altijd correct af
 * met een null-terminator.
 *
 * @param buf    Doelbuffer voor de ingelezen tekst.
 * @param maxlen Maximale lengte van de buffer.
 */
void UART2_ReadString(char *buf, uint32_t maxlen);

#endif /* INC_UART_H_ */

