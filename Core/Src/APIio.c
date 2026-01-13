/**
 * @file APIio.c
 * @brief Implementatie van de I/O-laag
 *
 *
 * @author Thijs, Joost, Luc
 * @version 1.1
 * @date 2025-11-20
 */

#include <string.h>
#include <stdlib.h>
#include "APIio.h"
#include "APIerror.h"
#include "logicLayer.h"
#include "APIdraw.h"


// Globalen voor command buffer en history
uint16_t command_buffer[MAX_COMMAND_BUFFER_SIZE];
uint16_t command_buffer_index = 0;
volatile uint32_t ms_tick_counter = 0;
int herhaal_hoog = 0;
static uint8_t cmd_starts[MAX_COMMAND_HISTORY_SIZE];
static uint8_t cmd_start_count = 0;
static char tekst_historie[MAX_TEKST_HISTORY][MAX_WOORD];
static int tekst_historie_index = 0;


/**
 * @brief Initialiseert de I/O-laag (VGA-scherm).
 *
 * Deze functie moet worden aangeroepen voordat andere API-I/O acties
 * worden uitgevoerd. Wrapt de hardware-driver UB_VGA_Screen_Init().
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
 * Valideert de kleur via kleur_omzetter() en voert foutcontrole uit
 * met Error_handling(). Bij geldige input wordt het scherm gevuld.
 *
 * @param kleur Kleur als string (bijv. "rood", "groen", "cyaan")
 * @return ErrorList Struct met foutstatus van input-validatie
 */
