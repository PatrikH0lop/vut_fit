/**
 * Projekt IFJ 2017
 * @brief Rozhrani generatoru vnitrniho kodu
 * @Author HanakJiri, xhanak33
 * @Author Matej Mitas, xmitas02
 * @file gvk_generator.h
 */

#ifndef IFJ17_FIT_2017_GVK_GENERATOR_H
#define IFJ17_FIT_2017_GVK_GENERATOR_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "utl_err.h"
#include "utl_mem.h"
#include "utl_debug.h"
#include "sya_stack.h"
#include "gvk_language_ifjcode17.h"


#define VAR "var_%d "
#define T_VAR "TF@var_%d "      //soucastny ramec
#define L_VAR "LF@var_%d "      //vrchol zasobnku ramcu
#define LAB "label_%d"
#define NEW_LINE "\n"

#define EAX "GF@var_eax "
#define EBX "GF@var_ebx "
#define ECX "GF@var_ecx "
#define EDX "GF@var_edx "

typedef struct {
    FILE *f;
    int id_label;
    int id_variable;
    Stack *s;
} Generator;

enum data_type_IFJcode17 {
    int_type,
    float_type,
    string_type,
    bool_type
};

/**
 * Inicializuje generator, zavede vystupni soubor
 * Zavede stack
 * @return instance generatoru
 */
Generator* init_generator();

/**
 * Ukonci zapis do souboru
 * Uvolni stack
 * @param g generator
 */
void destroy_generator(Generator *g);

/**
 * Urci, kde zacina scope
 * Vola se jen jednou
 * @param g generator
 */
void gen_scope_start(Generator *g);

/**
 * Secte dve cisla ze zasobniku, vysledek ulozi na vrchol zasobniku
 * @param g generator
 */
void gen_stack_operation_plus(Generator *g);

/**
 * Odecte dve cisla ze zasobniku, vysledek ulozi na vrchol zasobniku
 * @param g generator
 */
void gen_stack_operation_minus(Generator *g);

/**
 * Vynasobi dve cisla ze zasobniku, vysledek ulozi na vrchol zasobniku
 * @param g generator
 */
void gen_stack_operation_multiple(Generator *g);

/**
 * Provede celociselne deleni prvnich dvou hodnot na zasobniku, vysledek typu integer ulozi na zasobnik
 * @param g generator
 */
void gen_stack_operation_divide_int(Generator *g);

/**
 * Provede deleni prvnich dvou hodnot na zasobniku, vysledek je typu float
 * Oba operandy musi byt typu integer
 * @param g generator
 */
void gen_stack_operation_divide_float(Generator *g);

/**
 * Prohodi dve polozky nejblize vrcholu na zasobniku
 * @param g generator
 */
void gen_stack_operation_swap(Generator *g);

/**
 * Spoji dva retezce na vrcholu zasobniku
 * @param g generator
 */
void gen_stack_operation_concat_strings(Generator *g);

/**
 * Prevede vrchol zasobniku typu integer na float
 * @param g generator
 */
void gen_stack_convert_int2float_first(Generator *g);

/**
 * Prevede druhy prvek zasobniku typu integer na float
 * @param g generator
 */
void gen_stack_convert_int2float_second(Generator *g);

/**
 * Prevede vrchol zasobniku typu float na integer
 * @param g generator
 */
void gen_stack_convert_float2int_first(Generator *g);

/**
 * Prevede druhy prvek zasobniku typu float na integer
 * @param g generator
 */
void gen_stack_convert_float2int_second(Generator *g);

/**
 * Na stdout vypise obsah promene ulozene na vrcholu zasobniku
 * @param g generator
 */
void gen_stack_top_print(Generator *g);

/**
 * Upravi zadanou nazev potazmo i s cestou do vypsatelneho tvaru
 * @param g generator
 */
char * gen_out_file_name(char *name);

/**
 * Ulozi na zasobnik obsah registru
 * @param g generator
 * @param reg oznaceni registru (0-3)
 */
void gen_stack_push_register(Generator *g, int reg);


/**
 * Ulozi vrchol zsaobniku do registru
 * @param g generator
 * @param reg oznaceni registru (0-3)
 */
void gen_stack_pop_to_register(Generator *g, int reg);

/**
 * Ulozi na vrchol zasobniku obsah promenne var
 * @param g generator
 * @param var identifikace promenne, ktera se ma pushnout
 */
void gen_stack_push_var(Generator *g, int var);

/**
 * Ulozi na vrchol zasobniku konstantu value
 * @param g generator
 * @param type typ konstanty
 * @param value hodnota konstanty
 */
void gen_stack_push_constant(Generator *g, int type, char *value);

/**
 * Ulozi na vrchol zasobniku konstantu value
 * @param g generator
 * @param value hodnota konstanty
 */
void gen_stack_push_int(Generator *g, int value);

/**
 * Ulozi na vrchol zasobniku konstantu value
 * @param g generator
 * @param value hodnota konstanty
 */
void gen_stack_push_float(Generator *g, float value);

/**
 * Ulozi na vrchol zasobniku konstantu value
 * @param g generator
 * @param value hodnota konstanty
 */
