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

char aktuellerSpieler = FARBE_WEISS;
int zugnummer = 1;
char zugprotokoll[PROTOKOLL_MAX][16];
int zugprotokollAnzahl = 0;

/* ---------- 2. Hilfsfunktionen fuer Figuren ---------- */

/* 1, wenn auf dem Feld keine Figur steht. */
static int ist_leer(Feld feld)
{
    return feld.figur == FELD_LEER;
}

/* 1, wenn die Figur auf dem Feld dem Spieler (FARBE_WEISS oder FARBE_SCHWARZ) gehoert. */
static int gehoert_spieler(Feld feld, char spieler)
{
    return !ist_leer(feld) && feld.farbe == spieler;
}

/* 1, wenn beide Felder Figuren haben und die Farben verschieden sind. */
static int sind_gegner(Feld feldEins, Feld feldZwei)
{
    return !ist_leer(feldEins) && !ist_leer(feldZwei) && feldEins.farbe != feldZwei.farbe;
}

/* Baut ein Feld aus einem Zeichen der Spielstand-Datei (Gross = Weiss, klein = Schwarz). */
static Feld feld_aus_zeichen(char zeichen)
{
    Feld feld;
    if (zeichen == FELD_LEER) {
        feld.figur = FELD_LEER;
        feld.farbe = FARBE_KEINE;
    } else if (zeichen >= 'a' && zeichen <= 'z') {      /* Kleinbuchstabe = Schwarz */
        feld.figur = (char)(zeichen - 'a' + 'A');
        feld.farbe = FARBE_SCHWARZ;
    } else {                                            /* Grossbuchstabe = Weiss */
        feld.figur = zeichen;
        feld.farbe = FARBE_WEISS;
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
static int turm_darf_ziehen(Feld spielfeld[8][8], int startZeile, int startSpalte, int zielZeile, int zielSpalte)
{
    if (startZeile != zielZeile && startSpalte != zielSpalte) {
        return 0;
    }
    return weg_ist_frei(spielfeld, startZeile, startSpalte, zielZeile, zielSpalte);
}

/* Laeufer: nur diagonal (gleich viele Zeilen wie Spalten), Weg muss frei sein. */
static int laeufer_darf_ziehen(Feld spielfeld[8][8], int startZeile, int startSpalte, int zielZeile, int zielSpalte)
{
    if (abs(zielZeile - startZeile) != abs(zielSpalte - startSpalte)) {
        return 0;
    }
    return weg_ist_frei(spielfeld, startZeile, startSpalte, zielZeile, zielSpalte);
}

/* Springer: L-Form (2+1 Felder), darf Figuren ueberspringen. */
static int springer_darf_ziehen(int startZeile, int startSpalte, int zielZeile, int zielSpalte)
{
    int zeilenabstand = abs(zielZeile - startZeile);
    int spaltenabstand = abs(zielSpalte - startSpalte);
    return (zeilenabstand == 2 && spaltenabstand == 1) ||
           (zeilenabstand == 1 && spaltenabstand == 2);
}

/* Dame: Turm ODER Laeufer. */
static int dame_darf_ziehen(Feld spielfeld[8][8], int startZeile, int startSpalte, int zielZeile, int zielSpalte)
{
    return turm_darf_ziehen(spielfeld, startZeile, startSpalte, zielZeile, zielSpalte) ||
           laeufer_darf_ziehen(spielfeld, startZeile, startSpalte, zielZeile, zielSpalte);
}

/* Koenig: genau ein Feld in jede Richtung. */
static int koenig_darf_ziehen(int startZeile, int startSpalte, int zielZeile, int zielSpalte)
{
    return abs(zielZeile - startZeile) <= 1 && abs(zielSpalte - startSpalte) <= 1;
}

/*
 * Bauer:
 *  - geradeaus 1 Feld, wenn das Zielfeld leer ist
 *  - geradeaus 2 Felder aus der Grundreihe, wenn beide Felder leer sind
 *  - 1 Feld diagonal NUR zum Schlagen einer gegnerischen Figur
 * Nicht enthalten: en passant und Umwandlung.
 */
static int bauer_darf_ziehen(Feld spielfeld[8][8], int startZeile, int startSpalte, int zielZeile, int zielSpalte)
{
    Feld bauer = spielfeld[startZeile][startSpalte];
    int vorwaerts = bauer.farbe == FARBE_WEISS ? -1 : 1;     /* Weiss: Zeile wird kleiner, Schwarz: groesser */
    int startreihe = bauer.farbe == FARBE_WEISS ? 6 : 1;    /* Zeile (Index), in der die Bauern starten */
    int zeilenschritt = zielZeile - startZeile;
    int spaltenschritt = abs(zielSpalte - startSpalte);

    if (spaltenschritt == 0) {
        if (!ist_leer(spielfeld[zielZeile][zielSpalte])) {
            return 0;                             /* geradeaus nie auf eine Figur */
        }
        if (zeilenschritt == vorwaerts) {
            return 1;
        }
        return startZeile == startreihe && zeilenschritt == 2 * vorwaerts &&
               ist_leer(spielfeld[startZeile + vorwaerts][startSpalte]);
    }
    if (spaltenschritt == 1 && zeilenschritt == vorwaerts) {
        return sind_gegner(bauer, spielfeld[zielZeile][zielSpalte]);
    }
    return 0;
}

/* Waehlt die Regel passend zur Figur auf dem Startfeld. 1 = Zug erlaubt. */
static int figur_darf_ziehen(Feld spielfeld[8][8], int startZeile, int startSpalte, int zielZeile, int zielSpalte)
{
    /* Figurbuchstaben: T Turm, L Laeufer, S Springer, D Dame, K Koenig, B Bauer */
    switch (spielfeld[startZeile][startSpalte].figur) {
    case 'T': return turm_darf_ziehen(spielfeld, startZeile, startSpalte, zielZeile, zielSpalte);
    case 'L': return laeufer_darf_ziehen(spielfeld, startZeile, startSpalte, zielZeile, zielSpalte);
    case 'S': return springer_darf_ziehen(startZeile, startSpalte, zielZeile, zielSpalte);
    case 'D': return dame_darf_ziehen(spielfeld, startZeile, startSpalte, zielZeile, zielSpalte);
    case 'K': return koenig_darf_ziehen(startZeile, startSpalte, zielZeile, zielSpalte);
    case 'B': return bauer_darf_ziehen(spielfeld, startZeile, startSpalte, zielZeile, zielSpalte);
    default:  return 0;
    }
}

/* ---------- 4. Oeffentliche Funktionen ---------- */

void spielfeld_initialisieren(Feld spielfeld[8][8])
{
    const char *ausgangsstellung[8] = {
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
            spielfeld[zeile][spalte] = feld_aus_zeichen(ausgangsstellung[zeile][spalte]);
        }
    }
    aktuellerSpieler = FARBE_WEISS;
    zugnummer = 1;
    zugprotokollAnzahl = 0;
}

int feld_ermitteln(char feldname[], int *zeile, int *spalte)
{
    if (strlen(feldname) != 2) {
        return 0;
    }
    if (feldname[0] < 'a' || feldname[0] > 'h' || feldname[1] < '1' || feldname[1] > '8') {
        return 0;
    }
    *spalte = feldname[0] - 'a';          /* 'a' -> 0 ... 'h' -> 7 */
    *zeile = 8 - (feldname[1] - '0');     /* '8' -> 0 ... '1' -> 7 (Brett ist "kopfueber") */
    return 1;
}

void feld_name_bilden(int zeile, int spalte, char name[3])
{
    name[0] = (char)('a' + spalte);   /* Spalte 0 -> 'a' */
    name[1] = (char)('8' - zeile);    /* Zeile 0 -> '8' (Brett ist "kopfueber") */
    name[2] = '\0';
}

/* Haengt den Zug als Text ans Protokoll, z.B. "1. e2 -> e4". */
static void zug_protokollieren(int startZeile, int startSpalte, int zielZeile, int zielSpalte)
{
    char von[3], nach[3];

    if (zugprotokollAnzahl >= PROTOKOLL_MAX) {
        return;                       /* Protokoll voll: Zug zaehlt trotzdem */
    }
    feld_name_bilden(startZeile, startSpalte, von);
    feld_name_bilden(zielZeile, zielSpalte, nach);
    /* zugnummer % 1000: hoechstens 3 Stellen, damit der Text sicher in 16 Zeichen passt */
    snprintf(zugprotokoll[zugprotokollAnzahl], sizeof zugprotokoll[0],
             "%d. %s -> %s", zugnummer % 1000, von, nach);
    zugprotokollAnzahl++;
}

int startfeld_pruefen(Feld spielfeld[8][8], int zeile, int spalte)
{
    if (ist_leer(spielfeld[zeile][spalte])) {
        return ZUG_STARTFELD_LEER;
    }
    if (!gehoert_spieler(spielfeld[zeile][spalte], aktuellerSpieler)) {
        return ZUG_FALSCHE_FARBE;
    }
    return ZUG_OK;
}

int figur_bewegen(Feld spielfeld[8][8],
                  int startZeile, int startSpalte,
                  int zielZeile, int zielSpalte)
{
    Feld startFeld = spielfeld[startZeile][startSpalte];
    Feld zielFeld = spielfeld[zielZeile][zielSpalte];
    int startCode = startfeld_pruefen(spielfeld, startZeile, startSpalte);

    /* Pruefungen in dieser Reihenfolge; die erste, die fehlschlaegt, bricht ab. */
    if (startCode != ZUG_OK) {
        return startCode;
    }
    if (startZeile == zielZeile && startSpalte == zielSpalte) {
        return ZUG_UNGUELTIG;                         /* Figur bleibt stehen */
    }
    if (gehoert_spieler(zielFeld, aktuellerSpieler)) {
        return ZUG_UNGUELTIG;                         /* eigene Figur nicht schlagen */
    }
    if (!figur_darf_ziehen(spielfeld, startZeile, startSpalte, zielZeile, zielSpalte)) {
        return ZUG_UNGUELTIG;
    }

    /* Zug ist gueltig: protokollieren, ausfuehren (ueberschreibt ggf. Gegner = schlagen). */
    zug_protokollieren(startZeile, startSpalte, zielZeile, zielSpalte);
    spielfeld[zielZeile][zielSpalte] = startFeld;
    spielfeld[startZeile][startSpalte] = feld_aus_zeichen(FELD_LEER);

    zugnummer++;
    aktuellerSpieler = (aktuellerSpieler == FARBE_WEISS) ? FARBE_SCHWARZ : FARBE_WEISS;
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
static int ist_gueltiges_dateizeichen(char zeichen)
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
    char dateizeile[64];
    int neueZugnummer, neueAnzahl;
    char neuerSpieler;

    FILE *datei = fopen(dateiname, "r");
    if (datei == NULL) {
        return 0;
    }

    /* Zugnummer */
    if (!zeile_lesen(datei, dateizeile, sizeof dateizeile)) { fclose(datei); return 0; }
    neueZugnummer = atoi(dateizeile);
    if (neueZugnummer < 1) { fclose(datei); return 0; }

    /* Spieler */
    if (!zeile_lesen(datei, dateizeile, sizeof dateizeile)) { fclose(datei); return 0; }
    neuerSpieler = dateizeile[0];
    if ((neuerSpieler != FARBE_WEISS && neuerSpieler != FARBE_SCHWARZ) || dateizeile[1] != '\0') {
        fclose(datei);
        return 0;
    }

    /* Spielfeld: 8 Zeilen mit genau 8 gueltigen Zeichen */
    for (int zeile = 0; zeile < 8; zeile++) {
        if (!zeile_lesen(datei, dateizeile, sizeof dateizeile) || strlen(dateizeile) != 8) {
            fclose(datei);
            return 0;
        }
        for (int spalte = 0; spalte < 8; spalte++) {
            if (!ist_gueltiges_dateizeichen(dateizeile[spalte])) {
                fclose(datei);
                return 0;
            }
            neuesFeld[zeile][spalte] = feld_aus_zeichen(dateizeile[spalte]);
        }
    }

    /* Protokoll */
    if (!zeile_lesen(datei, dateizeile, sizeof dateizeile)) { fclose(datei); return 0; }
    neueAnzahl = atoi(dateizeile);
    if (neueAnzahl < 0 || neueAnzahl > PROTOKOLL_MAX) { fclose(datei); return 0; }
    for (int i = 0; i < neueAnzahl; i++) {
        if (!zeile_lesen(datei, dateizeile, sizeof dateizeile) || strlen(dateizeile) >= sizeof neuesProtokoll[0]) {
            fclose(datei);
            return 0;
        }
        strcpy(neuesProtokoll[i], dateizeile);
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
