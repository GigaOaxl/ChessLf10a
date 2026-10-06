# Frontend-Dokumentation

## Build und Start
```
make        # baut ./schach mit der Attrappe src/stub.c
./schach
```
Mit dem echten Backend: in `Makefile` `src/stub.c` durch dessen Datei ersetzen.

## Dateien
| Datei | Inhalt |
|---|---|
| `src/ui.h`, `src/ui.c` | Anzeige und Eingabe |
| `src/main.c` | Menueschleife |
| `src/backend.h` | Vertrag mit der Spiellogik (Kollege) |
| `src/stub.c` | Attrappe der Spiellogik, nur zum Testen |

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
