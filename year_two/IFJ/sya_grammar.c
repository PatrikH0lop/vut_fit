/**
 * Projekt IFJ 2017
 * @brief Gramatika syntaktickeho analyzatoru
 * @Author HanakJiri, xhanak33
 * @Author Matej Mitas, xmitas02
 * @file sya_grammar.c
 */

#include "sya_grammar.h"

/**
 * Tabulka pravidel
    0  ntr_start_state,
    1  ntr_par_declaration, //nepouzivane
    2  ntr_par_dec_multi,
    3  ntr_parameter,
    4  ntr_return,      //zastarale
    5  ntr_function,
    6  ntr_init,
    7  ntr_row,
    8  ntr_eol,
    9  ntr_statement,
    10 ntr_order,
    11 ntr_type,
    12 ntr_else,
    13 ntr_elseif,
    14 ntr_expr,
    15 ntr_print_list
 */

int rule_indexes[OFFSET][NTERM] = {
        //  1  2  3  4  5  6   7   8  9   10  11  12  13  14  15
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  13, 0,  0,  0,  0}, //t_kw_string,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  14, 0,  0,  0,  0}, //t_kw_boolean,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  15, 0,  0,  0,  0}, //t_kw_integer,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  16, 0,  0,  0,  0}, //t_kw_double,
        {2, 0, 0, 0, 0, 1, 0,  0,  1, 0,  0,  0,  0,  0,  0,  0}, //t_kw_scope,
        {0, 1, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_kw_as,
        {0, 0, 0, 0, 0, 0, 0,  0,  1, 0,  0,  0,  0,  0,  0,  0}, //t_kw_asc,
        {2, 0, 0, 0, 0, 3, 0,  0,  1, 0,  0,  0,  0,  0,  0,  0}, //t_kw_declare,
        {2, 0, 0, 0, 0, 4, 0,  0,  1, 0,  0,  0,  0,  0,  0,  0}, //t_kw_function,
        {0, 0, 0, 0, 0, 0, 0,  12, 1, 9,  0,  0,  0,  0,  0,  0}, //t_kw_dim,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_kw_for,
        {0, 0, 0, 0, 0, 0, 0,  1,  1, 0,  0,  0,  0,  0,  0,  0}, //t_kw_loop,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_kw_while,
        {0, 0, 0, 0, 0, 0, 0,  12, 1, 17, 0,  0,  0,  0,  0,  0}, //t_kw_do,
        {0, 0, 0, 0, 0, 0, 0,  12, 1, 23, 0,  0,  0,  0,  0,  0}, //t_kw_if,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_kw_then,
        {0, 0, 0, 0, 0, 0, 0,  1,  1, 0,  0,  0,  0,  22, 0,  0}, //t_kw_elseif,
        {0, 0, 0, 0, 0, 0, 0,  1,  1, 0,  0,  0,  21, 1,  0,  0}, //t_kw_else,
        {0, 0, 0, 0, 0, 0, 0,  1,  1, 0,  0,  0,  1,  1,  0,  0}, //t_kw_end,
        {0, 0, 0, 0, 0, 0, 0,  0,  1, 0,  0,  0,  0,  0,  0,  0}, //t_kw_chr,
        {0, 0, 0, 0, 0, 0, 0,  12, 1, 18, 0,  0,  0,  0,  0,  0}, //t_kw_input,
        {0, 0, 0, 0, 0, 0, 0,  12, 1, 19, 0,  0,  0,  0,  0,  0}, //t_kw_print,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_kw_length,
        {0, 0, 0, 0, 0, 0, 0,  12, 1, 8,  0,  0,  0,  0,  0,  0}, //t_kw_return,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_kw_subst,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_kw_continue,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_kw_next,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_kw_exit,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_kw_shared,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_kw_static,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  44, 20}, //t_kw_true,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  44, 20}, //t_kw_false,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  44, 20}, //t_integer,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  44, 20}, //t_double,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  44, 20}, //t_string,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  25, 0,  0,  0,  0,  0}, //t_plus_equal,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  26, 0,  0,  0,  0,  0}, //t_minus_equal,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  27, 0,  0,  0,  0,  0}, //t_asterisk_equal,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  28, 0,  0,  0,  0,  0}, //t_slash_equal,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  29, 0,  0,  0,  0,  0}, //t_backslash_equal,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_semicolon,
        {2, 0, 0, 0, 0, 0, 1,  1,  11,0,  0,  0,  0,  0,  0,  1}, //t_eoln,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_eof_unexpected,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_unexpected,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_plus,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  20}, //t_minus,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_asterisk,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_slash,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_backslash,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_less,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_higher,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_less_equal,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_higher_equal,
        {0, 0, 0, 0, 0, 0, 10, 0,  0, 0,  30, 0,  0,  0,  0,  0}, //t_equal,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_not_equal,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_kw_and,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_kw_or,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_kw_not,
        {0, 6, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  20}, //t_left_backet,
        {0, 0, 1, 1, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_right_backet,
        {0, 5, 0, 5, 0, 0, 0,  12, 1, 24, 0,  0,  0,  0,  0,  20}, //t_id,
        {0, 0, 7, 0, 0, 0, 1,  0,  0, 0,  0,  0,  0,  0,  0,  0}, //t_comma,
        {0, 0, 0, 0, 0, 0, 0,  0,  1, 0,  0,  0,  0,  0,  0,  0}, //t_eof,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  37, 20}, //t_unary_minus,
        {0, 0, 0, 0, 0, 0, 0,  0,  0, 0,  0,  0,  0,  0,  0,  20} //t_id_fce
};

/**
 * Precedencni tabulka pro vyhodnocovani vyrazu
 */
int table[HEAD][HEAD] = {
        {end, end, str, str, str, end, end, end, end, end, end, end, end, str, str, end, str, end, end, str, str},
        {end, end, str, str, str, end, end, end, end, end, end, end, end, str, str, end, str, end, end, str, str},
        {end, end, end, end, end, end, end, end, end, end, end, end, end, str, str, end, str, end, end, str, str},
        {end, end, end, end, end, end, end, end, end, end, end, end, end, str, str, end, str, end, end, str, str},
        {end, end, end, end, end, end, end, end, end, end, end, end, end, str, str, end, str, end, end, str, str},
        {str, str, str, str, str, err, err, err, err, err, err, end, end, str, str, end, str, end, end, str, str},
        {str, str, str, str, str, err, err, err, err, err, err, end, end, str, str, end, str, end, end, str, str},
        {str, str, str, str, str, err, err, err, err, err, err, end, end, str, str, end, str, end, end, str, str},
        {str, str, str, str, str, err, err, err, err, err, err, end, end, str, str, end, str, end, end, str, str},
        {str, str, str, str, str, err, err, err, err, err, err, end, end, str, str, end, str, end, end, str, str},
        {str, str, str, str, str, err, err, err, err, err, err, end, end, str, str, end, str, end, end, str, str},
        {str, str, str, str, str, str, str, str, str, str, str, end, end, str, str, end, str, end, end, str, str},
        {str, str, str, str, str, str, str, str, str, str, str, str, end, str, str, end, str, end, end, str, str},
        {str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, end, str, end, end, str, str},
        {str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, mov, str, mov, err, str, str},
        {end, end, end, end, end, end, end, end, end, end, end, end, end, end, err, end, err, end, end, end, err},
        {end, end, end, end, end, end, end, end, end, end, end, end, end, end, sea, end, err, end, end, err, err},
        {str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, mov, str, mov, err, str, str},
        {str, str, str, str, str, str, str, str, str, str, str, str, str, str, str, err, str, err, err, str, str},
        {end, end, end, end, end, end, end, end, end, end, end, end, end, end, str, end, mov, end, end, str, str},
        {sea, sea, sea, sea, sea, sea, sea, sea, sea, sea, sea, sea, sea, sea, mov, sea, sea, sea, sea, sea, sea}
};


Rule * grm_init_rule(int left_term, int rule_len, int id, ...) {

    // naalokuj misto pro samotny struct 'Rule' a flexibilni array
    Rule *new_rule = (Rule *) gc_new(sizeof(Rule) + (rule_len * sizeof(int)));

    // nastav argumenty
    new_rule->left_terminal = left_term;
    new_rule->rule_length = rule_len;
    new_rule->id = id;

    // napln flexibilni array ve structu
    va_list terms;                                  // delkove variabilni list terminalu
    va_start(terms, id);                            // adresa posledniho fixniho argumentu

    for (int i = 0; i < rule_len; i++)
        new_rule->terminal[i] = va_arg(terms, int);
        //printf("%d\n", va_arg(terms, int));       // Increments ap to the next argument

    va_end(terms);                                  // ukonceni pointrovani

    return new_rule;
}

/**
 * Priradi nove pravidlo
 * @param rules pole pravidel, kam se ma ulozit
 * @param idx na kterou pozici v poli pravidel se ma nove pravidlo ulozit
 * @param rule_to_add pravidlo, ktere se ma pridarit
 */
void grm_add_rule(Rule *rules[], int *idx, Rule *rule_to_add) {

    if (*idx < COUNT) {                             // pokud vubec mame kam davat
        rules[*idx] = rule_to_add;                  // prirad do promenne
        *idx += 1;
    } else {
        dbg_crash_msg("Chyba v gramatice");
        utl_exit(INNER_ERROR);
        return;
    }
}

void grm_init() {
    int i = 0;
    int *idx = &i;

    // inicializace pravidel
    grm_add_rule(r, idx, grm_init_rule(error, 0, 0));
    grm_add_rule(r, idx, grm_init_rule(epsilon, 0, 1));
    grm_add_rule(r, idx, grm_init_rule(ntr_start_state, 10, 2, ntr_eol, ntr_function, tr_kw_scope, tr_eoln, ntr_eol, ntr_row, ntr_eol, tr_kw_end, tr_kw_scope, ntr_eol));//2
    grm_add_rule(r, idx, grm_init_rule(ntr_function, 12, 3, tr_kw_declare, tr_kw_function, tr_id, tr_left_bracket, ntr_parameter, ntr_par_dec_multi, tr_right_bracket, tr_kw_as, ntr_type, tr_eoln, ntr_eol, ntr_function));
    grm_add_rule(r, idx, grm_init_rule(ntr_function, 16, 4, tr_kw_function, tr_id, tr_left_bracket, ntr_parameter, ntr_par_dec_multi, tr_right_bracket, tr_kw_as, ntr_type, tr_eoln, ntr_eol, ntr_row, tr_kw_end, tr_kw_function, tr_eoln, ntr_eol, ntr_function));
    grm_add_rule(r, idx, grm_init_rule(ntr_parameter, 3, 5, tr_id, tr_kw_as, ntr_type));
    grm_add_rule(r, idx, grm_init_rule(ntr_par_declaration, 4, 6, ntr_par_declaration, ntr_par_dec_multi, tr_right_bracket));       //nepouzivane
    grm_add_rule(r, idx, grm_init_rule(ntr_par_dec_multi, 3, 7, tr_comma, ntr_parameter, ntr_par_dec_multi));
    grm_add_rule(r, idx, grm_init_rule(ntr_return, 2, 8, tr_kw_return, ntr_expr));
    grm_add_rule(r, idx, grm_init_rule(ntr_statement, 5, 9, tr_kw_dim, tr_id, tr_kw_as, ntr_type, ntr_init));
    grm_add_rule(r, idx, grm_init_rule(ntr_init, 2, 10, tr_equal, ntr_expr));
    grm_add_rule(r, idx, grm_init_rule(ntr_eol, 2, 11, tr_eoln, ntr_eol));
    grm_add_rule(r, idx, grm_init_rule(ntr_row, 4, 12, ntr_statement, tr_eoln, ntr_eol, ntr_row));
    grm_add_rule(r, idx, grm_init_rule(ntr_type, 1, 13, tr_kw_string));
    grm_add_rule(r, idx, grm_init_rule(ntr_type, 1, 14, tr_kw_boolean));
    grm_add_rule(r, idx, grm_init_rule(ntr_type, 1, 15, tr_kw_integer));
    grm_add_rule(r, idx, grm_init_rule(ntr_type, 1, 16, tr_kw_double));
    grm_add_rule(r, idx, grm_init_rule(ntr_statement, 7, 17, tr_kw_do, tr_kw_while, ntr_expr, tr_eoln, ntr_eol, ntr_row, tr_kw_loop));
    grm_add_rule(r, idx, grm_init_rule(ntr_statement, 2, 18, tr_kw_input, tr_id));
    grm_add_rule(r, idx, grm_init_rule(ntr_statement, 4, 19, tr_kw_print, ntr_expr, tr_semicolon, ntr_print_list));
    grm_add_rule(r, idx, grm_init_rule(ntr_print_list, 3, 20, ntr_expr, tr_semicolon, ntr_print_list));
    grm_add_rule(r, idx, grm_init_rule(ntr_else, 4, 21, tr_kw_else, tr_eoln, ntr_eol, ntr_row));
    grm_add_rule(r, idx, grm_init_rule(ntr_elseif, 7, 22, tr_kw_elseif, ntr_expr, tr_kw_then, tr_eoln, ntr_eol, ntr_row, ntr_elseif));
    grm_add_rule(r, idx, grm_init_rule(ntr_statement, 10, 23, tr_kw_if, ntr_expr, tr_kw_then, tr_eoln, ntr_eol, ntr_row, ntr_elseif, ntr_else, tr_kw_end, tr_kw_if));
    grm_add_rule(r, idx, grm_init_rule(ntr_statement, 2, 24, tr_id, ntr_order));
    grm_add_rule(r, idx, grm_init_rule(ntr_order, 2, 25, tr_plus_equal, ntr_expr));
    grm_add_rule(r, idx, grm_init_rule(ntr_order, 2, 26, tr_minus_equal, ntr_expr));
    grm_add_rule(r, idx, grm_init_rule(ntr_order, 2, 27, tr_asterisk_equal, ntr_expr));
    grm_add_rule(r, idx, grm_init_rule(ntr_order, 2, 28, tr_slash_equal, ntr_expr));
    grm_add_rule(r, idx, grm_init_rule(ntr_order, 2, 29, tr_backslash_equal, ntr_expr));
    grm_add_rule(r, idx, grm_init_rule(ntr_order, 2, 30, tr_equal, ntr_expr));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 3, 32, ntr_expr, tr_asterisk, ntr_expr));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 3, 33, ntr_expr, tr_plus, ntr_expr));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 3, 34, ntr_expr, tr_slash, ntr_expr));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 3, 35, ntr_expr, tr_backslash, ntr_expr));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 3, 36, ntr_expr, tr_minus, ntr_expr));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 2, 37, ntr_expr, tr_unary_minus));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 2, 55, tr_id, tr_unary_minus));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 3, 38, tr_right_bracket, tr_left_bracket, tr_id_fce));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 4, 39, tr_right_bracket, ntr_expr, tr_left_bracket, tr_id_fce));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 1, 43, tr_id));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 1, 44, tr_integer));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 3, 31, tr_right_bracket, ntr_expr, tr_left_bracket));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 3, 45, ntr_expr, tr_less, ntr_expr));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 3, 46, ntr_expr, tr_higher, ntr_expr));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 3, 47, ntr_expr, tr_less_equal, ntr_expr));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 3, 48, ntr_expr, tr_higher_equal, ntr_expr));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 3, 49, ntr_expr, tr_kw_and, ntr_expr));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 3, 50, ntr_expr, tr_kw_or, ntr_expr));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 3, 51, ntr_expr, tr_equal, ntr_expr));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 3, 52, ntr_expr, tr_not_equal, ntr_expr));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 4, 53, tr_kw_chr, tr_left_bracket, tr_id, tr_right_bracket));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 4, 54, tr_kw_asc, tr_left_bracket, tr_id, tr_right_bracket));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 2, 57, ntr_expr, tr_kw_not));
    grm_add_rule(r, idx, grm_init_rule(ntr_expr, 2, 56, tr_id, tr_kw_not));
}


