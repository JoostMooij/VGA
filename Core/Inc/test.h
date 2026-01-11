/**
 * @file APIio_test.h
 * @brief Header voor de API IO test suite
 *
 * Bevat de functieprototypes voor het uitvoeren van alle IO-layer tests.
 * Include dit in main.c om de testfunctie aan te roepen.
 *
 * Auteur: Thijs
 * Datum: 2026-01-11
 */

#ifndef APIIO_TEST_H
#define APIIO_TEST_H

/**
 * @brief Voert alle API IO tests uit.
 *
 * Test alle functies systematisch met geldige en foutieve waarden.
 * Print resultaten via UART of printf.
 */
void test_APIio(void);

/**
 * @brief Voert alle API draw layer tests uit.
 *
 * Test alle functies systematisch met geldige en foutieve waarden.
 * Print resultaten via UART of printf.
 */
void test_APIdraw(void);
#endif // APIIO_TEST_H
