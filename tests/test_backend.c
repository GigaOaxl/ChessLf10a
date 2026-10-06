/* test_backend.c - testet die Spiellogik ohne Oberflaeche. Start: make test */
#include <stdio.h>
#include <string.h>
#include "backend.h"

static int fehler = 0;
static int gesamt = 0;

static void pruefe(const char *name, int bedingung)
{
    gesamt++;
    if (!bedingung) {
        printf("FEHLGESCHLAGEN: %s\n", name);
        fehler++;
    }
}

/* Zug in Schachnotation ausfuehren, z.B. zug(spielfeld, "e2", "e4"). Rueckgabe: Code von figur_bewegen. */
static int zug(Feld spielfeld[8][8], const char *von, const char *nach)
{
    char vonText[3], nachText[3];
    int startZeile, startSpalte, zielZeile, zielSpalte;
    strcpy(vonText, von);
    strcpy(nachText, nach);
    if (!feld_ermitteln(vonText, &startZeile, &startSpalte) || !feld_ermitteln(nachText, &zielZeile, &zielSpalte)) {
        return -1;
    }
    return figur_bewegen(spielfeld, startZeile, startSpalte, zielZeile, zielSpalte);
}

static char feld_an(Feld spielfeld[8][8], const char *name)
{
    char feldname[3];
    int zeile, spalte;
    strcpy(feldname, name);
    feld_ermitteln(feldname, &zeile, &spalte);
    return feld_zeichen(spielfeld[zeile][spalte]);
}

