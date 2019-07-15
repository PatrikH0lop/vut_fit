/**
 * Projekt IFJ 2017
 * @brief Typova kontrola, pretypovani
 * @Author Matej Mitas, xmitas02
 * @file sea_retype.c
 */

#include "sea_retype.h"

/**
 * Pomocne funkce
 */

// pomocna funkce pro vytisteni zasobniku
void stack_retype_print(Stack *s) {
    Stack_item *loop_item = s->stack_pointer; // priprav si promennou na loopovani
    //printf("----------- Tisknu stack -----------\n");
    // projed vsechny polozky
    while (loop_item != NULL) {

    	sea_dbg_msg("| ");

    	switch (loop_item->data) {
    		case 0:
    			sea_dbg_msg("int ");
    			break;
    		case 1:
    			sea_dbg_msg("double ");
    			break;
    		case 2:
    			sea_dbg_msg("string ");
    			break;
    		case 3:
    			sea_dbg_msg("bool ");
    			break;
    		default:
    			break;
    	}
        // nastav dalsi cyklus
        loop_item = loop_item->next_item;
    }
    // vyprintuj konec stacku
    sea_dbg_msg("| $ |\n");
    //printf("----------- Tisknu stack -----------\n");
}

void sea_retype_item_print(int i) {
	sea_dbg_msg(">Polozka je: ");
	switch (i) {
    	case 0:
    		sea_dbg_msg("int\n");
    		break;
    	case 1:
    		sea_dbg_msg("double\n");
    		break;
    	case 2:
    		sea_dbg_msg("string\n");
    		break;
    	case 3:
    		sea_dbg_msg("bool\n");
    		break;
    	default:
    		break;
    }
}


/**
 * Pravidla pro pretypovani
 */

// tabulka pro rozhodovani pravidel
// 0 ... error
// 1 ... nic nedelej, typy ok
// 2 ... int2float_first
// 3 ... int2float_last
int sea_retype_rules_math[4][4] = {
//   i  d  s  b
	{1, 2, 0, 0}, // int_type
	{3, 1, 0, 0}, // float_type
	{0, 0, 1, 0}, // string_type
	{0, 0, 0, 1}  // bool_type
};

// tabulka pro rozhodovani pravidel
// 0 ... error
// 1 ... nic nedelej, typy ok
// 2 ... float2int_last
// 3 ... int2float_last
int sea_retype_rules_asgn[4][4] = {
//   i  d  s  b
	{1, 3, 0, 0}, // int_type
	{2, 1, 0, 0}, // float_type
	{0, 0, 1, 0}, // string_type
	{0, 0, 0, 1}  // bool_type
};



/**
 * Vyrazy
 */


