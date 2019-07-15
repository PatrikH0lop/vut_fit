/**
 * Projekt IFJ 2017
 * @brief Promenne semantiky
 * @Author Petr Kapoun, xkapou04
 * @Author Patrik Holop, xholop01
 * @file sea_vars.h
 */

#ifndef IFJ17_FIT_2017_SEA_VARS_H
#define IFJ17_FIT_2017_SEA_VARS_H

#include "sya_stack.h"
#include "gvk_generator.h"

bool initializing;
bool declaring;
bool defining;
bool loading_param_type;
bool ending_scope;
int param_index;
bool was_scope;

// BOOLY PRO EXPR
bool logical_expr_active; /* Je aktivni pokud jsme prijali napr token t_kw_if
						   * po kterem ma nasledovat logicka podminka */
bool assign_expr_active; /* Podobne jak to nahore, jen si to musi pamatovat kam
				   * pak ulozit vysledek */
bool print_expr_active; /* Podobne jak to nahore, jen to tiskne rovnou */
bool funkction_expr_active; /* Podobne jak to nahore, jen se bude resit funkce */
bool return_expr_active; /* kontrola navratove hodnoty */
bool save_token_for_expr;

token* sem_expr_last_id; // posledni prichozi t_id mimo vyraz
token* sem_expr_last_assignment; // posledni prichozi =,+=,-=,*= atd.

char *last_var; //posledna aktivna premmena

int last_rule;
enum token_type last_tok_type;
Stack* sea_expr_type_stack; /* Zasobnik datovych typu duplikujici skutecny
							 * zasobnik v generatoru. Slouzi k odhaleni chyb
							 * a ke konverzi na spravne datove typy pred
							 * provedenim operace (+,-,and, or, <, <=)
							 * prirazenim do promenne, volanim funkce,
							 * vracenim hodnotz z funkce atd.*/

//globalny generator
Generator *gen;

// pocitadlo na casove znamky operandu ve vyrazech
int expr_time_stamps_cnt;
// stack na time stampy
Stack* time_stamp_stack;
// stack na gen_id docasnych promennych u kontroly parametru volani funkce
Stack* fn_tmp_gen_id_stack;
// fronta na prichozi tokeny, pamatuje si pouze operandy
Queue_sea *expr_tok_queue;
// fronta na prichozi tokeny, pamatuje si pouze nazvy funkce
Queue_sea *expr_tok_fn_queue;

#endif
