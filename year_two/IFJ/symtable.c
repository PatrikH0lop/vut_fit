/**
 * Projekt IFJ 2017
 * Implementace tabulky symbolu
 * @brief Implementace tabulky symbolu
 * @Author Matej Mitas, xmitas02
 * @Author Patrik Holop, xholop01
 * @file symtable.c
 */

#include "symtable.h"

// vytvoreni nove polozky stromu
Tree_node * sya_tree_create_node(char *str) {
    Tree_node *my_node = (Tree_node *) gc_new(sizeof(Tree_node));
    // misto pro stirng
    my_node->node_id = (char *) gc_new((sizeof(char) * strlen(str)));
    // zkopiruj
    strcpy(my_node->node_id, str);
    // polozka nema zadne deti
    my_node->left = NULL;
    my_node->right = NULL;
    // ukazatel na symbol
    my_node->sym_ptr = NULL;

    return my_node;
}

// porovnavani hodnot stringu
int sya_tree_str_cmp(char *str1, char *str2) {
    return (-1) * strcmp(str1, str2);
}

// pridej do stromu
Tree_node * sya_tree_insert(Tree_node **root, char *str) {
    // udelej si temp
    Tree_node *temp = sya_tree_create_node(str);
    // pridame do stromu, jinak zahodime
    if ( *root == NULL )
        *root = temp;
    else
        gc_delete(temp);
    // pokud uz strom neni prazdny, zavolej rekurzivne tuto funkci
    if (sya_tree_str_cmp((*root)->node_id, str) < 0) {
        sya_tree_insert(&(*root)->left, str);
    } else if (sya_tree_str_cmp((*root)->node_id, str) > 0) {
        sya_tree_insert(&(*root)->right, str);
    }
    // vrat temp
    return temp;
}

// odstran strom
void sya_tree_delete(Tree_node *root) {
    if( root != NULL ) {
        // odstran naalokovany string
        gc_delete(root->node_id);
        // odstran vsechny vedlejsi polozky
        sya_tree_delete(root->left);
        sya_tree_delete(root->right);
        // odstran cely strom
        //printf("%p\n", root);
        gc_delete(root);
    }
}

// vyhledej v ve stromu
Tree_node * sya_tree_search(Tree_node **leaf, char *str) {
    if (*leaf == NULL)
        return *leaf;
    else if (sya_tree_str_cmp((*leaf)->node_id, str) == 0)
        return *leaf;
    else {
        Tree_node *left = sya_tree_search(&(*leaf)->left, str);
        return left ? left : sya_tree_search(&(*leaf)->right, str);
    }
}

// vyhledej v ve stromu
Tree_node * sya_tree_search_param(Tree_node **leaf, int id) {
    if (*leaf == NULL)
        return *leaf;
    else if ((*leaf)->param_id == id)
        return *leaf;
    else {
        Tree_node *left = sya_tree_search_param(&(*leaf)->left, id);
        return left ? left : sya_tree_search_param(&(*leaf)->right, id);
    }
}

// vypis vysledek vyhledavani ve stromu
void sya_tree_print_result(Tree_node **root, char *str) {
    Tree_node *search = sya_tree_search(root, str);

    if (search != NULL)
        printf("Ano, je tam, hodnota: %s\n", search->node_id);
    else
        printf("Hodnota '%s' tam neni\n", str);
}

void sya_tree_replace_min(Tree_node **root, Tree_node *replace_ptr) {
    // pokud vyhledavame v prazdnem stromu
    if (root == NULL)
        return;

    if ((*root)->right == NULL) { // jsme na konci
        //
        gc_delete(replace_ptr->node_id);
        replace_ptr->node_id = (char *) gc_new(sizeof(char) * strlen((*root)->node_id));
        //
        strcpy(replace_ptr->node_id, (*root)->node_id);
        //
        replace_ptr->sym_ptr = (*root)->sym_ptr;


        // uloz pointer, aby jsi ho potom mohl vymazat
        Tree_node * tmp_item = *root;
        // napoj do listu
        *root = (*root)->left;
        // muzes ji uvolnit
        gc_delete(tmp_item);
    } else {
        sya_tree_replace_min(&(*root)->right, replace_ptr);
    }
}

