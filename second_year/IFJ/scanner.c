/**
 * Projekt IFJ 2017
 * @brief Implementacia lexikalneho analyzatoru
 * @Author PatrikHolop, xholop01
 * @Author PetrKapoun, xkapou04
 * @file scanner.c
 */

//kniznice a rozhrania
#include "token.h"
#include "scanner.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "utl_err.h"
#include "utl_debug.h"


//pripravy token po rozoznani lexemy
void prepare_token(token *t, char *string, FILE *f, int symbol, bool return_last) {
    if (return_last) {
        ungetc(symbol, f);
    }
    t->data_length = strlen(string);
    t->data = (char *) gc_new((t->data_length+1)*sizeof(char));
    strcpy(t->data, string);
    gc_delete(string);
    string = NULL;
}

//prida znak do retazca string
char *append_char(char *string, int value) {
    int length;
    if (string == NULL) {
        length = 0;
    } else {
        length = strlen(string);
    }
    char *new_string = (char *) gc_new((length+2) * sizeof(char));
    strcpy(new_string, string);
    gc_delete(string);
    string = NULL;
    new_string[length] = (char)value;
    new_string[length+1] = '\0';
    return new_string;
}

void check_keyword(token *t, char *str) {
    if (strcmp("as", str) == 0) {
        t->type = t_kw_as;
    }
    else if (strcmp("asc", str) == 0) {
        t->type = t_kw_asc;
    }
    else if (strcmp("declare", str) == 0) {
        t->type = t_kw_declare;
    }
    else if (strcmp("dim", str) == 0) {
        t->type = t_kw_dim;
    }
    else if (strcmp("do", str) == 0) {
        t->type = t_kw_do;
    }
    else if (strcmp("double", str) == 0) {
        t->type = t_kw_double;
    }
    else if (strcmp("else", str) == 0) {
        t->type = t_kw_else;
    }
    else if (strcmp("end", str) == 0) {
        t->type = t_kw_end;
    }
    else if (strcmp("chr", str) == 0) {
        t->type = t_kw_chr;
    }
    else if (strcmp("function", str) == 0) {
        t->type = t_kw_function;
    }
    else if (strcmp("if", str) == 0) {
        t->type = t_kw_if;
    }
    else if (strcmp("input", str) == 0) {
        t->type = t_kw_input;
    }
    else if (strcmp("integer", str) == 0) {
        t->type = t_kw_integer;
    }
    else if (strcmp("length", str) == 0) {
        t->type = t_kw_length;
    }
    else if (strcmp("loop", str) == 0) {
        t->type = t_kw_loop;
    }
    else if (strcmp("print", str) == 0) {
        t->type = t_kw_print;
    }
    else if (strcmp("return", str) == 0) {
        t->type = t_kw_return;
    }
    else if (strcmp("scope", str) == 0) {
        t->type = t_kw_scope;
    }
    else if (strcmp("string", str) == 0) {
        t->type = t_kw_string;
    }
    else if (strcmp("substr", str) == 0) {
        t->type = t_kw_substr;
    }
    else if (strcmp("then", str) == 0) {
        t->type = t_kw_then;
    }
    else if (strcmp("while", str) == 0) {
        t->type = t_kw_while;
    }
    else if (strcmp("and", str) == 0) {// rezervovana klicova slova
        t->type = t_kw_and;
    }
    else if (strcmp("boolean", str) == 0) {
        t->type = t_kw_boolean;
    }
    else if (strcmp("continue", str) == 0) {
        t->type = t_kw_continue;
    }
    else if (strcmp("elseif", str) == 0) {
        t->type = t_kw_elseif;
    }
    else if (strcmp("exit", str) == 0) {
        t->type = t_kw_exit;
    }
    else if (strcmp("false", str) == 0) {
        t->type = t_kw_false;
    }
    else if (strcmp("for", str) == 0) {
        t->type = t_kw_for;
    }
    else if (strcmp("next", str) == 0) {
        t->type = t_kw_next;
    }
    else if (strcmp("not", str) == 0) {
        t->type = t_kw_not;
    }
    else if (strcmp("or", str) == 0) {
        t->type = t_kw_or;
    }
    else if (strcmp("shared", str) == 0) {
        t->type = t_kw_shared;
    }
    else if (strcmp("static", str) == 0) {
        t->type = t_kw_static;
    }
    else if (strcmp("true", str) == 0) {
        t->type = t_kw_true;
    }
    else {
        t->type = t_id;
    }
}


