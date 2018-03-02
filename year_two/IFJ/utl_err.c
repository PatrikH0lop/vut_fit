/**
 * Projekt IFJ 2017
 * Modul pro chybove stavy
 * @brief Chybove stavy
 * @Author Matej Mitas, xmitas02
 * @file utl_err.c
 */

#include "utl_err.h"

void utl_exit(int exit_code) {
	// uklid po sobe pamet
	gc_cleanup();
	// muzes se ukoncit
	exit(exit_code);
}