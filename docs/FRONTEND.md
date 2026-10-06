# Frontend-Dokumentation

## Build und Start
```
make        # baut ./schach (Frontend + Backend)
./schach
```

## Dateien
| Datei | Inhalt |
|---|---|
| `src/ui.h`, `src/ui.c` | Anzeige und Eingabe |
| `src/main.c` | Menueschleife |
| `src/backend.h` | Vertrag mit der Spiellogik (Kollege) |
| `src/backend.c` | Spiellogik |

## Funktionen
Siehe `docs/ABGABE.md`. Nach aussen sichtbar sind nur `seite_anzeigen`, `text_einlesen`,
`feld_einlesen` und `zugfehler_text` (`ui.h`); der Rest in `ui.c` ist `static`.

## Early Exit bei der Zugeingabe
`zug_eingeben` (in `main.c`) meldet ein ungueltiges, leeres oder fremdes Startfeld sofort
(Pruefung per `startfeld_pruefen` aus dem Backend) und fragt dann nicht nach dem Zielfeld.

## Self-updating
`main.c` ruft nach jeder Aktion `seite_anzeigen` auf. Das Brett wird immer neu aus
dem Array gezeichnet, nichts wird angehaengt. Eine bewegte Figur steht deshalb nie doppelt.

## Beispielausgabe (nach e2 -> e4)
```
    a b c d e f g h
  +-----------------+
8 | t s l d k l s t |
7 | b b b b b b b b |
6 | . . . . . . . . |
5 | . . . . . . . . |
4 | . . . . B . . . |
3 | . . . . . . . . |
2 | B B B B . B B B |
1 | T S L D K L S T |
  +-----------------+

Zugnummer: 2
Spieler: Schwarz

Zugprotokoll:
1. e2 -> e4

Zug erfolgreich.
```
