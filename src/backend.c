/*
 * backend.c - Spiellogik (Experten). Setzt den Vertrag aus backend.h um.
 *
 * Aufbau der Datei:
 *   1. Spieldaten (globale Variablen)
 *   2. Hilfsfunktionen fuer Figuren
 *   3. Bewegungsregeln je Figur
 *   4. Oeffentliche Funktionen (die das Frontend aufruft)
 *   5. Speichern und Laden
 *
 * Koordinaten: spielfeld[0][0] = a8, spielfeld[7][7] = h1.
 * Jedes Feld ist ein Feld-struct (Figur + Farbe getrennt, siehe backend.h).
 * Weisse Figuren ziehen nach OBEN (Zeile wird kleiner), schwarze nach UNTEN.
 * In der Spielstand-Datei steht Weiss als Grossbuchstabe, Schwarz als Kleinbuchstabe.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "backend.h"

/* ---------- 1. Spieldaten ---------- */

char aktuellerSpieler = 'W';
int zugnummer = 1;
char zugprotokoll[PROTOKOLL_MAX][16];
int zugprotokollAnzahl = 0;

/* ---------- 2. Hilfsfunktionen fuer Figuren ---------- */

/* 1, wenn auf dem Feld keine Figur steht. */
static int ist_leer(Feld feld)
{
    return feld.figur == '.';
}

/* 1, wenn die Figur auf dem Feld dem Spieler 'W' oder 'S' gehoert. */
static int gehoert_spieler(Feld feld, char spieler)
{
    return !ist_leer(feld) && feld.farbe == spieler;
}

/* 1, wenn beide Felder Figuren haben und die Farben verschieden sind. */
static int sind_gegner(Feld feldA, Feld feldB)
{
    return !ist_leer(feldA) && !ist_leer(feldB) && feldA.farbe != feldB.farbe;
}

/* Baut ein Feld aus einem Zeichen der Spielstand-Datei (Gross = Weiss, klein = Schwarz). */
static Feld feld_aus_zeichen(char zeichen)
{
    Feld feld;
    if (zeichen == '.') {
        feld.figur = '.';
        feld.farbe = '-';
    } else if (zeichen >= 'a' && zeichen <= 'z') {
        feld.figur = (char)(zeichen - 'a' + 'A');
        feld.farbe = 'S';
    } else {
        feld.figur = zeichen;
        feld.farbe = 'W';
    }
    return feld;
}

/* Vorzeichen einer Zahl: -1, 0 oder +1. Gibt die Laufrichtung eines Zuges an. */
static int vorzeichen(int zahl)
{
    if (zahl > 0) {
        return 1;
    }
    if (zahl < 0) {
        return -1;
    }
    return 0;
}

/* ---------- 3. Bewegungsregeln je Figur ---------- */

/*
 * 1, wenn alle Felder ZWISCHEN Start und Ziel leer sind.
 * Gilt nur fuer gerade oder diagonale Zuege (Turm, Laeufer, Dame).
 * Wir gehen Schritt fuer Schritt vom Start zum Ziel und pruefen jedes Feld.
 */
static int weg_ist_frei(Feld spielfeld[8][8],
                        int startZeile, int startSpalte,
                        int zielZeile, int zielSpalte)
{
    int schrittZeile = vorzeichen(zielZeile - startZeile);
    int schrittSpalte = vorzeichen(zielSpalte - startSpalte);
    int zeile = startZeile + schrittZeile;
    int spalte = startSpalte + schrittSpalte;

    while (zeile != zielZeile || spalte != zielSpalte) {
        if (!ist_leer(spielfeld[zeile][spalte])) {
            return 0;
        }
        zeile += schrittZeile;
        spalte += schrittSpalte;
    }
    return 1;
}

/* Turm: nur gerade (gleiche Zeile ODER gleiche Spalte), Weg muss frei sein. */
static int turm_darf(Feld spielfeld[8][8], int sz, int ss, int zz, int zs)
{
    if (sz != zz && ss != zs) {
        return 0;
    }
    return weg_ist_frei(spielfeld, sz, ss, zz, zs);
}

