/**
* Projekt IFJ 2017
* @brief Implementace semantickeho analyzatoru
* @Author Petr Kapoun, xkapou04
* @Author Patrik Holop, xholop01
* @file sea_library.c
*/

#include "sea_library.h"
#include "gvk_generator.h"
#include "sya_terminal.h"
#include "sya_stack.h"
#include "scanner.h"


//pripravi semantiku
void sea_init() {
	sea_dbg_msg("inicializacia semantiky\n");
	initializing = false;
	declaring = false;
	defining = false;
	loading_param_type = false;
	ending_scope = false;
	was_scope = false;
	param_index = 0;

	// BOOLY PRO EXPR
	sea_expr_bools_false();
	sem_expr_last_id = gc_new(sizeof(token));
	sem_expr_last_assignment = gc_new(sizeof(token));
	sem_expr_last_id->type = t_unexpected;
	sem_expr_last_assignment->type = t_unexpected;

	last_rule = -1;
	last_tok_type = 0;

	expr_time_stamps_cnt = 0;
	// fronta pro EXPR
	expr_tok_queue = sea_q_init();
	expr_tok_fn_queue = sea_q_init();
	sea_expr_type_stack = stack_init();
	time_stamp_stack = stack_init();
	fn_tmp_gen_id_stack = stack_init();

	// debug nastaveni
	sea_dbg_expr_gen_msg("\n");
	sea_dbg_expr_msg("\n");

	// uvodne vygenerovanie hlavicky programu
	gen = init_generator();
}


//vracie true, ak uz bolo hlavne telo programu
bool was_active_scope() {
	return was_scope;
}

//vracia true, ak uz sa semantika nachadza v hlavnom tele
bool is_active_scope() {
	if (SYMT_LIST != NULL) {
		if (SYMT_LIST->act != NULL) {
			if (strcmp(SYMT_LIST->act->id, "scope") == 0) {
				return true;
			}
		}
	}
	return false;
}

//skontroluje spravnost typu parametru na danom indexe
bool check_param(char *name, enum data_type v_type) {
	if (param_index+1 >= MAX_PARAMS_LEN)
		return false;
	//Symbol *param = SYMT_LIST->act->params[param_index];
	Symbol *param = sya_param_find_by_id(param_index);
	if (param == NULL) {
		sea_dbg_msg("volany parameter <%s> nebol definovany v deklaracii (ind. %d)\n", name, param_index);
		param_index++;
		return false;
	}
	else if (param->type != v_type) {
		sea_dbg_msg("volany parameter <%s> ma iny typ v deklaracii ako definicii (ind. %d)\n", name, param_index);
		param_index++;
		return false;
	}
	else if (strcmp(param->id, name) != 0) {
		sea_dbg_msg("volany paramater <%s> ma ine id v deklaracii (<%s>) ako definicii, menim ho (ind. %d)\n", name, param->id, param_index);
		sya_param_change_id(param->id, name);
		//param_index++;
		//return false;
	}
	param_index++;
	return true;
}

//skontroluje, ci nenastalo definovanie mensieho poctu parametrov ako v deklaracii
bool check_remaining_param() {
	if (param_index+1 >= MAX_PARAMS_LEN) {
		sea_dbg_msg("prilis vela parametrov, nedefinovane chovanie\n");
		return true;
	}
	else {
		//Symbol *param = SYMT_LIST->act->params[param_index];
		Symbol *param = sya_param_find_by_id(param_index);
		if (param != NULL) {
			sea_dbg_msg("v deklaracii sa nachadza viac parametrov ako v definicii\n");
			return false;
		}
	}
	return true;
}

//skontroluje ci je id parameter
bool is_param(char *name) {
		sea_dbg_msg("kontrola parametru <%s> vo funkcii <%s>\n", name, SYMT_LIST->act->id);
		last_var = name;
		Symbol *temp = sya_param_find(name);
		if (temp == NULL) {
			sea_dbg_msg("parameter <%s> nebol najdeny vo funkcii <%s>\n", name, SYMT_LIST->act->id);
			return false;
		}
		sea_dbg_msg("parameter <%s> bol najdeny vo funkcii <%s>\n", name, SYMT_LIST->act->id);
		return true;
}

//skontroluje, ci je premenna deklarovana
bool is_declared(char *name) {
	sea_dbg_msg("kontrola deklaracie mena <%s>\n", name);
	last_var = name;
	Symbol *temp = sya_sym_find(name);
	if (temp == NULL) {
		sea_dbg_msg("<%s> nebolo najdene vo funkcii <%s>\n", name, SYMT_LIST->act->id);
		return false;
	}
	sea_dbg_msg("<%s> bolo najdene vo funkcii <%s>\n", name, SYMT_LIST->act->id);
	return true;
}

//skontroluje, ci id je meno funkcie
bool is_function(char *name) {
	Symt_table *temp = sya_symt_find(name);
	if (strcmp(name, "chr") == 0 || strcmp(name, "substr") == 0 || strcmp(name, "length") == 0 || strcmp(name, "asc") == 0) {
		return true;
	}
	if (temp == NULL) {
		sea_dbg_msg("funkcia <%s> nie je deklarovana ani definovana\n", name);
		return false;
	}
	sea_dbg_msg("funkcia <%s> je deklarovana alebo definovana\n", name);
	return true;
}

//kontroluje semantiku typov pri vytvoreni premennych a funkcii
int type_semantics(enum data_type v_type, char *init_value) {
	// dim a as integer
	if (initializing) {
		sea_dbg_msg("inicializacia <%s> v type_semantics\n\n", last_var);
		sya_sym_edit_type(last_var, v_type);
		sya_sym_edit_val(last_var, init_value);
		//vygeneruj implicitnu inicializaciu podla typu
		sea_dbg_msg("inicializujem premmenu <%s>\n", last_var);
		Symbol *temp = sya_sym_find(last_var);
		if (v_type == v_int) {
			//gen_stack_push_int(gen, 0);
            gen_stack_push_constant(gen, int_type, "0");
		}
		else if (v_type == v_double) {
			//gen_stack_push_float(gen, "0.0");
            gen_stack_push_constant(gen, float_type, "0.0");
		}
		else if (v_type == v_string) {
			//gen_stack_push_string(gen, "");
            gen_stack_push_constant(gen, string_type, "");
		}
		else if (v_type == v_bool) {
			//gen_stack_push_bool(gen, 0);
            gen_stack_push_constant(gen, bool_type, "false");
		}
		gen_order_assignment_from_stack(gen, temp->gen_id);
		initializing = false;
	}
	// declare function fce as integer
	else if (declaring && !loading_param_type) {
		sea_dbg_msg("nastavenie typu %d funkcii <%s> pri deklarovani\n", v_type, SYMT_LIST->act->id);
		SYMT_LIST->act->ret_type = v_type;
		declaring = false;
		param_index = 0;
		//popni vsetky hodnoty parametrov v opacnom poradi
		if (defining == true) {
			sea_dbg_msg("idem nacitavat parametre (pocet <%d>)\n", SYMT_LIST->act->params_count);
			for (int i = SYMT_LIST->act->params_count; i >= 0; i--) {
				Symbol *temp = sya_param_find_by_id(i);
				sea_dbg_msg("idem nacitavat parameter <%s>\n", temp->id);
				gen_order_assignment_from_stack(gen, temp->gen_id);
			}
			defining = false;
		}
	}
	else if (declaring && loading_param_type) {
		sea_dbg_msg("nastavenie typu parametru <%s> na %d\n", last_var, v_type);
		sya_param_add_type(last_var, v_type);
		loading_param_type = false;
	}
	else if (defining && !loading_param_type) {
		// skontroluje zvysne parametre
		if (!check_remaining_param()) {
			dbg_crash_msg("Zly pocet parametrov\n");
			utl_exit(SEA_PRG_ERR);
		}
		// typ definovanej funkcie sa nerovna typu declarovanej
		if (SYMT_LIST->act->ret_type != v_type) {
			sea_dbg_msg("definovana funkcia nema rovnaky typ ako deklarovana\n");
			dbg_crash_msg("Definovana funkcia ma iny typ ako deklarovana\n");
			utl_exit(SEA_PRG_ERR);
		}
		defining = false;
		param_index = 0;
		//popni vsetky hodnoty parametrov v opacnom poradi
		sea_dbg_msg("idem nacitavat parametre (pocet <%d>)\n", SYMT_LIST->act->params_count);
		for (int i = SYMT_LIST->act->params_count; i >= 0; i--) {
			Symbol *temp = sya_param_find_by_id(i);
			sea_dbg_msg("idem nacitavat parameter <%s>\n", temp->id);
			gen_order_assignment_from_stack(gen, temp->gen_id);
		}
	}
	else if (defining && loading_param_type) {
		sea_dbg_msg("kontrola typu parametru <%s> vo funkcii <%s>\n", last_var, SYMT_LIST->act->id);
		bool temp = check_param(last_var, v_type);
		if (!temp) {
			dbg_crash_msg("Parameter sa nenasiel\n");
			utl_exit(SEA_PRG_ERR);
		}
		loading_param_type = false;
	}
	return 0;
}


