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
Voor dit project is gebruikgemaakt van een duidelijk gescheiden high-level en low-level design. 
Deze aanpak zorgt voor overzicht, modulariteit en maakt de software makkelijker te onderhouden, testen en uitbreiden.

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

- Figuur van het Lowlevel design:
[Low_Level_design_software.pdf](https://github.com/user-attachments/files/24368309/Low_Level_design_software.pdf)
---


## Drie lagen model

<img width="318" height="419" alt="Image" src="https://github.com/user-attachments/assets/200018e6-ca82-4de9-a8a1-859ec3b89304" />

## Commando's

---
