/* ui.c - Anzeige und Eingabe (Frontend). */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "backend.h"
#include "ui.h"

static void bildschirm_leeren(void)
{
    printf("\033[H\033[2J\033[3J"); /* Cursor oben, Bildschirm und Scrollback leeren */
}

/* 1, wenn die Figur auf dem Feld dem Spieler gehoert, der gerade am Zug ist. */
static int ist_am_zug(Feld feld)
{
    return feld.figur != FELD_LEER && feld.farbe == aktuellerSpieler;
}

/* Beschriftung rechts neben dem Brett; der Spieler am Zug bekommt einen Pfeil. */
static const char *seitenbeschriftung(int zeile)
{
    if (zeile == 0) {
        return aktuellerSpieler == FARBE_SCHWARZ ? "  <== SCHWARZ (kleine Buchstaben) AM ZUG"
                                       : "  Schwarz (kleine Buchstaben)";
    }
    if (zeile == 7) {
        return aktuellerSpieler == FARBE_WEISS ? "  <== WEISS (GROSSE Buchstaben) AM ZUG"
                                       : "  Weiß (GROSSE Buchstaben)";
    }
    return "";
}

static void spielfeld_ausgeben(Feld spielfeld[8][8])
{
    printf("    a b c d e f g h\n");
    printf("  +-----------------+\n");
    for (int zeile = 0; zeile < 8; zeile++) {
        printf("%d |", 8 - zeile);
        for (int spalte = 0; spalte < 8; spalte++) {
            char figur = feld_zeichen(spielfeld[zeile][spalte]);
            if (ist_am_zug(spielfeld[zeile][spalte])) {
                printf(" \033[1m%c\033[0m", figur);   /* fett: diese Figuren darf man ziehen */
            } else {
                printf(" %c", figur);
            }
        }
        printf(" |%s\n", seitenbeschriftung(zeile));
    }
    printf("  +-----------------+\n");
}

#define PROTOKOLL_ANZEIGE 3 /* so viele letzte Zuege werden gezeigt */

static void status_ausgeben(void)
{
    if (aktuellerSpieler == FARBE_WEISS) {
        printf("\n>>> Zug %d: WEISS ist am Zug. Du bewegst die GROSSEN Buchstaben (unten). <<<\n", zugnummer);
    } else {
        printf("\n>>> Zug %d: SCHWARZ ist am Zug. Du bewegst die kleinen Buchstaben (oben). <<<\n", zugnummer);
    }
}

static void protokoll_ausgeben(void)
{
    int erster = zugprotokollAnzahl - PROTOKOLL_ANZEIGE;
    if (erster < 0) {
        erster = 0;
    }
    printf("Letzte Züge:\n");
    for (int i = erster; i < zugprotokollAnzahl; i++) {
        printf("%s\n", zugprotokoll[i]);
    }
}

static void menue_anzeigen(void)
{
    printf("\n=== C-SCHACH ===\n");
    printf("1 - Spielfeld anzeigen    4 - Spielstand laden\n");
    printf("2 - Figur bewegen         5 - Neues Spiel\n");
    printf("3 - Spielstand speichern  0 - Beenden\n\n");
}

void seite_anzeigen(Feld spielfeld[8][8], const char *meldung)
{
    bildschirm_leeren();
    spielfeld_ausgeben(spielfeld);
    status_ausgeben();
    protokoll_ausgeben();
    printf("%s\n", meldung);
    menue_anzeigen();
}

void text_einlesen(const char *frage, char eingabe[], int groesse)
{
    printf("%s", frage);
    if (fgets(eingabe, groesse, stdin) == NULL) {
        eingabe[0] = '\0';
        return;
    }
    eingabe[strcspn(eingabe, "\n")] = '\0';
}

int feld_einlesen(const char *frage, int *zeile, int *spalte)
{
    char feld[16];
    text_einlesen(frage, feld, sizeof feld);
    return feld_ermitteln(feld, zeile, spalte);
}

/* Erklaert, warum eine Figur nicht so ziehen darf (Meldung E7). Brett ist noch unveraendert. */
static const char *regel_text(Feld spielfeld[8][8], int startZeile, int startSpalte, int zielZeile, int zielSpalte)
{
    int dz = abs(zielZeile - startZeile);
    int ds = abs(zielSpalte - startSpalte);

    switch (spielfeld[startZeile][startSpalte].figur) {
    case 'T':
        return (dz > 0 && ds > 0) ? "Ein Turm kann sich nicht diagonal bewegen."
                                  : "Der Weg des Turms ist durch eine Figur blockiert.";
    case 'L':
        return (dz != ds) ? "Ein Läufer kann sich nur diagonal bewegen."
                          : "Der Weg des Läufers ist durch eine Figur blockiert.";
    case 'S': return "Ein Springer zieht nur in L-Form (2 Felder und 1 Feld seitlich).";
    case 'D': return "Eine Dame zieht nur gerade oder diagonal über freie Felder.";
    case 'K': return "Ein König zieht nur ein Feld weit.";
    default:  return "Ein Bauer zieht 1 Feld vor (2 aus der Grundreihe) und schlägt nur diagonal.";
    }
}

void zugfehler_text(int code, Feld spielfeld[8][8], int startZeile, int startSpalte, int zielZeile, int zielSpalte,
                    char meldung[], int groesse)
{
    if (code == ZUG_STARTFELD_LEER) {
        char startfeld[3];
        feld_name_bilden(startZeile, startSpalte, startfeld);
        snprintf(meldung, groesse, "FEHLER: Auf %s befindet sich keine Figur.", startfeld);
    } else if (code == ZUG_FALSCHE_FARBE) {
        snprintf(meldung, groesse, "FEHLER: Diese Figur gehört nicht zu %s.",
                 aktuellerSpieler == FARBE_WEISS ? "Weiß" : "Schwarz");
    } else if (startZeile == zielZeile && startSpalte == zielSpalte) {
        snprintf(meldung, groesse, "FEHLER: Start- und Zielfeld sind gleich.");
    } else {
        snprintf(meldung, groesse, "FEHLER: %s", regel_text(spielfeld, startZeile, startSpalte, zielZeile, zielSpalte));
    }
}