void grm_destroy() {
    for (int i = 0; i < COUNT; i++) {
        gc_delete(r[i]);
    }
}

Rule *get_rule(int term1, int term2) {
    return r[rule_indexes[term1][term2 - OFFSET]];
}

int compare_rules(Rule *rule) {
    int same = 1;                                       //1 = true, 0 = false
    for (int i = 30; i < COUNT;++i) {                   //30 => zacatek pravidel pro precedencni tabulku
        if (rule->rule_length == r[i]->rule_length) {
            for (int j = 0; j < r[i]->rule_length; ++j) {
                if (rule->terminal[j] != r[i]->terminal[j]) {
                    same = 0;
                    break;
                }
            }
            if (same) {
                dbg_msg("rule id: %2d    ", r[i]->id);
			sea_give_expr_rule(r[i]->id);
                sya_test_msg("e  %2d\n", r[i]->id);
                return r[i]->left_terminal;
            }
            same = 1;
        }
    }

    //kotrola, zda se jedna o funkci o n parametrech
    if (rule->terminal[0] == tr_right_bracket) {
        for (int i = 1; i < rule->rule_length; i+= 2) {
            if (rule->terminal[i] == ntr_expr && rule->terminal[i+1] == tr_comma) {
                continue;
            }

            if (rule->terminal[i] == ntr_expr && rule->terminal[i+1] == tr_left_bracket &&
                    rule->terminal[i+2] == tr_id_fce && rule->rule_length == i + 3) {
                dbg_msg("rule id: fce   ");
				sea_give_expr_rule(39);
                return ntr_expr;
            }

            return -1;
        }
    }
    return -1;
}

int get_action(int term1, int term2) {
    dbg_msg("mask  %2d, mask  %2d\n", term2 - PREC + 1, term1 - PREC + 1);
    return table[term1 - PREC][term2 - PREC];
}