void gen_stack_push_string(Generator *g, char *value);

/**
 * Ulozi na vrchol zasobniku konstantu value
 * @param g generator
 * @param value hodnota konstanty
 */
void gen_stack_push_bool(Generator *g, int value);

/**
 * Porovna dve hodnoty na vrcholu zasobniku, zda li je prvni mensi
 * @param g generator
 */
void gen_stack_eval_LT(Generator *g);

/**
 * Porovna dve hodnoty na vrcholu zasobniku, zda li je prvni vetsi
 * @param g generator
 */
void gen_stack_eval_GT(Generator *g);

/**
 * Porovna dve hodnoty na vrcholu zasobniku, zda li je prvni mensi  nebo jsou si rovny
 * @param g generator
 */
void gen_stack_eval_LTE(Generator *g);

/**
 * Porovna dve hodnoty na vrcholu zasobniku, zda li je prvni vetsi nebo jsou si rovny
 * @param g generator
 */
void gen_stack_eval_GTE(Generator *g);

/**
 * Porovna dve hodnoty na vrcholu zasobniku, zda li jsou si rovny
 * @param g generator
 */
void gen_stack_eval_EQ(Generator *g);

/**
 * Porovna dve hodnoty na vrcholu zasobniku, zda li jsou obe true
 * @param g generator
 */
void gen_stack_eval_AND(Generator *g);

/**
 * Porovna dve hodnoty na vrcholu zasobniku, zda li je alespon jedna true
 * @param g generator
 */
void gen_stack_eval_OR(Generator *g);

/**
 * Zneguje bool hodnotu na vrcholu zasobniku
 * @param g generator
 */
void gen_stack_eval_NOT(Generator *g);

/**
 * Do promenne var priradi hodnotu z vrcholu zasobniku, provede pop
 * Predpoklada stejne typy
 * @param g generator
 * @param var identifikace promenne, do ktere se ma ulozit vrchol zasobnku, provede pop
 */
void gen_order_assignment_from_stack(Generator *g, int var);

/**
 * K promenne var pricte hodnotu z vrcholu zasobniku, provede pop
 * Predpoklada stejne typy
 * @param g generator
 * @param var identifikace promenne, do ktere se ma ulozit vrchol zasobnku, provede pop
 */
void gen_order_assignment_plus_from_stack(Generator *g, int var);

/**
 * K promenne var odecte hodnotu z vrcholu zasobniku, provede pop
 * Predpoklada stejne typy
 * @param g generator
 * @param var identifikace promenne, do ktere se ma ulozit vrchol zasobnku, provede pop
 */
void gen_order_assignment_minus_from_stack(Generator *g, int var);

/**
 * Promennou var vynasobi hodnotou z vrcholu zasobniku, provede pop
 * Predpoklada stejne typy
 * @param g generator
 * @param var identifikace promenne, do ktere se ma ulozit vrchol zasobnku, provede pop
 */
void gen_order_assignment_asterisk_from_stack(Generator *g, int var);

/**
 * Promennou var vydeli hodnotou z vrcholu zasobniku, provede pop
 * Predpoklada stejne typy
 * @param g generator
 * @param var identifikace promenne, do ktere se ma ulozit vrchol zasobnku, provede pop
 */
void gen_order_assignment_slash_from_stack(Generator *g, int var);

/**
 * Promennou var vydeli celociselnou hodnotou z vrcholu zasobniku, provede pop
 * Predpoklada stejne typy
 * @param g generator
 * @param var identifikace promenne, do ktere se ma ulozit vrchol zasobnku, provede pop
 */
void gen_order_assignment_backslash_from_stack(Generator *g, int var);

/**
 * Do promenne var_dest priradi hodnotu z var_src
 * Predpoklada stejne typy obou promennych
 * @param g generator
 * @param var_src identifikace promenne, ze ktere se vezme hodnota
 * @param var_dest identifikace promenne, do ktere se hodnota ulozi
 */
void gen_order_assignment(Generator *g, int var_src, int var_dest);

/**
 * Ulozi do promenne var priradi konstantu value
 * @param g generator
 * @param type typ konstanty
 * @param var promenna, do ktere se ma priradit konstanta
 * @param value hodnota konstanty
 */
void gen_order_assignment_constant(Generator *g, int type, int var, char *value);

/**
 * Ulozi do promenne var priradi konstantu value
 * @param g generator
 * @param var promenna, do ktere se ma priradit konstanta
 * @param value hodnota konstanty
 */
void gen_order_assignment_int(Generator *g, int var, int value);

/**
 * Ulozi do promenne var priradi konstantu value
 * @param g generator
 * @param var promenna, do ktere se ma priradit konstanta
 * @param value hodnota konstanty
 */
void gen_order_assignment_float(Generator *g, int var, float value);

/**
 * Ulozi do promenne var priradi konstantu value
 * @param g generator
 * @param var promenna, do ktere se ma priradit konstanta
 * @param value hodnota konstanty
 */
void gen_order_assignment_string(Generator *g, int var, char *value);

/**
 * Ulozi do promenne var priradi konstantu value
 * @param g generator
 * @param var promenna, do ktere se ma priradit konstanta
 * @param value hodnota konstanty
 */
