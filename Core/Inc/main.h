/**
 * @file main.h
 * @brief Hoofdheaderbestand
 *
 * Dit bestand bevat alle globale include-bestanden die nodig zijn
 * voor de applicatie. Het fungeert als centrale verzamelplaats
 * voor hardware-, driver- en applicatielagen.
 *
 * @author
 * Thijs, Joost, Luc
 *
 * @version 1.0
 * @date 2025-11-20
 */

#ifndef __STM32F4_UB_MAIN_H
#define __STM32F4_UB_MAIN_H

/*-------------------------------------------------------
 * Include bestanden
 *-------------------------------------------------------*/

/**
 * @brief STM32F4 CMSIS en hardwaredefinities.
 */
#include "stm32f4xx.h"

/**
 * @brief Front-end laag voor gebruikersinteractie.
 */
#include "Front_layer.h"

/**
 * @brief UART-communicatiefunctionaliteit.
 */
#include "UART.h"

/**
 * @brief Standaard C string-functies.
 */
#include <string.h>

/**
 * @brief Standaard C I/O-functionaliteit.
 */
#include <stdio.h>

/**
 * @brief VGA-schermdriver voor STM32.
 */
#include "stm32_ub_vga_screen.h"

/**
 * @brief Logica-laag voor commandoverwerking.
 */
#include "logicLayer.h"

/**
 * @brief Wiskundige hulpfuncties.
 */
#include <math.h>

/**
 * @brief API-foutafhandeling en foutstructuren.
 */
#include "APIerror.h"

/**
 * @brief API-tekenfunctionaliteit (lijnen, figuren, tekst).
 */
#include "APIdraw.h"

/**
 * @brief API I/O-laag voor VGA en timing.
 */
#include "APIio.h"

/**
 * @brief Test- en debugfunctionaliteit.
 */
#include "test.h"

#endif
