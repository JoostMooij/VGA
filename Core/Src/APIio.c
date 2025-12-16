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
#include <stdlib.h>

#include "APIio.h"
#include "APIerror.h"
#include "logicLayer.h"
#include "stm32_ub_vga_screen.h"
#include "UART.h"

volatile uint32_t ms_tick_counter = 0;
volatile int command_buffer[MAX_COMMAND_HISTORY_SIZE];
volatile int command_buffer_index = 0;



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

    int params[] = {color};
    record_command(CMD_CLEAR, 1, params);

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

    //int params[] = {x, y, color};
    //record_command(CMD_SETPIXEL, 3, params);

    UB_VGA_SetPixel(x, y, color);
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

    int params[] = {ms};
    record_command(CMD_WACHT, 1, params);

    uint32_t eindtijd = ms_tick_counter + ms;

    // Wacht totdat de teller de doeltijd bereikt
    while (ms_tick_counter < eindtijd)
    {
        //Wait For Interrupt.
        __WFI();
    }

    return errors;
}

/**
 * @brief Slaat een commando ID en zijn parameters op in de lineaire buffer.
 */
void record_command(COMMANDO_TYPE type, int param_count, const int params[])
{
    // Controleer op buffer overflow
    if (command_buffer_index + 1 + param_count > MAX_COMMAND_HISTORY_SIZE) {
        return;
    }

    // 1. Schrijf het commando ID (CMD_ID)
    command_buffer[command_buffer_index++] = (int)type;

    // 2. Schrijf de parameters
    for (int i = 0; i < param_count; i++) {
        command_buffer[command_buffer_index++] = params[i];
    }
}


/**
 * @brief Retourneert de totale omvang (ID + parameters) in de buffer
 * voor een gegeven commando type.
 * Deze mapping is essentieel voor het navigeren door de platte buffer.
 */
static int get_command_size(COMMANDO_TYPE type) {
    switch (type) {
        case CMD_CLEAR:     return 2;    // 1 param + ID [9]
        case CMD_WACHT:     return 2;    // 1 param + ID [10]
        case CMD_SETPIXEL:  return 4;    // 3 params + ID (x, y, color) [11]
        case CMD_LIJN:      return 7;    // 6 params + ID (x1, y1, x2, y2, color, dikte) [12]
        case CMD_RECHTHOEK: return 7;    // 6 params + ID (x, y, w, h, color, gevuld) [13]
        case CMD_CIRKEL:    return 5;    // 4 params + ID (x0, y0, radius, color) [14]
        case CMD_TOREN:     return 6;    // 5 params + ID (x, y, grootte, color1, color2) [15]
        case CMD_FIGUUR:    return 12;   // 11 params + ID (x1..x5, y1..y5, color) [16]
        default:            return 0;    // Onbekend of CMD_ONBEKEND/CMD_HERHAAL
    }
}

/**
 * @brief Keert de VGA-kleurcode om naar de vereiste stringrepresentatie.
 * Noodzakelijk omdat API-functies strings verwachten, maar de buffer ints opslaat.
 */
static const char* get_color_string_from_code(int code) {
    // Gebruik de bekende namen en waarden uit de Kleur enum [17-20]
    if (code == ZWART) return "zwart";
    if (code == BLAUW) return "blauw";
    if (code == LICHTBLAUW) return "lichtblauw";
    if (code == GROEN) return "groen";
    if (code == LICHTGROEN) return "lichtgroen";
    if (code == CYAAN) return "cyaan";
    if (code == ROOD) return "rood";
    if (code == LICHTROOD) return "lichtrood";
    if (code == MAGENTA) return "magenta";
    if (code == LICHTMAGENTA) return "lichtmagenta";
    if (code == BRUIN) return "bruin";
    if (code == GEEL) return "geel";
    if (code == GRIJS) return "grijs";
    if (code == WIT) return "wit";
    if (code == ROZE) return "roze";
    if (code == PAARS) return "paars";
    return "wit"; // Standaardwaarde bij onbekende kleur
}


/**
 * @brief Voert de laatste 'aantal' commando's uit de buffer opnieuw uit, 'hoevaak' keer.
 *
 * @param aantal Het aantal laatste commando's dat herhaald moet worden.
 * @param hoevaak Het aantal keren dat de reeks commando's herhaald moet worden.
 * @return ErrorList Altijd NO_ERROR (of de fout van de laatste herhaalde functie).
 */
