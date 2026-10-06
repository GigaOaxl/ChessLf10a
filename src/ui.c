/* ui.c - Anzeige und Eingabe (Frontend). */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "backend.h"
#include "ui.h"

void bildschirm_leeren(void)
{
    printf("\033[H\033[2J\033[3J"); /* Cursor oben, Bildschirm und Scrollback leeren */
}

void spielfeld_ausgeben(char spielfeld[8][8])
{
    printf("    a b c d e f g h\n");
    printf("  +-----------------+\n");
    for (int zeile = 0; zeile < 8; zeile++) {
        printf("%d |", 8 - zeile);
        for (int spalte = 0; spalte < 8; spalte++) {
            printf(" %c", spielfeld[zeile][spalte]);
        }
        printf(" |\n");
    }
    printf("  +-----------------+\n");
}

#define PROTOKOLL_ANZEIGE 3 /* so viele letzte Zuege werden gezeigt */

void status_ausgeben(void)
{
    if (aktuellerSpieler == 'W') {
        printf("\nZug %d: Weiß ist am Zug (GROSSE Buchstaben, unten).\n", zugnummer);
    } else {
        printf("\nZug %d: Schwarz ist am Zug (kleine Buchstaben, oben).\n", zugnummer);
    }
}

void protokoll_ausgeben(void)
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

void menue_anzeigen(void)
{
    printf("\n=== C-SCHACH ===\n");
    printf("1 - Spielfeld anzeigen    4 - Spielstand laden\n");
    printf("2 - Figur bewegen         5 - Neues Spiel\n");
    printf("3 - Spielstand speichern  0 - Beenden\n\n");
}

void seite_anzeigen(char spielfeld[8][8], const char *meldung)
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

int feld_einlesen(const char *frage, char feld[], int *zeile, int *spalte)
{
    text_einlesen(frage, feld, 16);
    return feld_ermitteln(feld, zeile, spalte);
}

/* Erklaert, warum eine Figur nicht so ziehen darf (Meldung E7). Brett ist noch unveraendert. */
static const char *regel_text(char spielfeld[8][8], int sz, int ss, int zz, int zs)
{
    int dz = abs(zz - sz);
    int ds = abs(zs - ss);

    switch (tolower((unsigned char)spielfeld[sz][ss])) {
    case 't':
        return (dz > 0 && ds > 0) ? "Ein Turm kann sich nicht diagonal bewegen."
                                  : "Der Weg des Turms ist durch eine Figur blockiert.";
    case 'l':
        return (dz != ds) ? "Ein Läufer kann sich nur diagonal bewegen."
                          : "Der Weg des Läufers ist durch eine Figur blockiert.";
    case 's': return "Ein Springer zieht nur in L-Form (2 Felder und 1 Feld seitlich).";
    case 'd': return "Eine Dame zieht nur gerade oder diagonal über freie Felder.";
    case 'k': return "Ein König zieht nur ein Feld weit.";
    default:  return "Ein Bauer zieht 1 Feld vor (2 aus der Grundreihe) und schlägt nur diagonal.";
    }
}

void zugfehler_text(int code, char spielfeld[8][8], int sz, int ss, int zz, int zs,
                    const char *startfeld, char meldung[], int groesse)
{
    if (code == ZUG_STARTFELD_LEER) {
        snprintf(meldung, groesse, "FEHLER: Auf %s befindet sich keine Figur.", startfeld);
    } else if (code == ZUG_FALSCHE_FARBE) {
        snprintf(meldung, groesse, "FEHLER: Diese Figur gehört nicht zu %s.",
                 aktuellerSpieler == 'W' ? "Weiß" : "Schwarz");
    } else if (sz == zz && ss == zs) {
        snprintf(meldung, groesse, "FEHLER: Start- und Zielfeld sind gleich.");
    } else {
        snprintf(meldung, groesse, "FEHLER: %s", regel_text(spielfeld, sz, ss, zz, zs));
    }
}