//skontroluje ci bola definovana kazda declarovana funkcia
void sea_check_allfunc_def() {
	Symt_table *temp = SYMT_LIST->first;
	while (temp != NULL) {
		sea_dbg_msg("-> finalna kontrola definicie: <%s>\n", temp->id);
		if (!(temp->is_defined)) {
			sea_dbg_msg("-> funkcia <%s> nebola definovana\n", temp->id);
			dbg_crash_msg("Funkcia bola deklarovana ale nebola definovana\n");
			utl_exit(SEA_PRG_ERR);
		}
		temp = temp->next;
	}
}


int sea_give_next_token(token *tok, int rule_number) {
	/* Mensi predikce, rekne co bude za vyraz nasledujici, pokud prijde.
	 * Ulozi posledni t_id nebo operator pak nevidim do minulosti,
	 * abych vedel co s tim udelat */
	//sea_dbg_msg("rule: %d -> %s\n", rule_number, tok->data);
	if (last_rule == rule_number && last_tok_type == tok->type) {
		return 0; // odstrani duplikaty
	}
	//sea_dbg_expr_msg("---> sea_give_next_token dosel token\n", tok->data);
	// kradu tokeny pro expression, bude se vyhodnocovat az cely najednou
	if (save_token_for_expr) {
		sea_dbg_expr_msg("---> sea_give_next_token (je vyraz beru token <%s>)\n", tok->data);
		sea_add_tok_to_expr(tok);
		return 0;
	}
	sea_set_expr_by_token(tok);
	last_rule = rule_number;
	last_tok_type = tok->type;
	//ak sa nejedna o pravidlo ukoncujuce scope, ukonci pokus o ukoncenie
	if (tok->type != t_kw_end && tok->type != t_kw_function && tok->type != t_kw_scope && tok->type != t_kw_if) {
		ending_scope = false;
	}
	switch (rule_number) {
		case 1: //sluzi na rusenie aktivity scopov
			if (tok->type == t_kw_end) {
				ending_scope = true;
			}
			else if (tok->type == t_kw_function) {
				if (ending_scope == true) {
					sea_dbg_msg("zrusenie aktivity funkcie <%s>\n\n", SYMT_LIST->act->id);
					ending_scope = false;
					//gen_function_end(gen);
					if (SYMT_LIST->act->ret_type == v_int) {
						gen_stack_push_constant(gen, int_type, "0");
						gen_function_end(gen);
					}
					else if (SYMT_LIST->act->ret_type == v_double) {
						gen_stack_push_constant(gen, float_type, "0.0");
						gen_function_end(gen);
					}
					else if (SYMT_LIST->act->ret_type == v_bool) {
						gen_stack_push_constant(gen, bool_type, "false");
						gen_function_end(gen);
					}
					else if (SYMT_LIST->act->ret_type == v_string) {
						gen_stack_push_constant(gen, string_type, "");
						gen_function_end(gen);
					}
					SYMT_LIST->act = NULL;
				}
			}
			else if (tok->type == t_kw_scope) {
				if (ending_scope == true) {
					sea_dbg_msg("zrusenie aktivity hlavneho programu\n\n");
					//gen_function_end(gen);
					ending_scope = false;
				}
				else {
					//nastavenie aktivity scopu
					sea_dbg_msg("nastavenie aktivity hlavneho programu\n\n");
					//vygenerovanie hlavneho tela
					gen_scope_start(gen);
					sya_symt_list_act("scope");
					was_scope = true;
				}
			}
			else if (tok->type == t_kw_if) {
				if (ending_scope == true) {
					sea_dbg_msg("generujem ukoncenie IFu\n");
					// vygeneruj koniec podmineky
					gen_condition_end(gen);
					ending_scope = false;
				}
			}
			if (tok->type == t_kw_loop) {
				sea_dbg_msg("generujem koniec cyklu\n");
				gen_cycle_end(gen);
			}
			if (tok->type == t_kw_else) {
				sea_dbg_msg("generujem ELSE condition\n");
				gen_condition_else(gen);
			}
			break;
		case 9: // dim var
			initializing = true;
			if (tok->type == t_id) {
				sea_dbg_msg("--> vytvaranie novej premennej <%s> vo funkcii <%s>\n", tok->data, SYMT_LIST->act->id);
				//vytvor ak nie je deklarovana
				if (!is_function(tok->data) && !is_declared(tok->data) && !is_param(tok->data)) {
					//vygeneruj premennu
					int temp_name = gen_order_new_var(gen);
					//pridaj premennu do scopu
					sya_sym_add(tok->data, temp_name);
				}
				else {
					sea_dbg_msg("-------> ERROR_PRG_ERR <-------\n");
					dbg_crash_msg("Premenna uz bola deklarovana\n");
					utl_exit(SEA_PRG_ERR); //ak uz existuje, chyba
				}
			}
			break;
		case 10: // init stav prirazeni pri inicializaci =
			// nasleduje expr
			// symt_var_add_value(*sea_last_used->sym_ptr, tok->data);
			break;
		case 13: //string datatype
			if (tok->type == t_kw_string) {
				type_semantics(v_string, "!\"\"");
			}
			break;
		case 14: //boolean datatype
			if (tok->type == t_kw_boolean) {
				type_semantics(v_bool, "False");
			}
			break;
		case 15: //integer datatype
			if (tok->type == t_kw_integer) {
				type_semantics(v_int, "0");
			}
			break;
		case 16: //double datatype
			if (tok->type == t_kw_double) {
				type_semantics(v_double, "0.0");
			}
			break;
		case 43: // Priradeni
			sya_sym_edit_val(last_var, tok->data);
			break;
		// PRACA S FUNKCIAMI
		case 2: // vytvorenie scopu hlavneho tela programu
			sea_dbg_msg("\n---> vytvorenie hlavneho tela (scope)\n");
			sya_symt_create("scope");
			sya_symt_list_act("scope");
			SYMT_LIST->act->is_defined = true;
			break;
		case 3: //declare function fce
			declaring = true;
			if (tok->type == t_id) {
				if (!is_function(tok->data)) {
					sea_dbg_msg("\n---> deklarovanie funkcie <%s>\n", tok->data);
					sya_symt_create(tok->data);
					sea_dbg_msg("nastavenie aktivity funkcii <%s>\n", tok->data);
					sya_symt_list_act(tok->data);
					//last_var = tok->data;
					// vygeneruj novu funkciu
					//SYMT_LIST->act->gen_id = gen_function_start(gen);
				}
				else {
					dbg_crash_msg("Funkcia uz bola deklarovana\n");
					utl_exit(SEA_PRG_ERR);
				}
			}
			break;
		case 4: //define function fce
			defining = true;
			if (tok->type == t_id) {
				sea_dbg_msg("\n---> definovanie funkcie <%s>\n", tok->data);
				//ak uz existuje, kontroluj
				if (is_function(tok->data)) {
					// funkcia moze byt definovana len raz
					sea_dbg_msg("nastavenie aktivity funkcii <%s>\n", tok->data);
					sya_symt_list_act(tok->data);
					SYMT_LIST->act->gen_id = gen_function_start(gen);
					if (SYMT_LIST->act->is_defined == true) {
						sea_dbg_msg("funkcia uz je definovana, ERROR\n");
						dbg_crash_msg("Funkcia uz bola definovana\n");
						utl_exit(SEA_PRG_ERR);
					}
					else {
						SYMT_LIST->act->is_defined = true;
					}
				}
				// inak pokracuj ako pri deklarovani
				else {
					//last_var = tok->data;
					sea_dbg_msg("definovanie bez deklaracie funkcie <%s>\n", tok->data);
					//defining = false;
					declaring = true;
					sya_symt_create(tok->data);
					sya_symt_list_act(tok->data);
					SYMT_LIST->act->is_defined = true;
					// vygeneruj novu funkciu
					SYMT_LIST->act->gen_id = gen_function_start(gen);
				}
			}
			break;
		case 5: // parameter pri deklarovani
			if (tok->type == t_id) {
				loading_param_type = true;
				if (declaring && !defining) {
					if (!is_function(tok->data) && !is_declared(tok->data) && !is_param(tok->data)) {
						//vytvor paramater
						sea_dbg_msg("definovanie parametru <%s> vo funkcii <%s> | BEZ GENEROVANIA\n", tok->data, SYMT_LIST->act->id);
						//vygenerovanie parametru
						//int temp_name = gen_order_new_var(gen);
						//pridanie parametru funkcii
						sya_param_add(tok->data, -1);
						last_var = tok->data;
					}
					else {
						dbg_crash_msg("Nespravne meno parametru\n");
						utl_exit(SEA_PRG_ERR);
					}
				}
				else if (!declaring && defining) {
					//vygenerovanie a zmenu existujuceho gen_id parametru
					int temp_name = gen_order_new_var(gen);
					Symbol *temp = sya_param_find_by_id(param_index);
					if (temp == NULL) {
						sea_dbg_msg("premenna <%s> nebola deklarovana\n", tok->data);
						dbg_crash_msg("Parameter nebol deklarovany\n");
						utl_exit(SEA_PRG_ERR);
					}
					temp->gen_id = temp_name;
					last_var = tok->data;
				}
				else if (declaring && defining) {
					if (!is_function(tok->data) && !is_declared(tok->data) && !is_param(tok->data)) {
						//vytvor paramater
						sea_dbg_msg("definovanie parametru <%s> vo funkcii <%s>\n", tok->data, SYMT_LIST->act->id);
						//vygenerovanie parametru
						int temp_name = gen_order_new_var(gen);
						//pridanie parametru funkcii
						sya_param_add(tok->data, temp_name);
						last_var = tok->data;
					}
					else {
						dbg_crash_msg("Parameter nebol deklarovany\n");
						utl_exit(SEA_PRG_ERR);
					}
				}
				else
					last_var = tok->data;
			}
			break;
		case 18: // input var
			if (tok->type == t_id) {
				if (is_declared(tok->data)) {
					char *help_string = (char *) gc_new(sizeof(char));
					gen_order_write_string(gen, "?\\032");
					last_var = tok->data;
					Symbol *temp = sya_sym_find(tok->data);
					if (temp->type == v_int) {
						gen_order_read(gen, temp->gen_id, 0);
					}
					else if (temp->type == v_double) {
						gen_order_read(gen, temp->gen_id, 1);
					}
					else if (temp->type == v_string) {
						gen_order_read(gen, temp->gen_id, 2);
					}
					else if (temp->type == v_bool) {
						gen_order_read(gen, temp->gen_id, 3);
					}
				}
				else {
					sea_dbg_msg("premenna <%s> nebola najdena\n", tok->data);
					dbg_crash_msg("Premenna pre input nebola deklarovana\n");
					utl_exit(SEA_PRG_ERR);
				}
			}
		default:
			break;
	}
	return 0;
}

