/* main.c - Menueschleife. Nach jeder Aktion wird die Seite komplett neu gezeichnet. */
#include <stdio.h>
#include <stdlib.h>
#include "backend.h"
#include "ui.h"

#define DATEI_STANDARD "spielstand.txt"

/*
 * Fragt Start- und Zielfeld ab und fuehrt den Zug aus. Jeder Fehler beendet die Eingabe sofort:
 * ein ungueltiges, leeres oder fremdes Startfeld wird gemeldet, BEVOR das Zielfeld gefragt wird.
 */
static void zug_eingeben(Feld spielfeld[8][8], char meldung[], int groesse)
{
    char start[16], ziel[16];
    int startZeile, startSpalte, zielZeile, zielSpalte;

    if (!feld_einlesen("Figur auf Feld (z.B. e2): ", start, &startZeile, &startSpalte)) {
        snprintf(meldung, groesse, "FEHLER: Ungültiges Spielfeld.");
        return;
    }

    Feld figur = spielfeld[startZeile][startSpalte];
    if (figur.figur == '.') {
        zugfehler_text(ZUG_STARTFELD_LEER, spielfeld, startZeile, startSpalte, startZeile, startSpalte,
                       start, meldung, groesse);
        return;
    }
    if (figur.farbe != aktuellerSpieler) {
        zugfehler_text(ZUG_FALSCHE_FARBE, spielfeld, startZeile, startSpalte, startZeile, startSpalte,
                       start, meldung, groesse);
        return;
    }

    if (!feld_einlesen("Ziel-Feld (z.B. e4): ", ziel, &zielZeile, &zielSpalte)) {
        snprintf(meldung, groesse, "FEHLER: Ungültiges Spielfeld.");
        return;
    }

    int code = figur_bewegen(spielfeld, startZeile, startSpalte, zielZeile, zielSpalte);
    if (code == ZUG_OK) {
        snprintf(meldung, groesse, "Zug erfolgreich.");
    } else {
        zugfehler_text(code, spielfeld, startZeile, startSpalte, zielZeile, zielSpalte,
                       start, meldung, groesse);
    }
}

int main(void)
{
    Feld spielfeld[8][8];
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
            zug_eingeben(spielfeld, meldung, sizeof meldung);
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