/* Laeufer: nur diagonal (gleich viele Zeilen wie Spalten), Weg muss frei sein. */
static int laeufer_darf(Feld spielfeld[8][8], int sz, int ss, int zz, int zs)
{
    if (abs(zz - sz) != abs(zs - ss)) {
        return 0;
    }
    return weg_ist_frei(spielfeld, sz, ss, zz, zs);
}

/* Springer: L-Form (2+1 Felder), darf Figuren ueberspringen. */
static int springer_darf(int sz, int ss, int zz, int zs)
{
    int deltaZeile = abs(zz - sz);
    int deltaSpalte = abs(zs - ss);
    return (deltaZeile == 2 && deltaSpalte == 1) ||
           (deltaZeile == 1 && deltaSpalte == 2);
}

/* Dame: Turm ODER Laeufer. */
static int dame_darf(Feld spielfeld[8][8], int sz, int ss, int zz, int zs)
{
    return turm_darf(spielfeld, sz, ss, zz, zs) ||
           laeufer_darf(spielfeld, sz, ss, zz, zs);
}

/* Koenig: genau ein Feld in jede Richtung. */
static int koenig_darf(int sz, int ss, int zz, int zs)
{
    return abs(zz - sz) <= 1 && abs(zs - ss) <= 1;
}

/*
 * Bauer:
 *  - geradeaus 1 Feld, wenn das Zielfeld leer ist
 *  - geradeaus 2 Felder aus der Grundreihe, wenn beide Felder leer sind
 *  - 1 Feld diagonal NUR zum Schlagen einer gegnerischen Figur
 * Nicht enthalten: en passant und Umwandlung.
 */
static int bauer_darf(Feld spielfeld[8][8], int sz, int ss, int zz, int zs)
{
    Feld bauer = spielfeld[sz][ss];
    int richtung = bauer.farbe == 'W' ? -1 : 1;     /* Weiss: Zeile wird kleiner */
    int grundreihe = bauer.farbe == 'W' ? 6 : 1;    /* Zeile, in der die Bauern starten */
    int schritt = zz - sz;
    int seitwaerts = abs(zs - ss);

    if (seitwaerts == 0) {
        if (!ist_leer(spielfeld[zz][zs])) {
            return 0;                             /* geradeaus nie auf eine Figur */
        }
        if (schritt == richtung) {
            return 1;
        }
        return sz == grundreihe && schritt == 2 * richtung &&
               ist_leer(spielfeld[sz + richtung][ss]);
    }
    if (seitwaerts == 1 && schritt == richtung) {
        return sind_gegner(bauer, spielfeld[zz][zs]);
    }
    return 0;
}

/* Verteilt auf die Regel der jeweiligen Figur. 1 = Bewegung erlaubt. */
static int bewegung_erlaubt(Feld spielfeld[8][8], int sz, int ss, int zz, int zs)
{
    switch (spielfeld[sz][ss].figur) {
    case 'T': return turm_darf(spielfeld, sz, ss, zz, zs);
    case 'L': return laeufer_darf(spielfeld, sz, ss, zz, zs);
    case 'S': return springer_darf(sz, ss, zz, zs);
    case 'D': return dame_darf(spielfeld, sz, ss, zz, zs);
    case 'K': return koenig_darf(sz, ss, zz, zs);
    case 'B': return bauer_darf(spielfeld, sz, ss, zz, zs);
    default:  return 0;
    }
}

/* ---------- 4. Oeffentliche Funktionen ---------- */

void spielfeld_initialisieren(Feld spielfeld[8][8])
{
    const char *anfang[8] = {
        "tsldklst",     /* Zeile 8: schwarze Grundreihe */
        "bbbbbbbb",     /* Zeile 7: schwarze Bauern */
        "........",
        "........",
        "........",
        "........",
        "BBBBBBBB",     /* Zeile 2: weisse Bauern */
        "TSLDKLST"      /* Zeile 1: weisse Grundreihe */
    };

    for (int zeile = 0; zeile < 8; zeile++) {
        for (int spalte = 0; spalte < 8; spalte++) {
            spielfeld[zeile][spalte] = feld_aus_zeichen(anfang[zeile][spalte]);
        }
    }
    aktuellerSpieler = 'W';
    zugnummer = 1;
    zugprotokollAnzahl = 0;
}

