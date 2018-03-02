/**
 * Projekt IFJ 2017
 * @brief Implementace generatoru vnitrniho kodu
 * @Author HanakJiri, xhanak33
 * @Author Matej Mitas, xmitas02
 * @file gvk_generator.c
 */

#include "gvk_generator.h"

/**
 * Obecne funkce pro praci s generatorem
 */

Generator* init_generator() {

    FILE *f = stdout;

    //f = fopen("output.IFJcode17", "w");

    if (f == NULL) {
        dbg_crash_msg("Nepovedlo se zalozit vystpni soubor.");
        utl_exit(INNER_ERROR);
    }

    Generator *generator = (Generator *)gc_new(sizeof(Generator));
    Stack *stack = (Stack *)gc_new(sizeof(Stack));
    stack_init(stack);

    generator->id_label = 0;
    generator->id_variable = 0;
    generator->f = f;
    generator->s = stack;

    fprintf(generator->f, ".IFJcode17" NEW_LINE NEW_LINE);
    fprintf(generator->f, DEFVAR EAX NEW_LINE);
    fprintf(generator->f, DEFVAR EBX NEW_LINE);
    fprintf(generator->f, DEFVAR ECX NEW_LINE);
    fprintf(generator->f, DEFVAR EDX NEW_LINE);
    fprintf(generator->f, NEW_LINE);
    fprintf(generator->f, JUMP LAB NEW_LINE, generator->id_label);
    stack_push(generator->s, generator->id_label++);

    return generator;
}


void destroy_generator(Generator *g) {

    fprintf(g->f, NEW_LINE);
    //fclose(g->f);
    gc_delete(g->s);
    gc_delete(g);
}


void gen_scope_start(Generator *g) {

    dbg_gen_msg("gen_scope_start");
    fprintf(g->f, LABEL LAB NEW_LINE, stack_top(g->s)->data);
    stack_pop(g->s);                                                //ulozi label pro main scope
    fprintf(g->f, CREATEFRAME NEW_LINE);
}

/**
 * Matematicke operace se zasobnikem
 */

void gen_stack_operation_plus(Generator *g) {

    dbg_gen_msg("gen_stack_operation_plus");
    fprintf(g->f, ADDS NEW_LINE);
}


void gen_stack_operation_minus(Generator *g) {

    dbg_gen_msg("gen_stack_operation_minus");
    fprintf(g->f, SUBS NEW_LINE);
}


void gen_stack_operation_multiple(Generator *g) {

    dbg_gen_msg("gen_stack_operation_multiple");
    fprintf(g->f, MULS NEW_LINE);
}


void gen_stack_operation_divide_int(Generator *g) {

    dbg_gen_msg("#gen_stack_operation_divide_int");
    gen_stack_convert_int2float_first(g);
    gen_stack_convert_int2float_second(g);
    fprintf(g->f, DIVS NEW_LINE);
    gen_stack_convert_float2int_first(g);
}


void gen_stack_operation_divide_float(Generator *g) {

    dbg_gen_msg("gen_stack_operation_divide_float");
    fprintf(g->f, DIVS NEW_LINE);
}


void gen_stack_operation_swap(Generator *g) {

    dbg_gen_msg("gen_stack_operation_swap");

    fprintf(g->f, POPS EAX NEW_LINE);
    fprintf(g->f, POPS EBX NEW_LINE);

    fprintf(g->f, PUSHS EAX NEW_LINE);
    fprintf(g->f, PUSHS EBX NEW_LINE);
}


void gen_stack_operation_concat_strings(Generator *g) {

    dbg_gen_msg("gen_stack_operation_concat_strings");

    fprintf(g->f, POPS EAX NEW_LINE);
    fprintf(g->f, POPS EBX NEW_LINE);

    fprintf(g->f, CONCAT EAX EBX EAX NEW_LINE);
    fprintf(g->f, PUSHS EAX NEW_LINE);
}


void gen_stack_convert_int2float_first(Generator *g) {

    dbg_gen_msg("gen_stack_convert_int2float_first");
    fprintf(g->f, INT2FLOATS NEW_LINE);
}