//ziska dalsi token, pouzivana funkcia v SA
token *get_next_token(FILE *input_file) {
    //rozoznavany retazec
    char *string = (char *) gc_new(sizeof(char));
    string[0] = '\0';
    //aktualny stav
    enum state current_state = init;
    //vytvorenie tokenu
    token *final_token = (token *) gc_new(sizeof(token));

    //rozoznavanie lexemy
    while (true) {
        //nacitanie symbolu
        int symbol = getc(input_file);
        /* case insensitive, pro porovnani nazvu promennych v dalsich
        castech bude lepsi kdyz bude PrOm a proM prevedene na prom??? */
        if (isalpha(symbol) && current_state != string_next) {
            symbol = tolower(symbol);
        }

        switch (current_state) {
            case init:
                //init -> end of line
                if (symbol == '\n' || symbol==13) {
                    string = append_char(string, symbol);
                    prepare_token(final_token, string, input_file, symbol, false);
                    final_token->type = t_eoln;
                    dbg_scan_msg("eoln\n");
                    return final_token;
                }
                //init -> end of file
                else if (symbol == EOF) {
                    final_token->type = t_eof;
                    gc_delete(string);
                    final_token->data = "eof";
                    dbg_scan_msg("eof\n");
                    return final_token;
                }
                //init -> integer
                else if (isdigit(symbol)) {
                    current_state = type_integer;
                    string = append_char(string, symbol);
                }
                //init -> id
                else if (isalpha(symbol) || symbol == '_') {
                    current_state = id;
                    string = append_char(string,symbol);
                }
                //init -> string begin
                else if (symbol == '!') {
                    current_state = string_begin;
                    //string = append_char(string, symbol);
                }
                //init -> comment_line
                else if (symbol == 39) { //apostrof
                    current_state = comment_line;
                }
                //init -> plus
                else if (symbol == '+') {
                    current_state = plus;
                    string = append_char(string, symbol);
                }
                //init -> minus
                else if (symbol == '-') {
                    current_state = minus;
                    string = append_char(string, symbol);
                }
                //init -> asterisk
                else if (symbol == '*') {
                    current_state = asterisk;
                    string = append_char(string, symbol);
                }
                //init -> slash
                else if (symbol == '/') {
                    current_state = slash;
                }
                //init -> comma token -> init
                else if (symbol == ',') {
                    string = append_char(string, symbol);
                    prepare_token(final_token, string, input_file, symbol, false);
                    final_token->type = t_comma;
                    dbg_scan_msg("comma\n");
                    return final_token;
                }
                //init -> semicolon token -> init
                else if (symbol == ';') {
                    string = append_char(string, symbol);
                    prepare_token(final_token, string, input_file, symbol, false);
                    final_token->type = t_semicolon;
                    dbg_scan_msg("semicolon\n");
                    return final_token;
                }
                //init -> equal token -> init
                else if (symbol == '=') {
                    string = append_char(string, symbol);
                    prepare_token(final_token, string, input_file, symbol, false);
                    final_token->type = t_equal;
                    dbg_scan_msg("equal\n");
                    return final_token;
                }
                //init -> less
                else if (symbol == '<') {
                    current_state = less;
                    string = append_char(string, symbol);
                }
                //init -> higher
                else if (symbol == '>') {
                    current_state = higher;
                    string = append_char(string, symbol);
                }
                //init -> backslash
                else if (symbol == 92) { // zpetne lomitko
                    current_state = backslash;
                    string = append_char(string, symbol);
                }
                //init -> leftbracket token -> init
                else if (symbol == '(') {
                    string = append_char(string, symbol);
                    prepare_token(final_token, string, input_file, symbol, false);
                    final_token->type = t_left_bracket;
                    dbg_scan_msg("left_bracket\n");
                    return final_token;
                }
                //init -> rightbracket token -> init
                else if (symbol == ')') {
                    string = append_char(string, symbol);
                    prepare_token(final_token, string, input_file, symbol, false);
                    final_token->type = t_right_bracket;
                    dbg_scan_msg("right_bracket\n");
                    return final_token;
                }
                // white character
                else if (isblank(symbol)) {
                }
                // unexpected character
                else {
                    string = append_char(string, symbol);
                    prepare_token(final_token, string, input_file, symbol, false);
                    dbg_scan_msg("LEA_ERR\n");
                    dbg_crash_msg("Unexpected symbol\n");
                    utl_exit(LEA_ERR);
                }
                break;
            case id:
                //id -> id
                if (isdigit(symbol) || isalpha(symbol) || symbol == '_') {
                    string = append_char(string, symbol);
                }
                //id token TOKEN
                else {
                    prepare_token(final_token, string, input_file, symbol, true);
                    check_keyword(final_token, final_token->data);
                    if (final_token->type != t_id)
                      dbg_scan_msg("keyword\n");
                    else
                      dbg_scan_msg("id\n");
                    return final_token;
                }
                break;
            case type_integer:
                //integer -> integer
                if (isdigit(symbol)) {
                    string = append_char(string, symbol);
                }
                //integer -> double exponent (22e10 atd)
                else if (symbol == 'e' || symbol == 'E') {
                    string = append_char(string, symbol);
                    current_state = type_double_exp;
                }
                //integer -> double
                else if (symbol == '.') {
                    string = append_char(string, symbol);
                    current_state = to_double;
                }
                //integer token
                else {
                    prepare_token(final_token, string, input_file, symbol, true);
                    final_token->type = t_integer;
                    dbg_scan_msg("integer\n");
                    return final_token;
                }
                break;
            //used to lead at least one digit before double state
            case to_double:
                if (isdigit(symbol)) {
                    string = append_char(string, symbol);
                    current_state = type_double;
                }
                else {
                    prepare_token(final_token, string, input_file, symbol, true);
                    final_token->type = t_unexpected;
                    dbg_scan_msg("LEA ERR\n");
                    dbg_crash_msg("Unexpected symbol\n");
                    utl_exit(LEA_ERR);
                }
                break;
            case type_double_exp:
                //double_exp -> double_exp
                if (isdigit(symbol)) {
                    string = append_char(string, symbol);
                    current_state = type_double_exp_final;
                }
                //double_exp -> double_exp_final (22e10 atd)
                else if (symbol == '+' || symbol == '-') {
                    string = append_char(string, symbol);
                    current_state = type_double_exp_pl_min;
                }
                // token napr '22e', neocekavany?
                else {
                    prepare_token(final_token, string, input_file, symbol, true);
                    final_token->type = t_unexpected;
                    dbg_scan_msg("LEA ERR\n");
                    dbg_crash_msg("Unexpected symbol\n");
                    utl_exit(LEA_ERR);
                }
                break;
            case type_double_exp_pl_min:
                //double_exp -> double_exp
                if (isdigit(symbol)) {
                    string = append_char(string, symbol);
                    current_state = type_double_exp_final;
                }
                // token napr '22e+', neocekavany?
                else {
                    prepare_token(final_token, string, input_file, symbol, true);
                    final_token->type = t_unexpected;
                    dbg_scan_msg("LEA ERR\n");
                    dbg_crash_msg("Unexpected symbol\n");
                    utl_exit(LEA_ERR);
                }
                break;
            case type_double_exp_final:
                if (isdigit(symbol)) {
                    string = append_char(string, symbol);
                }
                else {
                    prepare_token(final_token, string, input_file, symbol, true);
                    final_token->type = t_double;
                    dbg_scan_msg("double exp\n");
                    return final_token;
                }
                break;
            case type_double:
                //double -> double
                if (isdigit(symbol)) {
                    string = append_char(string, symbol);
                }
                //double -> double exponent (22.065e10 atd)
                else if (symbol == 'e' || symbol == 'E') {
                    string = append_char(string, symbol);
                    current_state = type_double_exp;
                }
                else {
                    prepare_token(final_token, string, input_file, symbol, true);
                    final_token->type = t_double;
                    dbg_scan_msg("double\n");
                    return final_token;
                }
                break;
            case string_begin:
                //comment_block -> comment_end_try
                if (symbol == 34) { // ascii 34 = uvozovky"
                    //string = append_char(string, symbol);
                    current_state = string_next;
                }
                else if (symbol == EOF) {
                    gc_delete(string);
                    final_token->data = "eof";
                    final_token->type = t_eof_unexpected;
                    dbg_scan_msg("eof\n");
                    dbg_crash_msg("Unexpected symbol\n");
                    utl_exit(LEA_ERR);
                }
                else { // t_unexpected token
                    prepare_token(final_token, string, input_file, symbol, true);
                    final_token->type = t_unexpected;
                    dbg_scan_msg("LEA ERR\n");
                    dbg_crash_msg("Unexpected symbol\n");
                    utl_exit(LEA_ERR);
                }
                break;
            case string_next:
                // token t_string
                if (symbol == 34) { // ascii 34 = uvozovky"
                    //string = append_char(string, symbol);
                    prepare_token(final_token, string, input_file, symbol, false);
                    final_token->type = t_string;
                    dbg_scan_msg("string -> <%s>\n", final_token->data);
                    return final_token;
                }
                // zaciatok escape sekvencie
                else if (symbol == 92) {
                    dbg_scan_msg("escape sequence begin\n");
                    string = append_char(string, symbol);
                    current_state = escape_first;
                }
                // medzera -> treba je escapnut
                else if (symbol == 32) {
                    string = append_char(string, 92);
                    string = append_char(string, '0');
                    string = append_char(string, '3');
                    string = append_char(string, '2');
                }
                // neocakavane prerusenie stringu
                else if (symbol == EOF || symbol == '\n' || symbol == 13) {
                    gc_delete(string);
                    final_token->data = "eof";
                    final_token->type = t_eof_unexpected;
                    dbg_scan_msg("LEA ERR\n");
                    dbg_crash_msg("Unexpected symbol\n");
                    utl_exit(LEA_ERR);
                }
                else {	// prida se znak do string_next
                    string = append_char(string, symbol);
                }
                break;
            case escape_first:
                // koniec escape sekvencie pre specialne znaky
                if (symbol == 34) {
                    string = append_char(string, '0');
                    string = append_char(string, '3');
                    string = append_char(string, '4');
                    dbg_scan_msg("\" escape sequence end\n");
                    current_state = string_next;
                }
                else if (symbol == 't') {
                    string = append_char(string, '0');
                    string = append_char(string, '0');
                    string = append_char(string, '9');
                    dbg_scan_msg("tab escape sequence end\n");
                    current_state = string_next;
                }
                else if (symbol == 92) {
                    string = append_char(string, '0');
                    string = append_char(string, '9');
                    string = append_char(string, '2');
                    dbg_scan_msg("\\\\escape sequence end\n");
                    current_state = string_next;
                }
                else if (symbol == 'n') {
                    string = append_char(string, '0');
                    string = append_char(string, '1');
                    string = append_char(string, '0');
                    dbg_scan_msg("new line escape sequence end\n");
                    current_state = string_next;
                }
                // ciselna escape sekvenicia pre tvar /000 -> chyba
                else if (symbol == '0') {
                    string = append_char(string, '0');
                    current_state = escape_second_null;
                }
                // ciselne escape sekvencie tvaru /1dd
                else if (symbol == '1') {
                    string = append_char(string, symbol);
                    current_state = escape_second;
                }
                else if (symbol == '2') {
                    string = append_char(string, symbol);
                    current_state = escape_second_two;
                }
                // chybna escape sek.
                else {
                    gc_delete(string);
                    final_token->data = "eof";
                    final_token->type = t_eof_unexpected;
                    dbg_scan_msg("LEA ERR\n");
                    dbg_crash_msg("Unexpected symbol\n");
                    utl_exit(LEA_ERR);
                }
                break;
            // aktualny tvar \0dd
            case escape_second_null:
                // /dDd
                if (symbol == '0') {
                    string = append_char(string, symbol);
                    current_state = escape_third_null;
                }
                else if (isdigit(symbol)) {
                    string = append_char(string, symbol);
                    current_state = escape_third;
                }
                // chybna escape sek.
                else {
                    gc_delete(string);
                    final_token->data = "eof";
                    final_token->type = t_eof_unexpected;
                    dbg_scan_msg("LEA ERR\n");
                    dbg_crash_msg("Unexpected symbol in escape sequence\n");
                    utl_exit(LEA_ERR);
                }
                break;
            case escape_second:
                // /dDd
                if (isdigit(symbol)) {
                    string = append_char(string, symbol);
                    current_state = escape_third;
                }
                // chybna escape sek.
                else {
                    gc_delete(string);
                    final_token->data = "eof";
                    final_token->type = t_eof_unexpected;
                    dbg_scan_msg("LEA ERR\n");
                    dbg_crash_msg("Unexpected symbol in escape sequence\n");
                    utl_exit(LEA_ERR);
                }
                break;
            // aktualny stav \00d
            case escape_third_null:
                // prechod do automatu
                if (isdigit(symbol) && symbol!='0') {
                    string = append_char(string, symbol);
                    current_state = string_next;
                }
                // chybna escape sek.
                else {
                    gc_delete(string);
                    final_token->data = "eof";
                    final_token->type = t_eof_unexpected;
                    dbg_scan_msg("LEA ERR\n");
                    dbg_crash_msg("Unexpected symbol in escape sequence\n");
                    utl_exit(LEA_ERR);
                }
                break;
            case escape_third:
                // /ddD
                if (isdigit(symbol)) {
                    string = append_char(string, symbol);
                    dbg_scan_msg("escape sequence end\n");
                    current_state = string_next;
                }
                // chybna escape sek.
                else {
                    gc_delete(string);
                    final_token->data = "eof";
                    final_token->type = t_eof_unexpected;
                    dbg_scan_msg("LEA ERR\n");
                    dbg_crash_msg("Unexpected symbol in escape sequence\n");
                    utl_exit(LEA_ERR);
                }
                break;
            case escape_second_two:
                // /dDd
                if (symbol == '0' || symbol == '1' || symbol == '2' || symbol == '3' || symbol == '4') {
                    string = append_char(string, symbol);
                    current_state = escape_third;
                }
                else if (symbol == '5') {
                    string = append_char(string, symbol);
                    current_state = escape_third_two;
                }
                // chybna escape sek.
                else {
                    gc_delete(string);
                    final_token->data = "eof";
                    final_token->type = t_eof_unexpected;
                    dbg_scan_msg("LEA ERR\n");
                    dbg_crash_msg("Unexpected symbol in escape sequence\n");
                    utl_exit(LEA_ERR);
                }
                break;
            case escape_third_two:
                // /dDd
                if (symbol == '0' || symbol == '1' || symbol == '2' || symbol == '3' || symbol == '4' || symbol == '5') {
                    string = append_char(string, symbol);
                    dbg_scan_msg("escape sequence end\n");
                    current_state = string_next;
                }
                // chybna escape sek.
                else {
                    gc_delete(string);
                    final_token->data = "eof";
                    final_token->type = t_eof_unexpected;
                    dbg_scan_msg("LEA ERR\n");
                    dbg_crash_msg("Unexpected symbol in escape sequence\n");
                    utl_exit(LEA_ERR);
                }
                break;
            case comment_line:
                // token t_eoln
                if (symbol == '\n') {
                    string = append_char(string, symbol);
                    prepare_token(final_token, string, input_file, symbol, false);
                    final_token->type = t_eoln;
                    dbg_scan_msg("eoln\n");
                    return final_token;
                }
                // token t_eof
                else if (symbol == EOF) {
                    gc_delete(string);
                    final_token->data = "eof";
                    final_token->type = t_eof;
                    dbg_scan_msg("eof\n");
                    return final_token;
                }
                break;
            case comment_block:
                //comment_block -> comment_end_try
                if (symbol == 39) { // ascii 39 = apostrof
                    current_state = comment_end_try;
                }
                // token t_eof
                else if (symbol == EOF) {
                    gc_delete(string);
                    final_token->data = "eof";
                    final_token->type = t_eof;
                    dbg_scan_msg("eof\n");
                    dbg_crash_msg("Unexpected symbol\n");
                    utl_exit(LEA_ERR);
                }
                break;
            case comment_end_try:
                //comment_block -> comment_end_try
                if (symbol == '/') {
                    current_state = init;
                }
                else if (symbol == 39) { // ascii 39 = apostrof
                    current_state = comment_end_try;
                }
                // token t_eof
                else if (symbol == EOF) {
                    gc_delete(string);
                    final_token->data = "eof";
                    final_token->type = t_eof;
                    dbg_scan_msg("eof\n");
                    dbg_crash_msg("Unexpected symbol\n");
                    utl_exit(LEA_ERR);
                }
                else {
                    current_state = comment_block;
                }
                break;
            case plus:
                // token t_plus_equal
                if (symbol == '=') {
                    string = append_char(string, symbol);
                    prepare_token(final_token, string, input_file, symbol, false);
                    final_token->type = t_plus_equal;
                    dbg_scan_msg("plus equal\n");
                    return final_token;
                }
                // token t_plus
                else {
                    prepare_token(final_token, string, input_file, symbol, true);
                    final_token->type = t_plus;
                    dbg_scan_msg("plus\n");
                    return final_token;
                }
                break;
            case minus:
                // token t_minus_equal
                if (symbol == '=') {
                    string = append_char(string, symbol);
                    prepare_token(final_token, string, input_file, symbol, false);
                    final_token->type = t_minus_equal;
                    dbg_scan_msg("minux equal\n");
                    return final_token;
                }
                // token t_minus
                else {
                    prepare_token(final_token, string, input_file, symbol, true);
                    final_token->type = t_minus;
                    dbg_scan_msg("minus\n");
                    return final_token;
                }
                break;
            case asterisk:
                // token asterisk_equal
                if (symbol == '=') {
                    string = append_char(string, symbol);
                    prepare_token(final_token, string, input_file, symbol, false);
                    final_token->type = t_asterisk_equal;
                    dbg_scan_msg("asterisk equal\n");
                    return final_token;
                }
                // token asterisk
                else {
                    prepare_token(final_token, string, input_file, symbol, true);
                    final_token->type = t_asterisk;
                    dbg_scan_msg("asterisk\n");
                    return final_token;
                }
                break;
            case slash:
                //slash -> comment_block
                if (symbol == 39) { // ascii 39 = apostrof
                    current_state = comment_block;
                }
                // slash_equal token
                else if (symbol == '=') {
                    string = append_char(string, '/');
                    string = append_char(string, symbol);
                    prepare_token(final_token, string, input_file, symbol, false);
                    final_token->type = t_slash_equal;
                    dbg_scan_msg("slash\n");
                    return final_token;
                }
                // slash token
                else {
                    string = append_char(string, '/');
                    prepare_token(final_token, string, input_file, symbol, true);
                    final_token->type = t_slash;
                    dbg_scan_msg("slash\n");
                    return final_token;
                }
                break;
            case backslash:
                // blackslash_equal token
                if (symbol == '=') {
                    string = append_char(string, symbol);
                    prepare_token(final_token, string, input_file, symbol, false);
                    final_token->type = t_backslash_equal;
                    dbg_scan_msg("backslash equal\n");
                    return final_token;
                }
                // backslash token
                else {
                    prepare_token(final_token, string, input_file, symbol, true);
                    final_token->type = t_backslash;
                    dbg_scan_msg("backslash\n");
                    return final_token;
                }
                break;
            case less:
                //less -> not_equal
                if (symbol == '>') {
                    string = append_char(string, symbol);
                    prepare_token(final_token, string, input_file, symbol, false);
                    final_token->type = t_not_equal;
                    dbg_scan_msg("not equal\n");
                    return final_token;
                    break;
                }
                //less -> less_equal
                else if (symbol == '=') {
                    string = append_char(string, symbol);
                    prepare_token(final_token, string, input_file, symbol, false);
                    final_token->type = t_less_equal;
                    dbg_scan_msg("less equal\n");
                    return final_token;
                }
                //less token
                else {
                    prepare_token(final_token, string, input_file, symbol, true);
                    final_token->type = t_less;
                    dbg_scan_msg("less\n");
                    return final_token;
                }
                break;
            case higher:
                //higher -> higher_equal
                if (symbol == '=') {
                    string = append_char(string, symbol);
                    prepare_token(final_token, string, input_file, symbol, false);
                    final_token->type = t_higher_equal;
                    dbg_scan_msg("higher equal\n");
                    return final_token;
                }
                //higher token
                else {
                    prepare_token(final_token, string, input_file, symbol, true);
                    final_token->type = t_higher;
                    dbg_scan_msg("higher\n");
                    return final_token;
                }
                break;
            default:
                current_state = init;
        }
    }
    return NULL;
}