ErrorList clearscherm(const char *kleur)
{
    uint8_t color;
    ErrorList errors;
    int ik_heb_geactiveerd = 0;

    color = kleur_omzetter(kleur);
    errors = Error_handling(FUNC_clearscherm,
                            color, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    if (errors.error_var1)
        return errors;

    /* Commando opslaan indien nodig */
    if (herhaal_hoog == 0)
    {
        int params[] = {(int)color};
        record_command(CMD_CLEAR, 1, params);
        herhaal_hoog = 1;
        ik_heb_geactiveerd = 1;
    }

    /* Scherm vullen */
    UB_VGA_FillScreen(color);

    /* Herhaalstatus resetten */
    if (ik_heb_geactiveerd)
        herhaal_hoog = 0;

    return errors;
}

/**
 * @brief Tekent een pixel op het VGA-scherm.
 *
 * Wrapt UB_VGA_SetPixel() en valideert alle parameters via
 * Error_handling(). Bij ongeldige invoer worden fouten geretourneerd.
 *
 * @param x X-coördinaat van de pixel
 * @param y Y-coördinaat van de pixel
 * @param kleur Kleur als string (bijv. "rood", "groen")
 * @return ErrorList Struct met validatiestatus van x, y en kleur
 */
ErrorList drawPixel(int x, int y, const char *kleur)
{
    uint8_t color;
    ErrorList errors;
    int ik_heb_geactiveerd = 0;

    color = kleur_omzetter(kleur);
    errors = Error_handling(FUNC_drawPixel,
                            x, y, color,
                            0, 0, 0, 0, 0, 0, 0, 0);

    if (errors.error_var1 || errors.error_var2 || errors.error_var3)
        return errors;

    /* Commando opslaan indien nodig */
    if (herhaal_hoog == 0)
    {
        int params[] = {x, y, (int)color};
        record_command(CMD_SETPIXEL, 3, params);
        herhaal_hoog = 1;
        ik_heb_geactiveerd = 1;
    }

    /* Pixel tekenen */
    UB_VGA_SetPixel(x, y, color);

    /* Herhaalstatus resetten */
    if (ik_heb_geactiveerd)
        herhaal_hoog = 0;

    return errors;
}

/**
 * @brief Tekent een pixel op het VGA-scherm.
 *
 * Wrapt UB_VGA_SetPixel() en valideert alle parameters via Error_handling().
 * Bij ongeldige invoer worden fouten geretourneerd, anders wordt de pixel gezet.
 *
 * @param x     X-coördinaat van de pixel
 * @param y     Y-coördinaat van de pixel
 * @param kleur VGA-kleurcode
 * @return ErrorList Struct met validatiestatus van x, y en kleur
 */
ErrorList setPixel(int x, int y, int kleur)
{
    ErrorList errors;

    /* Controleer invoer */
    errors = Error_handling(FUNC_drawPixel, x, y, kleur, 0, 0, 0, 0, 0, 0, 0, 0);

    if (errors.error_var1 || errors.error_var2 || errors.error_var3)
        return errors;

    /* Pixel tekenen */
    UB_VGA_SetPixel(x, y, kleur);

    return errors;
}

/**
 * @brief Zet een Nederlandse kleurnaam om naar VGA-kleurcode.
 *
 * Wordt gebruikt door alle API-I/O functies om string-kleuren om te zetten
 * naar het kleurensysteem van de VGA-driver.
 *
 * @param input String met kleurnaam ("rood", "groen", "lichtblauw", ...)
 * @return uint8_t VGA-kleurcode. Geeft Ongeldige_kleur bij onbekende input
 */
uint8_t kleur_omzetter(const char *input)
{
    if (strcmp(input, "zwart") == 0) return ZWART;
    if (strcmp(input, "blauw") == 0) return BLAUW;
    if (strcmp(input, "lichtblauw") == 0) return LICHTBLAUW;
    if (strcmp(input, "groen") == 0) return GROEN;
    if (strcmp(input, "lichtgroen") == 0) return LICHTGROEN;
    if (strcmp(input, "cyaan") == 0) return CYAAN;
    if (strcmp(input, "lichtcyaan") == 0) return LICHTCYAAN;
    if (strcmp(input, "rood") == 0) return ROOD;
    if (strcmp(input, "lichtrood") == 0) return LICHTROOD;
    if (strcmp(input, "magenta") == 0) return MAGENTA;
    if (strcmp(input, "lichtmagenta") == 0) return LICHTMAGENTA;
    if (strcmp(input, "bruin") == 0) return BRUIN;
    if (strcmp(input, "geel") == 0) return GEEL;
    if (strcmp(input, "grijs") == 0) return GRIJS;
    if (strcmp(input, "wit") == 0) return WIT;
    if (strcmp(input, "roze") == 0) return ROZE;
    if (strcmp(input, "paars") == 0) return PAARS;

    return Ongeldige_kleur;
}

/**
 * @brief Configureert de SysTick timer voor 1 ms interrupts.
 *
 * Gebruikt SystemCoreClock (168 MHz) om een 1 ms tick te genereren.
 * Stappen zijn register-level via definities in stm32f4xx.h
 */
void SysTick_Init(void)
{
    uint32_t waarde;

    /* Bereken LOAD waarde voor 1 ms tick */
    waarde = (SystemCoreClock / 1000) - 1;

    /* Configureer SysTick registers */
    SysTick->LOAD = waarde;
    SysTick->VAL  = 0;
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk |
                    SysTick_CTRL_ENABLE_Msk;
}

/**
 * @brief SysTick Interrupt Service Routine (ISR)
 *
 * Wordt elke 1 ms aangeroepen door de SysTick timer.
 * Aangezien stm32f4xx_it.c ontbreekt, wordt deze hier direct gedefinieerd.
 */
void SysTick_Handler(void)
{
    ms_tick_counter++;
}

/**
 * @brief Vertraagt de uitvoering met een niet-blokkerende delay.
 *
 * Gebruikt SysTick interrupt om milliseconden te tellen.
 *
 * @param ms Vertraging in milliseconden
 * @return ErrorList Struct met foutstatus van input-validatie
 */
ErrorList wacht(int ms)
{
    ErrorList errors;
    int ik_heb_geactiveerd = 0;
    uint32_t eindtijd;

    errors = Error_handling(FUNC_wacht, ms, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (errors.error_var1)
        return errors;

    /* Commando opslaan indien nodig */
    if (herhaal_hoog == 0)
    {
        int params[] = {ms};
        record_command(CMD_WACHT, 1, params);
        herhaal_hoog = 1;
        ik_heb_geactiveerd = 1;
    }

    /* Niet-blokkerende delay met WFI */
    eindtijd = ms_tick_counter + ms;
    while (ms_tick_counter < eindtijd)
    {
        __WFI();
    }

    /* Herhaalstatus resetten */
    if (ik_heb_geactiveerd)
        herhaal_hoog = 0;

    return errors;
}

/**
 * @brief Slaat een commando en bijbehorende parameters op in de buffer.
 *
 * Maakt automatisch ruimte vrij door het oudste commando te verwijderen
 * als de buffer vol is.
 *
 * @param type Commando ID
 * @param param_count Aantal parameters
 * @param params Array met parameters
 */
void record_command(COMMANDO_TYPE type, int param_count, const int params[])
{
    int nieuwe_omvang = 1 + param_count;

    /* Ongeldige input blokkeren */
    if (param_count < 0 || nieuwe_omvang > MAX_COMMAND_BUFFER_SIZE)
        return;

    /* Ruimte maken indien nodig */
    while (cmd_start_count >= MAX_COMMAND_HISTORY_SIZE ||
           (command_buffer_index + nieuwe_omvang) > MAX_COMMAND_BUFFER_SIZE)
    {
        if (cmd_start_count <= 0)
            return; /* niets om te verwijderen */

        int oudste_pos = cmd_starts[0];

        if (oudste_pos < 0 || oudste_pos >= command_buffer_index)
            return; /* corruptie gedetecteerd */

        COMMANDO_TYPE oudste_type = (COMMANDO_TYPE)command_buffer[oudste_pos];
        int oudste_omvang = get_command_size(oudste_type);

        if (oudste_omvang <= 0 || oudste_omvang > command_buffer_index)
            oudste_omvang = 1;

        /* Buffer fysiek opschuiven */
        for (int i = 0; i < (command_buffer_index - oudste_omvang); i++)
        {
            command_buffer[i] = command_buffer[i + oudste_omvang];
        }

        command_buffer_index -= oudste_omvang;
        if (command_buffer_index < 0)
            command_buffer_index = 0;

        /* Start-offsets bijwerken */
        for (int i = 0; i < cmd_start_count - 1; i++)
        {
            cmd_starts[i] = cmd_starts[i + 1] - oudste_omvang;
        }

        cmd_start_count--;
    }

    /* Laatste veiligheidscheck */
    if (command_buffer_index + nieuwe_omvang > MAX_COMMAND_BUFFER_SIZE)
        return;

    /* Nieuw commando toevoegen */
    cmd_starts[cmd_start_count++] = (uint8_t)command_buffer_index;
    command_buffer[command_buffer_index++] = (uint16_t)type;

    for (int i = 0; i < param_count; i++)
    {
        command_buffer[command_buffer_index++] = (uint16_t)params[i];
    }
}

/**
 * @brief Retourneert de totale omvang (ID + parameters) in de buffer
 *        voor een gegeven commando type.
 * @param type Het commando type
 * @return int Totale omvang in de buffer
 *
 * Essentieel voor het navigeren door de platte buffer.
 */
int get_command_size(COMMANDO_TYPE type)
{
    switch (type)
    {
        case CMD_CLEAR:      return 2;
        case CMD_WACHT:      return 2;
        case CMD_SETPIXEL:   return 4;
        case CMD_LIJN:       return 7;
        case CMD_RECHTHOEK:  return 7;
        case CMD_CIRKEL:     return 5;
        case CMD_TOREN:      return 6;
        case CMD_FIGUUR:     return 12;
        case CMD_TEKST:      return 8;
        case CMD_BITMAP:     return 4;
        default:             return 0;  // Onbekend of CMD_ONBEKEND/CMD_HERHAAL
    }
}

/**
 * @brief Zet een VGA-kleurcode om naar de bijbehorende string.
 * @param code Numerieke VGA-kleurcode
 * @return const char* Stringrepresentatie van de kleur
 *
 * Noodzakelijk omdat API-functies strings verwachten, maar buffer numerieke codes opslaat.
 */
const char* get_color_string_from_code(int code)
{
    if (code == ZWART)        return "zwart";
    if (code == BLAUW)        return "blauw";
    if (code == LICHTBLAUW)   return "lichtblauw";
    if (code == GROEN)        return "groen";
    if (code == LICHTGROEN)   return "lichtgroen";
    if (code == CYAAN)        return "cyaan";
    if (code == LICHTCYAAN)   return "lichtcyaan";
    if (code == ROOD)         return "rood";
    if (code == LICHTROOD)    return "lichtrood";
    if (code == MAGENTA)      return "magenta";
    if (code == LICHTMAGENTA) return "lichtmagenta";
    if (code == BRUIN)        return "bruin";
    if (code == GEEL)         return "geel";
    if (code == GRIJS)        return "grijs";
    if (code == WIT)          return "wit";
    if (code == ROZE)         return "roze";
    if (code == PAARS)        return "paars";

    // Fallback voor onbekende/foute code
    return "zwart";
}

/**
 * @brief Herhaalt de laatste 'aantal' commando's uit de buffer 'hoevaak' keer.
 * @param aantal Aantal commando's om te herhalen
 * @param hoevaak Hoeveel keer de reeks herhalen
 * @return ErrorList Foutstatus van de laatste functie, of NO_ERROR
 */
ErrorList herhaal(int aantal, int hoevaak)
{
    ErrorList errors = Error_handling(FUNC_herhaal, aantal, hoevaak, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (errors.error_var1 || errors.error_var2)
        return errors;

    int te_herhalen = (aantal > cmd_start_count) ? cmd_start_count : aantal;
    int start_index_in_kleine_buffer = cmd_start_count - te_herhalen;

    herhaal_hoog = 1;  // Voorkom opname van herhaalde commando's

    for (int h = 0; h < hoevaak; h++)
    {
        for (int i = 0; i < te_herhalen; i++)
        {
            int pos = cmd_starts[start_index_in_kleine_buffer + i];
            COMMANDO_TYPE type = (COMMANDO_TYPE)command_buffer[pos];

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
                    bitMap(command_buffer[pos + 1], command_buffer[pos + 2], command_buffer[pos + 3]);
                    break;

                case CMD_TEKST:
                {
                    int tekst_idx = command_buffer[pos + 4];
                    int font_id   = command_buffer[pos + 5];
                    int schaal    = command_buffer[pos + 6];
                    int stijl_id  = command_buffer[pos + 7];

                    const char* f_naam  = (font_id == 1) ? "acorn" : "pearl";
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

/**
 * @brief Reserveert een slot in de tekst buffer en slaat de string op.
 * @param tekst De tekststring die opgeslagen moet worden
 * @return int Index van het opgeslagen tekstslot
 */
int reserveer_tekst_slot(const char* tekst)
{
    if (tekst_historie_index >= MAX_TEKST_HISTORY)
        tekst_historie_index = 0;

    strncpy(tekst_historie[tekst_historie_index], tekst, MAX_WOORD - 1);
    tekst_historie[tekst_historie_index][MAX_WOORD - 1] = '\0';

    return tekst_historie_index++;
}
