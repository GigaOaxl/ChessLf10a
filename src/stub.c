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

void spielfeld_initialisieren(Feld spielfeld[8][8])
{
    const char *start[8] = {
        "tsldklst", "bbbbbbbb", "........", "........",
        "........", "........", "BBBBBBBB", "TSLDKLST"
    };
    for (int zeile = 0; zeile < 8; zeile++) {
        for (int spalte = 0; spalte < 8; spalte++) {
            spielfeld[zeile][spalte] = feld_aus_zeichen(start[zeile][spalte]);
        }
    }
    aktuellerSpieler = 'W';
    zugnummer = 1;
    zugprotokollAnzahl = 0;
}

int figur_bewegen(Feld spielfeld[8][8],
                  int startZeile, int startSpalte,
                  int zielZeile, int zielSpalte)
{
    if (spielfeld[startZeile][startSpalte].figur == '.') {
        return ZUG_STARTFELD_LEER;
    }
    if (spielfeld[startZeile][startSpalte].farbe != aktuellerSpieler) {
        return ZUG_FALSCHE_FARBE;
    }
    snprintf(zugprotokoll[zugprotokollAnzahl++], 16, "%d. %c%c -> %c%c", zugnummer % 1000,
             'a' + startSpalte, '8' - startZeile, 'a' + zielSpalte, '8' - zielZeile);
    zugnummer++;
    aktuellerSpieler = (aktuellerSpieler == 'W') ? 'S' : 'W';
    spielfeld[zielZeile][zielSpalte] = spielfeld[startZeile][startSpalte];
    spielfeld[startZeile][startSpalte] = feld_aus_zeichen('.');
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

int spielstand_speichern(Feld spielfeld[8][8], const char *dateiname)
{
    FILE *datei = fopen(dateiname, "w");
    if (datei == NULL) {
        return 0;
    }
    for (int zeile = 0; zeile < 8; zeile++) {
        for (int spalte = 0; spalte < 8; spalte++) {
            fputc(feld_zeichen(spielfeld[zeile][spalte]), datei);
        }
        fputc('\n', datei);
    }
    fclose(datei);
    return 1;
}

int spielstand_laden(Feld spielfeld[8][8], const char *dateiname)
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
        for (int spalte = 0; spalte < 8; spalte++) {
            spielfeld[zeile][spalte] = feld_aus_zeichen(zeilentext[spalte]);
        }
    }
    fclose(datei);
    return 1;
}
