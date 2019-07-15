/**
 * Projekt IFJ 2017
 * @brief Parser
 * @Author HanakJiri, xhanak33
 * @file sya_parser.c
 */

#include "sya_parser.h"
#include "utl_debug.h"
#include "sea_library.h"

token * solve_expr(Stack *stack, FILE *f, token *tok) {

    Rule *rule = grm_init_rule(ntr_expr, 20, 0);                    //postupne sesatvene pravidlo, ktere se ma aplikovat
    int ter_top;                                                    //prvni terminal na zasobiku
    int compare;                                                    //prohazovani pri vkladani na stack
    int mask_token;                                                 //maskovani vlastni hodnoty za promenou

    stack_push(stack, tr_eof);                                      //zarazka na zasobniku
    ter_top = stack_first_terminal(stack)->data;

    dbg_msg("----------Zacatek vyrazu-------------\n");

    do {                                                            //postupne projde cely vyraz
        dbg_msg("input %2d, stack %2d    ", tok->type + 1, ter_top + 1);
        dbg_print_stack(stack);

        if ((tok->type == tr_id && is_function(tok->data)) || tok->type == tr_kw_length ||          //zkontroluje, zda se nejedna o nazev funkce
                tok->type == tr_kw_substr || tok->type == tr_kw_asc || tok->type == tr_kw_chr) {
            tok->type = tr_id_fce;
        }

        if (tok->type == tr_minus) {                                //rozliseni binarniho a unarniho minus
            if (!(stack_top(stack)->data == tr_right_bracket || stack_top(stack)->data == tr_id || stack_top(stack)->data == ntr_expr)) {
                tok->type = tr_unary_minus;
            }
        }

        if (tok->type >= 30 && tok->type <= 34) {                   //maskovani konsatnt za promennou
            mask_token = tr_id;
        } else if (tok->type < tr_plus) {                           //maskovani obecnych terminalu za konec vyrazu
            mask_token = tr_eof;
        } else {
            mask_token = tok->type;
        }

        dbg_msg("mask  %2d, stack %2d\n", mask_token + 1, ter_top + 1);

        switch (get_action(ter_top, mask_token)) {
            case mov:                                                 //=
                dbg_msg("=\n");
                stack_push(stack, mask_token);

                sea_give_next_token(tok, rule->id);

                gc_delete(tok);
                tok = get_next_token(f);
                break;

            case str:                                                 //<
                dbg_msg("<\n");

                //vlozi zacatek pravidla za prvni terminal na satcku, pokud je to eof, vlozi pred
                if (stack_first_terminal(stack)->data < OFFSET) {
                    stack_push_before_terminal(stack, expr_str);
                } else {
                    stack_push_behind_terminal(stack, expr_str);
                }
                stack_push(stack, mask_token);

                sea_give_next_token(tok, rule->id);

                gc_delete(tok);
                tok = get_next_token(f);
                break;

            case end:                                                 //>
                dbg_msg(">\n");

                //zahodi terminaly, ktere jsou soucasti pravidla, nastavi pravidlo
                rule->rule_length = 0;
                for (int i = 0; ter_top != expr_str; ++i) {
                    ter_top = stack_top(stack)->data;
                    rule->terminal[i] = ter_top;
                    rule->rule_length++;

                    dbg_msg("%d ", rule->terminal[i] + 1);

                    stack_pop(stack);
                    ter_top = stack_top(stack)->data;
                }
                stack_pop(stack);

                compare = compare_rules(rule);
                if (compare == -1) {
                    dbg_msg("schazi pravidlo\n");
                    utl_exit(SYA_ERR);
                } else if (compare != ntr_parameter) {
                    stack_push(stack, compare);
                    dbg_msg("left terminal  %d\n", compare + 1);
                } else {
                    dbg_msg("parametr\n");
                }


                sea_give_next_token(tok, rule->id);
                break;

            case err:
                dbg_crash_msg("Chyba ve vyrazu\n");
                utl_exit(SYA_ERR);
                break;
            case sea:
                dbg_crash_msg("Chyba ve vyrazu\n");
                utl_exit(SEA_PRG_ERR);
                break;
            default:
                dbg_crash_msg("Vynitrni chyba.\n");
                utl_exit(INNER_ERROR);
        }
        ter_top = stack_first_terminal(stack)->data;

    } while (!(mask_token == tr_eof && ter_top == tr_eof));

    stack_pop(stack);                                               //leva strana posledniho pravidla
    stack_pop(stack);                                               //zarazka

    dbg_msg("-----------Konec vyrazu-------------\n");

    return tok;
}

void run_parser(FILE *f) {

    grm_init();                                                     //inicializace gramatiky
    Stack *stack = stack_init();                                    //inicializace zasobniku
    sea_init();                                                     //inicializace semantiky

    stack_push(stack, tr_eof);                                      //priprava zasobniku
    stack_push(stack, ntr_start_state);

    token *tok;
    Rule *rule;
    tok = get_next_token(f);

    while (!stack_empty(stack)) {                                   //postupne precte vsechny tokeny
        dbg_msg("input %2d, stack %2d\n", tok->type + 1, stack_top(stack)->data + 1);
        if (stack_top(stack)->data < ntr_start_state && tok->type < ntr_start_state) {
            if (stack_top(stack)->data == tok->type) {                  //shoda tokenu a terminalu -> pop

                dbg_msg("shoda zasobnkiu a vstupu    -> pop %2d \n", stack_top(stack)->data + 1);

                stack_pop(stack);

                sea_give_next_token(tok, rule->id);

                gc_delete(tok);
                tok = get_next_token(f);
                continue;

            } else {
                dbg_crash_msg("Spatny token na vstupu\n");
                utl_exit(SYA_ERR);
            }
        } else if (stack_top(stack)->data == ntr_expr) {            //vyhodnoceni vyrazu
            stack_pop(stack);
            sea_begin_expression();
            tok = solve_expr(stack, f, tok);
            sea_end_of_expression();
            continue;
        }

        rule = get_rule(tok->type, stack_top(stack)->data);         //zjisti se pravidlo, kterym se ma pokracovat

        //osetreni proti duplicitnimu scope
        if (tok->type == tr_kw_scope && was_active_scope() && stack_top(stack)->data == ntr_eol && rule->id == 1) {
            dbg_crash_msg("Duplicitni scope \n");
            utl_exit(SYA_ERR);
        }


        dbg_msg("      -> aplikuji pravidlo %2d - ", rule->id);
        sya_test_msg("r  %2d\n", rule->id);

        if (rule->id == 0) {
            dbg_crash_msg("\n Chyba syntaxe\n");
            utl_exit(SYA_ERR);
        } else if (rule->id == 8 && is_active_scope()) {            //return mimo funkci
            dbg_crash_msg("\n Unexpected return\n");
            utl_exit(SYA_ERR);
        }

        //nahrazeni non-terminalu pravidlem
        stack_pop(stack);
        for (int i = rule->rule_length - 1; i >= 0 ; --i) {
            stack_push(stack, rule->terminal[i]);
        }

        sea_give_next_token(tok, rule->id);

        dbg_print_stack(stack);
    }

    // post programova kontrola definicie vsetkych deklarovanych premmennych
    sea_check_allfunc_def();


    stack_destroy(stack);                                           //korektni ukonceni parseru
    grm_destroy();
}
