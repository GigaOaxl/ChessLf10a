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

## Funktionen (ui.c)
| Funktion | Aufgabe |
|---|---|
| `bildschirm_leeren` | Konsole leeren (ANSI) |
| `spielfeld_ausgeben` | Brett mit Rahmen, a-h, 8-1 zeichnen |
| `status_ausgeben` | Zugnummer und Spieler am Zug |
| `protokoll_ausgeben` | letzte 5 Zuege |
| `menue_anzeigen` | Hauptmenue 1-5, 0 |
| `seite_anzeigen` | kompletter Redraw: leeren, Brett, Status, Protokoll, Meldung, Menue |
| `text_einlesen` | eine Zeile lesen |
| `feld_einlesen` | Feld wie "e2" lesen und per `feld_ermitteln` pruefen |
| `zugfehler_text` | Fehlercode -> Meldung `FEHLER: ...` |

## Early Exit bei der Zugeingabe
`zug_eingeben` (in `main.c`) meldet ein ungueltiges, leeres oder fremdes Startfeld sofort und fragt
dann gar nicht erst nach dem Zielfeld.

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
