/* ui.c - Anzeige und Eingabe (Frontend). */
#include <stdio.h>
#include <string.h>
#include "backend.h"
#include "ui.h"

void bildschirm_leeren(void)
{
    printf("\033[H\033[J");
}

void spielfeld_ausgeben(char spielfeld[8][8])
{
    printf("  a b c d e f g h\n");
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

void menue_anzeigen(void)
{
    printf("\n========================\n");
    printf("C-SCHACH\n");
    printf("========================\n\n");
    printf("1 - Spielfeld anzeigen\n");
    printf("2 - Figur bewegen\n");
    printf("3 - Spielstand speichern\n");
    printf("4 - Spielstand laden\n");
    printf("5 - Neues Spiel\n");
    printf("0 - Beenden\n\n");
}

void seite_anzeigen(char spielfeld[8][8], const char *meldung)
{
    bildschirm_leeren();
    spielfeld_ausgeben(spielfeld);
    printf("\n%s\n", meldung);
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

void zugfehler_text(int code, const char *startfeld, char meldung[], int groesse)
{
    if (code == ZUG_STARTFELD_LEER) {
        snprintf(meldung, groesse, "FEHLER: Auf %s befindet sich keine Figur.", startfeld);
    } else if (code == ZUG_FALSCHE_FARBE) {
        snprintf(meldung, groesse, "FEHLER: Diese Figur gehört nicht zum Spieler am Zug.");
    } else {
        snprintf(meldung, groesse, "FEHLER: Ungültiger Zug.");
    }
}
