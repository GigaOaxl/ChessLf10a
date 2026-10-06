    a b c d e f g h
  +-----------------+
8 | t s l . k l s t |
7 | b b b . b b b b |
6 | . . . . . . . . |
5 | . . . d . . . . |
4 | . . . . . . . . |
3 | . . . . . . . . |
2 | B B B B . B B B |
1 | T S L D K L S T |
  +-----------------+

Zug 5: Weiß ist am Zug (GROSSE Buchstaben, unten).
Letzte Züge:
2. d7 -> d5
3. e4 -> d5
4. d8 -> d5
Zug erfolgreich.

=== C-SCHACH ===
1 - Spielfeld anzeigen    4 - Spielstand laden
2 - Figur bewegen         5 - Neues Spiel
3 - Spielstand speichern  0 - Beenden

```

## Dokumentation der Funktionen
### Spiellogik (`backend.h`)
| Funktion | Aufgabe |
|---|---|
| `spielfeld_initialisieren` | Ausgangsstellung, setzt Spieler, Zugnummer und Protokoll zurück |
| `feld_ermitteln` | `"e2"` in Zeile/Spalte umwandeln, prüft die Eingabe (1 gültig, 0 ungültig) |
| `startfeld_pruefen` | Prüft nur das Startfeld (leer / fremde Figur), Grundlage des Early Exits |
| `figur_bewegen` | Prüft und führt einen Zug aus. Gibt `ZUG_OK` oder einen Fehlercode zurück |
| `spielstand_speichern` | Schreibt Zugnummer, Spieler, Brett und Protokoll in eine Textdatei |
| `spielstand_laden` | Liest und prüft die Datei, bei Fehler bleibt der Spielstand unverändert |

### Frontend (`ui.h`)
| Funktion | Aufgabe |
|---|---|
| `bildschirm_leeren` | Konsole und Scrollback leeren |
| `spielfeld_ausgeben` | Brett mit Rahmen, Spalten a-h und Zeilen 8-1 |
| `status_ausgeben` | Zugnummer und Spieler am Zug |
| `protokoll_ausgeben` | Letzte 3 Züge |
| `menue_anzeigen` | Hauptmenü |
| `seite_anzeigen` | Kompletter Redraw (leeren, Brett, Status, Protokoll, Meldung, Menü) |
| `text_einlesen` / `feld_einlesen` | Zeile lesen / Feld wie `e2` lesen und prüfen |
| `zugfehler_text` | Fehlercode in verständliche Meldung (z. B. "Ein Turm kann sich nicht diagonal bewegen.") |

## Beschreibung der Schachregeln
Siehe Abschnitt "Schachregeln im Spiel" im `README.md`.

## Zugprotokoll und Speicherformat
Beispiel: `docs/beispiel_spielstand.txt` (Zugnummer, Spieler, 8 Brettzeilen, Anzahl Züge, Zugliste).
Format-Erklärung im `README.md`.

## Testfälle
`make test` führt 45 automatische Tests der Spiellogik aus (gültige und ungültige Züge, Schlagen, Speichern/Laden).