void sea_set_expr_by_token(token *tok) {
	switch (tok->type) {
	case tr_id:
		// at vim kam ulozit promennou
		sem_expr_last_id->type = t_id;
		sem_expr_last_id->data_length = tok->data_length;
		sem_expr_last_id->data = tok->data;
		sea_dbg_expr_msg("---> sea_set_expr_by_token - <%s>, (ulozit t_id)\n", tok->data);
		break;
	case tr_kw_print:
		sea_dbg_expr_msg("---> sea_set_expr_by_token - <%s>, (bude print vyraz)\n", tok->data);
		sea_expr_bools_false();
		print_expr_active = true;
		break;
	case tr_kw_while:
	case tr_kw_elseif:
	case tr_kw_if:
		sem_expr_last_assignment->type = tok->type;
		sem_expr_last_assignment->data_length = tok->data_length;
		sem_expr_last_assignment->data = tok->data;
		sea_dbg_expr_msg("---> sea_set_expr_by_token - <%s>, (bude logicky vyraz)\n", tok->data);
		sea_expr_bools_false();
		logical_expr_active = true;
		break;
	case tr_kw_return:
		sea_dbg_expr_msg("---> sea_set_expr_by_token - <%s>, (bude return vyraz)\n", tok->data);
		sea_expr_bools_false();
		return_expr_active = true;
		break;
	case t_plus_equal:
	case t_minus_equal:
	case t_asterisk_equal:
	case t_slash_equal:
	case t_backslash_equal:
	case t_equal:
		sem_expr_last_assignment->type = tok->type;
		sem_expr_last_assignment->data_length = tok->data_length;
		sem_expr_last_assignment->data = tok->data;
		sea_dbg_expr_msg("---> sea_set_expr_by_token - <%s>, (ulozit operator)\n", tok->data);
		sea_expr_bools_false();
		assign_expr_active = true;
		break;
	default:
		break;
	}
}

void sea_expr_bools_false() {
	logical_expr_active = false;
	assign_expr_active = false;
	save_token_for_expr = false;
	print_expr_active = false;
	return_expr_active = false;
}

