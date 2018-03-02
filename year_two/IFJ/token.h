/**
 * Projekt IFJ 2017
 * @brief Definicia tokenu rozoznavaneho pri lexikalnej analyze
 * @Author PatrikHolop, xholop01
 * @Author PetrKapoun, xkapou04
 * @file token.h
 */

#ifndef TOKEN_H
#define TOKEN_H

/**
 * Typy rozoznanych tokenov
 */
enum token_type {
    t_kw_string,
    t_kw_boolean,
    t_kw_integer,
    t_kw_double,
    t_kw_scope,
    t_kw_as,
    t_kw_asc,
    t_kw_declare,
    t_kw_function,
    t_kw_dim,
    t_kw_for,
    t_kw_loop,
    t_kw_while,
    t_kw_do,
    t_kw_if,
    t_kw_then,
    t_kw_elseif,
    t_kw_else,
    t_kw_end,
    t_kw_chr,
    t_kw_input,
    t_kw_print,
    t_kw_length,
    t_kw_return,
    t_kw_substr,
    t_kw_continue,
    t_kw_next,
    t_kw_exit,
    t_kw_shared,
    t_kw_static,
    t_kw_true,
    t_kw_false,
    t_integer,
    t_double,
    t_string,
    t_plus_equal,
    t_minus_equal,
    t_asterisk_equal,
    t_slash_equal,
    t_backslash_equal,
    t_semicolon,
    t_eoln,
    t_eof_unexpected, /* u stringu !"strinEOF !EOF !"EOF, v data je
                      k precteni token ktery se mel puvodne nacist*/
    t_unexpected,
    t_plus,
    t_minus,
    t_asterisk,
    t_slash,
    t_backslash,
    t_less,
    t_higher,
    t_less_equal,
    t_higher_equal,
    t_equal,
    t_not_equal,
    t_kw_and,
    t_kw_or,
    t_kw_not,
    t_left_bracket,
    t_right_bracket,
    t_id,
    t_comma,
    t_eof
};

/**
 * Typ token, ktory vracia LA
 */
typedef struct {
    enum token_type type;
    char *data;
    int data_length;
} token;

#endif
