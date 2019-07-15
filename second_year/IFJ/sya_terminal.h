/**
 * Projekt IFJ 2017
 * @brief Rozhrani pro terminaly, non-terminaly
 * @Author HanakJiri, xhanak33
 * @file sya_terminal.h
 */

#ifndef IFJ17_FIT_2017_SYA_TERMINAL_H
#define IFJ17_FIT_2017_SYA_TERMINAL_H

#define OFFSET 65       //pocet terminalu = ofset pro non-terminaly
#define NTERM 16        //pocet non terminalu
#define PREC 44         //offset pro hlavicku precedencni tabulky

#include <stdbool.h>

enum terminal_type {
    //tokeny - terminaly
    tr_kw_string,
    tr_kw_boolean,
    tr_kw_integer,
    tr_kw_double,
    tr_kw_scope,
    tr_kw_as,
    tr_kw_asc,
    tr_kw_declare,
    tr_kw_function,
    tr_kw_dim,
    tr_kw_for,
    tr_kw_loop,
    tr_kw_while,
    tr_kw_do,
    tr_kw_if,
    tr_kw_then,
    tr_kw_elseif,
    tr_kw_else,
    tr_kw_end,
    tr_kw_chr,
    tr_kw_input,
    tr_kw_print,
    tr_kw_length,
    tr_kw_return,
    tr_kw_substr,
    tr_kw_continue,
    tr_kw_next,
    tr_kw_exit,
    tr_kw_shared,
    tr_kw_static,
    tr_kw_true,
    tr_kw_false,
    tr_integer,
    tr_double,
    tr_string,
    tr_plus_equal,
    tr_minus_equal,
    tr_asterisk_equal,
    tr_slash_equal,
    tr_backslash_equal,
    tr_semicolon,
    tr_eoln,
    tr_eof_unexpected, /* u stringu !"strinEOF !EOF !"EOF, v data je
                      k precteni token ktery se mel puvodne nacist*/
    tr_unexpected,

    tr_plus,             // +
    tr_minus,            // -
    tr_asterisk,         // *
    tr_slash,            // /
    tr_backslash,        //
    tr_less,             // <
    tr_higher,           // >
    tr_less_equal,       // <=
    tr_higher_equal,     // >=
    tr_equal,            // =
    tr_not_equal,        // <>
    tr_kw_and,           // and
    tr_kw_or,            // or
    tr_kw_not,           // not
    tr_left_bracket,     // (
    tr_right_bracket,    // )
    tr_id,               // id
    tr_comma,            // ,
    tr_eof,              // $
    tr_unary_minus,      // -u
    tr_id_fce,           // fce

    //pocatecni stav
    ntr_start_state,

    //funkce
    ntr_par_declaration,
    ntr_par_dec_multi,
    ntr_parameter,
    ntr_return,
    ntr_function,

    //promenne
    ntr_init,

    //obecne stavy
    ntr_row,
    ntr_eol,
    ntr_statement,
    ntr_order,
    ntr_type,

    //podminka
    ntr_else,
    ntr_elseif,

    ntr_expr,
    ntr_print_list,

    expr_str,
    expr_end,

    error,
    epsilon


};


typedef struct {
    enum terminal_type type;
    bool is_terminal;
    char *data;
    int data_length;
} Terminal;

#endif //IFJ17_FIT_2017_SYA_TERMINAL_H