int main(void)
{
    Feld spielfeld[8][8];
    int zeile, spalte;
    char eingabe[8];

    /* Testfall 1: Startaufstellung */
    spielfeld_initialisieren(spielfeld);
    pruefe("Start: a8 ist schwarzer Turm", feld_zeichen(spielfeld[0][0]) == 't');
    pruefe("Start: h1 ist weisser Turm", feld_zeichen(spielfeld[7][7]) == 'T');
    pruefe("Start: e1 ist weisser Koenig", feld_an(spielfeld, "e1") == 'K');
    pruefe("Start: Weiss beginnt", aktuellerSpieler == FARBE_WEISS && zugnummer == 1);

    /* feld_ermitteln (S2, Testfall 4) */
    strcpy(eingabe, "a8");  pruefe("a8 -> [0][0]", feld_ermitteln(eingabe, &zeile, &spalte) && zeile == 0 && spalte == 0);
    strcpy(eingabe, "h1");  pruefe("h1 -> [7][7]", feld_ermitteln(eingabe, &zeile, &spalte) && zeile == 7 && spalte == 7);
    strcpy(eingabe, "z1");  pruefe("z1 ungueltig", !feld_ermitteln(eingabe, &zeile, &spalte));
    strcpy(eingabe, "a9");  pruefe("a9 ungueltig", !feld_ermitteln(eingabe, &zeile, &spalte));
    strcpy(eingabe, "x");   pruefe("x ungueltig", !feld_ermitteln(eingabe, &zeile, &spalte));
    strcpy(eingabe, "abc"); pruefe("abc ungueltig", !feld_ermitteln(eingabe, &zeile, &spalte));

    /* Testfall 2: e2 -> e4, Spielerwechsel, Zugnummer, Protokoll */
    pruefe("e2-e4 ok", zug(spielfeld, "e2", "e4") == ZUG_OK);
    pruefe("e2 leer, e4 Bauer", feld_an(spielfeld, "e2") == '.' && feld_an(spielfeld, "e4") == 'B');
    pruefe("Schwarz am Zug, Zugnummer 2", aktuellerSpieler == FARBE_SCHWARZ && zugnummer == 2);
    pruefe("Protokoll: 1. e2 -> e4", zugprotokollAnzahl == 1 && strcmp(zugprotokoll[0], "1. e2 -> e4") == 0);

    /* Testfall 9: gegnerische Figur bewegen (Weiss versucht schwarzen Bauern) */
    pruefe("Schwarz darf keine weisse Figur ziehen", zug(spielfeld, "e4", "e5") == ZUG_FALSCHE_FARBE);
    pruefe("falscher Zug aendert nichts", aktuellerSpieler == FARBE_SCHWARZ && feld_an(spielfeld, "e4") == 'B');

    /* feld_name_bilden: Umkehrung von feld_ermitteln */
    {
        char name[3];
        feld_name_bilden(7, 4, name);
        pruefe("Zeile 7, Spalte 4 heisst e1", strcmp(name, "e1") == 0);
        feld_name_bilden(0, 0, name);
        pruefe("Zeile 0, Spalte 0 heisst a8", strcmp(name, "a8") == 0);
    }

    /* startfeld_pruefen (Early Exit im Frontend) */
    pruefe("Startfeld e5 leer", startfeld_pruefen(spielfeld, 3, 4) == ZUG_STARTFELD_LEER);
    pruefe("Startfeld e7 gehoert Schwarz", startfeld_pruefen(spielfeld, 1, 4) == ZUG_OK);
    pruefe("Startfeld e4 (Weiss) gehoert nicht Schwarz", startfeld_pruefen(spielfeld, 4, 4) == ZUG_FALSCHE_FARBE);

    /* Testfall 3: leeres Startfeld */
    pruefe("e5 leer", zug(spielfeld, "e5", "e6") == ZUG_STARTFELD_LEER);

    /* Turm / Laeufer / Springer (E7) */
    spielfeld_initialisieren(spielfeld);
    pruefe("Turm a1-b3 diagonal abgelehnt", zug(spielfeld, "a1", "b3") == ZUG_UNGUELTIG);
    pruefe("Turm a1-a3 durch eigenen Bauern abgelehnt", zug(spielfeld, "a1", "a3") == ZUG_UNGUELTIG);
    pruefe("Laeufer c1-e3 durch Bauern abgelehnt", zug(spielfeld, "c1", "e3") == ZUG_UNGUELTIG);
    pruefe("Springer g1-f3 ok", zug(spielfeld, "g1", "f3") == ZUG_OK);
    pruefe("Springer ueberspringt Figuren", feld_an(spielfeld, "f3") == 'S' && feld_an(spielfeld, "g1") == '.');
    pruefe("eigene Figur nicht schlagen", zug(spielfeld, "b8", "d7") == ZUG_UNGUELTIG);
    pruefe("Springer b8-c6 ok", zug(spielfeld, "b8", "c6") == ZUG_OK);

    /* Bauer (Erweiterung) */
    spielfeld_initialisieren(spielfeld);
    pruefe("Bauer 2 Felder aus Grundreihe", zug(spielfeld, "d2", "d4") == ZUG_OK);
    pruefe("Schwarzer Bauer 1 Feld", zug(spielfeld, "d7", "d6") == ZUG_OK);
    pruefe("Bauer 3 Felder abgelehnt", zug(spielfeld, "a2", "a5") == ZUG_UNGUELTIG);
    pruefe("Bauer rueckwaerts abgelehnt", zug(spielfeld, "d4", "d3") == ZUG_UNGUELTIG);
    pruefe("Bauer diagonal ohne Gegner abgelehnt", zug(spielfeld, "d4", "e5") == ZUG_UNGUELTIG);

    /* Bauer-Doppelschritt ueber besetztes Zwischenfeld (Laeufer steht auf e3) */
    spielfeld_initialisieren(spielfeld);
    zug(spielfeld, "d2", "d3"); zug(spielfeld, "a7", "a6");
    zug(spielfeld, "c1", "e3"); zug(spielfeld, "a6", "a5");
    pruefe("Bauer e2-e4 ueber besetztes e3 abgelehnt", zug(spielfeld, "e2", "e4") == ZUG_UNGUELTIG);

    /* Testfall 10: Schlagen */
    spielfeld_initialisieren(spielfeld);
    zug(spielfeld, "e2", "e4");
    zug(spielfeld, "d7", "d5");
    pruefe("Bauer schlaegt diagonal e4xd5", zug(spielfeld, "e4", "d5") == ZUG_OK);
    pruefe("Gegner entfernt, Bauer auf d5", feld_an(spielfeld, "d5") == 'B' && feld_an(spielfeld, "e4") == '.');
    pruefe("Bauer geradeaus blockiert", zug(spielfeld, "d8", "d7") == ZUG_OK);   /* Dame d8-d7 */

    /* Dame und Koenig */
    spielfeld_initialisieren(spielfeld);
    zug(spielfeld, "e2", "e4"); zug(spielfeld, "e7", "e5");
    pruefe("Dame d1-h5 diagonal", zug(spielfeld, "d1", "h5") == ZUG_OK);
    pruefe("Koenig e8-e7 ok", zug(spielfeld, "e8", "e7") == ZUG_OK);
    pruefe("Koenig e1-e3 (2 Felder) abgelehnt", zug(spielfeld, "e1", "e3") == ZUG_UNGUELTIG);
    pruefe("Koenig e1-e2 ok", zug(spielfeld, "e1", "e2") == ZUG_OK);

    /* Neues Spiel setzt zurueck */
    spielfeld_initialisieren(spielfeld);
    pruefe("Neues Spiel setzt Daten zurueck", zugnummer == 1 && zugprotokollAnzahl == 0 && aktuellerSpieler == FARBE_WEISS);

    /* Testfaelle 5, 7: Speichern und Laden mit allen Zusatzdaten */
    zug(spielfeld, "e2", "e4"); zug(spielfeld, "e7", "e5"); zug(spielfeld, "g1", "f3");
    pruefe("Speichern ok", spielstand_speichern(spielfeld, "test_spielstand.txt") == 1);
    spielfeld_initialisieren(spielfeld);
    pruefe("Laden ok", spielstand_laden(spielfeld, "test_spielstand.txt") == 1);
    pruefe("Geladen: Brett", feld_an(spielfeld, "e4") == 'B' && feld_an(spielfeld, "e5") == 'b' && feld_an(spielfeld, "f3") == 'S');
    pruefe("Geladen: Spieler und Zugnummer", aktuellerSpieler == FARBE_SCHWARZ && zugnummer == 4);
    pruefe("Geladen: Protokoll", zugprotokollAnzahl == 3 && strcmp(zugprotokoll[2], "3. g1 -> f3") == 0);

    /* Testfall 8: falsche Datei */
    pruefe("Datei nicht vorhanden", spielstand_laden(spielfeld, "gibt_es_nicht.txt") == 0);
    {
        FILE *datei = fopen("test_kaputt.txt", "w");
        fputs("hallo\nwelt\n", datei);
        fclose(datei);
    }
    pruefe("Kaputte Datei abgelehnt", spielstand_laden(spielfeld, "test_kaputt.txt") == 0);
    pruefe("Kaputte Datei aendert Spielstand nicht", feld_an(spielfeld, "e4") == 'B' && zugnummer == 4);

    remove("test_spielstand.txt");
    remove("test_kaputt.txt");

    printf("%d Tests, %d fehlgeschlagen\n", gesamt, fehler);
    return fehler != 0;
}