void gen_stack_convert_int2float_second(Generator *g) {

    dbg_gen_msg("gen_stack_convert_int2float_second");
    fprintf(g->f, POPS EAX NEW_LINE);

    fprintf(g->f, INT2FLOATS NEW_LINE);
    fprintf(g->f, PUSHS EAX NEW_LINE);
}


void gen_stack_convert_float2int_first(Generator *g) {

    dbg_gen_msg("gen_stack_convert_float2int_first");
    fprintf(g->f, FLOAT2R2EINTS NEW_LINE);
}


void gen_stack_convert_float2int_second(Generator *g) {

    dbg_gen_msg("gen_stack_convert_float2int_second");
    fprintf(g->f, POPS EAX NEW_LINE);

    fprintf(g->f, FLOAT2R2EINTS NEW_LINE);
    fprintf(g->f, PUSHS EAX NEW_LINE);
}


/**
 * Ukladani na stack
 */

void gen_stack_push_register(Generator *g, int reg) {

    switch (reg) {
        case 0:
            fprintf(g->f, PUSHS EAX NEW_LINE);
            break;
        case 1:
            fprintf(g->f, PUSHS EBX NEW_LINE);
            break;
        case 2:
            fprintf(g->f, PUSHS ECX NEW_LINE);
            break;
        case 3:
            fprintf(g->f, PUSHS EDX NEW_LINE);
            break;
        default:
            dbg_crash_msg("Spatny registr.");
            utl_exit(INNER_ERROR);
    }
}


void gen_stack_pop_to_register(Generator *g, int reg) {

    switch (reg) {
        case 0:
            fprintf(g->f, POPS EAX NEW_LINE);
            break;
        case 1:
            fprintf(g->f, POPS EBX NEW_LINE);
            break;
        case 2:
            fprintf(g->f, POPS ECX NEW_LINE);
            break;
        case 3:
            fprintf(g->f, POPS EDX NEW_LINE);
            break;
        default:
            dbg_crash_msg("Spatny registr.");
            utl_exit(INNER_ERROR);
    }
}


void gen_stack_push_var(Generator *g, int var) {

    dbg_gen_msg("gen_stack_push_var");
    fprintf(g->f, PUSHS T_VAR NEW_LINE, var);
}


void gen_stack_push_constant(Generator *g, int type, char *value) {

    dbg_gen_msg("gen_stack_push_constant");
    fprintf(g->f, PUSHS);

    switch (type) {
        case 0:
            fprintf(g->f, INT AS "%s" NEW_LINE, value);
            break;
        case 1:
            fprintf(g->f, FLOAT AS "%s" NEW_LINE, value);
            break;
        case 2:
            fprintf(g->f, STRING AS "%s" NEW_LINE, value);
            break;
        case 3:
            fprintf(g->f, BOOL AS "%s" NEW_LINE, value);
            break;
        default:
            dbg_crash_msg("Spatny enum");
            utl_exit(INNER_ERROR);
            break;
    }
}


void gen_stack_push_int(Generator *g, int value) {

    dbg_gen_msg("gen_stack_push_int");
    fprintf(g->f, PUSHS INT AS "%d" NEW_LINE, value);
}


void gen_stack_push_float(Generator *g, float value) {

    dbg_gen_msg("gen_stack_push_float");
    fprintf(g->f, PUSHS FLOAT AS "%f" NEW_LINE, value);
}


void gen_stack_push_string(Generator *g, char *value) {

    dbg_gen_msg("gen_stack_push_string");
    fprintf(g->f, PUSHS STRING AS "%s" NEW_LINE, value);
}


void gen_stack_push_bool(Generator *g, int value) {

    dbg_gen_msg("gen_stack_push_bool");
    fprintf(g->f, PUSHS BOOL AS "%s" NEW_LINE, (value == 0) ? "false" : "true");
}

/**
 * Porovnavani aritmeticky
 */

void gen_stack_eval_LT(Generator *g) {

    dbg_gen_msg("gen_stack_eval_LT");
    fprintf(g->f, LTS NEW_LINE);
}


