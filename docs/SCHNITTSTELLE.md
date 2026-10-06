# Schnittstelle Frontend <-> Spiellogik

Das Brett ist `Feld spielfeld[8][8]` mit `Feld { char figur; char farbe; }` (siehe `backend.h`).
Fuer Anzeige und Datei wandelt `feld_zeichen` ein Feld in ein Zeichen um.

Der Vertrag steht in `src/backend.h` (Funktionen dort kommentiert, Uebersicht in `docs/ABGABE.md`).
Neu fuer das Frontend: `startfeld_pruefen` meldet leeres oder fremdes Startfeld sofort (Early Exit).

## Fehlercode -> Meldung (macht das Frontend)
| Code | Text |
|---|---|
| `ZUG_STARTFELD_LEER` | FEHLER: Auf <feld> befindet sich keine Figur. |
| `ZUG_FALSCHE_FARBE` | FEHLER: Diese Figur gehoert nicht zu <Spieler>. |
| `ZUG_UNGUELTIG` | FEHLER: Ungueltiger Zug. |

## Experten-Daten (extern in backend.h)
Der Kollege definiert sie, das Frontend liest sie nur zur Anzeige:
`aktuellerSpieler` ('W'/'S'), `zugnummer` (beginnt bei 1), `zugprotokoll[][16]`
(z.B. "1. e2 -> e4") und `zugprotokollAnzahl`.
`spielfeld_initialisieren` setzt diese Werte auch zurueck (Neues Spiel).
