/**
* Projekt IFJ 2017
* @brief Rozhrani semantickeho analyzatoru
* @Author Petr Kapoun, xkapou04
* @Author Patrik Holop, xholop01
* @file sea_library.h
*/

#ifndef IFJ17_FIT_2017_SEM_LIB_H
#define IFJ17_FIT_2017_SEM_LIB_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// prace s pameti a errory
#include "utl_err.h"
#include "utl_debug.h"
#include "utl_mem.h"

#include "sea_queue.h"
#include "sea_vars.h"
#include "sea_retype.h"
#include "sya_terminal.h"
#include "sya_stack.h"
#include "symtable.h"
#include "gvk_generator.h"
#include "token.h"


/* Typ operandu pri reseni vyrazu
*/
enum operand_type {
	sea_type_error,
	sea_type_string,
	sea_type_boolean,
	sea_type_integer,
	sea_type_double
};

//pomocne funkcie sea_library

/**
 * initializuje semantiku pred prvym pouzitim
 */
void sea_init();

/**
 * @brief Zisti, ci je premenna deklarovana v ramci aktualneho scopu
 * @param name Nazov premmenj
 * @returns True, ak uz deklarovana bola, inak false
 */
bool is_declared(char *name);

/**
 * @brief Zisti, ci je nazov meno deklarovanej funkcie
 * @param name Nazov id
 * @returns True, ak je to nazov deklar. funckcie, inak False
 */
bool is_function(char *name);

/**
 * @brief Funkcia na rozoznanie, ci uz bolo platne telo
 * @returns True, ak bola semantika v hlavnom tele programu, inak false
 */
bool was_active_scope();

/**
 * @brief Funkcia na rozoznanie platnosti scopu
 * @returns True, ak je v hlavnom tele programu, inak false
 */
bool is_active_scope();

// funkcie semantickych pravidiel

/**
 * @brief Funkcia obaluje semanticke pravidla pre datove typy
 * @param v_type datovy typ, s ktorym sa pracuje
 * @param init_value v pripade inicializacie hodnota, moze byt nevyuzite
 */
int type_semantics(enum data_type v_type, char *init_value);

/**
 * @brief Funkcia skontroluje, ci vsetky deklarovane funkcie boli definovane
 * @note vola sa na konci programu
 */
void sea_check_allfunc_def();

// funkcne funkcie sea


/**
* @brief Posle token a momentalni pravidlo k semanticke kontrole
* @param tok Token ke zpracovani
* @param rule_number Cislo pravidla
*/
int sea_give_next_token(token *tok, int rule_number);

///funkce vyrazu

/**
* @brief Zjisti jestli je token dulezity pro urceni ktery bude vyraz
* @param tok zkoumany token
*/
void sea_set_expr_by_token(token *tok);

/**
* @brief Nastavi boolove hodnoty pro expr na false
*/
void sea_expr_bools_false();

/**
* @brief Vlozi do fronty token, u t_id kontroluje deklaraci
* @param tok Pridany token
*/
void sea_add_tok_to_expr(token *tok);

/**
* @brief Signal zacatku vyrazu (aby bral tokeny)
*/
void sea_begin_expression();

/**
* @brief Signal konce vyrazu (aby vyhodnotil cely vyraz)
*/
void sea_end_of_expression();

/**
* @brief Prohodi 2 vrchni polozky na zasobniku typu
*/
void sea_type_var_stack_swap();

/**
* @brief Funkce na zaklade signalu z parseru provede typovou kontrolu a
* vygeneruje operaci i s pretypovanim
* @param rule_number cislo pravidla
* @warning not used
*/
void sea_give_expr_rule(int rule_number);

/**
* @brief Kontrola jestli prisel spravny pocet parametru pri volani funkce, vypopuje time_stack
* @param stamp cislo pravidla
* @param params pocet parametru funkce
*/
void sea_time_stamp_check(int stamp, int params);

/**
* @brief V time stamp zasobniku necha nizsi hodnotu z dvou na vrchu
*/
void sea_time_stamp_reduce();

/**
* @brief Rekne jestli je token operator
* @param tok zkoumany token
* @returns true pokud je operator
*/
bool sea_is_operator(token* tok);

/**
* @brief Zjisti jestli je token porovnavaci operator
* @param tok zkoumany token
* @returns true pokud je porovnavaci operator
*/
bool sea_is_compare_operator(token* tok);

/**
* @brief Zjisti typ operandu
* @param tok token operandu
* @returns typ operandu
*/
enum data_type sea_get_tok_var_type(token* tok);
//bool sea_can_retype(enum operand_type type_from, enum operand_type type_to);

/**
* @brief Zjisti hodnotu reprezentujici id (promennou) v generatoru
* @param sym_id nazev promenne
* @returns int hodnotu reprezentujici id
*/
int sea_find_var_gen_id(char *sym_id);

/**
* @brief Zjisti hodnotu reprezentujici id (funkci) v generatoru
* @param sym_id nazev funkce
* @returns int hodnotu reprezentujici funkci
*/
int sea_find_function_gen_id(char *sym_id);

/**
* @brief Zjisti typ promenne
* @param sym_id nazev promenne
* @returns int hodnotu reprezentujici typ
*/
enum data_type_IFJcode17 sea_find_var_type(char *sym_id);

/**
* @brief Zjisti navratovy typ funkce
* @param sym_id nazev funkce
* @returns int hodnotu reprezentujici navratovy typ funkce
*/
enum data_type_IFJcode17 sea_find_function_ret_type(char *sym_id);

#endif