void gen_stack_eval_GT(Generator *g) {

    dbg_gen_msg("gen_stack_eval_GT");
    fprintf(g->f, GTS NEW_LINE);
}


void gen_stack_eval_LTE(Generator *g) {

    dbg_gen_msg("#gen_stack_eval_LTE");

    fprintf(g->f, POPS EAX NEW_LINE);
    fprintf(g->f, POPS EBX NEW_LINE);

    fprintf(g->f, PUSHS EBX NEW_LINE);
    fprintf(g->f, PUSHS EAX NEW_LINE);
    fprintf(g->f, LTS NEW_LINE);

    fprintf(g->f, PUSHS EBX NEW_LINE);
    fprintf(g->f, PUSHS EAX NEW_LINE);
    fprintf(g->f, EQS NEW_LINE);

    fprintf(g->f, ORS NEW_LINE);
}


void gen_stack_eval_GTE(Generator *g) {

    dbg_gen_msg("#gen_stack_eval_GTE");

    fprintf(g->f, POPS EAX NEW_LINE);
    fprintf(g->f, POPS EBX NEW_LINE);

    fprintf(g->f, PUSHS EBX NEW_LINE);
    fprintf(g->f, PUSHS EAX NEW_LINE);
    fprintf(g->f, GTS NEW_LINE);

    fprintf(g->f, PUSHS EBX NEW_LINE);
    fprintf(g->f, PUSHS EAX NEW_LINE);
    fprintf(g->f, EQS NEW_LINE);

    fprintf(g->f, ORS NEW_LINE);
}


void gen_stack_eval_EQ(Generator *g) {

    dbg_gen_msg("gen_stack_eval_EQ");
    fprintf(g->f, EQS NEW_LINE);
}

/**
 * Porovnavani logicky
 */

void gen_stack_eval_AND(Generator *g) {

    dbg_gen_msg("gen_stack_eval_AND");
    fprintf(g->f, ANDS NEW_LINE);
}


void gen_stack_eval_OR(Generator *g) {

    dbg_gen_msg("gen_stack_eval_OR");
    fprintf(g->f, ORS NEW_LINE);
}


void gen_stack_eval_NOT(Generator *g) {

    dbg_gen_msg("gen_stack_eval_NOT");
    fprintf(g->f, NOTS NEW_LINE);
}


void gen_stack_top_print(Generator *g) {

    dbg_gen_msg("gen_stack_top_print");
    fprintf(g->f, POPS EAX NEW_LINE);
    fprintf(g->f, WRITE EAX NEW_LINE);
}

/**
 * Prirazeni
 */

void gen_order_assignment_from_stack(Generator *g, int var) {

    dbg_gen_msg("gen_order_assignment_from_stack");
    fprintf(g->f, POPS T_VAR NEW_LINE, var);
}

void gen_order_assignment_plus_from_stack(Generator *g, int var) {

    dbg_gen_msg("gen_order_assignment_plus_from_stack");
    fprintf(g->f, PUSHS T_VAR NEW_LINE, var);
    fprintf(g->f, ADDS NEW_LINE);
    fprintf(g->f, POPS T_VAR NEW_LINE, var);
}


void gen_order_assignment_minus_from_stack(Generator *g, int var) {

    dbg_gen_msg("gen_order_assignment_minus_from_stack");
    fprintf(g->f, POPS EAX NEW_LINE);
    fprintf(g->f, PUSHS T_VAR NEW_LINE, var);
    fprintf(g->f, PUSHS EAX NEW_LINE);
    fprintf(g->f, SUBS NEW_LINE);
    fprintf(g->f, POPS T_VAR NEW_LINE, var);
}


void gen_order_assignment_asterisk_from_stack(Generator *g, int var) {

    dbg_gen_msg("gen_order_assignment_asterisk_from_stack");
    fprintf(g->f, PUSHS T_VAR NEW_LINE, var);
    fprintf(g->f, MULS NEW_LINE);
    fprintf(g->f, POPS T_VAR NEW_LINE, var);
}