void sya_tree_delete_node(Tree_node **root, char *str) {
    // pokud vyhledavame v prazdnem stromu
    if (*root == NULL)
        return;

    // hledame polozku, musime se zanorit
    if (sya_tree_str_cmp((*root)->node_id, str) > 0) {
        // leva vetev
        sya_tree_delete_node(&(*root)->left, str);
    } else if (sya_tree_str_cmp((*root)->node_id, str) < 0) {
        // prava vetev
        sya_tree_delete_node(&(*root)->right, str);
    } else { // polozku jsme nasli

        // pokud maji jenom jedno dite
        if ((*root)->left == NULL) {
            // zaloha
            Tree_node * tmp_item = *root;
            // prenes pointer
            *root = tmp_item->right;
            // pryc s tim
            gc_delete(tmp_item);
        } else if ((*root)->right == NULL) {
            // zaloha
            Tree_node * tmp_item = *root;
            // prenes pointer
            *root = tmp_item->left;
            // pryc s tim
            gc_delete(tmp_item);
        } else {
            // pokud maji dve deti
            sya_tree_replace_min(&(*root)->left, *root);
        }
    }
}

// Function to print binary tree in 2D
// It does reverse inorder traversal
void print2DUtil(Tree_node **root, int space) {
    // Base case
    if (*root == NULL)
        return;

    // Increase distance between levels
    space += 6;

    // Process right child first
    print2DUtil(&(*root)->right, space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = 6; i < space; i++)
        printf(" ");
    printf("%s\n", (*root)->node_id);

    // Process left child
    print2DUtil(&(*root)->left, space);
}

// Wrapper over print2DUtil()
void print2D(Tree_node **root) {
    printf("\n----------- Tisknu strom -------------\n");
   // Pass initial space count as 0
   print2DUtil(root, 0);
}



/**
 * Prace s tabulkami
 */




// vytvori novou tabulku
Symt_table * sya_symt_init(char *symt_id) {
	// alokace mista pro tabulku
	Symt_table *new_table = (Symt_table *) gc_new(sizeof(Symt_table));
	// alokace mista pro identifikator
	new_table->id = (char *) gc_new((sizeof(char) * strlen(symt_id)) + 1);
	// ulozeni identifikatoru na misto
	strcpy(new_table->id, symt_id);
	// nove vytvorena tabulky nemusi nic obsahovat
	new_table->root = NULL;
	new_table->param_root = NULL;
	new_table->params_count = -1;
	new_table->next = NULL;
	new_table->is_defined = false;
	// vrat tabulku
	return new_table;
};

bool sya_symt_is_empty(Symt_table * symt) {
	return (symt) ? symt->root : false;
}

bool sya_symt_is_active() {
	return (SYMT_LIST->act) ? true : false;
}

// inicializuje global tabulek funkci
void sya_symt_list_init() {
	if (!SYMT_LIST) {
		SYMT_LIST = (Symt_table_list *) gc_new(sizeof(Symt_table_list));
		SYMT_LIST->first = NULL;
		SYMT_LIST->act = NULL;
	}
};

// vytvori a prida novou tabulku symbolu
void sya_symt_create(char *symt_id) {
	// vytvorime tabulku
	Symt_table *new_table = sya_symt_init(symt_id);
	// pokud neni list inicializovany, vytvor list
	if (!SYMT_LIST)
		sya_symt_list_init();
	// pridame ji do listu
	if (SYMT_LIST->first == NULL) {
		// proste pridej na zacatek
		SYMT_LIST->first = new_table;
	} else {
		// promenne pro projeti listu
		Symt_table *prev = NULL;
		Symt_table *curr = SYMT_LIST->first;
		// dojed na konec listu
		while (curr != NULL) {
			prev = curr;
			curr = curr->next;
		}
		prev->next = new_table;
	}
};

