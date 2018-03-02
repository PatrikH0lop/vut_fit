/**
 * Projekt IFJ 2017
 * @brief Hlavny modul interpretu
 * @Author HanakJiri, xhanak33
 * @Author PatrikHolop, xholop01
 * @Author PetrKapoun, xkapou04
 * @Author Matej Mitas, xmitas02
 * @file main.c
 */

#include "main.h"

int debug_stdin = 1;

int main(int argc, char *args[]) {
    // inicializace GC
    gc_init();

    // beh samotneho programu
    if (debug_stdin) {
        run_parser(stdin);
    }
    else {
        FILE *f;
        f = fopen(args[1], "r");

        if (f != NULL) {
            run_parser(f);
            fclose(f);
        } else {
            printf("Nevalidni soubor\n");
        }
    }
    utl_exit(PRG_OK);
}