int feld_ermitteln(char feld[], int *zeile, int *spalte)
{
    if (strlen(feld) != 2) {
        return 0;
    }
    if (feld[0] < 'a' || feld[0] > 'h' || feld[1] < '1' || feld[1] > '8') {
        return 0;
    }
    *spalte = feld[0] - 'a';          /* 'a' -> 0 ... 'h' -> 7 */
    *zeile = 8 - (feld[1] - '0');     /* '8' -> 0 ... '1' -> 7 (Brett ist "kopfueber") */
    return 1;
}

/* Haengt den Zug als Text ans Protokoll, z.B. "1. e2 -> e4". */
static void zug_protokollieren(int sz, int ss, int zz, int zs)
{
    if (zugprotokollAnzahl >= PROTOKOLL_MAX) {
        return;                       /* Protokoll voll: Zug zaehlt trotzdem */
    }
    snprintf(zugprotokoll[zugprotokollAnzahl], sizeof zugprotokoll[0],
             "%d. %c%c -> %c%c", zugnummer % 1000,
             'a' + ss, '8' - sz, 'a' + zs, '8' - zz);
    zugprotokollAnzahl++;
}

int figur_bewegen(Feld spielfeld[8][8],
                  int startZeile, int startSpalte,
                  int zielZeile, int zielSpalte)
{
    Feld figur = spielfeld[startZeile][startSpalte];
    Feld ziel = spielfeld[zielZeile][zielSpalte];

    /* Pruefungen in dieser Reihenfolge; die erste, die fehlschlaegt, bricht ab. */
    if (ist_leer(figur)) {
        return ZUG_STARTFELD_LEER;
    }
    if (!gehoert_spieler(figur, aktuellerSpieler)) {
        return ZUG_FALSCHE_FARBE;
    }
    if (startZeile == zielZeile && startSpalte == zielSpalte) {
        return ZUG_UNGUELTIG;                         /* Figur bleibt stehen */
    }
    if (gehoert_spieler(ziel, aktuellerSpieler)) {
        return ZUG_UNGUELTIG;                         /* eigene Figur nicht schlagen */
    }
    if (!bewegung_erlaubt(spielfeld, startZeile, startSpalte, zielZeile, zielSpalte)) {
        return ZUG_UNGUELTIG;
    }

    /* Zug ist gueltig: protokollieren, ausfuehren (ueberschreibt ggf. Gegner = schlagen). */
    zug_protokollieren(startZeile, startSpalte, zielZeile, zielSpalte);
    spielfeld[zielZeile][zielSpalte] = figur;
    spielfeld[startZeile][startSpalte] = feld_aus_zeichen('.');

    zugnummer++;
    aktuellerSpieler = (aktuellerSpieler == 'W') ? 'S' : 'W';
    return ZUG_OK;
}

/* ---------- 5. Speichern und Laden ----------
 *
 * Dateiformat (Textdatei):
 *   Zeile 1:      Zugnummer
 *   Zeile 2:      aktueller Spieler (W oder S)
 *   Zeile 3-10:   Spielfeld, 8 Zeilen mit je 8 Zeichen (Zeile 8 zuerst)
 *   Zeile 11:     Anzahl Protokollzeilen
 *   danach:       je eine Zeile pro Zug, z.B. "1. e2 -> e4"
 */