// odstraneni parametru
/*Symbol * sya_param_destroy() {
	// vytvor symbol
	if (sya_symt_is_active()) {
		for (int i = 0; i < MAX_PARAMS_LEN; i++) {
			Symbol *curr = SYMT_LIST->act->params[i];

			if (curr != NULL) {
				gc_delete(curr->id);
				gc_delete(curr->val);
				gc_delete(curr);
			}
		}
	}
	return NULL;
}*/

// odstran celou tabulku
void sya_symt_delete(Symt_table *table) {
	if (table) {
		// odstran id tabulky
		gc_delete(table->id);
		// odstrani parametry, pokud tam nejake jsou
		//sya_param_destroy();
		// odstran cely strom
		sya_tree_delete(table->root);
		// odstran tabulku
		gc_delete(table);
	}
}

// vymaze tabulky symbolu s id == symt_id
void sya_symt_list_delete(char *symt_id) {
	if (SYMT_LIST) {
		Symt_table *prev = NULL;
		Symt_table *curr = SYMT_LIST->first;
		// dojed na konec listu
		while (curr != NULL) {
			// nasli jsme
			if (strcmp(curr->id, symt_id) == 0) {
				// presunume bud prvni nebo n-tou polozku
				if (curr == SYMT_LIST->first)
					SYMT_LIST->first = curr->next;
				else
					prev->next = curr->next;
				// ztracime aktivitu tabulky
				if (curr == SYMT_LIST->act)
					SYMT_LIST->act = NULL;
				// odstran tabulku
				sya_symt_delete(curr);
				// konec
				return;
			}
			// loopuj
			prev = curr;
			curr = curr->next;
		}
	}
}

// vymaze cely list
void sya_symt_list_destroy() {
	// pridame ji do listu
	if (SYMT_LIST) {
		Symt_table *prev = NULL;
		Symt_table *curr = SYMT_LIST->first;
		// dojed na konec listu
		while (curr != NULL) {
			prev = curr;
			curr = curr->next;
			// odstran tabulku
			sya_symt_delete(prev);
		}
		// vymaz zbytek
		gc_delete(curr);
		gc_delete(SYMT_LIST);
	}
}

// najde v listu
Symt_table * sya_symt_find(char *symt_id) {
	// pridame ji do listu
	if (SYMT_LIST) {
		Symt_table *prev = NULL;
		Symt_table *curr = SYMT_LIST->first;
		// dojed na konec listu
		while (curr != NULL) {
			if (strcmp(curr->id, symt_id) == 0)
				return curr;
			curr = curr->next;
		}
	}
	return NULL;
}

// vyhleda a nastavi aktivitu na tabulku symbolu
void sya_symt_list_act(char *symt_id) {
	Symt_table *found = sya_symt_find(symt_id);
	// nastav aktivitu
	if (found) {
		SYMT_LIST->act = found;
	}
}

/**
 * Prace se symboly
 */

// prida novy symbol
Symbol * sya_sym_init(char *sym_id, int gen_id) {
	// vytvor novy symbol
	Symbol *new_symbol = (Symbol *) gc_new(sizeof(Symbol));
	// vychozi datovy typ je void
	new_symbol->type = v_void;
	// generatorovy identifikator
	new_symbol->gen_id = gen_id;
	// identifikator
	new_symbol->id = (char *) gc_new((sizeof(char) * strlen(sym_id)) + 1);
	// ulozeni identifikatoru na misto
	strcpy(new_symbol->id, sym_id);
	// hodnota je po incializaci nulova
	new_symbol->val = NULL;
	// definovano
	new_symbol->defined = false;
	// vrat nove vytvoreny symbol
	return new_symbol;
}

// najde symbol
Symbol * sya_sym_find(char *sym_id) {
	if (sya_symt_is_active()) {
		// najdi uzel ve stromu
		Tree_node *found_node = sya_tree_search(&(SYMT_LIST)->act->root, sym_id);
		// pokud nasel
		if (found_node)
			return found_node->sym_ptr;
	} else {
		dbg_symtable_msg("Nemas aktivni tabulku\n");
	}
	return NULL;
}