void sea_add_tok_to_expr(token *tok) {
	switch (tok->type) {
	case tr_id_fce:
		sea_dbg_expr_msg("---> sea_add_tok_to_expr: Bude kontrola deklarace funkce\n");
		if (!is_function(tok->data)) {
			sea_dbg_expr_msg("---> sea_add_tok_to_expr: token t_id <%s> neni deklarovany)\n", tok->data);
			dbg_crash_msg("Nedeklarovana funkce ve vyrazu\n");
			utl_exit(SEA_PRG_ERR);
		}
		sea_dbg_expr_msg("---> sea_add_tok_to_expr: Pridam do fronty funkci <%s><%d>\n", tok->data, tok->type);
		expr_time_stamps_cnt++;
		if (sea_q_s_add(expr_tok_fn_queue, tok, expr_time_stamps_cnt) == 1) { // fronta add
			sea_dbg_expr_msg("---> sea_add_tok_to_expr: Padlo pridani do fronty funkci\n");
			dbg_crash_msg("malloc error\n");
			utl_exit(UTL_MEM_ERR);
		}
		sea_dbg_expr_msg("---> sea_add_tok_to_expr: Pridano\n");
		return;
	case t_id:
		sea_dbg_expr_msg("---> sea_add_tok_to_expr: Bude kontrola deklarace promenne\n");
		if (!is_declared(tok->data) && !is_param(tok->data)) {
			sea_dbg_expr_msg("---> sea_add_tok_to_expr: token t_id <%s> neni deklarovany)\n", tok->data);
			dbg_crash_msg("Nedeklarovana promenna ve vyrazu\n");
			utl_exit(SEA_PRG_ERR);
		}
	//case t_kw_asc:
	//case t_kw_chr:
	case t_kw_true:
	case t_kw_false:
	case t_integer:
	case t_double:
	case t_string:
		sea_dbg_expr_msg("---> sea_add_tok_to_expr: Pridam do fronty <%s><%d>\n", tok->data, tok->type);
		expr_time_stamps_cnt++;
		if (sea_q_add(expr_tok_queue, tok, expr_time_stamps_cnt) == 1) { // fronta add
			sea_dbg_expr_msg("---> sea_add_tok_to_expr: Padlo pridani do fronty\n");
			dbg_crash_msg("malloc error\n");
			utl_exit(UTL_MEM_ERR);
		}
		sea_dbg_expr_msg("---> sea_add_tok_to_expr: Pridano\n");
		return;
	default:
		break;
	}
	sea_dbg_expr_msg("---> sea_add_tok_to_expr: nechci token <%s> \n", tok->data);
}

void sea_begin_expression() {
	sea_dbg_expr_msg("---> sea_begin_expression (ceka na tokeny pro frontu)\n");
	save_token_for_expr = true;
	stack_push(time_stamp_stack, 0);
	stack_push(time_stamp_stack, 0);
	stack_push(time_stamp_stack, 0);
	if (logical_expr_active) {
		if (sem_expr_last_assignment->type == tr_kw_while) {
			gen_cycle_start(gen);
		}
		else if (sem_expr_last_assignment->type == tr_kw_elseif) {
			gen_condition_end_if_branch(gen);
		}
	}
}

void sea_end_of_expression() {
	save_token_for_expr = false;
	sea_dbg_expr_msg("---> sea_end_of_expression (zacina vyhodnocovani)\n");
	if (logical_expr_active) {
		sea_dbg_expr_msg("---> sea_end_of_expression (logicky vyraz)\n");
		switch (sem_expr_last_assignment->type)	{
		case tr_kw_while:
			gen_stack_push_constant(gen, bool_type, "true");
			gen_cycle_eval_condition(gen);
			sea_dbg_expr_gen_msg("---> GENERATE WHILE CONDITION\n");
			break;
		case tr_kw_elseif:
			gen_stack_push_constant(gen, bool_type, "true");
			gen_condition_elseif(gen);
			sea_dbg_expr_gen_msg("---> GENERATE ELSE-IF CONDITION\n");
			break;
		case tr_kw_if:
			gen_stack_push_constant(gen, bool_type, "true");
			gen_condition_if(gen);
			sea_dbg_expr_gen_msg("---> GENERATE IF CONDITION\n");
			break;
		default:
			break;
		}
	}
	else if (assign_expr_active) {
		//sea_dbg_expr_gen_msg("---> GENERATE POPS %s\n",	sem_expr_last_id->data);
		sea_dbg_expr_msg("---> sea_end_of_expression (pocetni vyraz)\n");
		if (!is_declared(sem_expr_last_id->data) && !is_param(sem_expr_last_id->data)) {
			sea_dbg_expr_msg("---> sea_end_of_expression: token t_id <%s> neni deklarovany)\n", sem_expr_last_id->data);
			dbg_crash_msg("Nedeklarovana promenna pro prirazeni\n");
			utl_exit(SEA_PRG_ERR);
		}
		if (sem_expr_last_assignment->type != t_equal) {
			sea_dbg_expr_gen_msg("---> GENERATE PUSHS %s \n", sem_expr_last_id->data);
			gen_stack_push_var(gen, sea_find_var_gen_id(sem_expr_last_id->data)); // PUSH GENERATOR STACK
			stack_push(sea_expr_type_stack, sea_find_var_type(sem_expr_last_id->data)); // PUSH TYPE STACK, pro pricteni, odecteni
			switch (sem_expr_last_assignment->type) {
			case t_plus_equal:
				sea_dbg_expr_gen_msg("---> GENERATE ADDS\n");
				sea_retype_expr(t_plus, sea_expr_type_stack, gen);
				break;
			case t_minus_equal:
				sea_dbg_expr_gen_msg("---> GENERATE SWAP\n");
				sea_type_var_stack_swap();
				gen_stack_operation_swap(gen);
				sea_dbg_expr_gen_msg("---> GENERATE SUBS\n");
				sea_retype_expr(t_minus, sea_expr_type_stack, gen);
				break;
			case t_asterisk_equal:
				sea_dbg_expr_gen_msg("---> GENERATE MULS\n");
				sea_retype_expr(t_asterisk, sea_expr_type_stack, gen);
				break;
			case t_slash_equal:
				sea_dbg_expr_gen_msg("---> GENERATE SWAP\n");
				sea_type_var_stack_swap();
				gen_stack_operation_swap(gen);
				sea_dbg_expr_gen_msg("---> GENERATE DIVS\n");
				sea_retype_expr(t_slash, sea_expr_type_stack, gen);
				break;
			case t_backslash_equal:
				sea_dbg_expr_gen_msg("---> GENERATE SWAP\n");
				sea_type_var_stack_swap();
				gen_stack_operation_swap(gen);
				sea_dbg_expr_gen_msg("---> GENERATE DIVS\n");
				sea_retype_expr(t_backslash, sea_expr_type_stack, gen);
				break;
			default:
				break;
			} // vysledek je uz na zasobniku
		}
		sea_dbg_expr_msg("---> sea_set_expr_by_token - <%s>, (ulozit operator)\n", sem_expr_last_id->data);
		sea_dbg_expr_gen_msg("---> GENERATE POPS %s\n", sem_expr_last_id->data);
		stack_push(sea_expr_type_stack, sea_find_var_type(sem_expr_last_id->data)); // PUSH TYPE STACK, pred prirazenim
		sea_retype_asgn(sea_expr_type_stack, gen);
		gen_order_assignment_from_stack(gen, sea_find_var_gen_id(sem_expr_last_id->data));
	}
	else if (print_expr_active) {
		sea_dbg_expr_msg("---> sea_end_of_expression (print vyraz)\n");
		sea_dbg_expr_gen_msg("---> GENERATE PRINT STRING\n");
		gen_stack_top_print(gen);
	}
	else if (return_expr_active) {
		sea_dbg_expr_msg("---> sea_end_of_expression (return vyraz)\n");
		sea_dbg_expr_gen_msg("---> GENERATE END FUNCTION\n");
		stack_push(sea_expr_type_stack, sea_find_function_ret_type(SYMT_LIST->act->id));
		sea_retype_asgn(sea_expr_type_stack, gen);
		gen_function_end(gen);
	}
	// cistim frontu
	sea_dbg_expr_msg("---> sea_end_of_expression (cistim frontu)\n");
	sea_q_delete_all(expr_tok_queue);
	expr_time_stamps_cnt = 0;
}

