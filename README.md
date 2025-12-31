# Software Ontwikkeling user manual

- Cursus: TEET-VESOFTON-16, Software Ontwikkeling.
- Jaar:       3
- Periode:    2
- Docenten:   Franc van der Bent, Michiel Scager.
- Thema's:    Application-design: 3-Tier Model.
              LIB-creation.
              Function Prototype-consistency: API-design.
              Documentatie generator: Doxygen.
              Development: Git, Scrum.
Gemaakt door:
- Luc van den Engel 1857017
- Joost Mooij 1855994
- Thijs Buisman 1855662

## Inhoudsopgave

- [Low level design](#Low-level-design)
- [Drie lagen model](#Drie-lagen-model)
- [Commando's](#Commando's)

---

## Low level design
Dit hoofdstuk beschrijft het low level design van het systeem zoals weergegeven in Figuur X. 
Het ontwerp is opgebouwd uit meerdere lagen die elk een eigen verantwoordelijkheid hebben. 
Door deze gelaagde opzet blijft de code overzichtelijk, beter te debuggen en makkelijker uit te breiden.

**Architectuuroverzicht**

- Het systeem is opgedeeld in vier hoofdlagen:
- Front layer
- Logic layer
- API
- Hardware (VGA)
Elke laag communiceert alleen met de laag direct onder zich. 
Hierdoor is de afhankelijkheid tussen modules beperkt en blijft de implementatie modulair.

**Front Layer**

De front layer is verantwoordelijk voor het ontvangen van gebruikersinput via de UART-interface.
De flow start bij Start/Terminal, waarna de invoer wordt opgehaald met UART_receive().
Vervolgens wordt de ontvangen string gecontroleerd en geparsed met FL_Parse_String().
Als deze functie een fout teruggeeft (bijvoorbeeld wanneer de string te lang is), wordt direct een foutmelding gegenereerd (String is too long) en stopt de verdere verwerking.
Als de input geldig is, wordt deze doorgestuurd naar de logic layer voor verdere verwerking.

**Logic Layer**

De logic layer bevat de kernlogica van het systeem. Hier wordt bepaald welk commando is ingevoerd en welke actie daarbij hoort.
- Met matchesCommand() wordt gecontroleerd of het ontvangen commando bekend is.
- Als het commando onbekend is, wordt handleUnknownCommand() aangeroepen.
- Bekende commando’s worden verwerkt door parseCommand(), die het commando doorstuurt naar de juiste parserfunctie.
Voor elk type commando is een aparte parsefunctie aanwezig, zoals:
- parseLijn()
- parseRechthoek()
- parseTekst()
- parseBitmap()
- parseClearscherm()
Binnen deze functies worden meerdere controles uitgevoerd, zoals:
- trimWhitespace() om onnodige spaties te verwijderen
- hasExtraCharacters() om foutieve input te detecteren
- checkAttribute() voor het valideren van extra parameters
- getColorValue() om de juiste kleurwaarde op te halen
Bij elke stap kan een fout optreden. In dat geval wordt errorHandling() aangeroepen en wordt er een fout teruggegeven aan de bovenliggende laag.

**API layer**

De API-laag vormt de brug tussen de logica en de hardware. Deze laag bevat functies zoals:
- API_draw_line()
- API_draw_rectangle()
- API_draw_text()
- API_draw_bitmap()
- API_clearscreen()
Voor tekst wordt daarnaast API_put_char() gebruikt.
Alle API-functies zetten de logische tekenopdrachten om naar pixelacties.
De API roept uiteindelijk UB_VGA_SetPixel() aan om individuele pixels op het scherm te zetten via het RAM geheugen.

**Figuur van het Lowlevel design:**
<img width="1023" height="839" alt="Image" src="https://github.com/user-attachments/assets/3b0e59a3-8a8b-44c0-a868-6fc30afc5c98" />
[Low_Level_design_software.pdf](https://github.com/user-attachments/files/24368309/Low_Level_design_software.pdf)

---

## Drie lagen architectuur
Dit project is opgebouwd volgens een 3-lagen model om de software overzichtelijk, modulair en goed onderhoudbaar te houden.
Door de verantwoordelijkheden te scheiden is de code makkelijker te testen, uit te breiden en te debuggen.
De drie lagen zijn:
- Front layer
- Logic layer
- I/O layer
Elke laag heeft een duidelijke taak en communiceert alleen met de laag erboven of eronder.

**Overzicht van de datastroom**

Input komt binnen via een terminal (bijvoorbeeld Termite) of een Python testscript.
Deze input wordt als ASCII string via UART (RX) ontvangen en loopt vervolgens door alle lagen heen.
Aan het einde resulteert dit in een aanpassing van de VGA output.

Eventuele fouten worden als string of error code weer teruggestuurd naar bovenliggende lagen.

**Front layer**

De front layer is verantwoordelijk voor het ontvangen en voorbereiden van gebruikersinput.
Taken:
- Ontvangen van ASCII input via UART
- Parseren en structureren van commando’s
- Terugsturen van foutmeldingen als string
Belangrijkste component:
- Command_struct()
Deze functie zet ruwe input om naar een gestructureerd commando dat door de logic layer verwerkt kan worden.
Ongeldige of foutieve input wordt hier al afgevangen.

**Logic layer**

De logic layer bevat de beslislogica van het systeem.
Taken:
- Interpreteren van commando’s
- Dispatchen van commando’s naar de juiste functie
- Teruggeven van error codes (int)
Belangrijkste component:
- Switch_dispatch()
Op basis van het ontvangen commando wordt bepaald welke actie uitgevoerd moet worden, bijvoorbeeld het aanpassen van een pixel of het aanroepen van een API-functie in de I/O layer.

**I/O layer**

De I/O layer regelt alle hardware- en buffergerelateerde functionaliteit.
Taken:
- Beheer van buffers en arrays
- API-aanroepen voor VGA-functionaliteit
- Schrijven naar VGA RAM
- Refreshen van het VGA-scherm
- Afhandelen van hardware-fouten
Belangrijkste componenten:
- EE_API_LIB
- Task_manager()
- Call_Buffer()
- APE_VGA_Function()
- Refresh_vga()
- uint8_t VGA_RAM[]
Wanneer bijvoorbeeld Set_pixel(X, Y, kleur) wordt aangeroepen, wordt via deze laag de juiste pixel in het VGA RAM aangepast en het scherm ververst.

**Errorhandeling**

Fouten worden laag-voor-laag teruggekoppeld:
- I/O layer → error code (int)
- Logic layer → error code (int)
- Front layer → foutmelding als string
De Error_handler zorgt ervoor dat fouten centraal worden afgehandeld en correct worden teruggestuurd naar de gebruiker.

**Figuur van het 3 lagen model:**

<img width="318" height="419" alt="Image" src="https://github.com/user-attachments/assets/200018e6-ca82-4de9-a8a1-859ec3b89304" />

---

## Commando's
Hieronder staan alle beschikbare commando’s die via de terminal of een script kunnen worden aangestuurd.
Elk commando wordt als ASCII-input verstuurd en door het systeem verwerkt volgens het 3-lagen model.

###Lijn

Commando:
lijn, x1, y1, x2, y2, kleur, dikte
Met dit commando wordt een lijn getekend tussen twee punten op het scherm.
- x1, y1: Coördinaten van het beginpunt
- x2, y2: Coördinaten van het eindpunt
- kleur: Kleur van de lijn
- dikte: Dikte van de lijn in pixel

Voorbeeld:
lijn, 10, 20, 100, 200, rood, 2
Tekent een rode lijn van (10, 20) naar (100, 200) met een dikte van 2 pixels.

###Rechthoek

Commando:
rechthoek, x_lup, y_lup, breedte, hoogte, kleur, gevuld
Met dit commando wordt een rechthoek getekend.
- x_lup, y_lup: Linkerbovenhoek van de rechthoek
- breedte: Breedte in pixels
- hoogte: Hoogte in pixels
- kleur: Kleur van de rechthoek
- gevuld:
  1 = gevuld
  0 = niet gevuld

Voorbeeld:
rechthoek, 50, 50, 100, 200, groen, 1
Tekent een groene, gevulde rechthoek.

###Cirkel

Commando:
cirkel, x, y, radius, kleur
Met dit commando wordt een cirkel getekend.
- x, y: Middelpunt van de cirkel
- radius: Straal van de cirkel in pixels
- kleur: Kleur van de cirkel

Voorbeeld:
cirkel, 120, 80, 30, blauw
Tekent een blauwe cirkel met radius 30.

###Tekst

Commando:
tekst, x, y, kleur, tekst, fontnaam, fontgrootte, fontstijl
Met dit commando wordt tekst op het scherm weergegeven.
- x, y: Linkerbovenhoek van de tekst
- kleur: Kleur van de tekst
- tekst: De weer te geven string
- fontnaam: Naam van het lettertype
- fontgrootte: Grootte van de tekst
- fontstijl: Stijl van de tekst (bijv. normaal, vet, cursief)

Voorbeeld:
tekst, 20, 50, blauw, "Hallo wereld", Arial, 2, vet

###Bitmap

Commando:
bitmap, nr, x_lup, y_lup
Met dit commando wordt een vooraf gedefinieerde bitmap getekend.
- nr: Bitmapnummer
- x_lup, y_lup: Linkerbovenhoek van de bitmap

Voorbeeld:
bitmap, 1, 100, 100

###Clearscherm

Commando:
clearscherm, kleur
Wist het volledige scherm en vult het met één kleur.
- kleur: Kleur van de achtergrond

Voorbeeld:
clearscherm, zwart

### Bonus commandos

###Wacht

Commando:
wacht, msecs
Laat het programma een aantal milliseconden wachten.
- msecs: Aantal milliseconden

Voorbeeld:
wacht, 500

###Herhaal

Commando:
herhaal, aantal, hoevaak
Herhaalt eerder uitgevoerde commando’s.
- aantal: Aantal commando’s dat herhaald wordt
- hoevaak: Hoe vaak deze herhaald worden

Voorbeeld:
herhaal, 2, 5

##Figuur

Commando:
figuur, x1, y1, x2, y2, x3, y3, x4, y4, x5, y5, kleur
Tekent een figuur bestaande uit vijf hoekpunten.
- x1 t/m x5, y1 t/m y5: Coördinaten van de hoekpunten
- kleur: Kleur van de figuur

Voorbeeld:
figuur, 10,10, 50,10, 60,40, 30,70, 10,40, geel

###Toren

Commando:
toren, x, y, grootte, kleur1, kleur2
Samengesteld commando dat een toren tekent.
- x, y: Startpositie
- grootte: Grootte van de toren
- kleur1: Hoofdkleur
- kleur2: Accentkleur

Voorbeeld:
toren, 100, 50, 3, grijs, rood

###SetPixel (Bonus aplicatie met spyder)

Commando:
setpixel, x, y, kleur
Zet één pixel op het scherm. Wordt vooral gebruikt voor het pixel-voor-pixel tekenen van een afbeelding.
- x, y: Pixelpositie
- kleur: Kleur van de pixel

Voorbeeld:
setpixel, 10, 10, wit

###Kleuren

Voor alle commando’s waarbij een kleur wordt gebruikt, kan één van de onderstaande kleuren worden opgegeven.
De kleur wordt als tekst meegegeven in het commando.
Beschikbare kleuren:
- zwart
- blauw
- lichtblauw
- groen
- lichtgroen
- cyaan
- lichtcyaan
- rood
- lichtrood
- magenta
- lichtmagenta
- bruin
- geel
- grijs
- wit
- roze
- paars

Voorbeeld:
lijn, 10, 10, 100, 100, lichtgroen, 2

---
