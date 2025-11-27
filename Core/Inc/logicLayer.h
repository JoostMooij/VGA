#ifndef LOGIC_LAYER_H
#define LOGIC_LAYER_H

#define MAX_INPUT 256
#define MAX_WOORD 64

typedef enum
{
    CMD_ONBEKEND = 0,
    CMD_LIJN,
    CMD_RECHTHOEK,
    CMD_TEKST,
    CMD_BITMAP,
    CMD_CLEAR,
    CMD_WACHT,
    CMD_HERHAAL,
    CMD_CIRKEL,
    CMD_FIGUUR,
    CMD_TOREN
} COMMANDO_TYPE;

int string_ophalen(char input[]);
void haal_commando(const char *input, char *woord);
COMMANDO_TYPE bepaal_commando(const char *woord);
COMMANDO_TYPE verwerk_commando(const char *input);
const char* commando_naar_string(COMMANDO_TYPE cmd);

/* Wordt extern geïmplementeerd in test_logicLayer.c */
void string_naar_vga(const char *str);

#endif
