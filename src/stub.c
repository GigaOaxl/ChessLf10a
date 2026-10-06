/*
 * stub.c - ATTRAPPE der Spiellogik, nur zum Testen des Frontends.
 * Wird durch das echte Backend des Kollegen ersetzt (siehe backend.h).
 */
#include <stdio.h>
#include <string.h>
#include "backend.h"

char aktuellerSpieler = 'W';
int zugnummer = 1;
char zugprotokoll[PROTOKOLL_MAX][16];
int zugprotokollAnzahl = 0;

void spielfeld_initialisieren(char spielfeld[8][8])
{
    const char *start[8] = {
        "tlsdkslt", "bbbbbbbb", "........", "........",
        "........", "........", "BBBBBBBB", "TLSDKSLT"
    };
    for (int zeile = 0; zeile < 8; zeile++) {
        memcpy(spielfeld[zeile], start[zeile], 8);
    }
    aktuellerSpieler = 'W';
    zugnummer = 1;
    zugprotokollAnzahl = 0;
}

int figur_bewegen(char spielfeld[8][8],
                  int startZeile, int startSpalte,
                  int zielZeile, int zielSpalte)
{
    if (spielfeld[startZeile][startSpalte] == '.') {
        return ZUG_STARTFELD_LEER;
    }
    int figurIstWeiss = spielfeld[startZeile][startSpalte] >= 'A' && spielfeld[startZeile][startSpalte] <= 'Z';
    if (figurIstWeiss != (aktuellerSpieler == 'W')) {
        return ZUG_FALSCHE_FARBE;
    }
    snprintf(zugprotokoll[zugprotokollAnzahl++], 16, "%d. %c%c -> %c%c", zugnummer % 1000,
             'a' + startSpalte, '8' - startZeile, 'a' + zielSpalte, '8' - zielZeile);
    zugnummer++;
    aktuellerSpieler = (aktuellerSpieler == 'W') ? 'S' : 'W';
    spielfeld[zielZeile][zielSpalte] = spielfeld[startZeile][startSpalte];
    spielfeld[startZeile][startSpalte] = '.';
    return ZUG_OK;
}

int feld_ermitteln(char feld[], int *zeile, int *spalte)
{
    if (strlen(feld) != 2 || feld[0] < 'a' || feld[0] > 'h' || feld[1] < '1' || feld[1] > '8') {
        return 0;
    }
    *spalte = feld[0] - 'a';
    *zeile = 8 - (feld[1] - '0');
    return 1;
}

int spielstand_speichern(char spielfeld[8][8], const char *dateiname)
{
    FILE *datei = fopen(dateiname, "w");
    if (datei == NULL) {
        return 0;
    }
    for (int zeile = 0; zeile < 8; zeile++) {
        fprintf(datei, "%.8s\n", spielfeld[zeile]);
    }
    fclose(datei);
    return 1;
}

int spielstand_laden(char spielfeld[8][8], const char *dateiname)
{
    char zeilentext[16];
    FILE *datei = fopen(dateiname, "r");
    if (datei == NULL) {
        return 0;
    }
    for (int zeile = 0; zeile < 8; zeile++) {
        if (fgets(zeilentext, sizeof zeilentext, datei) == NULL) {
            fclose(datei);
            return 0;
        }
        memcpy(spielfeld[zeile], zeilentext, 8);
    }
    fclose(datei);
    return 1;
}
