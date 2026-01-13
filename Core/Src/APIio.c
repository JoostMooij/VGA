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

#include "string.h"
#include "stdlib.h"
#include "APIio.h"
#include "APIerror.h"
#include "logicLayer.h"
#include "APIdraw.h"

uint16_t command_buffer[MAX_COMMAND_BUFFER_SIZE];
uint16_t command_buffer_index = 0;
volatile uint32_t ms_tick_counter = 0;
int herhaal_hoog = 0;

static uint8_t cmd_starts[MAX_COMMAND_HISTORY_SIZE];
static uint8_t cmd_start_count = 0;

#define MAX_TEKST_HISTORY 4

static char tekst_historie[MAX_TEKST_HISTORY][MAX_WOORD];
static int tekst_historie_index = 0;

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
    ErrorList errors = Error_handling(FUNC_clearscherm, color, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (errors.error_var1) return errors;

    int ik_heb_geactiveerd = 0;
    if (herhaal_hoog == 0)
    {
        int params[] = {(int)color};
        record_command(CMD_CLEAR, 1, params);
        herhaal_hoog = 1;      // Zet vlag op 1
        ik_heb_geactiveerd = 1; // Eigenaar van de vlag
    }

    UB_VGA_FillScreen(color);

    if (ik_heb_geactiveerd)
    {
        herhaal_hoog = 0;
    }
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
    ErrorList errors = Error_handling(FUNC_drawPixel, x, y, color, 0, 0, 0, 0, 0, 0, 0, 0);
    if (errors.error_var1 || errors.error_var2 || errors.error_var3) return errors;

    int ik_heb_geactiveerd = 0;
    if (herhaal_hoog == 0)
    {
        int params[] = {x, y, (int)color};
        record_command(CMD_SETPIXEL, 3, params);
        herhaal_hoog = 1;
        ik_heb_geactiveerd = 1;
    }

    UB_VGA_SetPixel(x, y, color);

    if (ik_heb_geactiveerd)
    {
        herhaal_hoog = 0;
    }
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
    return Ongeldige_kleur;
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
    ErrorList errors = Error_handling(FUNC_wacht, ms, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (errors.error_var1) return errors;

    int ik_heb_geactiveerd = 0;
    if (herhaal_hoog == 0)
    {
        int params[] = {ms};
        record_command(CMD_WACHT, 1, params);
        herhaal_hoog = 1;
        ik_heb_geactiveerd = 1;
    }

    uint32_t eindtijd = ms_tick_counter + ms;
    while (ms_tick_counter < eindtijd)
    {
        __WFI();
    }

    if (ik_heb_geactiveerd)
    {
        herhaal_hoog = 0;
    }
    return errors;
}
/**
 * @brief Slaat een commando ID en zijn parameters op in de lineaire buffer.
 * Maakt automatisch ruimte vrij door het oudste commando te verwijderen als de buffer vol is.
 */
void record_command(COMMANDO_TYPE type, int param_count, const int params[])
{
    int nieuwe_omvang = 1 + param_count;

    // Ongeldige input blokkeren
    if (param_count < 0 || nieuwe_omvang > MAX_COMMAND_BUFFER_SIZE)
        return;

    // Ruimte maken indien nodig
    while (cmd_start_count >= MAX_COMMAND_HISTORY_SIZE ||
          (command_buffer_index + nieuwe_omvang) > MAX_COMMAND_BUFFER_SIZE)
    {
        if (cmd_start_count <= 0)
            return; // niets om te verwijderen → abort

        int oudste_pos = cmd_starts[0];

        if (oudste_pos < 0 || oudste_pos >= command_buffer_index)
            return; // corruptie gedetecteerd

        COMMANDO_TYPE oudste_type =
            (COMMANDO_TYPE)command_buffer[oudste_pos];

        int oudste_omvang = get_command_size(oudste_type);

        if (oudste_omvang <= 0 ||
            oudste_omvang > command_buffer_index)
            oudste_omvang = 1;

        // Buffer fysiek opschuiven
        for (int i = 0; i < (command_buffer_index - oudste_omvang); i++)
        {
            command_buffer[i] =
                command_buffer[i + oudste_omvang];
        }

        command_buffer_index -= oudste_omvang;
        if (command_buffer_index < 0)
            command_buffer_index = 0;

        // Start-offsets bijwerken
        for (int i = 0; i < cmd_start_count - 1; i++)
        {
            cmd_starts[i] =
                cmd_starts[i + 1] - oudste_omvang;
        }

        cmd_start_count--;
    }

    // Laatste veiligheidscheck
    if (command_buffer_index + nieuwe_omvang > MAX_COMMAND_BUFFER_SIZE)
        return;

    // Nieuw commando toevoegen
    cmd_starts[cmd_start_count++] =
        (uint8_t)command_buffer_index;

    command_buffer[command_buffer_index++] =
        (uint16_t)type;

    for (int i = 0; i < param_count; i++)
    {
        command_buffer[command_buffer_index++] =
            (uint16_t)params[i];
    }
}

/**
 * @brief Retourneert de totale omvang (ID + parameters) in de buffer
 * voor een gegeven commando type.
 * Deze mapping is essentieel voor het navigeren door de platte buffer.
 */
static int get_command_size(COMMANDO_TYPE type)
{
    switch (type)
    {
        case CMD_CLEAR:     return 2;
        case CMD_WACHT:     return 2;
        case CMD_SETPIXEL:  return 4;
        case CMD_LIJN:      return 7;
        case CMD_RECHTHOEK: return 7;
        case CMD_CIRKEL:    return 5;
        case CMD_TOREN:     return 6;
        case CMD_FIGUUR:    return 12;
        case CMD_TEKST:     return 8;
        case CMD_BITMAP:    return 4;
        default:            return 0;    // Onbekend of CMD_ONBEKEND/CMD_HERHAAL
    }
}


/**
 * @brief Keert de VGA-kleurcode om naar de vereiste stringrepresentatie.
 * Noodzakelijk omdat API-functies strings verwachten (const char *),
 * maar de buffer alleen de numerieke codes opslaat.
 */
static const char* get_color_string_from_code(int code)
{
    // Gebruik de kleurdefinities uit APIio.h (497, 498)
    if (code == ZWART) return "zwart";
    if (code == BLAUW) return "blauw";
    if (code == LICHTBLAUW) return "lichtblauw"; // [1, 2]
    if (code == GROEN) return "groen";
    if (code == LICHTGROEN) return "lichtgroen";
    if (code == CYAAN) return "cyaan";
    // ... Voeg alle overige kleuren toe (ROOD, WIT, etc.) ...
    if (code == ROOD) return "rood"; // [2]
    if (code == GEEL) return "geel"; // [2]
    if (code == GRIJS) return "grijs"; // [2]
    if (code == WIT) return "wit"; // [2]
    if (code == MAGENTA) return "magenta"; // [2]
    if (code == PAARS) return "paars"; // [2]

    // Fallback voor onbekende/foute code (gebruikt 0 als fallback in kleur_omzetter)
    if (code == 0) return "zwart";

    return "wit";
}

/**
 * @brief Voert de laatste 'aantal' commando's uit de buffer opnieuw uit, 'hoevaak' keer.
 *
 * @param aantal Het aantal laatste commando's dat herhaald moet worden.
 * @param hoevaak Het aantal keren dat de reeks commando's herhaald moet worden.
 * @return ErrorList Altijd NO_ERROR (of de fout van de laatste herhaalde functie).
 */
ErrorList herhaal(int aantal, int hoevaak)
{
   ErrorList errors = Error_handling(FUNC_herhaal, aantal,hoevaak,0,0,0,0,0,0,0,0,0);
	if (errors.error_var1 || errors.error_var2)
	{
		return errors;
	}
    // Bepaal hoeveel commando's er daadwerkelijk herhaald kunnen worden
    int te_herhalen = (aantal > cmd_start_count) ? cmd_start_count : aantal;
    int start_index_in_kleine_buffer = cmd_start_count - te_herhalen;

    herhaal_hoog = 1; // Voorkom dat herhaalde commando's opnieuw worden opgenomen [6]

    for (int h = 0; h < hoevaak; h++)
    {
        for (int i = 0; i < te_herhalen; i++)
        {
            // Haal de exacte startpositie in de grote buffer op uit de kleine buffer
            int pos = cmd_starts[start_index_in_kleine_buffer + i];
            COMMANDO_TYPE type = (COMMANDO_TYPE)command_buffer[pos];

            // Roep de juiste functie aan met variabelen direct uit de grote buffer [7, 8]
            switch (type)
            {
                case CMD_CLEAR:
                    clearscherm(get_color_string_from_code(command_buffer[pos + 1]));
                    break;
                case CMD_WACHT:
                    wacht(command_buffer[pos + 1]);
                    break;
                case CMD_LIJN:
                    lijn(command_buffer[pos + 1], command_buffer[pos + 2],
                         command_buffer[pos + 3], command_buffer[pos + 4],
                         get_color_string_from_code(command_buffer[pos + 5]),
                         command_buffer[pos + 6]);
                    break;
                case CMD_RECHTHOEK:
                    rechthoek(command_buffer[pos + 1], command_buffer[pos + 2],
                              command_buffer[pos + 3], command_buffer[pos + 4],
                              get_color_string_from_code(command_buffer[pos + 5]),
                              command_buffer[pos + 6]);
                    break;
                case CMD_CIRKEL:
                    cirkel(command_buffer[pos + 1], command_buffer[pos + 2],
                           command_buffer[pos + 3],
                           get_color_string_from_code(command_buffer[pos + 4]));
                    break;
                case CMD_TOREN:
                    toren(command_buffer[pos + 1], command_buffer[pos + 2],
                          command_buffer[pos + 3],
                          get_color_string_from_code(command_buffer[pos + 4]),
                          get_color_string_from_code(command_buffer[pos + 5]));
                    break;
                case CMD_BITMAP:
                    bitMap(command_buffer[pos+1], command_buffer[pos+2], command_buffer[pos+3]);
                    break;
                case CMD_TEKST:
                {
                    int tekst_idx  = command_buffer[pos + 4];
                    int font_id    = command_buffer[pos + 5];
                    int schaal     = command_buffer[pos + 6];
                    int stijl_id   = command_buffer[pos + 7];

                    // Vertaal ID's terug naar strings
                    const char* f_naam = (font_id == 1) ? "acorn" : "pearl";
                    const char* f_stijl = "normaal";
                    if (stijl_id == 1) f_stijl = "vet";
                    else if (stijl_id == 2) f_stijl = "cursief";

                    tekst(command_buffer[pos + 1],
                          command_buffer[pos + 2],
                          get_color_string_from_code(command_buffer[pos + 3]),
                          tekst_historie[tekst_idx],
                          f_naam,
                          schaal,
                          f_stijl);
                }
                break;
                default: break;
            }
        }
    }
    herhaal_hoog = 0;
    return errors;
}

int reserveer_tekst_slot(const char* tekst) {
    // Gebruik de nieuwe constante voor de circulaire buffer
    if (tekst_historie_index >= MAX_TEKST_HISTORY) tekst_historie_index = 0;

    strncpy(tekst_historie[tekst_historie_index], tekst, MAX_WOORD - 1);
    tekst_historie[tekst_historie_index][MAX_WOORD - 1] = '\0';
    return tekst_historie_index++;
}