void gen_order_assignment_slash_from_stack(Generator *g, int var) {

    dbg_gen_msg("gen_order_assignment_slash_from_stack");
    fprintf(g->f, POPS EAX NEW_LINE);
    fprintf(g->f, PUSHS T_VAR NEW_LINE, var);
    fprintf(g->f, PUSHS EAX NEW_LINE);
    fprintf(g->f, DIVS NEW_LINE);
    fprintf(g->f, POPS T_VAR NEW_LINE, var);
}


void gen_order_assignment_backslash_from_stack(Generator *g, int var) {

    dbg_gen_msg("gen_order_assignment_slash_from_stack");
    fprintf(g->f, POPS EAX NEW_LINE);
    fprintf(g->f, PUSHS T_VAR NEW_LINE, var);
    fprintf(g->f, INT2FLOATS NEW_LINE);
    fprintf(g->f, PUSHS EAX NEW_LINE);
    fprintf(g->f, INT2FLOATS NEW_LINE);
    fprintf(g->f, DIVS NEW_LINE);
    fprintf(g->f, FLOAT2R2EINTS NEW_LINE);
    fprintf(g->f, POPS T_VAR NEW_LINE, var);
}

void gen_order_assignment(Generator *g, int var_src, int var_dest) {

    dbg_gen_msg("gen_order_assignment");
    fprintf(g->f, MOVE T_VAR T_VAR NEW_LINE, var_dest, var_src);
}

void gen_order_assignment_constant(Generator *g, int type, int var, char *value) {

    dbg_gen_msg("gen_order_assignment_constant");
    fprintf(g->f, MOVE T_VAR, var);

    switch (type) {
        case 0:
            fprintf(g->f, INT AS "%s" NEW_LINE, value);
            break;
        case 1:
            fprintf(g->f, FLOAT AS "%s" NEW_LINE, value);
            break;
        case 2:
            fprintf(g->f, STRING AS "%s" NEW_LINE, value);
            break;
        case 3:
            fprintf(g->f, BOOL AS "%s" NEW_LINE, value);
            break;
        default:
            dbg_crash_msg("Spatny enum");
            utl_exit(INNER_ERROR);
            break;
    }
}


void gen_order_assignment_int(Generator *g, int var, int value) {

    dbg_gen_msg("gen_order_assignment_int");
    fprintf(g->f, MOVE T_VAR INT AS "%d" NEW_LINE, var, value);
}


void gen_order_assignment_float(Generator *g, int var, float value) {

    dbg_gen_msg("gen_order_assignment_float");
    fprintf(g->f, MOVE T_VAR FLOAT AS "%f" NEW_LINE, var, value);
}


void gen_order_assignment_string(Generator *g, int var, char *value) {

    dbg_gen_msg("gen_order_assignment_string");
    fprintf(g->f, MOVE T_VAR STRING AS "%s" NEW_LINE, var, value);
}


void gen_order_assignment_bool(Generator *g, int var, int value) {

    dbg_gen_msg("gen_order_assignment_bool");
    fprintf(g->f, MOVE T_VAR BOOL AS "%s" NEW_LINE, var, (value == 0) ? "false" : "true");
}


void gen_order_write_string(Generator *g, char *s) {

    dbg_gen_msg("gen_order_write_string");
    fprintf(g->f, WRITE STRING AS "%s" NEW_LINE, s);
}

void gen_order_write_var(Generator *g, int var) {

    dbg_gen_msg("gen_order_write_var");
    fprintf(g->f, WRITE T_VAR NEW_LINE, var);
}

void gen_order_read(Generator *g, int var, int type) {

    dbg_gen_msg("gen_order_read");
    fprintf(g->f, READ T_VAR, var);

    switch (type) {
        case 0:
            fprintf(g->f, INT NEW_LINE);
            break;
        case 1:
            fprintf(g->f, FLOAT NEW_LINE);
            break;
        case 2:
            fprintf(g->f, STRING NEW_LINE);
            break;
        case 3:
            fprintf(g->f, BOOL NEW_LINE);
            break;
        default:
            dbg_crash_msg("Spatny enum");
            utl_exit(INNER_ERROR);
            break;
    }
}


