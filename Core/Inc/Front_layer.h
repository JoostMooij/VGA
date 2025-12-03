/*
 * Front_layer.h
 * Header bestand voor commando verwerking
 */

#ifndef FRONT_LAYER_H
#define FRONT_LAYER_H

#ifdef __cplusplus
extern "C" {
#endif

// ---------------- Definities ----------------

// Kleuren definitie (nodig voor de rest van je programma)
typedef enum {
    ZWART,
    BLAUW,
    LICHTBLAUW,
    GROEN,
    LICHTGROEN,
    CYAAN,
    LICHTCYAAN,
    ROOD,
    LICHTROOD,
    MAGENTA,
    LICHTMAGENTA,
    BRUIN,
    GEEL,
    GRIJS,
    WIT,
    ROZE,
    PAARS
} Kleur;

// De structuur die de data opslaat die uit de parser komt.
// Deze velden MOETEN bestaan omdat front_layer.c ze invult.
typedef struct {
    char command[32];       // Bijv: "lijn", "tekst"

    int x[16];              // Opslag voor coordinaten, groottes, radii, diktes, etc.
    int y[16];              // Opslag voor y-coordinaten

    char color_name[32];    // Opslag voor de kleur string (bijv. "ROOD")
    char text_arg[64];      // Opslag voor tekst-inhoud of de 2e kleur bij 'toren'
} UserInput_t;


// ---------------- Functie Prototypes ----------------

/**
 * @brief Leest UART input en vult de UserInput_t struct
 */
void Handel_UART_Input(UserInput_t *in);


#ifdef __cplusplus
}
#endif

#endif // FRONT_LAYER_H
