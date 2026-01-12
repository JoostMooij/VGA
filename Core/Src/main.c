//--------------------------------------------------------------
// File     : main.c
// Doel     : Eenvoudige UART2 communicatie (Hello naar Termite)
// MCU      : STM32F4xx
//--------------------------------------------------------------

#include "main.h"
#define BUFFER_SIZE 128
char buffer[BUFFER_SIZE]; // De buffer moet buiten main gedefinieerd zijn als je deze overal wilt gebruiken

int main(void)
{
    SystemInit();
    SystemCoreClockUpdate();
    UART2_Init(115200);
    (void)API_init_io();
    (void)clearscherm("wit");
//
//    (void)clearscherm("wit");
//	(void)wacht(1000);
//	(void)lijn(10, 10, 10, 150, "rood", 10);
//	(void)bitMap(1,20,20);
//	(void)tekst(1, 40, "zwart", "ABCDEFGHIJKLMNOPQRSTUVWXYZ ", "acorn", 1, "vet");
//	(void)wacht(1000);
//	(void)rechthoek(20,20,40,80,"geel", 1);
//	(void)wacht(1000);
//	(void)figuur(10,10,20,20,60,50,80,100,110,114,"groen");
//	(void)wacht(1000);
//	(void)cirkel(70,50,25,"blauw");
//	(void)wacht(1000);
//	(void)toren(100,100,20,"groen","geel");
//	(void)wacht(1000);
//	(void)clearscherm("wit");
//	(void)wacht(1000);
//	(void)herhaal(14, 2);
//	(void)clearscherm("rood");

    (void)clearscherm("zwart");
    (void)lijn(10, 10, 100, 100, "rood", 4);
    (void)lijn(100, 1, 100, 200, "groen", 3);
    (void)clearscherm("wit");
    (void)rechthoek(10, 10, 300, 200, "zwart", 1);
    (void)rechthoek(10, 10, 150, 100, "lichtblauw", 0);
    (void)rechthoek(160, 110, 150, 100, "geel", 1);
    (void)clearscherm("zwart");
    (void)wacht(100);
    (void)tekst(1, 20, "wit", "the quick brown fox jumps over the lazy dog", "pearl", 1, "normaal");
    (void)tekst(1, 100, "magenta", "the quick brown fox jumps over the lazy dog", "acorn", 2, "vet");
    (void)tekst(10, 200, "lichtcyaan", "the quick brown fox jumps over the lazy dog", "pearl", 1, "cursief");
    (void)clearscherm("wit");
    (void)bitMap(1, 50, 50);
    (void)clearscherm("wit");
    (void)bitMap(2, 50, 50);
    (void)clearscherm("wit");
    (void)bitMap(3, 50, 50);
    (void)clearscherm("bruin");
    (void)bitMap(4, 50, 50);
    (void)clearscherm("wit");
    (void)cirkel(120, 160, 25, "groen");
    (void)clearscherm("geel");
    (void)figuur(10, 10, 100, 100, 200, 150, 150, 210, 80, 210, "rood");
    (void)clearscherm("magenta");
    (void)rechthoek(10, 20, 200, 100, "zwart", 1);
    (void)lijn(10, 10, 100, 100, "blauw", 4);
    (void)tekst(10, 50, "groen", "Beep boop ik ben een robot", "pearl", 2, "normaal");
    (void)wacht(2000);
    (void)clearscherm("zwart");
    (void)tekst(10, 10, "geel", "Bijna klaar!!", "pearl", 2, "vet");
    (void)tekst(10, 60, "grijs", "nu alleen nog de herhaalfunctie", "acorn", 1, "cursief");
    (void)wacht(2000);
    (void)herhaal(9, 4);

//    test_APIio();
//    test_APIdraw();
    UART2_WriteString("> ");
    UserInput_t input;

    while (1)
    {
    	if (uart_command_ready)
    	{
    	    // Kopieer de ontvangen string naar de input struct
    	    memset(&input, 0, sizeof(UserInput_t));
    	    strncpy(input.full_command, uart_rx_buffer, MAX_CMD_LENGTH - 1);

    	    // Debug print: hele string zichtbaar maken
    	    UART2_WriteString("\r\n[RX]: \"");
    	    UART2_WriteString(input.full_command);
    	    UART2_WriteString("\"\r\n");

    	    char dbg[64];
    	    snprintf(dbg, sizeof(dbg), "[LEN]: %d\r\n", (int)strlen(input.full_command));
    	    UART2_WriteString("Dit is de string die naar string_ophalen gaat: ");
    	    UART2_WriteString(input.full_command);  // print de string
    	    UART2_WriteString("\r\n");
    	    UART2_WriteString(dbg);

    	    // Verwerk het commando
    	    Handel_UART_Input(&input);

    	    // Stuur de string door naar string_ophalen
    	    string_ophalen(input.full_command);

    	    // Prompt opnieuw
    	    UART2_WriteString("> ");

    	    // Reset de flag en index voor nieuwe input
    	    uart_command_ready = 0;
    	    uart_rx_index = 0;
    	}
    }
}