void sea_type_var_stack_swap() {
	int tmp_first = (stack_top(sea_expr_type_stack))->data;
	stack_pop(sea_expr_type_stack);
	int tmp_second = (stack_top(sea_expr_type_stack))->data;
	stack_pop(sea_expr_type_stack);
	stack_push(sea_expr_type_stack, tmp_first);
	stack_push(sea_expr_type_stack, tmp_second);
}

void sea_give_expr_rule(int rule_number) {
	// pomocne promenne
	token *tmp_tok; // nacita se do ni token z fronty
	char *eptr; // kontrolni pri retezcovych konverznich instrukci
	int tmp_param_counter = 0; // pocita kolik ma funkce parametu
	sea_dbg_expr_msg("---> sea_give_expr_rule (provedu pravidlo) - %d\n", rule_number);
	switch (rule_number) {
	case 31: // nic nedelat, zavorky nechci
		break;
	case 32:
		sea_dbg_expr_gen_msg("---> GENERATE MULS\n");
		sea_dbg_expr_msg("---> sea_give_expr_rule, (generovani operace <*> nad zasobnikem)\n");
		sea_time_stamp_reduce();
		sea_retype_expr(t_asterisk, sea_expr_type_stack, gen);
		break;
	case 33:
		sea_dbg_expr_gen_msg("---> GENERATE ADDS\n");
		sea_dbg_expr_msg("---> sea_give_expr_rule, (generovani operace <+> nad zasobnikem)\n");
		sea_time_stamp_reduce();
		sea_retype_expr(t_plus, sea_expr_type_stack, gen);
		break;
	case 34:
		sea_dbg_expr_gen_msg("---> GENERATE DIVS\n");
		sea_dbg_expr_msg("---> sea_give_expr_rule, (generovani operace </> nad zasobnikem)\n");
		sea_time_stamp_reduce();
		sea_retype_expr(t_slash, sea_expr_type_stack, gen);
		break;
	case 35:
		sea_dbg_expr_gen_msg("---> GENERATE DIVS\n");
		sea_dbg_expr_msg("---> sea_give_expr_rule, (generovani operace <\\> nad zasobnikem)\n");
		sea_time_stamp_reduce();
		sea_retype_expr(t_backslash, sea_expr_type_stack, gen);
		break;
	case 36:
		sea_dbg_expr_gen_msg("---> GENERATE SUBS\n");
		sea_dbg_expr_msg("---> sea_give_expr_rule, (generovani operace <-> nad zasobnikem)\n");
		sea_time_stamp_reduce();
		sea_retype_expr(t_minus, sea_expr_type_stack, gen);
		break;
	case 37: // generovat unarni minus - mam `a`, udelam `0-a` a `a` ma hodnotu `-a`
		sea_dbg_expr_gen_msg("---> GENERATE PUSHS 0\n");
		if (stack_top(sea_expr_type_stack)->data == int_type) {
			gen_stack_push_int(gen, 0);
		}
		else if (stack_top(sea_expr_type_stack)->data == float_type) {
			gen_stack_push_float(gen, 0);
		}
		else { //string_type || bool_type
			dbg_crash_msg("S timto typem nelze provest unarni minus\n");
			utl_exit(SEA_TYPE_ERR);
		}
		sea_dbg_expr_gen_msg("---> GENERATE SWAP\n");
		gen_stack_operation_swap(gen);
		sea_dbg_expr_gen_msg("---> GENERATE SUBS\n");
		gen_stack_operation_minus(gen);
		break;
	case 38:
		tmp_tok = sea_q_up(expr_tok_fn_queue);
		stack_push(time_stamp_stack, sea_q_up_stamp(expr_tok_fn_queue));
		sea_q_delete(expr_tok_fn_queue);
		sea_dbg_expr_msg("---> sea_give_expr_rule: nazev funkce <%s>, nema parametry\n", tmp_tok->data);
		if (strcmp(tmp_tok->data, "asc") == 0) {
			sea_dbg_expr_msg("---> sea_give_expr_rule: Asc nemuze byt bez parametru\n");
			dbg_crash_msg("Funkci <asc> nelze volat bez parametru\n");
			utl_exit(SEA_TYPE_ERR);
		}
		else if (strcmp(tmp_tok->data, "chr") == 0) {
			sea_dbg_expr_msg("---> sea_give_expr_rule: Chr nemuze byt bez parametru\n");
			dbg_crash_msg("Funkci <chr> nelze volat bez parametru\n");
			utl_exit(SEA_TYPE_ERR);
		}
		else if (strcmp(tmp_tok->data, "length") == 0) {
			sea_dbg_expr_msg("---> sea_give_expr_rule: Lenght nemuze byt bez parametru\n");
			dbg_crash_msg("Funkci <length> nelze volat bez parametru\n");
			utl_exit(SEA_TYPE_ERR);
		}
		else if (strcmp(tmp_tok->data, "substr") == 0) {
			sea_dbg_expr_msg("---> sea_give_expr_rule: Substr nemuze byt bez parametru\n");
			dbg_crash_msg("Funkci <substr> nelze volat bez parametru\n");
			utl_exit(SEA_TYPE_ERR);
		}
		else if (!is_function(tmp_tok->data)) {
			sea_dbg_expr_msg("---> sea_give_expr_rule: spatne poradi tokenu ve fronte\n", tmp_tok->data);
			dbg_crash_msg("Funkce neexistuje\n");
			utl_exit(SEA_PRG_ERR);
		}
		Symt_table *tmp_table = sya_symt_find(tmp_tok->data);
		if (tmp_table == NULL) {
			dbg_crash_msg("Funkce neexistuje\n");
			utl_exit(SEA_PRG_ERR);
		}
		else if (tmp_table->params_count >= 0) {
			dbg_crash_msg("Spatny pocet parametru funkce\n");
			utl_exit(SEA_TYPE_ERR);
		}
		stack_push(sea_expr_type_stack, sea_find_function_ret_type(tmp_tok->data));
		gen_call_function(gen, sea_find_function_gen_id(tmp_tok->data));
		break;
	case 39:
		// nastrel pruchodu parametru
		tmp_tok = sea_q_up(expr_tok_fn_queue);
		int tmp_stamp = sea_q_up_stamp(expr_tok_fn_queue);
		int tmp_stamp_check;
		int tmp_gen_id;
		int tmp_gen_id_2;
		sea_q_delete(expr_tok_fn_queue);
		sea_dbg_expr_msg("---> sea_give_expr_rule: nazev funkce <%s>\n", tmp_tok->data);
		if (strcmp(tmp_tok->data, "asc") == 0) {
			sea_dbg_expr_msg("---> sea_give_expr_rule: Vyhodnotim klicovou funkci ASC\n");
			sea_time_stamp_check(tmp_stamp, 2);
			sea_dbg_expr_msg("---> sea_give_expr_rule: Kontrola time stamp OK\n");
			sea_dbg_expr_msg("---> sea_give_expr_rule: Typova kontrola param 1, stack top<%d>\n", (stack_top(sea_expr_type_stack))->data);
			stack_push(sea_expr_type_stack, int_type); // priprava na konverzi 1
			sea_retype_asgn(sea_expr_type_stack, gen); // kontroluju prvni type
			sea_dbg_expr_msg("---> sea_give_expr_rule: Typova kontrola parametru 1 OK\n");
			stack_pop(sea_expr_type_stack); // odstranim prvni type stack
			tmp_gen_id = gen_order_new_var(gen); // generuji tmp promennou
			gen_order_assignment_from_stack(gen, tmp_gen_id); // popnu do ni
			sea_dbg_expr_msg("---> sea_give_expr_rule: Typova kontrola param 2, stack top<%d>\n", (stack_top(sea_expr_type_stack))->data);
			stack_push(sea_expr_type_stack, string_type); // priprava na konverzi 2
			sea_retype_asgn(sea_expr_type_stack, gen); // kontroluju druhy type
			sea_dbg_expr_msg("---> sea_give_expr_rule: Typova kontrola parametru 2 OK\n");
			stack_pop(sea_expr_type_stack); // odstranim druhy type stack
			gen_stack_push_var(gen, tmp_gen_id); // push zpet druhy parametr
			stack_push(sea_expr_type_stack, int_type); // navratova hodnota
			built_in_function_Asc(gen);
		}
		else if (strcmp(tmp_tok->data, "length") == 0) {
			sea_dbg_expr_msg("---> sea_give_expr_rule: Vyhodnotim klicovou funkci Lenght\n");
			sea_time_stamp_check(tmp_stamp, 1);
			sea_dbg_expr_msg("---> sea_give_expr_rule: Kontrola time stamp OK\n");
			sea_dbg_expr_msg("---> sea_give_expr_rule: Typova kontrola param, stack top<%d>\n", (stack_top(sea_expr_type_stack))->data);
			stack_push(sea_expr_type_stack, string_type);
			sea_retype_asgn(sea_expr_type_stack, gen);
			sea_dbg_expr_msg("---> sea_give_expr_rule: Typova kontrola parametru 1 OK\n");
			stack_pop(sea_expr_type_stack); // odstranim type stack
			stack_push(sea_expr_type_stack, int_type); // navratova hodnota
			built_in_function_Length(gen);
		}
		else if (strcmp(tmp_tok->data, "substr") == 0) {
			sea_dbg_expr_msg("---> sea_give_expr_rule: Vyhodnotim klicovou funkci Substr\n");
			sea_time_stamp_check(tmp_stamp, 3);
			sea_dbg_expr_msg("---> sea_give_expr_rule: Kontrola time stamp OK\n");
			sea_dbg_expr_msg("---> sea_give_expr_rule: Typova kontrola param 1, stack top<%d>\n", (stack_top(sea_expr_type_stack))->data);
			stack_push(sea_expr_type_stack, int_type); // priprava na konverzi 1
			sea_retype_asgn(sea_expr_type_stack, gen); // kontroluju prvni type
			sea_dbg_expr_msg("---> sea_give_expr_rule: Typova kontrola param 1 OK\n");
			stack_pop(sea_expr_type_stack); // odstranim prvni type stack
			tmp_gen_id = gen_order_new_var(gen); // generuji tmp promennou
			gen_order_assignment_from_stack(gen, tmp_gen_id); // popnu do ni
			sea_dbg_expr_msg("---> sea_give_expr_rule: Typova kontrola param 2, stack top<%d>\n", (stack_top(sea_expr_type_stack))->data);
			stack_push(sea_expr_type_stack, int_type); // priprava na konverzi 2
			sea_retype_asgn(sea_expr_type_stack, gen); // kontroluju druhy type
			sea_dbg_expr_msg("---> sea_give_expr_rule: Typova kontrola param 2 OK\n");
			stack_pop(sea_expr_type_stack); // odstranim druhy type stack
			tmp_gen_id_2 = gen_order_new_var(gen); // generuji tmp promennou
			gen_order_assignment_from_stack(gen, tmp_gen_id_2); // popnu do ni
			sea_dbg_expr_msg("---> sea_give_expr_rule: Typova kontrola param 3, stack top<%d>\n", (stack_top(sea_expr_type_stack))->data);
			stack_push(sea_expr_type_stack, string_type); // priprava na konverzi 3
			sea_retype_asgn(sea_expr_type_stack, gen); // kontroluju treti type
			sea_dbg_expr_msg("---> sea_give_expr_rule: Typova kontrola param 3 OK\n");
			stack_pop(sea_expr_type_stack); // odstranim treti type stack
			gen_stack_push_var(gen, tmp_gen_id_2); // push zpet druhy parametr
			gen_stack_push_var(gen, tmp_gen_id); // push zpet druhy parametr
			stack_push(sea_expr_type_stack, string_type); // navratova hodnota
			built_in_function_SubStr(gen);
		}
		else if (strcmp(tmp_tok->data, "chr") == 0) {
			sea_dbg_expr_msg("---> sea_give_expr_rule: Vyhodnotim klicovou funkci CHR\n");
			sea_time_stamp_check(tmp_stamp, 1);
			sea_dbg_expr_msg("---> sea_give_expr_rule: Kontrola time stamp OK\n");
			sea_dbg_expr_msg("---> sea_give_expr_rule: Typova kontrola param, stack top<%d>\n", (stack_top(sea_expr_type_stack))->data);
			stack_push(sea_expr_type_stack, int_type);
			sea_retype_asgn(sea_expr_type_stack, gen);
			sea_dbg_expr_msg("---> sea_give_expr_rule: Typova kontrola parametru 1 OK\n");
			stack_pop(sea_expr_type_stack); // odstranim type stack
			stack_push(sea_expr_type_stack, string_type); // navratova hodnota
			built_in_function_Chr(gen);
		}
		else if (!is_function(tmp_tok->data)) {
			sea_dbg_expr_msg("---> sea_give_expr_rule: spatne poradi tokenu ve fronte\n", tmp_tok->data);
			dbg_crash_msg("Funkce neexistuje\n");
			utl_exit(SEA_PRG_ERR);
		}
		else {
			Symt_table *tmp_table = sya_symt_find(tmp_tok->data);
			if (tmp_table == NULL) {
				dbg_crash_msg("Funkce neexistuje\n");
				utl_exit(SEA_PRG_ERR);
			}
			sea_time_stamp_check(tmp_stamp, tmp_table->params_count + 1);
			sea_dbg_expr_msg("---> sea_give_expr_rule: Kontrola time stamp OK\n");
			sea_dbg_expr_msg("---> sea_give_expr_rule: Mame <%d> parametru (indexovano od nuly)\n", tmp_table->params_count);
			for (int i = tmp_table->params_count; i >= 0; i--) {
				Symbol *tmp_symbol = sya_param_find_in_symt_by_id(tmp_tok->data, i);
				if (tmp_symbol == NULL) {
					sea_dbg_expr_msg("---> sea_give_expr_rule: Parametr na indexu <%d> neexistuje\n", i);
					dbg_crash_msg("Parametr na indexu neexistuje\n");
					utl_exit(SEA_PRG_ERR);
				}
				else {
					sea_dbg_expr_msg("---> sea_give_expr_rule: Parametr na indexu <%d> ma nazev <%s>\n", i, tmp_symbol->id);
				}
				sea_dbg_expr_msg("---> sea_give_expr_rule: Typova kontrola, stack top<%d>", (stack_top(sea_expr_type_stack))->data);
				switch (tmp_symbol->type) {
				case v_int:
					stack_push(sea_expr_type_stack, int_type);
					sea_dbg_expr_msg(", pak push int\n");
					break;
				case v_double:
					stack_push(sea_expr_type_stack, float_type);
					sea_dbg_expr_msg(", pak push float\n");
					break;
				case v_char:
				case v_string:
					stack_push(sea_expr_type_stack, string_type);
					sea_dbg_expr_msg(", pak push string\n");
					break;
				case v_bool:
					stack_push(sea_expr_type_stack, bool_type);
					sea_dbg_expr_msg(", pak push bool\n");
					break;
				case v_void:
					sea_dbg_expr_msg(", pak nelze void push\n __________ERROR__________");
					dbg_crash_msg("Spatny datovy typ operandu\n");
					utl_exit(SEA_PRG_ERR);
					break;
				}
				sea_retype_asgn(sea_expr_type_stack, gen);
				sea_dbg_expr_msg("---> sea_give_expr_rule: Generuji pomocnou promennou");
				tmp_gen_id = gen_order_new_var(gen);
				stack_push(fn_tmp_gen_id_stack, tmp_gen_id);
				sea_dbg_expr_msg(" a plnim ji\n");
				gen_order_assignment_from_stack(gen, tmp_gen_id);
				stack_pop(sea_expr_type_stack); // v nem to uz nechci nikdy ani videt
			}
			sea_dbg_expr_msg("---> sea_give_expr_rule: Budu pushovat parametry zpet\n");
			for (int i = tmp_table->params_count; i >= 0; i--) {
				sea_dbg_expr_msg("|");
				gen_stack_push_var(gen, (stack_top(fn_tmp_gen_id_stack))->data);
				stack_pop(fn_tmp_gen_id_stack);
			}
			sea_dbg_expr_msg("\n---> sea_give_expr_rule, Parametry zkontrolovane na zasobniku\n");
			stack_push(sea_expr_type_stack, sea_find_function_ret_type(tmp_tok->data));
			sea_dbg_expr_msg("---> sea_give_expr_rule: Navratova typ funkce je <%d>\n", sea_find_function_ret_type(tmp_tok->data));
			stack_push(time_stamp_stack, tmp_stamp);
			gen_call_function(gen, sea_find_function_gen_id(tmp_tok->data));
		}
		break;
	case 43: // ID
		// vezmu token z fronty a dam ho na zasobnik
		sea_dbg_expr_msg("---> sea_give_expr_rule, (generovani PUSHS id na zasobnik)\n");
		tmp_tok = sea_q_up(expr_tok_queue);
		stack_push(time_stamp_stack, sea_q_up_stamp(expr_tok_queue));
		sea_q_delete(expr_tok_queue);
		// type control
		sea_dbg_expr_msg("---> sea_give_expr_rule, (generovani PUSHS vzal jsem z fronty)<%s>\n", tmp_tok->data);
		sea_dbg_expr_gen_msg("---> GENERATE PUSHS %s", tmp_tok->data);
		if (tmp_tok->type == t_id) {
			stack_push(sea_expr_type_stack, sea_find_var_type(tmp_tok->data));
			gen_stack_push_var(gen, sea_find_var_gen_id(tmp_tok->data));
		}
		else {
			switch (sea_get_tok_var_type(tmp_tok)) {
			case v_int:
				sea_dbg_expr_gen_msg("---> (as int)\n");
				stack_push(sea_expr_type_stack, int_type);
				//gen_stack_push_int(gen, tmp_tok->data);
                gen_stack_push_constant(gen, int_type, tmp_tok->data);
				break;
			case v_double:
				sea_dbg_expr_gen_msg("---> (as double)\n");
				stack_push(sea_expr_type_stack, float_type);
				//gen_stack_push_float(gen, tmp_tok->data);
                gen_stack_push_constant(gen, float_type, tmp_tok->data);
				break;
			case v_string:
				sea_dbg_expr_gen_msg("---> (as string)\n");
				stack_push(sea_expr_type_stack, string_type);
				//gen_stack_push_string(gen, tmp_tok->data);
                gen_stack_push_constant(gen, string_type, tmp_tok->data);
				break;
			case v_bool:
				sea_dbg_expr_gen_msg("---> (as bool)\n");
				stack_push(sea_expr_type_stack, bool_type);
				if (tmp_tok->type == t_kw_true) {
					//gen_stack_push_bool(gen, "true"); // nebo 0?
                    gen_stack_push_constant(gen, bool_type, "true");
				}
				else {
					//gen_stack_push_bool(gen, "false"); // nebo 1?
                    gen_stack_push_constant(gen, bool_type, "false");
				}
				break;
			}
		}
		sea_dbg_expr_msg("---> sea_give_expr_rule, (generovani PUSHS mam jeho typ)\n");
		break;
	case 44: // INTEGER - nikdy nenastane
		/*tmp_tok = sea_q_up(expr_tok_queue);
		sea_q_delete(expr_tok_queue);
		sea_dbg_expr_gen_msg("---> GENERATE PUSHS %s (as int)\n", tmp_tok->data);
		stack_push(sea_expr_type_stack, int_type);
		enum data_type tmp_type = sea_get_tok_var_type(tmp_tok);
		gen_stack_push_int(gen, strtol(tmp_tok->data, &eptr, 10));*/
		break;
	case 45: // LESS
		sea_time_stamp_reduce();
		sea_retype_expr(t_less, sea_expr_type_stack, gen);
		break;
	case 46: // HIGHER
		sea_time_stamp_reduce();
		sea_retype_expr(t_higher, sea_expr_type_stack, gen);
		break;
	case 47: // LESS EQUAL
		sea_time_stamp_reduce();
		sea_retype_expr(t_less_equal, sea_expr_type_stack, gen);
		break;
	case 48: // HIGHER EQUAL
		sea_time_stamp_reduce();
		sea_retype_expr(t_higher_equal, sea_expr_type_stack, gen);
		break;
	case 49: // AND
		sea_time_stamp_reduce();
		sea_retype_expr(t_kw_and, sea_expr_type_stack, gen);
		break;
	case 50: // OR
		sea_time_stamp_reduce();
		sea_retype_expr(t_kw_or, sea_expr_type_stack, gen);
		break;
	// CHYBI MI PRAVIDLO NOT
	case 51: // EQUAL
		sea_time_stamp_reduce();
		sea_retype_expr(t_equal, sea_expr_type_stack, gen);
		break;
	case 52: // NOT EQUAL
		sea_time_stamp_reduce();
		sea_retype_expr(t_not_equal, sea_expr_type_stack, gen);
		break;
	case 53: // CHR - nikdy nenastane
		break;
	case 54: // ASC - nikdy nenastane
		break;
	case 55: // unary minus
		sea_give_expr_rule(43);
		sea_give_expr_rule(37);
		break;
	case 56: // NOT
	case 57:
		sea_retype_expr(t_kw_not, sea_expr_type_stack, gen);
		break;
	default:
		break;
	}
}

