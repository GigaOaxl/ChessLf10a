# Schnittstelle Frontend <-> Spiellogik

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

## Fuer Experten-Stufe (spaeter, M4)
Der Kollege stellt zusaetzlich bereit: `aktuellerSpieler` ('W'/'S'), `zugnummer`
und das Zugprotokoll. Das Frontend liest sie nur zur Anzeige.
