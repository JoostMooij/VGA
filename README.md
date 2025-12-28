# Software Ontwikkeling

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

- [High & low level design](#High-&-low-level-design)
- [Drie lagen model](#Drie-lagen-model)
- [Commando's](#Commando's)


## High & low level design
Voor dit project is gebruikgemaakt van een duidelijk gescheiden high-level en low-level design. 
Deze aanpak zorgt voor overzicht, modulariteit en maakt de software makkelijker te onderhouden, testen en uitbreiden.

### High-level design
Het high-level design beschrijft de globale architectuur van de applicatie en de onderlinge communicatie tussen de verschillende onderdelen. De software is opgezet volgens het 3-lagenmodel:

- Front layer
  Deze laag verzorgt de communicatie met de gebruiker. Input wordt aangeleverd via een terminal (zoals Termite) of via een Python testscript.
  De input bestaat uit ASCII-tekens die via UART worden ontvangen. Deze data wordt omgezet naar een gestructureerd commando (command_struct) en doorgestuurd naar de logic layer.
  Eventuele fouten worden als string teruggekoppeld naar de gebruiker.

- Logic layer
  De logic layer bevat de kernlogica van de applicatie. Binnen deze laag worden de ontvangen commando’s geïnterpreteerd en via een switch_dispatch doorgestuurd naar de bijbehorende functie.
  Deze laag bepaalt wat er moet gebeuren, maar is onafhankelijk van de hardware. Fouten worden teruggegeven als foutcodes (int).

- I/O layer
  De I/O layer handelt de hardware-afhankelijke functionaliteit af. Hier bevinden zich onder andere de EE_API_LIB en de VGA-functionaliteit.
  Deze laag schrijft direct naar het VGA-geheugen en zorgt ervoor dat wijzigingen uiteindelijk zichtbaar worden op het VGA-scherm.

### Low-level design
Het low-level design beschrijft de concrete implementatie van functies, datastructuren en de onderlinge aanroepen tussen de lagen.

- Command verwerking
  De ontvangen ASCII-input wordt geparsed naar een command_struct.
  Dit struct bevat het commando en de bijbehorende parameters, zoals X- en Y-coördinaten en kleurinformatie.

- Dispatch mechanisme
  Met behulp van een switch_dispatch-constructie wordt elk commando gekoppeld aan de juiste functie.
  Dit maakt het eenvoudig om nieuwe commando’s toe te voegen zonder bestaande code aan te passen.

- API en libraries
  De logic layer communiceert met de I/O layer via functies uit de EE_API_LIB.
  Deze API zorgt voor consistente function prototypes en een duidelijke scheiding tussen logica en hardware.

- VGA aansturing
  Pixelinformatie wordt opgeslagen in een buffer (VGA_RAM1). Wanneer een pixel wordt aangepast, wordt deze buffer bijgewerkt en vervolgens wordt Refresh_vga() aangeroepen om het beeld te verversen.
  De VGA-buffer wordt daarna uitgelezen en weergegeven op het VGA-scherm.

- Foutafhandeling
  Fouten worden centraal afgehandeld via een error handler.
  Elke functie retourneert een foutcode (int), zodat fouten eenvoudig te traceren zijn door de volledige softwareketen.
---


## Drie lagen model


## Commando's

---
