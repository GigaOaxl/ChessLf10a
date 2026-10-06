/* ui.h - Anzeige und Eingabe (Frontend). Zeigt nur an, aendert nie das Spielfeld. */
#ifndef UI_H
#define UI_H

/* Loescht die Konsole und setzt den Cursor nach oben links. */
void bildschirm_leeren(void);

/* Zeichnet das Brett mit Rahmen, Spalten a-h und Zeilen 8-1 komplett neu. */
void spielfeld_ausgeben(char spielfeld[8][8]);

#endif
