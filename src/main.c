/* main.c - Menueschleife (wird in M3 ausgebaut). Aktuell: Brett + ein Testzug. */
#include <stdio.h>
#include "backend.h"
#include "ui.h"

int main(void)
{
    char spielfeld[8][8];

    spielfeld_initialisieren(spielfeld);
    spielfeld_ausgeben(spielfeld);

    figur_bewegen(spielfeld, 6, 4, 4, 4); /* e2 -> e4 */
    spielfeld_ausgeben(spielfeld);
    return 0;
}
