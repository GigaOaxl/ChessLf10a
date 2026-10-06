# PRD - Frontend Konsolenschach

## Ziel
Eine simple Konsolenoberflaeche, die sich nach jeder Aktion selbst aktualisiert.
Ein gezogener Bauer steht danach nur noch auf dem neuen Feld, nie doppelt.

## Aufteilung
| Wer | Aufgabe |
|---|---|
| Frontend (dieses Dokument) | Anzeige, Menue, Eingabe, Meldungen, `main.c` |
| Kollege | Spiellogik, Zugregeln, Speichern/Laden (`backend.h`) |

## Nicht-Ziele
Schachregeln, Dateiformat, Farben/Grafik. Alles bewusst einfach (KISS).

## Prinzip "self updating"
Das Array `spielfeld` ist die einzige Wahrheit. Nach jeder Aktion:
1. Bildschirm loeschen
2. Brett neu aus dem Array zeichnen
3. Statuszeile (Erfolg/Fehler) ausgeben
4. Menue ausgeben

Die Oberflaeche speichert selbst keinen Spielzustand.

## Anforderungen
| Nr | Anforderung | Quelle |
|---|---|---|
| F1 | Brett mit Rahmen, Spalten a-h, Zeilen 8-1 | B3 |
| F2 | Menue mit 1-5 und 0 | B6, S7 |
| F3 | Eingabe Startfeld/Zielfeld (z.B. e2) | B5 |
| F4 | Fehlertexte `FEHLER: ...` | S2, S3, S6 |
| F5 | Nach gueltigem Zug "Zug erfolgreich." + neues Brett | S8 |
| F6 | Anzeige Spieler, Zugnummer, Zugprotokoll | E1, E3, E5 |

## Akzeptanz
- Nach `e2 -> e4` zeigt das Brett den Bauern nur auf e4.
- Jeder Fehler erscheint als Einzeiler.
- Code ist ohne lange Kommentare lesbar (sprechende deutsche Namen).

## Meilensteine
M1 Vertrag + Doku | M2 Brettanzeige | M3 Menue/Eingabe | M4 Experten-Anzeige | M5 Tests + Doku
