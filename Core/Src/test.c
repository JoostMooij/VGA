#include "APIio.h"
#include "APIerror.h"
#include "logicLayer.h"
#include "APIdraw.h"
#include <stdio.h>

/* ===== HULPFUNCTIES ===== */

static int ErrorList_isOk(const ErrorList* e)
{
    const int* vars[] = {
        &e->error_var1, &e->error_var2, &e->error_var3,
        &e->error_var4, &e->error_var5, &e->error_var6,
        &e->error_var7, &e->error_var8, &e->error_var9,
        &e->error_var10, &e->error_var11
    };

    for (int i = 0; i < 11; i++)
        if (*vars[i] != NO_ERROR)
            return 0;

    return 1;
}

static void UART_Report(const char* naam, const ErrorList* e)
{
    char buf[128];

    if (ErrorList_isOk(e))
    {
        sprintf(buf, "[PASS] %s\n", naam);
        UART2_WriteString(buf);
        return;
    }

    sprintf(buf, "[FAIL] %s\n", naam);
    UART2_WriteString(buf);

    const int* vars[] = {
        &e->error_var1, &e->error_var2, &e->error_var3,
        &e->error_var4, &e->error_var5, &e->error_var6,
        &e->error_var7, &e->error_var8, &e->error_var9,
        &e->error_var10, &e->error_var11
    };

    for (int i = 0; i < 11; i++)
    {
        if (*vars[i] != NO_ERROR)
        {
            sprintf(buf, "  error_var%d: %s\n",
                    i + 1, errorCodeToString(*vars[i]));
            UART2_WriteString(buf);
        }
    }
}

/* ===== API IO TEST ===== */

void test_APIio(void)
{
    UART2_WriteString("\n=== API IO LAYER TEST START ===\n");

    ErrorList err;

    // clearscherm
    err = clearscherm("zwart");
    UART_Report("clearscherm(zwart)", &err);
    err = clearscherm("foutkleur");
    UART_Report("clearscherm(fout)", &err);

    // drawPixel
    err = drawPixel(1, 1, "rood");
    UART_Report("drawPixel geldig", &err);
    err = drawPixel(-1, 1, "rood");
    UART_Report("drawPixel x te klein fout", &err);
    err = drawPixel(321, 1, "rood");
    UART_Report("drawPixel x te groot fout", &err);
    err = drawPixel(1, -1, "rood");
    UART_Report("drawPixel y te klein fout", &err);
    err = drawPixel(1, 241, "rood");
    UART_Report("drawPixel y te groot fout", &err);
    err = drawPixel(1, 1, "foutkleur");
    UART_Report("drawPixel kleur fout", &err);

    // setPixel
    err = setPixel(1, 1, ROOD);
    UART_Report("setPixel geldig", &err);
    err = setPixel(-1, 1, ROOD);
    UART_Report("setPixel x te klein fout", &err);
    err = setPixel(321, 1, ROOD);
    UART_Report("setPixel x te groot fout", &err);
    err = setPixel(1, -1, ROOD);
    UART_Report("setPixel y te klein fout", &err);
    err = setPixel(1, 241, ROOD);
    UART_Report("setPixel y te groot fout", &err);
    err = setPixel(1, 1, -1);
    UART_Report("setPixel kleur fout", &err);

    // wacht
    err = wacht(10);
    UART_Report("wacht(10)", &err);
    err = wacht(0);
    UART_Report("wacht fout", &err);

    // herhaal
    err = herhaal(2, 2);
    UART_Report("herhaal geldig", &err);
    err = herhaal(0, 2);
    UART_Report("herhaal aantal te klein fout", &err);
    err = herhaal(21, 2);
    UART_Report("herhaal aantal te groot fout", &err);
    err = herhaal(2, 50);
    UART_Report("herhaal hoevaak te klein fout", &err);
    err = herhaal(2, 0);
    UART_Report("herhaal hoevaak te groot fout", &err);

    UART2_WriteString("=== API IO LAYER TEST END ===\n");
}