int gen_order_new_var(Generator *g) {

    dbg_gen_msg("gen_order_new_var");
    fprintf(g->f, DEFVAR T_VAR NEW_LINE, g->id_variable);
    return (g->id_variable++);
}

/**
 * Funkce
 */

void gen_call_function(Generator *g, int function) {

    dbg_gen_msg("gen_call_function");
    fprintf(g->f, CALL LAB NEW_LINE, function);
}


int gen_function_start(Generator *g) {

    dbg_gen_msg("gen_function_start");
    fprintf(g->f, LABEL LAB NEW_LINE, g->id_label);
    fprintf(g->f, PUSHFRAME NEW_LINE);
    fprintf(g->f, CREATEFRAME NEW_LINE);

    return g->id_label++;
}


void gen_function_end(Generator *g) {

    dbg_gen_msg("gen_function_end");
    fprintf(g->f, POPFRAME NEW_LINE);
    fprintf(g->f, RETURN NEW_LINE);
}

/**
 * Podminky
 */

void gen_condition_if(Generator *g) {

    dbg_gen_msg("gen_condition_if");
    stack_push(g->s, g->id_label++);                            //oznaceni konce podminky
    fprintf(g->f, JUMPIFNEQS LAB NEW_LINE, g->id_label);        //skok na dalsi else(if) vetev
    stack_push(g->s, g->id_label++);                            //oznaceni dalsi else(if) vetve k provadeni
}


void gen_condition_end_if_branch(Generator *g) {

    dbg_gen_msg("end_if_branch");
    int tmp = stack_top(g->s)->data;
    stack_pop(g->s);

    fprintf(g->f, JUMP LAB NEW_LINE, stack_top(g->s)->data);    //pokud se predchozi vetev provedla, skoc na konec cele podminky
    stack_push(g->s, tmp);

    fprintf(g->f, LABEL LAB NEW_LINE, stack_top(g->s)->data);   //oznaceni soucastne vetve
    stack_pop(g->s);                                            //zahodim label
}

void gen_condition_elseif(Generator *g) {

    dbg_gen_msg("gen_condition_elseif");

    fprintf(g->f, JUMPIFNEQS LAB NEW_LINE, g->id_label);        //v pripade neplatnosti podminky pokracuj dalsi else(if) vetvi
    stack_push(g->s, g->id_label++);
}


void gen_condition_else(Generator *g) {

    dbg_gen_msg("gen_condition_else");
    int tmp = stack_top(g->s)->data;
    stack_pop(g->s);

    fprintf(g->f, JUMP LAB NEW_LINE, stack_top(g->s)->data);    //pokud se predchozi vetev provedla, skoc na konec cele podminky
    stack_push(g->s, tmp);

    fprintf(g->f, LABEL LAB NEW_LINE, stack_top(g->s)->data);   //oznaceni soucastne vetve
    stack_pop(g->s);                                            //zahodim label

    stack_push(g->s, g->id_label++);                            //nadbytecny label, slouzi pro obecnost ukonceni podminky
}

void gen_condition_end(Generator *g) {

    dbg_gen_msg("gen_condition_end");
    fprintf(g->f, LABEL LAB NEW_LINE, stack_top(g->s)->data);   //zacatek prazdne vetve
    stack_pop(g->s);

    fprintf(g->f, LABEL LAB NEW_LINE, stack_top(g->s)->data);   //konec cele podminky
    stack_pop(g->s);
}

/**
 * Cykly
 */

void gen_cycle_start(Generator *g) {

    dbg_gen_msg("gen_cycle_start");
    fprintf(g->f, LABEL LAB NEW_LINE, g->id_label);
    stack_push(g->s, g->id_label++);
}


void gen_cycle_eval_condition(Generator *g) {

    dbg_gen_msg("gen_cycle_eval_condition");
    fprintf(g->f, JUMPIFNEQS LAB NEW_LINE, g->id_label);
    stack_push(g->s, g->id_label++);
}


