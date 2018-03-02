/**
 * Projekt IFJ 2017
 * @brief Hlavickovy soubor parseru
 * @Author HanakJiri, xhanak33
 * @file sya_parser.h
 */

#ifndef IFJ17_FIT_2017_SYA_PARSER_H
#define IFJ17_FIT_2017_SYA_PARSER_H

#include "sya_terminal.h"
#include "sya_stack.h"
#include "sya_grammar.h"
#include "token.h"
#include "scanner.h"
#include "utl_err.h"

/**
 * Sestavuje vyrazy na zaklade precedencni tabulky
 * @param stack zasobnik terminalu
 * @param f vstupni soubor
 * @param tok posledni nacteny terminal
 * @return vrati posledni nacteny terminal
 */
token * solve_expr(Stack *stack, FILE *f, token *tok);

/**
 * Syntakticky analyzator
 * @return 0, pokud preklad probehl v poradku
 */
void run_parser(FILE *f);

#endif //IFJ17_FIT_2017_SYA_PARSER_H