// prida novy symbol
void sya_sym_add(char *sym_id, int gen_id) {
	if (sya_symt_is_active()) {
		// init symbol
		Symbol *new_symbol = sya_sym_init(sym_id, gen_id);
		// inituj uzel ve stromu
		Tree_node *new_node = sya_tree_insert(&(SYMT_LIST)->act->root, sym_id);
		// pridej ho do tabulky symbolu
		new_node->sym_ptr = new_symbol;
	} else {
		dbg_symtable_msg("Nemas aktivni tabulku\n");
	}
}

// edituje hodnotu symbolu
void sya_sym_edit_val(char *sym_id, char *new_val) {
	Symbol *found_symbol = sya_sym_find(sym_id);
	// nasli jsme
	if (found_symbol) {
		if (!found_symbol->val) {
			// alocuj plac
			found_symbol->val = (char *) gc_new((sizeof(char) * strlen(new_val)) + 1);
			found_symbol->defined = !found_symbol->defined;
		}
		else if (strlen(found_symbol->val) != strlen(new_val)) {
			// zmen misto
			found_symbol->val = (char *) gc_resize(found_symbol->val, (sizeof(char) * strlen(new_val)) + 1);
		}
		// zkopiruj do pametoveho mista
		strcpy(found_symbol->val, new_val);
	}
}

// precte hodnotu symbolu
void sya_sym_read(char *sym_id) {
	Symbol *found_symbol = sya_sym_find(sym_id);
	// nasli jsme
	if (found_symbol)
		dbg_symtable_msg(">Hodnota symbolu '%s' je '%s', datovy typ '%d', tedy je definovana '%d'\n", found_symbol->id, found_symbol->val, found_symbol->type, found_symbol->defined);
}

// edituje datovy typ symbolu
void sya_sym_edit_type(char *sym_id, enum data_type type) {
	Symbol *found_symbol = sya_sym_find(sym_id);
	// nasli jsme
	if (found_symbol) {
		found_symbol->type = type;
	}
}

// vymaze symbol
void sya_sym_delete(char *sym_id) {
	// zkus ho najit vymazat
	sya_tree_delete_node(&(SYMT_LIST)->act->root, sym_id);
}


/**
 * Prace s parametry (coz jsou prakticky symboly, nicmene je lepsi to oddelit)
 */

void sya_param_add(char *sym_id, int gen_id) {
	if (sya_symt_is_active()) {
		// init symbol
		Symbol *new_symbol = sya_sym_init(sym_id, gen_id);
		// inituj uzel ve stromu
		Tree_node *new_node = sya_tree_insert(&(SYMT_LIST)->act->param_root, sym_id);
		// incrementuj counter pro pridane indexy
		SYMT_LIST->act->params_count += 1;
		// pridej do parametru
		new_node->param_id = SYMT_LIST->act->params_count;
		// pridej ho do tabulky symbolu
		new_node->sym_ptr = new_symbol;
	} else {
		dbg_symtable_msg("Nemas aktivni tabulku\n");
	}
}

Symbol * sya_param_find(char *sym_id) {
	if (sya_symt_is_active()) {
		// najdi uzel ve stromu
		Tree_node *found_node = sya_tree_search(&(SYMT_LIST)->act->param_root, sym_id);
		// pokud nasel
		if (found_node)
			return found_node->sym_ptr;
	} else {
		dbg_symtable_msg("Nemas aktivni tabulku\n");
	}
	return NULL;
}

int sya_param_count(char *symt_id) {

	Symt_table *found_table = sya_symt_find(symt_id);

	if (found_table)
		return found_table->params_count;
	else
		dbg_symtable_msg("Nenasel jsem tabulku\n");
	return -1;
}

Symbol * sya_param_find_by_id(int id) {
	if (sya_symt_is_active()) {
		// najdi uzel ve stromu
		Tree_node *found_node = sya_tree_search_param(&(SYMT_LIST)->act->param_root, id);
		// pokud nasel
		if (found_node)
			return found_node->sym_ptr;
	} else {
		dbg_symtable_msg("Nemas aktivni tabulku\n");
	}
	return NULL;
}