void gen_cycle_end(Generator *g) {

    dbg_gen_msg("gen_cycle_end");
    int tmp = stack_top(g->s)->data;
    stack_pop(g->s);

    fprintf(g->f, JUMP LAB NEW_LINE, stack_top(g->s)->data);
    stack_pop(g->s);

    fprintf(g->f, LABEL LAB NEW_LINE, tmp);
}

void check_type(Generator *g) {

    dbg_gen_msg("check_type");
    fprintf(g->f, POPS EAX NEW_LINE);
    fprintf(g->f, PUSHS EAX NEW_LINE);
    fprintf(g->f, TYPE EAX EAX NEW_LINE);
    fprintf(g->f, PUSHS EAX NEW_LINE);
}


void built_in_function_Length(Generator *g) {

    dbg_gen_msg("built_in_function_Length");
    fprintf(g->f, POPS EAX NEW_LINE);

    fprintf(g->f, STRLEN EAX EAX NEW_LINE);
    fprintf(g->f, PUSHS EAX NEW_LINE);
}


void built_in_function_SubStr(Generator *g) {

    dbg_gen_msg("built_in_function_SubStr");

    fprintf(g->f, POPS ECX NEW_LINE);                               //pocet
    fprintf(g->f, POPS EBX NEW_LINE);                               //index
    fprintf(g->f, SUB EBX EBX INT AS "1" NEW_LINE);
    fprintf(g->f, POPS EAX NEW_LINE);                               //zdroj

    fprintf(g->f, STRLEN EDX EAX NEW_LINE);                         //edx=len(eax)
    fprintf(g->f, PUSHS EBX NEW_LINE);
    fprintf(g->f, PUSHS INT AS "0" NEW_LINE);
    fprintf(g->f, LTS NEW_LINE);

    fprintf(g->f, PUSHS BOOL AS FALSE_VALUE NEW_LINE);
    gen_condition_if(g);                                            //pokud je vstupni retezec ok (delsi jak 0)

        fprintf(g->f, PUSHS ECX NEW_LINE);
        fprintf(g->f, PUSHS INT AS "1" NEW_LINE);
        fprintf(g->f, LTS NEW_LINE);

        fprintf(g->f, PUSHS ECX NEW_LINE);
        fprintf(g->f, PUSHS EDX NEW_LINE);
        fprintf(g->f, PUSHS EBX NEW_LINE);
        fprintf(g->f, SUBS NEW_LINE);

        fprintf(g->f, GTS NEW_LINE);
        fprintf(g->f, ORS NEW_LINE);

        fprintf(g->f, PUSHS BOOL AS FALSE_VALUE NEW_LINE);
        gen_condition_if(g);                                            //substring nepretece a je delsi nebo roven 0
            fprintf(g->f, ADD EDX EBX ECX NEW_LINE);                    //oznaceni konce substringu
        gen_condition_end(g);
    

        fprintf(g->f, MOVE ECX STRING AS "" NEW_LINE);

        gen_cycle_start(g);                                             //vyhodnoceni obratky cyklu

        fprintf(g->f, PUSHS EBX NEW_LINE);
        fprintf(g->f, PUSHS EDX NEW_LINE);
        fprintf(g->f, LTS NEW_LINE);
        fprintf(g->f, PUSHS BOOL AS TRUE_VALUE NEW_LINE);

        gen_cycle_eval_condition(g);                                    //samotne provedeni substringu
            fprintf(g->f, PUSHS EDX NEW_LINE);
            fprintf(g->f, GETCHAR EDX EAX EBX NEW_LINE);                //ziskam znak
            fprintf(g->f, CONCAT ECX ECX EDX NEW_LINE);                 //pripojim znak k prodretezci
            fprintf(g->f, ADD EBX EBX INT AS "1" NEW_LINE);             //inkremetuji ECX
            fprintf(g->f, POPS EDX NEW_LINE);
        gen_cycle_end(g);

        fprintf(g->f, PUSHS ECX NEW_LINE);                              //vratim podretezec

    gen_condition_else(g);                                              //vstupni retezec je prazdny
        gen_stack_push_constant(g, string_type, "\0");

    gen_condition_end(g);

}


