/**
 * Projekt IFJ 2017
 * Rozhrani tabulky symbolu
 * @brief Rozhrani tabulky symbolu
 * @Author Matej Mitas, xmitas02
 * @Author Patrik Holop, xholop01
 * @file symtable.h
 */

#ifndef IFJ17_FIT_2017_SYMTABLE_H
#define IFJ17_FIT_2017_SYMTABLE_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// prace s pameti a errory

#include "sya_data_types.h"
#include "utl_mem.h"
#include "utl_debug.h"

#define MAX_PARAMS_LEN 15

/**
 * Typ obecneho symbolu
 */
typedef struct {
    enum data_type type; // datovy typ
    char *id; // identifikator
    int gen_id; // generatorove id
    char *val; // hodnota
    // promenne pro SEA
    bool defined;
} Symbol;

/**
 * Typ symbolu ve kterem jsou ulozene promenne
 */
typedef struct Tree_node {
    char *node_id; // identifikator symbolu
    int param_id;
    Symbol *sym_ptr; // typ symbolu pro promennou
    // polozky stromu
    struct Tree_node *left; // leva vetev stromu
    struct Tree_node *right; // prava vetev stromu
} Tree_node;

/**
 * Typ datove tabulky
 */
typedef struct Symt_table {
	// info k soucasne tabulce
	char *id; // id pouzivane pri generovani
	int gen_id; // koren stromu, nad kterym je tabulka generovana
	// koreny stromu
	Tree_node *root; // koren pro promenne
	Tree_node *param_root; // koren pro parametry
	// pocet parametru
	int params_count;
	// ukazatel na dalsi polozku
	struct Symt_table *next;
	// navratovy typ
	enum data_type ret_type;
	// ak uz bola definovana
	bool is_defined;
} Symt_table;


/**
 * Linearni list pro ulozeni tabulekc
 */
typedef struct Symt_table_list {
	struct Symt_table *first;
	struct Symt_table *act;
} Symt_table_list;

// linearni list polozek seznamu tabulek
Symt_table_list *SYMT_LIST;


/**
 * Implementace tabulky symbolu
 */


/**
 * Kontrola, jestli je mame aktivni tabulku
 * @return pravdivostni hodnota o stavu aktivity
 */
bool sya_symt_is_active();

/**
 * Kontrola, jestli je tabulka prazdna
 * @return pravdivostni hodnota o stavu
 */
bool sya_symt_is_empty(Symt_table * symt);

/**
 * Incializuj tabulku pro jeji vytvoreni
 * @param nazev tabulky
 * @return nove vytvorena tabulka
 */
Symt_table *sya_symt_init(char *sym_id);

/**
 * Vytvor novou tabulku
 * @param nazev tabulky
 */
void sya_symt_create(char *symt_id);

/**
 * Vymaz binarni strom na ukladani parametru
 * @return hodnota parametru
 */
Symbol *sya_param_destroy();

/**
 * Vymaz tabulku s identifikatorem
 * @param nazev tabulky
 */
void sya_symt_delete(Symt_table *table);

/**
 * Najdi v tabulku, implicitne pracujuci s aktivni tabulkou
 * @param nazev tabulky
 * @return ukazatel na nalezenou tabulku
 */
Symt_table *sya_symt_find(char *symt_id);

/**
 * Nastav aktivitu na tabulku
 * @param nazev tabulky
 */
void sya_symt_list_act(char *symt_id);

/**
 * Pridej novy symbol do aktivni tabulky
 * @param nazev symbolu
 * @param id pro generator
 */
void sya_sym_add(char *sym_id, int gen_id);

/**
 * Najdi symbol
 * @param nazev symbolu
 * @return ukazatel na nalezeny symbol
 */
Symbol *sya_sym_find(char *sym_id);

/**
 * Uprav hodnotu symbolu
 * @param nazev symbolu
 * @param nova hodnota
 */
void sya_sym_edit_val(char *sym_id, char *new_val);

/**
 * Uprav datovy typ symbolu
 * @param nazev symbolu
 * @param nova datovy typ
 */
void sya_sym_edit_type(char *sym_id, enum data_type type);

/**
 * Vymaz symbol
 * @param nazev symbolu
 */
void sya_sym_delete(char *sym_id);

/**
 * Pridej parametr (pro funkce)
 * @param nazev parametru
 * @param id pro generator
 */
void sya_param_add(char *sym_id, int gen_id);

/**
 * Pridej hodnotu do parametru (pro funkce)
 * @param nazev parametru
 * @param nova hodnota
 */
void sya_param_add_val(char *sym_id, char *new_val);

/**
 * Pridej datovy typ parametru (pro funkce)
 * @param nazev parametru
 * @param novy typ
 */
void sya_param_add_type(char *sym_id, enum data_type type);

/**
 * Najdi parametr podle nazvu
 * @param nazev parametru
 * @return ukazatel na nalezeny symbol
 */
Symbol *sya_param_find(char *sym_id);

/**
 * Najdi parametr pro indexu
 * @param index parametru
 * @return ukazatel na nalezeny symbol
 */
Symbol * sya_param_find_by_id(int id);

/**
 * Vymen nazev parametru
 * @param nazev parametru
 * @param novy nazev parametru
 */
void sya_param_change_id(char *old_id, char *new_id);

/**
 * Vytisteni kontrolnich informaci o tabulkce symbolu
 * @param nazev tabulky
 */
void sya_print_table(char *symt_id);

/**
 * Vytisteni binarniho stromu v tabulkce symbolu
 * @param Koren stromu
 */
void inorder(Tree_node** root);

/**
 * Pomocna funkce pro vytisteni datoveho typu
 * @param datovy typ polozky
 */
void sya_retype_item_print(int i);

/**
 * Vyhledani parametru v aktivni tabulce
 * @param nazev tabulky symbolu ve ktere hledame
 * @param pozice parametru
 * @return Symbol v tabulce parametru
 */
Symbol * sya_param_find_in_symt_by_id(char *symt_id, int id);


/**
 * Implementace binarniho stromu
 */

/**
 * Vytvoreni uzlu stromu
 * @param nazev uzlu
 * @return uzel
 */
Tree_node * sya_tree_create_node(char *str);

/**
 * Pomocna funkce pro porovnani dvou stringu
 * @param prvni string
 * @param druhy string
 */
int sya_tree_str_cmp(char *str1, char *str2);

/**
 * Vloz do stromu
 * @param koren stromu
 * @return nazev uzlu
 */
Tree_node * sya_tree_insert(Tree_node **root, char *str);

/**
 * Vymaz cely strom
 * @param koren stromu
 */
void sya_tree_delete(Tree_node *root);

/**
 * Najdi uzel obalujici symbol
 * @param koren stromu
 * @return nazev podle ktereho hledat
 */
Tree_node * sya_tree_search(Tree_node **leaf, char *str);

/**
 * Najdi uzel obalujici param s id
 * @param koren stromu
 * @return id podle ktereho hledat
 */
Tree_node * sya_tree_search_param(Tree_node **leaf, int id);

/**
 * Pomocna funkce pro vytisne hledane polozky
 * @param koren stromu
 * @param nazev, podle ktereho hledat
 */
void sya_tree_print_result(Tree_node **root, char *str);

/**
 * Najdi nejmensi polozku v prave vetvi
 * @param koren stromu
 * @param pointer k prepsani
 */
void sya_tree_replace_min(Tree_node **root, Tree_node *replace_ptr);

/**
 * Vymaz jeden uzel
 * @param koren stromu
 * @param nazev uzlu
 */
void sya_tree_delete_node(Tree_node **root, char *str);

#endif