int spielstand_speichern(Feld spielfeld[8][8], const char *dateiname)
{
    FILE *datei = fopen(dateiname, "w");
    if (datei == NULL) {
        return 0;
    }

    fprintf(datei, "%d\n%c\n", zugnummer, aktuellerSpieler);
    for (int zeile = 0; zeile < 8; zeile++) {
        for (int spalte = 0; spalte < 8; spalte++) {
            fputc(feld_zeichen(spielfeld[zeile][spalte]), datei);
        }
        fputc('\n', datei);
    }
    fprintf(datei, "%d\n", zugprotokollAnzahl);
    for (int i = 0; i < zugprotokollAnzahl; i++) {
        fprintf(datei, "%s\n", zugprotokoll[i]);
    }

    /* fclose kann beim Schreiben noch scheitern (z.B. Platte voll). */
    if (fclose(datei) != 0) {
        return 0;
    }
    return 1;
}

/* Liest eine Zeile ohne Zeilenumbruch. 1 = gelesen, 0 = Dateiende/Fehler. */
static int zeile_lesen(FILE *datei, char puffer[], int groesse)
{
    if (fgets(puffer, groesse, datei) == NULL) {
        return 0;
    }
    puffer[strcspn(puffer, "\r\n")] = '\0';
    return 1;
}

/* 1, wenn das Zeichen eine erlaubte Brettfigur ist (oder '.'). */
static int ist_brettzeichen(char zeichen)
{
    return zeichen != '\0' && strchr(".KDTLSBkdtlsb", zeichen) != NULL;
}

/*
 * Wir lesen zuerst in lokale Zwischenspeicher und uebernehmen erst am Ende.
 * So bleibt der laufende Spielstand unveraendert, falls die Datei kaputt ist.
 */
int spielstand_laden(Feld spielfeld[8][8], const char *dateiname)
{
    Feld neuesFeld[8][8];
    char neuesProtokoll[PROTOKOLL_MAX][16];
    char zeile[64];
    int neueZugnummer, neueAnzahl;
    char neuerSpieler;

    FILE *datei = fopen(dateiname, "r");
    if (datei == NULL) {
        return 0;
    }

    /* Zugnummer */
    if (!zeile_lesen(datei, zeile, sizeof zeile)) { fclose(datei); return 0; }
    neueZugnummer = atoi(zeile);
    if (neueZugnummer < 1) { fclose(datei); return 0; }

    /* Spieler */
    if (!zeile_lesen(datei, zeile, sizeof zeile)) { fclose(datei); return 0; }
    neuerSpieler = zeile[0];
    if ((neuerSpieler != 'W' && neuerSpieler != 'S') || zeile[1] != '\0') {
        fclose(datei);
        return 0;
    }

    /* Spielfeld: 8 Zeilen mit genau 8 gueltigen Zeichen */
    for (int z = 0; z < 8; z++) {
        if (!zeile_lesen(datei, zeile, sizeof zeile) || strlen(zeile) != 8) {
            fclose(datei);
            return 0;
        }
        for (int s = 0; s < 8; s++) {
            if (!ist_brettzeichen(zeile[s])) {
                fclose(datei);
                return 0;
            }
            neuesFeld[z][s] = feld_aus_zeichen(zeile[s]);
        }
    }

    /* Protokoll */
    if (!zeile_lesen(datei, zeile, sizeof zeile)) { fclose(datei); return 0; }
    neueAnzahl = atoi(zeile);
    if (neueAnzahl < 0 || neueAnzahl > PROTOKOLL_MAX) { fclose(datei); return 0; }
    for (int i = 0; i < neueAnzahl; i++) {
        if (!zeile_lesen(datei, zeile, sizeof zeile) || strlen(zeile) >= sizeof neuesProtokoll[0]) {
            fclose(datei);
            return 0;
        }
        strcpy(neuesProtokoll[i], zeile);
    }
    fclose(datei);

    /* Alles war gueltig: jetzt erst die echten Daten ueberschreiben. */
    memcpy(spielfeld, neuesFeld, sizeof neuesFeld);
    zugnummer = neueZugnummer;
    aktuellerSpieler = neuerSpieler;
    zugprotokollAnzahl = neueAnzahl;
    for (int i = 0; i < neueAnzahl; i++) {
        strcpy(zugprotokoll[i], neuesProtokoll[i]);
    }
    return 1;
}
