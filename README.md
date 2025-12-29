# Software Ontwikkeling user manual

Cursus: TEET-VESOFTON-16, Software Ontwikkeling.
Jaar:       3
Periode:    2
Docenten:   Franc van der Bent, Michiel Scager.
Thema's:    Application-design: 3-Tier Model.
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
###Architectuuroverzicht
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


## Drie lagen model

<img width="318" height="419" alt="Image" src="https://github.com/user-attachments/assets/200018e6-ca82-4de9-a8a1-859ec3b89304" />

## Commando's

---
