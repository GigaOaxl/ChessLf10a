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

/* Zug in Schachnotation ausfuehren, z.B. zug(f, "e2", "e4"). Rueckgabe: Code von figur_bewegen. */
static int zug(Feld f[8][8], const char *von, const char *nach)
{
    char a[3], b[3];
    int sz, ss, zz, zs;
    strcpy(a, von);
    strcpy(b, nach);
    if (!feld_ermitteln(a, &sz, &ss) || !feld_ermitteln(b, &zz, &zs)) {
        return -1;
    }
    return figur_bewegen(f, sz, ss, zz, zs);
}

static char feld_an(Feld f[8][8], const char *name)
{
    char n[3];
    int z, s;
    strcpy(n, name);
    feld_ermitteln(n, &z, &s);
    return feld_zeichen(f[z][s]);
}

int main(void)
{
    Feld f[8][8];
    int z, s;
    char eingabe[8];

    /* Testfall 1: Startaufstellung */
    spielfeld_initialisieren(f);
    pruefe("Start: a8 ist schwarzer Turm", feld_an(f, "a8") == 't');
    pruefe("Start: h1 ist weisser Turm", feld_an(f, "h1") == 'T');
    pruefe("Start: e1 ist weisser Koenig", feld_an(f, "e1") == 'K');
    pruefe("Feld trennt Figur und Farbe", f[7][4].figur == 'K' && f[7][4].farbe == 'W' && f[0][4].farbe == 'S');
    pruefe("Start: Weiss beginnt", aktuellerSpieler == 'W' && zugnummer == 1);

    /* feld_ermitteln (S2, Testfall 4) */
    strcpy(eingabe, "a8");  pruefe("a8 -> [0][0]", feld_ermitteln(eingabe, &z, &s) && z == 0 && s == 0);
    strcpy(eingabe, "h1");  pruefe("h1 -> [7][7]", feld_ermitteln(eingabe, &z, &s) && z == 7 && s == 7);
    strcpy(eingabe, "z1");  pruefe("z1 ungueltig", !feld_ermitteln(eingabe, &z, &s));
    strcpy(eingabe, "a9");  pruefe("a9 ungueltig", !feld_ermitteln(eingabe, &z, &s));
    strcpy(eingabe, "x");   pruefe("x ungueltig", !feld_ermitteln(eingabe, &z, &s));
    strcpy(eingabe, "abc"); pruefe("abc ungueltig", !feld_ermitteln(eingabe, &z, &s));

    /* Testfall 2: e2 -> e4, Spielerwechsel, Zugnummer, Protokoll */
    pruefe("e2-e4 ok", zug(f, "e2", "e4") == ZUG_OK);
    pruefe("e2 leer, e4 Bauer", feld_an(f, "e2") == '.' && feld_an(f, "e4") == 'B');
    pruefe("Schwarz am Zug, Zugnummer 2", aktuellerSpieler == 'S' && zugnummer == 2);
    pruefe("Protokoll: 1. e2 -> e4", zugprotokollAnzahl == 1 && strcmp(zugprotokoll[0], "1. e2 -> e4") == 0);

    /* Testfall 9: gegnerische Figur bewegen (Weiss versucht schwarzen Bauern) */
    pruefe("Schwarz darf keine weisse Figur ziehen", zug(f, "e4", "e5") == ZUG_FALSCHE_FARBE);
    pruefe("falscher Zug aendert nichts", aktuellerSpieler == 'S' && feld_an(f, "e4") == 'B');

    /* Testfall 3: leeres Startfeld */
    pruefe("e5 leer", zug(f, "e5", "e6") == ZUG_STARTFELD_LEER);

    /* Turm / Laeufer / Springer (E7) */
    spielfeld_initialisieren(f);
    pruefe("Turm a1-b3 diagonal abgelehnt", zug(f, "a1", "b3") == ZUG_UNGUELTIG);
    pruefe("Turm a1-a3 durch eigenen Bauern abgelehnt", zug(f, "a1", "a3") == ZUG_UNGUELTIG);
    pruefe("Laeufer c1-e3 durch Bauern abgelehnt", zug(f, "c1", "e3") == ZUG_UNGUELTIG);
    pruefe("Springer g1-f3 ok", zug(f, "g1", "f3") == ZUG_OK);
    pruefe("Springer ueberspringt Figuren", feld_an(f, "f3") == 'S' && feld_an(f, "g1") == '.');
    pruefe("eigene Figur nicht schlagen", zug(f, "b8", "d7") == ZUG_UNGUELTIG);
    pruefe("Springer b8-c6 ok", zug(f, "b8", "c6") == ZUG_OK);

    /* Bauer (Erweiterung) */
    spielfeld_initialisieren(f);
    pruefe("Bauer 2 Felder aus Grundreihe", zug(f, "d2", "d4") == ZUG_OK);
    pruefe("Schwarzer Bauer 1 Feld", zug(f, "d7", "d6") == ZUG_OK);
    pruefe("Bauer 3 Felder abgelehnt", zug(f, "a2", "a5") == ZUG_UNGUELTIG);
    pruefe("Bauer rueckwaerts abgelehnt", zug(f, "d4", "d3") == ZUG_UNGUELTIG);
    pruefe("Bauer diagonal ohne Gegner abgelehnt", zug(f, "d4", "e5") == ZUG_UNGUELTIG);

    /* Testfall 10: Schlagen */
    spielfeld_initialisieren(f);
    zug(f, "e2", "e4");
    zug(f, "d7", "d5");
    pruefe("Bauer schlaegt diagonal e4xd5", zug(f, "e4", "d5") == ZUG_OK);
    pruefe("Gegner entfernt, Bauer auf d5", feld_an(f, "d5") == 'B' && feld_an(f, "e4") == '.');
    pruefe("Bauer geradeaus blockiert", zug(f, "d8", "d7") == ZUG_OK);   /* Dame d8-d7 */

    /* Dame und Koenig */
    spielfeld_initialisieren(f);
    zug(f, "e2", "e4"); zug(f, "e7", "e5");
    pruefe("Dame d1-h5 diagonal", zug(f, "d1", "h5") == ZUG_OK);
    pruefe("Koenig e8-e7 ok", zug(f, "e8", "e7") == ZUG_OK);
    pruefe("Koenig e1-e3 (2 Felder) abgelehnt", zug(f, "e1", "e3") == ZUG_UNGUELTIG);
    pruefe("Koenig e1-e2 ok", zug(f, "e1", "e2") == ZUG_OK);

    /* Neues Spiel setzt zurueck */
    spielfeld_initialisieren(f);
    pruefe("Neues Spiel setzt Daten zurueck", zugnummer == 1 && zugprotokollAnzahl == 0 && aktuellerSpieler == 'W');

    /* Testfaelle 5, 7: Speichern und Laden mit allen Zusatzdaten */
    zug(f, "e2", "e4"); zug(f, "e7", "e5"); zug(f, "g1", "f3");
    pruefe("Speichern ok", spielstand_speichern(f, "test_spielstand.txt") == 1);
    spielfeld_initialisieren(f);
    pruefe("Laden ok", spielstand_laden(f, "test_spielstand.txt") == 1);
    pruefe("Geladen: Brett", feld_an(f, "e4") == 'B' && feld_an(f, "e5") == 'b' && feld_an(f, "f3") == 'S');
    pruefe("Geladen: Spieler und Zugnummer", aktuellerSpieler == 'S' && zugnummer == 4);
    pruefe("Geladen: Protokoll", zugprotokollAnzahl == 3 && strcmp(zugprotokoll[2], "3. g1 -> f3") == 0);

    /* Testfall 8: falsche Datei */
    pruefe("Datei nicht vorhanden", spielstand_laden(f, "gibt_es_nicht.txt") == 0);
    {
        FILE *d = fopen("test_kaputt.txt", "w");
        fputs("hallo\nwelt\n", d);
        fclose(d);
    }
    pruefe("Kaputte Datei abgelehnt", spielstand_laden(f, "test_kaputt.txt") == 0);
    pruefe("Kaputte Datei aendert Spielstand nicht", feld_an(f, "e4") == 'B' && zugnummer == 4);

    remove("test_spielstand.txt");
    remove("test_kaputt.txt");

    printf("%d Tests, %d fehlgeschlagen\n", gesamt, fehler);
    return fehler != 0;
}