void gen_order_assignment_bool(Generator *g, int var, int value);

/**
 * Na stdout zapise retezec s
 * Predpoklada spravny tvar retezce
 * @param g generator
 * @param s retezec, ktery se ma tisknout na stdout
 */
void gen_order_write_string(Generator *g, char *s);

/**
 * Na stdout zapise retezec s
 * Predpoklada spravny tvar retezce
 * @param g generator
 * @param var promenna, jejiz obsah se ma vytisknout
 */
void gen_order_write_var(Generator *g, int var);

/**
 * Precte hodnotu z stdin a zapise ji do var
 * @param g generator
 * @param var identifikace promenne, do ktere se nacte vstup z stdin
 * @param type typ promenne
 */
void gen_order_read(Generator *g, int var, int type);

/**
 * Deklarace nove promenne
 * @param g generator
 * @return identifikaci nove vytvorene promenne
 */
int gen_order_new_var(Generator *g);

/**
 * Volani drive definovane funkce.
 * Parametry ocekava an zasobniku, posledni je na vrcholu
 * Vysledek funkce ulozi zpatky na vrchol zasobniku
 * @param g generator
 * @param function identifikace volane funkce
 */
void gen_call_function(Generator *g, int function);

/**
 * Znaci, kdy zacina funkce
 * Parametry ma ulozene na zasobniku, posledni je na vrcholu
 * @param g generator
 * @return identifikace funkce
 */
int gen_function_start(Generator *g);

/**
 * Znaci, kde konci funkce
 * Pred zavolanim je potreba ulozit vysledek funkce na zasobnik
 * @param g generator
 */
void gen_function_end(Generator *g);

/**
 * Na zaklade bool hodnoty na vrcholu zasobniku provede sekvenci prikazu
 * Pokud podminka neplati a vyskytuje se else nebo else if, skoci na nej
 * @param g generator
 */
void gen_condition_if(Generator *g);

/**
 * Na zaklade bool hodnoty na vrcholu zasobniku provede sekvenci prikazu
 * @param g generator
 */
void gen_condition_end_if_branch(Generator *g);

/**
 * Vlozi else if vetev
 * Ocekava bool hodnotu na vrcholu datoveho zasobniku
 * Predpoklada drivejsi volani gen_condition_eval_condition
 * @param g generator
 */
void gen_condition_elseif(Generator *g);

/**
 * Vlozi else vetev
 * Predpoklada drivejsi volani condition_if
 * @param g generator
 */
void gen_condition_else(Generator *g);

/**
 * Zacatek cyklu while
 * @param g generator
 */
void gen_cycle_start(Generator *g);

/**
 * Na zaklade bool hodnoty na vrcholu zasobniku provede jednu obratku cyklu while
 * Predpoklada drivejsi volani cycle_start
 * @param g generator

 */
void gen_cycle_eval_condition(Generator *g);

/**
 * Ukonci provadeni cyklu
 * Predpoklada drivejsi volani cycle_eval_condition
 * @param g generator
 */
void gen_cycle_end(Generator *g);

/**
 * Na vrchol zasobniku zapise v textove podobe nazev datoveho typu promenne, ktera je aktualne na vrcholu zasobniku.
 * Promenna zustane na zasobniku.
 * @param g generator
 */
void check_type(Generator *g);

/**
 * Vrati delku (pocet znaku) retezce zadaneho jedinym parametrem s
 * @param g generator
 */
void built_in_function_Length(Generator *g);

/**
 * Vrati podretezec zadaneho retezce s. Druhym parametrem i je dan zacatek
 * pozadovaneho podretezce (pocitano od jedničcy) a treti parametr n urcuje delku podretezce.
 * Parametry jsou ulozeny v zasobnku od vrcholu v poradi: n, i, s
 * Je-li s prazdne nebo i ≤ 0, vraci prazdný retezec. Je-li n < 0 nebo n > Length (s) − i,
 * jsou jako retezec vraceny od i-tého znaku vsechny zbyvajici znaky retezce s.
 * @param g generator
 */
void built_in_function_SubStr(Generator *g);

/**
 * Vrati ordinalni hodnotu (ASCII) znaku na pozici i v retezci s. Je-li pozice mimo meze retezce, vraci 0.
 * Pozice i je ulozena na vrcholu zasobniku, retezc s je hned za vrcholem zasobniku.
 * @param g generator
 */
void built_in_function_Asc(Generator *g);

/**
 * Na vrchol zasobnku vrati jednoznakovy retezec se znakem, jehoz ASCII kód je zadan parametrem P na vrcholu zasobniku.
 * Pripad, kdy je P mimo interval [0; 255], ma nedefinovane chovani.
 * @param g generator
 */
void built_in_function_Chr(Generator *g);

/**
 * Vytiskne novy radek
 * @param g generator
 */
void gen_print_new_line(Generator *g);

/**
 * Vygeneruje koniec podmienky
 * @param g generator
 */
void gen_condition_end(Generator *g);


#endif //IFJ17_FIT_2017_GVK_GENERATOR_H
