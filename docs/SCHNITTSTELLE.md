# Schnittstelle Frontend <-> Spiellogik

Das Brett ist `Feld spielfeld[8][8]` mit `Feld { char figur; char farbe; }` (siehe `backend.h`).
Fuer Anzeige und Datei wandeln `feld_zeichen` / `feld_aus_zeichen` zwischen Feld und Zeichen um.

Der Vertrag steht in `src/backend.h`. Das Frontend ruft nur diese Funktionen auf.

| Funktion | Zweck | Rueckgabe |
|---|---|---|
| `spielfeld_initialisieren` | Ausgangsstellung | - |
| `feld_ermitteln` | "e2" -> Zeile/Spalte | 1 gueltig, 0 ungueltig |
| `figur_bewegen` | Figur verschieben | `ZUG_OK` oder Fehlercode |
| `spielstand_speichern` | Array in Datei | 1 ok, 0 Fehler |
| `spielstand_laden` | Datei ins Array | 1 ok, 0 Fehler |

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
