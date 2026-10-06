# ChessLf10a - Konsolenschach in C

Unterrichtsprojekt (Fachinformatiker/in Anwendungsentwicklung). Gewaehlte Stufe: **EXPERTEN**
(schliesst BASIS und STANDARD ein). Aufgabenstellung: `docs/PRG_Builder_Schach.docx`.

## Bauen und starten
```
make          # baut ./schach (Frontend + echtes Backend)
./schach
make test     # automatische Tests der Spiellogik
```

## Aufbau
| Datei | Inhalt |
|---|---|
| `src/main.c` | Menueschleife |
| `src/ui.c`, `src/ui.h` | Anzeige, Eingabe, Fehlertexte (Frontend) |
| `src/backend.h` | Vertrag zwischen Frontend und Spiellogik |
| `src/backend.c` | Spiellogik: Zugregeln, Spielerwechsel, Protokoll, Speichern/Laden |
| `tests/test_backend.c` | 51 automatische Tests |
| `docs/` | PRD, Schnittstelle, Frontend-Doku, Aufgabenstellung |

Details: `docs/FRONTEND.md`, `docs/SCHNITTSTELLE.md`.

## Stand der Aufgaben

Legende: ✅ fertig und getestet | ⚠️ teilweise | ❌ offen

### BASIS (50 Punkte)
| Nr | Aufgabe | Status | Wo |
|---|---|---|---|
| B1 | Zweidimensionales Array `Feld spielfeld[8][8]` (zuerst `char`, jetzt `struct Feld`) | ✅ | `main.c`, `backend.h` |
| B2 | `spielfeld_initialisieren` (Ausgangsstellung) | ✅ | `backend.c` |
| B3 | `spielfeld_ausgeben` | ✅ | `ui.c` |
| B4 | `figur_bewegen` | ✅ | `backend.c` |
| B5 | Zug eingeben, Koordinaten in Indizes umwandeln | ✅ | `ui.c`, `feld_ermitteln` |
| B6 | Hauptmenue | ✅ | `ui.c`, `main.c` |
| Zusatz | Speichern (3) und Laden (4) | ✅ | `backend.c` |

### STANDARD (75 Punkte)
| Nr | Aufgabe | Status | Wo |
|---|---|---|---|
| S1 | Funktionen strukturiert (`feld_ermitteln` usw.) | ✅ | `backend.c`, `ui.c` |
| S2 | Eingaben pruefen (`z1`, `a9`, `x`, `abc`) | ✅ | `feld_ermitteln` |
| S3 | Leeres Startfeld erkennen | ✅ | `figur_bewegen` |
| S4 | `spielstand_speichern` | ✅ | `backend.c` |
| S5 | `spielstand_laden` | ✅ | `backend.c` |
| S6 | Dateifehler behandeln | ✅ | `backend.c`, `main.c` |
| S7 | Menue 1-5 und 0 | ✅ | `ui.c` |
| S8 | Nach gueltigem Zug "Zug erfolgreich." + Brett | ✅ | `main.c` |

### EXPERTEN (100 Punkte)
| Nr | Aufgabe | Status | Hinweis |
|---|---|---|---|
| E1 | Spieler am Zug (`aktuellerSpieler`), Wechsel nach jedem Zug | ✅ | Anzeige `Zug N: Weiß ist am Zug ...` |
| E2 | Nur eigene Figuren bewegen | ✅ | `FEHLER: Diese Figur gehört nicht zu ...` |
| E3 | Zugnummer | ✅ | zaehlt jeden Halbzug (1. Weiss, 2. Schwarz, ...) |
| E4 | Spielstand mit Spieler und Zugnummer | ✅ | Format siehe unten |
| E5 | Zugprotokoll | ✅ | `1. e2 -> e4`, Anzeige der letzten 3 Zuege, Datei enthaelt alle |
| E6 | Figuren schlagen | ✅ | |
| E7 | Regeln Turm, Laeufer, Springer | ✅ | Fehlertexte je Figur im Frontend (`ui.c`), z.B. `Ein Turm kann sich nicht diagonal bewegen.` |
| Erweiterung | Dame, Koenig, Bauer | ✅ | Bauer ohne en passant und Umwandlung |

### Weitere Punkte der Aufgabenstellung
| Punkt | Status |
|---|---|
| Optional: `struct Feld` (Figur + Farbe getrennt) | ✅ `backend.h`, Datei-Format blieb unveraendert |
| 10 Testfaelle | ✅ Faelle 1-5 und 7-10 als automatische Tests (`make test`), Fall 6 (Beenden) nur manuell im Menue |
| Abgabe: Screenshot des Spielfelds | ✅ Textausgabe in `docs/ABGABE.md` |
| Abgabe: kurze Beschreibung der Loesung | ✅ `docs/ABGABE.md` |
| Abgabe: Dokumentation der Funktionen | ✅ `docs/ABGABE.md` (Backend und Frontend) |
| Abgabe: Beschreibung der implementierten Schachregeln | ✅ Abschnitt "Schachregeln im Spiel" |
| Abgabe: gespeicherter Spielstand als Beispiel | ✅ `docs/beispiel_spielstand.txt` |
| Reflexionsaufgabe (7 Fragen) | ✅ `docs/REFLEXION.md` |

## Schachregeln im Spiel
- Weiss (Grossbuchstaben) beginnt, danach wechseln die Spieler.
- Turm: gerade, Laeufer: diagonal, Dame: beides, jeweils nur bei freiem Weg.
- Springer: L-Form, darf Figuren ueberspringen.
- Koenig: ein Feld in jede Richtung.
- Bauer: 1 Feld vor, 2 Felder aus der Grundreihe, diagonal nur zum Schlagen.
- Schlagen: die gegnerische Figur auf dem Zielfeld wird ueberschrieben. Eigene Figuren duerfen nicht geschlagen werden.
- **Nicht enthalten:** Schach, Schachmatt, Rochade, en passant, Bauernumwandlung. Der Koenig kann also ins Schach ziehen.

## Spielstand-Format (`spielstand.txt`)
```
3                  Zeile 1: Zugnummer
W                  Zeile 2: Spieler am Zug (W oder S)
tsldklst           Zeilen 3-10: Spielfeld, Zeile 8 zuerst, 8 Zeichen je Zeile
bbbb.bbb
........
....b...
....B...
........
BBBB.BBB
TSLDKLST
2                  Zeile 11: Anzahl Protokollzeilen
1. e2 -> e4        danach: je Zug eine Zeile
2. e7 -> e5
```
Beim Laden wird die Datei komplett geprueft. Ist sie fehlerhaft, bleibt der laufende Spielstand unveraendert.

## Hinweis zur Aufgabenstellung
Das Beispielbrett im Aufgabenblatt zeigt `T L S D K S L T` (Laeufer auf b, Springer auf c).
Das ist falsch. Wir verwenden die echte Aufstellung `T S L D K L S T`.

## Bekannte Grenzen
- Jedes Feld ist ein `Feld`-struct mit `figur` und `farbe`. In der Spielstand-Datei steht Weiss als Grossbuchstabe, Schwarz als Kleinbuchstabe (`feld_zeichen` in `backend.h`).
- Das Protokoll fasst 500 Zuege; danach werden Zuege noch ausgefuehrt, aber nicht mehr protokolliert.
- Der Bildschirm wird mit ANSI-Escape-Codes geleert (Linux-Terminal, moderne Windows-Terminals).
