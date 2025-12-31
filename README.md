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


## Low level design
Dit hoofdstuk beschrijft het low level design van het systeem zoals weergegeven in Figuur X. 
Het ontwerp is opgebouwd uit meerdere lagen die elk een eigen verantwoordelijkheid hebben. 
Door deze gelaagde opzet blijft de code overzichtelijk, beter te debuggen en makkelijker uit te breiden.
### Architectuuroverzicht
- Het systeem is opgedeeld in vier hoofdlagen:
- Front layer
- Logic layer
- API
- Hardware (VGA)
Elke laag communiceert alleen met de laag direct onder zich. 
Hierdoor is de afhankelijkheid tussen modules beperkt en blijft de implementatie modulair.

---

### Front Layer

De front layer is verantwoordelijk voor het ontvangen van gebruikersinput via de UART-interface.
De flow start bij Start/Terminal, waarna de invoer wordt opgehaald met UART_receive().
Vervolgens wordt de ontvangen string gecontroleerd en geparsed met FL_Parse_String().
Als deze functie een fout teruggeeft (bijvoorbeeld wanneer de string te lang is), wordt direct een foutmelding gegenereerd (String is too long) en stopt de verdere verwerking.
Als de input geldig is, wordt deze doorgestuurd naar de logic layer voor verdere verwerking.

---

### Logic Layer
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

---

### API layer

De API-laag vormt de brug tussen de logica en de hardware. Deze laag bevat functies zoals:
- API_draw_line()
- API_draw_rectangle()
- API_draw_text()
- API_draw_bitmap()
- API_clearscreen()
Voor tekst wordt daarnaast API_put_char() gebruikt.
Alle API-functies zetten de logische tekenopdrachten om naar pixelacties.
De API roept uiteindelijk UB_VGA_SetPixel() aan om individuele pixels op het scherm te zetten via het RAM geheugen.

---

### Figuur van het Lowlevel design:
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

--- 

### Overzicht van de datastroom
Input komt binnen via een terminal (bijvoorbeeld Termite) of een Python testscript.
Deze input wordt als ASCII string via UART (RX) ontvangen en loopt vervolgens door alle lagen heen.
Aan het einde resulteert dit in een aanpassing van de VGA output.

Eventuele fouten worden als string of error code weer teruggestuurd naar bovenliggende lagen.

---

### Front layer
De front layer is verantwoordelijk voor het ontvangen en voorbereiden van gebruikersinput.
Taken:
- Ontvangen van ASCII input via UART
- Parseren en structureren van commando’s
- Terugsturen van foutmeldingen als string
Belangrijkste component:
- Command_struct()
Deze functie zet ruwe input om naar een gestructureerd commando dat door de logic layer verwerkt kan worden.
Ongeldige of foutieve input wordt hier al afgevangen.

---

### Logic layer
De logic layer bevat de beslislogica van het systeem.
Taken:
- Interpreteren van commando’s
- Dispatchen van commando’s naar de juiste functie
- Teruggeven van error codes (int)
Belangrijkste component:
- Switch_dispatch()
Op basis van het ontvangen commando wordt bepaald welke actie uitgevoerd moet worden, bijvoorbeeld het aanpassen van een pixel of het aanroepen van een API-functie in de I/O layer.

---

### I/O layer
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

---

### Errorhandeling
Fouten worden laag-voor-laag teruggekoppeld:
- I/O layer → error code (int)
- Logic layer → error code (int)
- Front layer → foutmelding als string
De Error_handler zorgt ervoor dat fouten centraal worden afgehandeld en correct worden teruggestuurd naar de gebruiker.

---

### Figuur van het 3 lagen model:
<img width="318" height="419" alt="Image" src="https://github.com/user-attachments/assets/200018e6-ca82-4de9-a8a1-859ec3b89304" />

## Commando's

---