void sea_time_stamp_check(int stamp, int params) {
	int tmp_stamp;
	sea_dbg_expr_msg("---> sea_time_stamp_check, stamp<%d>,params<%d> : ", stamp, params);
	for (int i = 0; i < params; i++) {
		tmp_stamp = (stack_top(time_stamp_stack))->data;
		sea_dbg_expr_msg("<%d> ", tmp_stamp);
		if (tmp_stamp == 0) {
			sea_dbg_expr_msg("FAIL\n");
			dbg_crash_msg("Spatny pocet parametru funkce\n");
			utl_exit(SEA_TYPE_ERR);
		}
		else if (tmp_stamp < stamp) {
			sea_dbg_expr_msg("FAIL\n");
			dbg_crash_msg("Spatny pocet parametru funkce\n");
			utl_exit(SEA_TYPE_ERR);
		}
		stack_pop(time_stamp_stack);
		sea_dbg_expr_msg("OK, ");
	}
	tmp_stamp = (stack_top(time_stamp_stack))->data;
	sea_dbg_expr_msg("\n---> sea_time_stamp_check, stamp<%d>,params<%d> : predchazejici <%d> ", stamp, params, tmp_stamp);
	if (tmp_stamp > stamp) {
		sea_dbg_expr_msg("FAIL\n");
		dbg_crash_msg("Spatny pocet parametru funkce\n");
		utl_exit(SEA_TYPE_ERR);
	}
	sea_dbg_expr_msg("OK\n");
}