void herhaal(int aantal, int hoevaak)
{
    ErrorList errors = Error_handling(FUNC_herhaal, aantal,hoevaak,0,0,0,0,0,0,0,0,0);
    if (errors.error_var1 || errors.error_var2)
    {
        return errors;
    }
    // 1. Zoek de startposities van alle commando's in de buffer
    int cmd_starts[MAX_COMMAND_HISTORY_SIZE];
    int cmd_start_count = 0;
    int i = 0;
    const int MAX_PARAMS_PLUS_ID = 12;

    // Lineaire scan om alle commando-starts te identificeren en de offsets te berekenen
    while (i < command_buffer_index) {
        COMMANDO_TYPE type = (COMMANDO_TYPE)command_buffer[i];
        int size = get_command_size(type);

        // Sla alleen geldige commando's (ID > 0) op
        if (size > 0 && type != CMD_HERHAAL) {
            cmd_starts[cmd_start_count++] = i;
            i += size;
        } else {
            // Overslaan/stoppen bij een onbekende of CMD_HERHAAL (herhaling van herhaling wordt genegeerd)
            if (size == 0) i++;
            else break;
        }
    }

    // Bepaal de effectieve index van de eerste herhaling
    int actual_aantal = (aantal > cmd_start_count) ? cmd_start_count : aantal;
    int start_index_offset = cmd_start_count - actual_aantal;

    // 2. Herhaal de gevonden commando's (Outer loop: hoevaak)
    for (int k = 0; k < hoevaak; k++)
    {
        // Inner loop: loop langs de geselecteerde commando's
        for (int j = 0; j < actual_aantal; j++)
        {
            int current_buffer_start = cmd_starts[start_index_offset + j];
            COMMANDO_TYPE type = (COMMANDO_TYPE)command_buffer[current_buffer_start];

            // Haal de parameters op (ID staat op index 0, parameters beginnen op index 1)
            int param_count = get_command_size(type) - 1;
            int params[MAX_PARAMS_PLUS_ID - 1];

            for (int p = 0; p < param_count; p++) {
                // Kopieer de parameterwaarden (coördinaat, dikte, of numerieke kleurcode)
                params[p] = command_buffer[current_buffer_start + 1 + p];
            }

            // 3. Roept de overeenkomstige functie aan (gebruikt directe int/string conversie)
            switch (type) {
                case CMD_CLEAR:
                    // Parameters: {color_code} (1)
                    clearscherm(get_color_string_from_code(params[0]));
                    break;
                case CMD_WACHT:
                    // Parameters: {ms} (1)
                	wacht(params[0]);
                    break;
//                case CMD_SETPIXEL:
//                    // Parameters: {x, y, color_code} (3)
//                    drawPixel(params[0], params[1], get_color_string_from_code(params[2]));
//                    break;
                case CMD_LIJN:
                    // Parameters: {x1, y1, x2, y2, color_code, dikte} (6)
                    lijn(params[0], params[1], params[2], params[3],
                                  get_color_string_from_code(params[4]), params[5]);
                    break;
                case CMD_RECHTHOEK:
                    // Parameters: {x, y, w, h, color_code, gevuld} (6)
                    rechthoek(params[0], params[1], params[2], params[3],
                                       get_color_string_from_code(params[4]), params[5]);
                    break;
                case CMD_CIRKEL:
                    // Parameters: {x0, y0, radius, color_code} (4)
                    cirkel(params[0], params[1], params[2],
                                    get_color_string_from_code(params[3]));
                    break;
                case CMD_FIGUUR:
                    // Parameters: {x1..x5, y1..y5, color_code} (11)
                    figuur(params[0], params[1], params[2], params[3], params[4], params[5],
                                    params[6], params[7], params[8], params[9],
                                    get_color_string_from_code(params[10]));
                    break;
                case CMD_TOREN:
                    // Parameters: {x, y, grootte, color1_code, color2_code} (5)
                    toren(params[0], params[1], params[2],
                                   get_color_string_from_code(params[3]),
                                   get_color_string_from_code(params[4]));
                    break;
                default:
                    // Niets doen
                    break;
            }
        }
    }

}