void sea_retype_expr_math(Sea_retype_prop *prop, Generator *g) {
	/**
	 * String
	 */
	// technicky vzato staci pouze porovnavat prvni
	// protoze do teto fce se muzou dostat jenom 2 stringy
	if (prop->o1 == string_type) {
		if (prop->sign == t_plus) {
			// vysledek je string
			prop->result = string_type;
		} else {
			sea_dbg_msg(">Mame dva string, ale zvolil jsi jinou operaci nez scitani\n");
			dbg_crash_msg("Type Error\n");
			utl_exit(SEA_TYPE_ERR);
		}
	}

	/**
	 * Zde jiz pracujeme pouze s cisly
	 */

	// stejne datovy typy, ale musi se jeste oba pretypovat
	// na cilovy datovy typ
	if (prop->action == 1) {
		// pokud mame deleni pouze intu
		if (prop->sign == t_backslash) {
			//  pretypujeme doubly na int
			if (prop->o1 == float_type) {
				gen_stack_convert_float2int_first(g);
				gen_stack_convert_float2int_second(g);
				prop->o1 = int_type;
				prop->o2 = int_type;
			}
		}
		// pokud mame deleni pouze floatu
		else if (prop->sign == t_slash) {
			// pretypujeme inty na double
			if (prop->o1 == int_type) {
				gen_stack_convert_int2float_first(g);
				gen_stack_convert_int2float_second(g);
				prop->o1 = float_type;
				prop->o2 = float_type;
			}
		}
	}
	// Jine datovy typy, pretypovavame podle pravidla
	else {
		if (prop->sign == t_backslash) {

			if (prop->o1 == float_type) {
				sea_dbg_msg(">FLOAT2INT FIRST\n");
				gen_stack_convert_float2int_first(g);
				// int 
				prop->o1 = int_type;
			} else if (prop->o2 == float_type) {
				sea_dbg_msg(">FLOAT2INT LAST\n");
				gen_stack_convert_float2int_second(g);
				// 
				prop->o2 = int_type;
			}

		} else {
			switch (prop->action) {
				case 2:
					sea_dbg_msg(">INT2FLOAT FIRST\n");
					gen_stack_convert_int2float_first(g);
					prop->o1 = float_type;
					break;
				case 3:
					sea_dbg_msg(">INT2FLOAT LAST\n");
					gen_stack_convert_int2float_second(g);
					prop->o2 = float_type;
					break;
				default:
					break;
			}
		}
	}

	/**
	 * Samotne akce
	 */
	switch (prop->sign) {
		// plus
		case t_plus:
			// u stringu budeme konkatenovat
			if (prop->o1 == string_type) {
				sea_dbg_msg(">CONCAT\n");
				gen_stack_operation_concat_strings(g);
			} else {
				sea_dbg_msg(">ADDS\n");
				gen_stack_operation_plus(g);
			}
			break;
		// minus
	    case t_minus:
	    	sea_dbg_msg(">SUBS\n");
	    	gen_stack_operation_minus(g);
	    	break;
	    // krat
	    case t_asterisk:
	    	sea_dbg_msg(">MULS\n");
	    	gen_stack_operation_multiple(g);
	    	break;
	    // deleno
	    case t_slash:
	    	sea_dbg_msg(">DIVS FLOAT\n");
	    	gen_stack_operation_divide_float(g);
	    	break;
	    // deleno celociselne
	    case t_backslash:
	    	sea_dbg_msg(">DIVS INT\n");
	    	gen_stack_operation_divide_int(g);
	    	break;
	    default:
	    	break;
	}

	// pushni na stack
	// zde je opet jedno, jaky zvolim operand, protoze pri kazde akci musi byt oba stejne
	prop->result = prop->o1;
}

void sea_retype_expr_cmp(Sea_retype_prop *prop, Generator *g) {
	// pokud ne, pretypovat na stejny typ
	if (prop->action != 1) {
		switch (prop->action) {
			case 2:
				sea_dbg_msg(">FLOAT2INT LAST\n");
				gen_stack_convert_float2int_second(g);
				prop->o1 = float_type;
				break;
			case 3:
				sea_dbg_msg(">INT2FLOAT LAST\n");
				gen_stack_convert_int2float_second(g);
				prop->o2 = float_type;
				break;
			default:
				break;
		}
	}

	/**
	 * Samotne akce
	 */

	switch (prop->sign) {
		case t_less:
			sea_dbg_msg(">LT\n");
			gen_stack_eval_LT(g);
			break;
	    case t_higher:
	    	sea_dbg_msg(">GT\n");
	    	gen_stack_eval_GT(g);
	    	break;
	    case t_less_equal:
	    	sea_dbg_msg(">LTE\n");
	    	gen_stack_eval_LTE(g);
	    	break;
	    case t_higher_equal:
			sea_dbg_msg(">GTE\n");
			gen_stack_eval_GTE(g);
			break;
	    case t_equal:
	    	sea_dbg_msg(">EQ\n");
	    	gen_stack_eval_EQ(g);
	    	break;
	    case t_not_equal:
	    	sea_dbg_msg(">NEQ\n");
	    	gen_stack_eval_EQ(g);
			gen_stack_eval_NOT(g);
	    	break;
	    default:
	    	break;
	}

	// zde neni treba premyslet nad navratovou hodnotou
	// vysledek vzdy jako bool
	prop->result = bool_type;
}

void sea_retype_expr_log(Sea_retype_prop *prop, Generator *g) {
	// oba musi byt bool, neni co porovnat
	if (prop->o1 == bool_type && prop->o2 == bool_type) {
		/**
		 * Samotne akce
		 */
		switch (prop->sign) {
			case t_kw_and:
				sea_dbg_msg(">ANDS\n");
				gen_stack_eval_AND(g);
				break;
		    case t_kw_or:
		    	sea_dbg_msg(">ORS\n");
		    	gen_stack_eval_OR(g);
		    	break;
		    case t_kw_not:
		    	sea_dbg_msg(">NOTS\n");
		    	gen_stack_eval_NOT(g);
		    	break;
		    default:
		    	break;
		}
		// zde neni treba premyslet nad navratovou hodnotou
		prop->result = bool_type;
	} else {
		sea_dbg_msg(">Logicke operatory umi pracovat pouze s booly\n");
		dbg_crash_msg("Type Error\n");
		utl_exit(SEA_TYPE_ERR);
	}
}