void sea_time_stamp_reduce() {
	int a = (stack_top(time_stamp_stack))->data;
	stack_pop(time_stamp_stack);
	int b = (stack_top(time_stamp_stack))->data;
	stack_pop(time_stamp_stack);
	if (a > b) {
		stack_push(time_stamp_stack, b);
	}
	else {
		stack_push(time_stamp_stack, a);
	}
}

bool sea_is_operator(token* tok) {
	sea_dbg_expr_msg("---> sea_is_operator (porovnam)\n");
	switch (tok->type) {
	case t_plus:
	case t_minus:
	case t_slash:
	case t_backslash:
	case t_asterisk:
		sea_dbg_expr_msg("---> sea_is_operator (je to operator)\n");
		return true;
	default:
		sea_dbg_expr_msg("---> sea_is_operator (neni to operator)\n");
		return false;
	}
	return false;
}

bool sea_is_compare_operator(token* tok) {
	sea_dbg_expr_msg("---> sea_is_compare_operator (porovnam)\n");
	switch (tok->type) {
	case t_less:
	case t_higher:
	case t_less_equal:
	case t_higher_equal:
	case t_equal:
	case t_not_equal:
		sea_dbg_expr_msg("---> sea_is_compare_operator (je to operator)\n");
		return true;
	default:
		sea_dbg_expr_msg("---> sea_is_compare_operator (neni to operator)\n");
		return false;
	}
	return false;
}