void test_APIdraw(void)
{
    UART2_WriteString("\n=== API DRAW LAYER TEST START ===\n");
    ErrorList err;

    // lijn
    err = lijn(1, 1, 100, 50, "rood", 1);
    UART_Report("lijn geldig", &err);
    err = lijn(-1, 1, 10, 10, "rood", 1);
    UART_Report("lijn x1 te klein", &err);
    err = lijn(1, -1, 10, 10, "rood", 1);
    UART_Report("lijn y1 te klein", &err);
    err = lijn(1, 1, 500, 10, "rood", 1);
    UART_Report("lijn x2 te groot", &err);
    err = lijn(1, 1, 10, 300, "rood", 1);
    UART_Report("lijn y2 te groot", &err);
    err = lijn(1, 1, 10, 10, "foutkleur", 1);
    UART_Report("lijn kleur fout", &err);
    err = lijn(1, 1, 10, 10, "rood", 0);
    UART_Report("lijn dikte te klein", &err);

    // rechthoek
    err = rechthoek(10, 10, 50, 30, "groen", 1);
    UART_Report("rechthoek gevuld geldig", &err);
    err = rechthoek(10, 10, 50, 30, "groen", 0);
    UART_Report("rechthoek rand geldig", &err);
    err = rechthoek(-5, 10, 50, 30, "groen", 1);
    UART_Report("rechthoek x te klein", &err);
    err = rechthoek(10, -5, 50, 30, "groen", 1);
    UART_Report("rechthoek y te klein", &err);
    err = rechthoek(10, 10, 0, 30, "groen", 1);
    UART_Report("rechthoek breedte fout", &err);
    err = rechthoek(10, 10, 50, 0, "groen", 1);
    UART_Report("rechthoek hoogte fout", &err);
    err = rechthoek(10, 10, 50, 30, "foutkleur", 1);
    UART_Report("rechthoek kleur fout", &err);

    // cirkel
    err = cirkel(50, 50, 20, "blauw");
    UART_Report("cirkel geldig", &err);
    err = cirkel(-10, 50, 20, "blauw");
    UART_Report("cirkel x fout", &err);
    err = cirkel(50, -10, 20, "blauw");
    UART_Report("cirkel y fout", &err);
    err = cirkel(50, 50, -5, "blauw");
    UART_Report("cirkel radius fout", &err);
    err = cirkel(50, 50, 20, "foutkleur");
    UART_Report("cirkel kleur fout", &err);

    // toren
    err = toren(100, 100, 20, "geel", "rood");
    UART_Report("toren geldig", &err);
    err = toren(-10, 100, 20, "geel", "rood");
    UART_Report("toren x fout", &err);
    err = toren(100, 500, 20, "geel", "rood");
    UART_Report("toren y fout", &err);
    err = toren(100, 100, 0, "geel", "rood");
    UART_Report("toren grootte fout", &err);
    err = toren(100, 100, 20, "foutkleur", "rood");
    UART_Report("toren kleur1 fout", &err);
    err = toren(100, 100, 20, "geel", "foutkleur");
    UART_Report("toren kleur2 fout", &err);

    // bitMap
    err = bitMap(1, 10, 10);
    UART_Report("bitMap geldig", &err);
    err = bitMap(20, 10, 10);
    UART_Report("bitMap nr fout", &err);

    // tekst
    err = tekst(10, 10, "zwart", "Hallo", "pearl", 1, "normaal");
    UART_Report("tekst geldig", &err);
    err = tekst(-5, 10, "zwart", "Hallo", "pearl", 1, "normaal");
    UART_Report("tekst x fout", &err);
    err = tekst(10, -5, "zwart", "Hallo", "pearl", 1, "normaal");
    UART_Report("tekst y fout", &err);
    err = tekst(10, 10, "foutkleur", "Hallo", "pearl", 1, "normaal");
    UART_Report("tekst kleur fout", &err);
    err = tekst(10, 10, "zwart", "", "pearl", 1, "normaal");
    UART_Report("tekst leeg fout", &err);
    err = tekst(10, 10, "zwart", "Hallo", "pearl", 1, "vet");
    UART_Report("tekst vet stijl geldig", &err);

    UART2_WriteString("=== API DRAW LAYER TEST END ===\n");
}
