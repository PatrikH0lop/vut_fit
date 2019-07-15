/**
 * Projekt IFJ 2017
 * @brief Rozhrani pro typovou kontrolu, pretypovani
 * @Author Matej Mitas, xmitas02
 * @file sea_retype.h
 */

#ifndef IFJ17_FIT_2017_SEA_RETYPE_H
#define IFJ17_FIT_2017_SEA_RETYPE_H

#include "sea_library.h"
#include "gvk_generator.h"
#include "sea_vars.h"
#include "sya_terminal.h"
#include "sya_stack.h"
#include "utl_err.h"
#include "utl_mem.h"
#include "token.h"

 // struct pro pretypovani
typedef struct {
	int o1; 		// operand 1
	int o2; 		// operand 2
	int action; 	// akce
	int result;		// to, co potom rveme na stack
					// token type
	enum token_type sign;
	//
} Sea_retype_prop;


/**
 * Vytiskne pretypovaci zasobnik
 * @param zasobnik pretypovani
 */
void stack_retype_print(Stack *s);

/**
 * Vytiskne jednu hodnotu jako jeji datovy typ
 * @param ciselnik hodnot
 */
void sea_retype_item_print(int i);

/**
* Inicializace struktury pro pretypovani
* @param token operator pro dalsi rozhodovaci logiku
* @return struktura pro pretypovani
*/
Sea_retype_prop * sea_retype_init(enum token_type operator);

/**
* Pretypovani matematickych vyrazu (+-*\/)
* @param struktura pro pretypovani
* @param struktura pro generator
* @return
*/
void sea_retype_expr_math(Sea_retype_prop *prop, Generator *g);

/**
* Pretypovani pro prirazeni ve matematickych vyrazech (=<>)
* @param struktura pro pretypovani
* @param struktura pro generator
* @return
*/
void sea_retype_expr_cmp(Sea_retype_prop *prop, Generator *g);

/**
* Pretypovani logickych vyrazu (&|!)
* @param struktura pro pretypovani
* @param struktura pro generator
* @return
*/
void sea_retype_expr_log(Sea_retype_prop *prop, Generator *g);

/**
* Obecna funkce pro pretypovani vyrazu
* @param token operator pro dalsi rozhodovaci logiku
* @param typovy zasobnik
* @param struktura pro generator
*/
void sea_retype_expr(enum token_type operator, Stack *s, Generator *g);

/**
* Obecna funkce pro pretypovani prirazeni
* @param typovy zasobnik
* @param struktura pro generator
*/
void sea_retype_asgn(Stack *s, Generator *g);

#endif
