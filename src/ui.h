/* ui.h - Anzeige und Eingabe (Frontend). Zeigt nur an, aendert nie das Spielfeld. */
#ifndef UI_H
#define UI_H

#include "backend.h"

/* Der komplette Redraw: leeren, Brett, Status, Protokoll, Meldung, Menue. Aufruf nach jeder Aktion. */
void seite_anzeigen(Feld spielfeld[8][8], const char *meldung);

/* Fragt den Benutzer nach Text und liest eine Zeile (ohne Zeilenumbruch). */
void text_einlesen(const char *frage, char eingabe[], int groesse);

/* Liest ein Feld wie "e2". Rueckgabe: 1 = gueltig, 0 = ungueltig. */
int feld_einlesen(const char *frage, int *zeile, int *spalte);

/* Schreibt die Meldung fuer einen Zugfehler (Code aus backend.h) in meldung.
   Das Spielfeld muss noch vor dem Zug stehen, damit die Figur erkennbar ist. */
void zugfehler_text(int code, Feld spielfeld[8][8], int startZeile, int startSpalte,
                    int zielZeile, int zielSpalte, char meldung[], int groesse);

#endif
