/**
 * Projekt IFJ 2017
 * @brief Typy a funkcie definovane v LA
 * @Author PatrikHolop, xholop01
 * @Author PetrKapoun, xkapou04
 * @file scanner.h
 */

#ifndef IFJ17_FIT_2017_SCANNER_H
#define IFJ17_FIT_2017_SCANNER_H

#include "token.h"
#include <stdbool.h>
#include <stdio.h>

/**
 * Stavy konecneho automatu v LA
 */
enum state {
    init,
    id,
    type_integer,
    to_double,
    type_double,
    type_double_final,
    type_double_exp,
    type_double_exp_pl_min,
    type_double_exp_final, // byl spatny navrh
    string_begin,
    string_next, // nelze pouzit "string" - keyword
    escape_first,
    escape_second,
    escape_second_two,
    escape_third,
    escape_third_two,
    escape_second_null,
    escape_third_null,
    plus,
    minus,
    asterisk,
    slash,
    backslash,
    comment_line,
    comment_block,
    comment_end_try,
    less,
    higher
};

/**
 * @brief Funkcia na urcenie typu dalsej lexemy
 * @param input_file Subor, z ktoreho sa nacitava vstup, pripadne stdin
 * @Returns Ukazatel na token
 */
token *get_next_token(FILE *input_file);


/**
 * @brief Funkcia zada udaje do tokenu,
          pripadne vrati znak na vstup f
 * @param t Token, ktory bude funkcia menit
 * @param string Ukazatel na string, ktory obsahuje data tokenu
 * @param f Subor, v ktorom je zdrojovy subor, pripadne stdin
 * @param symbol Symbol, ktory bol ziskany naviac a vracia sa do vstupneho bufferu
 * @param return_last Pokud je true, provede se vraceni symbolu do souboru f
 */
void prepare_token(token *t, char *string, FILE *f, int symbol, bool return_last);

/**
 * Funkcia prida znak na koniec retazca
 * @Returns String s pridanym znakom
 * @param string Ukazatel na string, do ktoreho pridavame znak
 * @param value Znak, ktory sa prida na koniec stringu
 */
char *append_char(char *string, int value);

/**
 * @brief Nastavi token *t->type podle *str pokud je keyword, tak pro kazdy je
 *		specialni token->type, jinak t->type = t_id
 * @param t Token, ktery funkce meni
 * @param str Ukazatel na string, ktery porovnava
 */
void check_keyword(token *t, char *str);


#endif
