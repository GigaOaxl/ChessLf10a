/* ui.h - Anzeige und Eingabe (Frontend). Zeigt nur an, aendert nie das Spielfeld. */
#ifndef UI_H
#define UI_H

/* Loescht die Konsole und setzt den Cursor nach oben links. */
void bildschirm_leeren(void);

/* Zeichnet das Brett mit Rahmen, Spalten a-h und Zeilen 8-1 komplett neu. */
void spielfeld_ausgeben(char spielfeld[8][8]);

/* Zeigt Spieler am Zug und Zugnummer (E1, E3). */
void status_ausgeben(void);

/* Zeigt die letzten Zuege des Zugprotokolls (E5). */
void protokoll_ausgeben(void);

/* Zeigt das Hauptmenue (S7). */
void menue_anzeigen(void);

/* Der komplette Redraw: leeren, Brett, Meldung, Menue. Aufruf nach jeder Aktion. */
void seite_anzeigen(char spielfeld[8][8], const char *meldung);

/* Fragt den Benutzer nach Text und liest eine Zeile (ohne Zeilenumbruch). */
void text_einlesen(const char *frage, char eingabe[], int groesse);

/* Liest ein Feld wie "e2". Rueckgabe: 1 = gueltig, 0 = ungueltig (Text bleibt in feld). */
int feld_einlesen(const char *frage, char feld[], int *zeile, int *spalte);

/* Schreibt die Meldung fuer einen Zugfehler (Code aus backend.h) in meldung.
   Das Spielfeld muss noch vor dem Zug stehen, damit die Figur erkennbar ist. */
void zugfehler_text(int code, char spielfeld[8][8], int startZeile, int startSpalte,
                    int zielZeile, int zielSpalte,
                    const char *startfeld, char meldung[], int groesse);

#endif
