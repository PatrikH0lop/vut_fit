/**
 * Projekt IFJ 2017
 * @brief Hlavickovy soubor pro gramatiku syntaktickeho analyzatoru
 * @Author HanakJiri, xhanak33
 * @Author Matej Mitas, xmitas02
 * @file sya_grammar.h
 */

#ifndef IFJ17_FIT_2017_SYA_GRAMMAR_H
#define IFJ17_FIT_2017_SYA_GRAMMAR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "sya_terminal.h"
#include "utl_debug.h"
#include "sea_library.h"
#include "utl_mem.h"
#include "utl_err.h"

#define COUNT 55        //pocet pravidel gramatiky
#define HEAD 21         //velikost hlavicky u precedencni tabulky

/**
 * Struktura pravidla
 * left_terminal leva strana pravidla
 * rule_length delka prave strany pravidla
 * id identifikace pravidla
 * terminal prava strana pravidla
 */
typedef struct Rule {
    int left_terminal;
    int rule_length;
    int id;
    int terminal[];
} Rule;

/**
 * Vycet akci pro precedencni tabulku
 */
enum action {
    mov,                // =
    str,                // <
    end,                // >
    err,                //chyba syntaxe
    sea                 //chyba semantiky
};

/**
 * List vsech pravidel
 */
Rule *r[COUNT];

/**
 * Inicializuje gramatiku, alokuje pamet pro gramaticka pravdila
 * @return
 */
void grm_init();

/**
 * Zrusi gramaticka pravidla, uvolni pamet
 */
void grm_destroy();

/**
 * Vrati pravidlo, ktere se ma vykonat
 * @param term1 non terminal na vrcholu zasobniku
 * @param term2 vstupni terminal(token)
 * @return ukazatel an pravidlo
 */
Rule *get_rule(int term1, int term2);

/**
 * Porovna pravidlo s pravidly v gramatice
 * @param rule1 porovnavané pravidlo
 * @return pri shode vrati levou stranu pravidla, jinak -1
 */
int compare_rules(Rule *rule);

/**
 * Urci akci, ktera se ma provest na zaklade terminalu(tokenu) na vstupu a termionalu nejblize vrcholu zasobniku
 * @param term1 terminal nejblize vrcholu zasobniku
 * @param term2 terminal na vstupu
 * @return akce, ktera se ma provest
 */
int get_action(int term1, int term2);

/**
 * Vytvori pravidlo na zaklade poskytnutych parametru
 * @param left_term leva strana pravidla
 * @param rule_len delka prave strany pravidla
 * @param id oznaceni pravidla
 * @param ... terminaly prave strany pravidla v poctu rule_len
 * @return ukazatel na nove vytvorene pravidlo
 */
Rule *grm_init_rule(int left_term, int rule_len, int id, ...);

#endif //IFJ17_FIT_2017_SYA_GRAMMAR_H
