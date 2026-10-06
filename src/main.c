/* main.c - Menueschleife. Nach jeder Aktion wird die Seite komplett neu gezeichnet. */
#include <stdio.h>
#include <stdlib.h>
#include "backend.h"
#include "ui.h"

#define SPIELSTAND_DATEI "spielstand.txt"

/* Menuepunkte (so steht es auch im Menue). */
enum {
    MENUE_BEENDEN = 0,
    MENUE_ANZEIGEN,
    MENUE_ZIEHEN,
    MENUE_SPEICHERN,
    MENUE_LADEN,
    MENUE_NEUES_SPIEL
};

/*
 * Fragt Start- und Zielfeld ab und fuehrt den Zug aus. Jeder Fehler beendet die Eingabe sofort:
 * ein ungueltiges, leeres oder fremdes Startfeld wird gemeldet, BEVOR das Zielfeld gefragt wird.
 */
static void zug_eingeben(Feld spielfeld[8][8], char meldung[], int groesse)
{
    int startZeile, startSpalte, zielZeile, zielSpalte;

    if (!feld_einlesen("Figur auf Feld (z.B. e2): ", &startZeile, &startSpalte)) {
        snprintf(meldung, groesse, "FEHLER: Ungültiges Spielfeld.");
        return;
    }

    int code = startfeld_pruefen(spielfeld, startZeile, startSpalte);
    if (code != ZUG_OK) {
        /* Es gibt noch kein Zielfeld: fuer diese Fehler zaehlt nur das Startfeld, daher Ziel = Start. */
        zugfehler_text(code, spielfeld, startZeile, startSpalte, startZeile, startSpalte, meldung, groesse);
        return;
    }

    if (!feld_einlesen("Ziel-Feld (z.B. e4): ", &zielZeile, &zielSpalte)) {
        snprintf(meldung, groesse, "FEHLER: Ungültiges Spielfeld.");
        return;
    }

    code = figur_bewegen(spielfeld, startZeile, startSpalte, zielZeile, zielSpalte);
    if (code == ZUG_OK) {
        snprintf(meldung, groesse, "Zug erfolgreich.");
    } else {
        zugfehler_text(code, spielfeld, startZeile, startSpalte, zielZeile, zielSpalte, meldung, groesse);
    }
}

int main(void)
{
    Feld spielfeld[8][8];
    char meldung[100] = "Willkommen! Weiß beginnt.";   /* Text unter dem Brett (Erfolg oder FEHLER) */
    char auswahlText[16];
    int auswahl = -1;

    spielfeld_initialisieren(spielfeld);

    while (auswahl != MENUE_BEENDEN) {
        seite_anzeigen(spielfeld, meldung);
        meldung[0] = '\0';

        text_einlesen("Auswahl (Zahl): ", auswahlText, sizeof auswahlText);
        auswahl = atoi(auswahlText);

        switch (auswahl) {
        case MENUE_BEENDEN:
            break;
        case MENUE_ANZEIGEN:
            break;                      /* nichts zu tun: die Seite wird ohnehin neu gezeichnet */
        case MENUE_ZIEHEN:
            zug_eingeben(spielfeld, meldung, sizeof meldung);
            break;
        case MENUE_SPEICHERN:
            if (spielstand_speichern(spielfeld, SPIELSTAND_DATEI)) {
                snprintf(meldung, sizeof meldung, "Spielstand gespeichert.");
            } else {
                snprintf(meldung, sizeof meldung, "FEHLER: Spielstand konnte nicht gespeichert werden.");
            }
            break;
        case MENUE_LADEN: {
            char dateiname[64];
            text_einlesen("Dateiname: ", dateiname, sizeof dateiname);
            if (spielstand_laden(spielfeld, dateiname)) {
                snprintf(meldung, sizeof meldung, "Spielstand geladen.");
            } else {
                snprintf(meldung, sizeof meldung, "FEHLER: Spielstand konnte nicht geladen werden.");
            }
            break;
        }
        case MENUE_NEUES_SPIEL:
            spielfeld_initialisieren(spielfeld);
            snprintf(meldung, sizeof meldung, "Neues Spiel gestartet.");
            break;
        default:
            snprintf(meldung, sizeof meldung, "FEHLER: Ungültige Auswahl.");
        }
    }
    return 0;
}
