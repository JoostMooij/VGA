/**
 * @file APIio.c
 * @brief Implementatie van de I/O-laag van de EE-API-LIB.
 *
 * Dit bestand wrapt hardware-drivers (UB_VGA, UART) naar nette API-functies.
 * Alle functies retourneren een ErrorList-struct om foutafhandeling uniform te maken.
 * Deze I/O-laag vormt de basis voor hogere API-lagen zoals APIdraw.
 *
 * @author Thijs
 * @version 1.1
 * @date 2025-11-20
 */

#include <stdint.h>
#include <string.h>

#include "APIio.h"
#include "APIerror.h"
#include "stm32_ub_vga_screen.h"

volatile uint32_t ms_tick_counter = 0;

/**
 * @brief Initialiseert de I/O-laag (VGA-scherm).
 *
 * Deze functie moet worden aangeroepen voordat andere API-I/O acties worden
 * uitgevoerd. Wrapt de UB_VGA_Screen_Init hardware-driver.
 *
 * @return void
 */
void API_init_io(void)
{
    UB_VGA_Screen_Init();
    SysTick_Init();
}

/**
 * @brief Leegt het volledige VGA-scherm met de opgegeven kleur.
 *
 * @param kleur De kleur als string (bijv. "rood", "groen", "cyaan").
 * @return ErrorList Struct met foutstatus van input-validatie.
 *
 * Valideert kleur via kleur_omzetter() en voert foutcontrole uit met
 * Error_handling(). Bij geldige input wordt het scherm gevuld.
 */
ErrorList clearscherm(const char *kleur)
{
    uint8_t color = kleur_omzetter(kleur);
    ErrorList errors = Error_handling(FUNC_clearscherm,color,0,0,0,0,0,0,0,0,0,0);
    if (errors.error_var1)
    {
        return errors;
    }
    UB_VGA_FillScreen(color);
    return errors;
}

/**
 * @brief Tekent een pixel op het VGA-scherm.
 *
 * @param x     X-coördinaat van de pixel.
 * @param y     Y-coördinaat van de pixel.
 * @param kleur Kleur als string (bijv. "rood", "groen").
 *
 * @return ErrorList Struct met validatiestatus van x, y en kleur.
 *
 * Wrapt UB_VGA_SetPixel() en valideert alle parameters via Error_handling().
 * Bij ongeldige invoer worden fouten geretourneerd, anders wordt de pixel gezet.
 */
ErrorList drawPixel(int x, int y, const char *kleur)
{
    uint8_t color = kleur_omzetter(kleur);
    ErrorList errors = Error_handling(FUNC_drawPixel, x, y, color,0,0,0,0,0,0,0,0);
    if (errors.error_var1 || errors.error_var2 || errors.error_var3)
    {
        return errors;
    }
    UB_VGA_SetPixel(x, y, color);
    return errors;
}

/**
 * @brief Tekent een pixel op het VGA-scherm.
 *
 * @param x     X-coördinaat van de pixel.
 * @param y     Y-coördinaat van de pixel.
 * @param kleur Kleur als string (bijv. "rood", "groen").
 *
 * @return ErrorList Struct met validatiestatus van x, y en kleur.
 *
 * Wrapt UB_VGA_SetPixel() en valideert alle parameters via Error_handling().
 * Bij ongeldige invoer worden fouten geretourneerd, anders wordt de pixel gezet.
 */
ErrorList setPixel(int x, int y, int kleur)
{
    ErrorList errors = Error_handling(FUNC_drawPixel, x, y, kleur,0,0,0,0,0,0,0,0);
    if (errors.error_var1 || errors.error_var2 || errors.error_var3)
    {
        return errors;
    }
    UB_VGA_SetPixel(x, y, kleur);
    return errors;
}

/**
 * @brief Zet een ingevoerde Nederlandse kleurnaam om naar een VGA-kleurcode.
 *
 * @param input String met kleurnaam ("rood", "groen", "lichtblauw", ...).
 * @return uint8_t Bijbehorende VGA-kleurcode. Geeft 0 terug bij ongeldige kleur.
 *
 * Wordt gebruikt door alle API-I/O functies om string-kleuren om te zetten
 * naar het kleurensysteem van de VGA-driver.
 */
uint8_t kleur_omzetter(const char *input)
{
    if (strcmp(input, "zwart") == 0)
    {
    	return ZWART;
    }
    if (strcmp(input, "blauw") == 0)
    {
    	return BLAUW;
    }
    if (strcmp(input, "lichtblauw") == 0)
    {
    	return LICHTBLAUW;
    }
    if (strcmp(input, "groen") == 0)
    {
    	return GROEN;
    }
    if (strcmp(input, "lichtgroen") == 0)
    {
    	return LICHTGROEN;
    }
    if (strcmp(input, "cyaan") == 0)
    {
    	return CYAAN;
    }
    if (strcmp(input, "lichtcyaan") == 0)
    {
    	return LICHTCYAAN;
    }
    if (strcmp(input, "rood") == 0)
    {
    	return ROOD;
    }
    if (strcmp(input, "lichtrood") == 0)
    {
    	return LICHTROOD;
    }
    if (strcmp(input, "magenta") == 0)
    {
    	return MAGENTA;
    }
    if (strcmp(input, "lichtmagenta") == 0)
    {
    	return LICHTMAGENTA;
    }
    if (strcmp(input, "bruin") == 0)
    {
    	return BRUIN;
    }
    if (strcmp(input, "geel") == 0)
    {
    	return GEEL;
    }
    if (strcmp(input, "grijs") == 0)
    {
    	return GRIJS;
    }
    if (strcmp(input, "wit") == 0)
    {
    	return WIT;
    }
    if (strcmp(input, "roze") == 0)
    {
    	return ROZE;
    }
    if (strcmp(input, "paars") == 0)
    {
    	return PAARS;
    }
    return 0;
}


/**
 * @brief Configureert de SysTick timer om elke milliseconde (1 ms) een interrupt te genereren.
 *
 * @note Gebruikt de SystemCoreClock (168 MHz)
 */
void SysTick_Init(void)
{
    // Configureert de SysTick timer voor 1 ms tick, gebaseerd op 168 MHz HCLK
    uint32_t waarde = (SystemCoreClock / 1000) - 1;

    // Stappen zijn register-level (via definities in stm32f4xx.h)
    SysTick->LOAD = waarde;
    SysTick->VAL = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;
}

/**
 * @brief SysTick Interrupt Service Routine (ISR)
 *
 * Dit is de feitelijke interrupt handler die elke 1 ms wordt aangeroepen.
 * Aangezien stm32f4xx_it.c ontbreekt, wordt deze hier direct gedefinieerd.
 */
void SysTick_Handler(void)
{
    ms_tick_counter++;
}

/**
 * @brief Functie om de executie te vertragen met een niet-blokkerende interrupt-gebaseerde delay.
 *
 * @param ms De vertragingstijd in milliseconden.
 * @return ErrorList Struct met foutstatus.
 */
ErrorList wacht(int ms)
{
    ErrorList errors = Error_handling(FUNC_wacht, ms,0,0,0,0,0,0,0,0,0,0);
    if (errors.error_var1)
    {
        return errors;
    }

    uint32_t eindtijd = ms_tick_counter + ms;

    // Wacht totdat de teller de doeltijd bereikt
    while (ms_tick_counter < eindtijd)
    {
        //Wait For Interrupt.
        __WFI();
    }

    return errors;
}
