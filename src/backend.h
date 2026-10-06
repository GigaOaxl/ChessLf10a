/*
 * backend.h - VERTRAG zwischen Frontend (ui.c, main.c) und Spiellogik.
 *
 * Die Spiellogik (Kollege) implementiert diese Funktionen in eigener .c-Datei.
 * Das Frontend ruft sie nur auf und zeigt das Ergebnis an.
 * Aenderungen an diesem Vertrag nur nach Absprache.
 *
 * Koordinaten: spielfeld[0][0] = a8, spielfeld[7][7] = h1.
 */
#ifndef BACKEND_H
#define BACKEND_H

/* Rueckgabecodes von figur_bewegen: 0 = Zug ausgefuehrt, alles andere = Fehler. */
enum {
    ZUG_OK = 0,
    ZUG_STARTFELD_LEER,     /* auf dem Startfeld steht keine Figur */
    ZUG_FALSCHE_FARBE,      /* Figur gehoert dem anderen Spieler (Experten) */
    ZUG_UNGUELTIG           /* Zug verstoesst gegen die Bewegungsregeln (Experten) */
};

/* Spieldaten (Experten). Der Kollege definiert sie, das Frontend liest sie nur. */
#define PROTOKOLL_MAX 500
extern char aktuellerSpieler;               /* 'W' = Weiss, 'S' = Schwarz */
extern int  zugnummer;                      /* Nummer des naechsten Zuges, beginnt bei 1 */
extern char zugprotokoll[PROTOKOLL_MAX][16]; /* je Zeile z.B. "1. e2 -> e4" */
extern int  zugprotokollAnzahl;             /* Anzahl gespeicherter Zuege */

/* Stellt die Ausgangsstellung her (setzt auch Spieler, Zugnummer, Protokoll zurueck). */
void spielfeld_initialisieren(char spielfeld[8][8]);

/* Wandelt z.B. "e2" in Zeile/Spalte um. Rueckgabe: 1 = gueltig, 0 = ungueltig. */
int feld_ermitteln(char feld[], int *zeile, int *spalte);

/* Verschiebt eine Figur. Rueckgabe: ZUG_OK oder ein Fehlercode (siehe oben). */
int figur_bewegen(char spielfeld[8][8],
                  int startZeile, int startSpalte,
                  int zielZeile, int zielSpalte);

/* Datei-Funktionen. Rueckgabe: 1 = Erfolg, 0 = Fehler. */
int spielstand_speichern(char spielfeld[8][8], const char *dateiname);
int spielstand_laden(char spielfeld[8][8], const char *dateiname);

#endif