Symbol * sya_param_find_in_symt_by_id(char *symt_id, int id) {

	Symt_table *found_table = sya_symt_find(symt_id);

	if (found_table) {
		// najdi uzel ve stromu
		Tree_node *found_node = sya_tree_search_param(&(found_table)->param_root, id);
		// pokud nasel
		if (found_node) {
			return found_node->sym_ptr;
		}
		else {
			dbg_symtable_msg("Nic\n");
		}
	} else {
		dbg_symtable_msg("Tabulka nenalezena\n");
	}
	return NULL;
}

void sya_param_add_type(char *sym_id, enum data_type type) {
	Symbol *found_symbol = sya_param_find(sym_id);
	// nasli jsme
	if (found_symbol) {
		found_symbol->type = type;
	}
}

void sya_param_add_val(char *sym_id, char *new_val) {
	Symbol *found_symbol = sya_param_find(sym_id);
	// nasli jsme
	if (found_symbol) {
		if (!found_symbol->val) {
			// alocuj plac
			found_symbol->val = (char *) gc_new((sizeof(char) * strlen(new_val)) + 1);
			found_symbol->defined = !found_symbol->defined;
		}
		else if (strlen(found_symbol->val) != strlen(new_val)) {
			// zmen misto
			found_symbol->val = (char *) gc_resize(found_symbol->val, (sizeof(char) * strlen(new_val)) + 1);
		}
		// zkopiruj do pametoveho mista
		strcpy(found_symbol->val, new_val);
	}
}

void sya_param_change_id(char *old_id, char *new_id) {

	//sya_tree_delete_node(&(SYMT_LIST)->act->param_root, old_id);

	Tree_node *found_node = sya_tree_search(&(SYMT_LIST)->act->param_root, old_id);

	if (found_node) {
		// vytvor novy node
		Tree_node * new_node = sya_tree_insert(&(SYMT_LIST)->act->param_root, new_id);

		// v symbolu vymaz index
		gc_delete(found_node->sym_ptr->id);
		found_node->sym_ptr->id = (char *) gc_new((sizeof(char) * strlen(new_id)));
		strcpy(found_node->sym_ptr->id, new_id);

		// zkopiruj obsah stareho nodu do noveho
		new_node->sym_ptr = found_node->sym_ptr;
		// odstran nam polozku
		sya_tree_delete_node(&(SYMT_LIST)->act->param_root, old_id);
	}
}

void sya_param_read(char *sym_id) {
	Symbol *found_symbol = sya_param_find(sym_id);
	// nasli jsme
	if (found_symbol)
		dbg_symtable_msg(">Hodnota parametru '%s' je '%s', datovy typ '%d', tedy je definovana '%d'\n", found_symbol->id, found_symbol->val, found_symbol->type, found_symbol->defined);
}

void sya_retype_item_print(int i) {
	switch (i) {
    	case 0:
    		printf("void\n");
    		break;
    	case 1:
    		printf("int\n");
    		break;
    	case 2:
    		printf("double\n");
    		break;
    	case 3:
    		printf("string\n");
    		break;
    	case 5:
    		printf("bool\n");
    		break;
    	default:
    		break;
    }
}

void inorder(Tree_node** root){

    if ((*root) != NULL) {
	    inorder(&(*root)->left);
	    printf("%s   ", (*root)->node_id);
	   	sya_retype_item_print((*root)->sym_ptr->type);
	    inorder(&(*root)->right);
    }
}

void sya_print_table(char *symt_id) {

	Symt_table *found_table = sya_symt_find(symt_id);

	if (found_table) {

		printf("Nazev funkce: 		%s\n", symt_id);
		printf("Navratova hodnota:	");

		sya_retype_item_print(found_table->ret_type);

		printf("\nParametry:\n");

		for (int i = found_table->params_count; i >= 0; i--) {

			Symbol *param = sya_param_find_in_symt_by_id(symt_id, i);

			printf("%s  ", param->id);
			//
			sya_retype_item_print(param->type);
		}

		printf("\nPromenne:\n");

		inorder(&(found_table)->root);

	} else {
		dbg_symtable_msg("Tabulka nenalezena\n");
	}
}