enum data_type sea_get_tok_var_type(token* tok) {
	sea_dbg_expr_msg("---> sea_get_tok_var_type - <%s>, (jakeho typu je promenna?)\n", tok->data);
	if (tok->type == t_id) {
		// nejak mrknout do tabulky symbolu
		sea_dbg_expr_msg("---> sea_get_tok_var_type - <%s>, (token t_id)\n", tok->data);
		Symbol *temp = sya_sym_find(tok->data);
		return temp->type;
	}
	else if (tok->type == t_integer) {
		sea_dbg_expr_msg("---> sea_get_tok_var_type - <%s>, (token t_integer)\n", tok->data);
		return v_int;
	}
	else if (tok->type == t_double) {
		sea_dbg_expr_msg("---> sea_get_tok_var_type - <%s>, (token t_double)\n", tok->data);
		return v_double;
	}
	else if (tok->type == t_string) {
		sea_dbg_expr_msg("---> sea_get_tok_var_type - <%s>, (token t_string)\n", tok->data);
		return v_string;
	}
	else if (tok->type == t_kw_false || tok->type == t_kw_true) {
		sea_dbg_expr_msg("---> sea_get_tok_var_type - <%s>, (token bool true/false)\n", tok->data);
		return v_bool;
	}
	dbg_crash_msg("Spatny datovy typ operandu\n");
	utl_exit(SEA_TYPE_ERR);
	return v_bool;
}

int sea_find_var_gen_id(char *sym_id) {
	Symbol *tmp_symbol = sya_sym_find(sym_id);
	if (tmp_symbol != NULL) {
		return tmp_symbol->gen_id;
	}
	tmp_symbol = sya_param_find(sym_id);
	if (tmp_symbol != NULL) {
		return tmp_symbol->gen_id;
	}
	dbg_crash_msg("Nedari se ziskat generovaci ID promenne\n");
	utl_exit(SEA_TYPE_ERR);
	return 0;
}

int sea_find_function_gen_id(char *sym_id) {
	Symt_table *tmp_table = sya_symt_find(sym_id);
	if (tmp_table != NULL) {
		return tmp_table->gen_id;
	}
	dbg_crash_msg("Nedari se ziskat generovaci ID funkce\n");
	utl_exit(SEA_TYPE_ERR);
	return 0;
}

enum data_type_IFJcode17 sea_find_var_type(char *sym_id) {
	Symbol *tmp_symbol = sya_sym_find(sym_id);
	if (tmp_symbol == NULL) {
		tmp_symbol = sya_param_find(sym_id);
		if (tmp_symbol == NULL) {
			dbg_crash_msg("Nedari se ziskat typ promenne\n");
			utl_exit(SEA_TYPE_ERR);
		}
	}
	switch (tmp_symbol->type) {
	case v_int:
		return int_type;
	case v_double:
		return float_type;
	case v_char:
	case v_string:
		return string_type;
	case v_bool:
		return bool_type;
	case v_void:
		dbg_crash_msg("Nedari se ziskat typ promenne\n");
		utl_exit(SEA_TYPE_ERR);
		break;
	default:
		break;
	}
	dbg_crash_msg("Nedari se ziskat typ promenne\n");
	utl_exit(SEA_TYPE_ERR);
	return bool_type;
}

enum data_type_IFJcode17 sea_find_function_ret_type(char *sym_id) {
	Symt_table *tmp_table = sya_symt_find(sym_id);
	if (tmp_table == NULL) {
		dbg_crash_msg("Nedari se ziskat return type funkce\n");
		utl_exit(SEA_TYPE_ERR);
	}
	switch (tmp_table->ret_type) {
	case v_int:
		return int_type;
	case v_double:
		return float_type;
	case v_char:
	case v_string:
		return string_type;
	case v_bool:
		return bool_type;
	case v_void:
		dbg_crash_msg("Nedari se ziskat return type funkce\n");
		utl_exit(SEA_TYPE_ERR);
		break;
	default:
		break;
	}
	dbg_crash_msg("Nedari se ziskat return type funkce\n");
	utl_exit(SEA_TYPE_ERR);
	return bool_type;
}
