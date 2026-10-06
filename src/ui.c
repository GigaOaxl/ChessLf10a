/* ui.c - Anzeige und Eingabe (Frontend). */
#include <stdio.h>
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
