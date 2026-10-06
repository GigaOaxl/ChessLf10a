/* main.c - Menueschleife. Nach jeder Aktion wird die Seite komplett neu gezeichnet. */
#include <stdio.h>
#include <stdlib.h>
#include "backend.h"
#include "ui.h"

#define DATEI_STANDARD "spielstand.txt"

int main(void)
{
    char spielfeld[8][8];
    char meldung[100] = "Willkommen! Weiß beginnt.";
    char eingabe[16];
    int auswahl = -1;

    spielfeld_initialisieren(spielfeld);

    while (auswahl != 0) {
        seite_anzeigen(spielfeld, meldung);
        meldung[0] = '\0';

        text_einlesen("Auswahl (Zahl): ", eingabe, sizeof eingabe);
        auswahl = atoi(eingabe);

        if (auswahl == 1) {
            /* nichts zu tun: die Seite wird ohnehin neu gezeichnet */
        } else if (auswahl == 2) {
            char start[16], ziel[16];
            int startZeile, startSpalte, zielZeile, zielSpalte;

            if (!feld_einlesen("Figur auf Feld (z.B. e2): ", start, &startZeile, &startSpalte) ||
                !feld_einlesen("Ziel-Feld (z.B. e4): ", ziel, &zielZeile, &zielSpalte)) {
                snprintf(meldung, sizeof meldung, "FEHLER: Ungültiges Spielfeld.");
            } else {
                int code = figur_bewegen(spielfeld, startZeile, startSpalte, zielZeile, zielSpalte);
                if (code == ZUG_OK) {
                    snprintf(meldung, sizeof meldung, "Zug erfolgreich.");
                } else {
                    zugfehler_text(code, start, meldung, sizeof meldung);
                }
            }
        } else if (auswahl == 3) {
            if (spielstand_speichern(spielfeld, DATEI_STANDARD)) {
                snprintf(meldung, sizeof meldung, "Spielstand gespeichert.");
            } else {
                snprintf(meldung, sizeof meldung, "FEHLER: Spielstand konnte nicht gespeichert werden.");
            }
        } else if (auswahl == 4) {
            char dateiname[64];
            text_einlesen("Dateiname: ", dateiname, sizeof dateiname);
            if (spielstand_laden(spielfeld, dateiname)) {
                snprintf(meldung, sizeof meldung, "Spielstand geladen.");
            } else {
                snprintf(meldung, sizeof meldung, "FEHLER: Spielstand konnte nicht geladen werden.");
            }
        } else if (auswahl == 5) {
            spielfeld_initialisieren(spielfeld);
            snprintf(meldung, sizeof meldung, "Neues Spiel gestartet.");
        } else if (auswahl != 0) {
            snprintf(meldung, sizeof meldung, "FEHLER: Ungültige Auswahl.");
        }
    }
    return 0;
}