void built_in_function_Asc(Generator *g) {

    dbg_gen_msg("built_in_function_Asc");

    fprintf(g->f, POPS EBX NEW_LINE);                               //ulozim si parametry do promennych
    fprintf(g->f, SUB EBX EBX INT AS "1" NEW_LINE);
    fprintf(g->f, POPS EAX NEW_LINE);

    fprintf(g->f, STRLEN ECX EAX NEW_LINE);                         //zjistim delku retezce

    fprintf(g->f, PUSHS EBX NEW_LINE);                              //zkontorluji meze
    fprintf(g->f, PUSHS ECX NEW_LINE);
    fprintf(g->f, LTS NEW_LINE);

    fprintf(g->f, PUSHS EBX NEW_LINE);
    fprintf(g->f, PUSHS INT AS "-1" NEW_LINE);
    fprintf(g->f, GTS NEW_LINE);

    fprintf(g->f, ANDS NEW_LINE);

    fprintf(g->f, PUSHS BOOL AS TRUE_VALUE NEW_LINE);
    fprintf(g->f, EQS NEW_LINE);

    fprintf(g->f, PUSHS BOOL AS TRUE_VALUE NEW_LINE);
    gen_condition_if(g);

    fprintf(g->f, PUSHS EAX NEW_LINE);                              //provedu konverzi
    fprintf(g->f, PUSHS EBX NEW_LINE);

    fprintf(g->f, STRI2INTS NEW_LINE);

    gen_condition_else(g);                                          //index mimo meze
    fprintf(g->f, PUSHS INT AS "0" NEW_LINE);

    gen_condition_end(g);

}


void built_in_function_Chr(Generator *g) {

    dbg_gen_msg("built_in_function_Chr");
    fprintf(g->f, POPS EAX NEW_LINE);

    fprintf(g->f, PUSHS EAX NEW_LINE);                              //zkontorluji meze
    fprintf(g->f, PUSHS INT AS "256" NEW_LINE);
    fprintf(g->f, LTS NEW_LINE);

    fprintf(g->f, PUSHS EAX NEW_LINE);
    fprintf(g->f, PUSHS INT AS "-1" NEW_LINE);
    fprintf(g->f, GTS NEW_LINE);

    fprintf(g->f, ANDS NEW_LINE);

    fprintf(g->f, PUSHS BOOL AS TRUE_VALUE NEW_LINE);
    fprintf(g->f, EQS NEW_LINE);

    fprintf(g->f, PUSHS BOOL AS TRUE_VALUE NEW_LINE);
    gen_condition_if(g);

    fprintf(g->f, PUSHS EAX NEW_LINE);                       //provedu konverzi
    fprintf(g->f, INT2CHARS NEW_LINE);

    gen_condition_else(g);                                          //hodnota mimo meze
    gen_stack_push_constant(g, string_type, "err");

    gen_condition_end(g);

}


/**
 * Pomocna funkce
 */

void gen_print_new_line(Generator *g) {
    fprintf(g->f, NEW_LINE);
}

/**
 * Testovani
 */

char * gen_out_file_name(char *str) {
    // delka cele te vychytavky
    int original_len = strlen(str);
    // promenne na indexy
    char *last_slash_i = strchr(str, '/');
    char *last_dot_i = strchr(str, '.');
    // nema cenu s tim nic delat, nezmeni se
    if (!last_slash_i && !last_dot_i) {
        return str;
    } else {
        if (!last_slash_i)
            last_slash_i = str; // zacatek je posunut
        else if (!last_dot_i)
            last_dot_i = str + (original_len - 1); // dojed na konec
        else if (last_slash_i)
            last_slash_i += 1;
        // nova delka
        int new_len = last_dot_i - last_slash_i;
        // misto pro novy string
        char *new_name = (char *) gc_new(sizeof(char) * (new_len + 1));
        // zkopiruj
        strncpy(new_name, last_slash_i, new_len);
        // vrat
        return new_name;
    }
}