// inicializace hlavniho objektu pro pretypovani
Sea_retype_prop * sea_retype_expr_init(enum token_type operator) {
	if (operator) {
		// namalocuj
		Sea_retype_prop *new_prop = (Sea_retype_prop *) gc_new(sizeof(Sea_retype_prop));
		// manualne inicilizujeme na -1, default 0, coz je 'int'
		new_prop->o1 = -1;
		new_prop->o2 = -1;
		// to same pro akci
		new_prop->action = -1;
		// nastav vysledek
		new_prop->result = -1;
		// nastav operator
		new_prop->sign = operator;
		//
		return new_prop;
	}
	return NULL;
}

void sea_retype_expr(enum token_type operator, Stack *s, Generator *g) {
	// mame vubec na stacku?
	// explicitne pracujeme s dvema polozkami
	if (stack_len(s) >= 2) {
		/**
		 * Inicializace
		 */
		Sea_retype_prop *prop = sea_retype_expr_init(operator);

		/**
		 * Nastaveni typu operandu
		 */
		prop->o1 = (stack_top(s))->data;
		stack_pop(s); // na stacku uz neni potreba
		prop->o2 = (stack_top(s))->data;
		stack_pop(s); // na stacku uz neni potreba

		/**
		 * Akce, pokud chyba, ukonci to
		 */
		prop->action = sea_retype_rules_math[prop->o1][prop->o2];
		// error, porovnavame podle tabulky
		if (!prop->action) {
			sea_dbg_msg(">Chyba pri pretypovani\n");
			dbg_crash_msg("Type Error\n");
			utl_exit(SEA_TYPE_ERR);
		}

		/**
		 * Modus operandi
		 */

		if ((operator >= t_plus && operator <= t_backslash) ||  (operator >= t_plus_equal && operator <= t_backslash_equal))
			sea_retype_expr_math(prop, gen);
		else if (operator >= t_less && operator <= t_not_equal)
			sea_retype_expr_cmp(prop, gen);
		else if (operator >= t_kw_and && operator <= t_kw_not)
			sea_retype_expr_log(prop, gen);
		else
			sea_dbg_msg(">Ty fogo, co mi to posilas za token, jako?\n");

		/**
		 * Precti vysledek operace (s uspesnym pretypovanim) a uloz na stack
		 */

		if (prop->result > -1) {
			stack_push(s, prop->result);
		}

		// odstraneni structu pro zabaleni dat
		gc_delete(prop);

	} else {
		sea_dbg_msg(">Nedostatek polozek EXPR.\n");
	}
}

/**
 * Prirazeni
 */
void sea_retype_asgn(Stack *s, Generator *g) {
	// mame vubec na stacku?
	// explicitne pracujeme s dvema polozkami
	if (stack_len(s) >= 2) {
		/**
		 * Nastaveni typu operandu
		 */
		int o1 = (stack_top(s))->data;
		stack_pop(s); // na stacku uz neni potreba
		int o2 = (stack_top(s))->data;
		stack_pop(s); // na stacku uz neni potreba
		// promenna na akci
		int action = -1;
		/**
		 * Akce, pokud chyba, ukonci to
		 */
		action = sea_retype_rules_asgn[o1][o2];
		// error, porovnavame podle tabulky
		if (!action) {
			sea_dbg_msg(">Chyba pri pretypovani\n");
			dbg_crash_msg("Type Error\n");
			utl_exit(SEA_TYPE_ERR);
		}

		// pretypovani na polozku
		if (action != 1) {
			//sea_retype_item_print(o1);
			//sea_retype_item_print(o2);

			switch (action) {
				case 3:
					sea_dbg_msg(">FLOAT2INT FIRST\n");
					gen_stack_convert_float2int_first(g);
					o1 = int_type;
					break;
				case 2:
					sea_dbg_msg(">INT2FLOAT FIRST\n");
					gen_stack_convert_int2float_first(g);
					o1 = float_type;
					break;
				default:
					break;
			}
		}
		// uz si s tim delej co chces
		stack_push(s, o1);

	} else {
		sea_dbg_msg(">Nedostatek polozek ASGN.\n");
	}
}
