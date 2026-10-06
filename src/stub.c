/*
 * stub.c - ATTRAPPE der Spiellogik, nur zum Testen des Frontends.
 * Wird durch das echte Backend des Kollegen ersetzt (siehe backend.h).
 */
#include <string.h>
#include "backend.h"

void spielfeld_initialisieren(char spielfeld[8][8])
{
    const char *start[8] = {
        "tlsdkslt", "bbbbbbbb", "........", "........",
        "........", "........", "BBBBBBBB", "TLSDKSLT"
    };
    for (int zeile = 0; zeile < 8; zeile++) {
        memcpy(spielfeld[zeile], start[zeile], 8);
    }
}

int figur_bewegen(char spielfeld[8][8],
                  int startZeile, int startSpalte,
                  int zielZeile, int zielSpalte)
{
    spielfeld[zielZeile][zielSpalte] = spielfeld[startZeile][startSpalte];
    spielfeld[startZeile][startSpalte] = '.';
    return ZUG_OK;
}
