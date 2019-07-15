/**
 * Projekt IFJ 2017
 * @brief Implementace stacku
 * @Author Matej Mitas, xmitas02
 * @Author Hanak Jiri, xhanak33
 * @file sya_stack.c
 */

#include "sya_stack.h"

// vytvoreni zasobniku
Stack * stack_init() {
    Stack *s = (Stack *) gc_new(sizeof(Stack));
    
    if (s == NULL)
        return NULL;
    
    // prazdny stack
    s->stack_pointer = NULL;
    
    return s;
}

// funkce pro zjisteni vrcholu zasobniku
Stack_item * stack_top(Stack *s) {
    return s->stack_pointer;
}

// kontrola, zda je zasobnik prazdny
bool stack_empty(Stack *s) {
    return stack_top(s) == NULL ? true : false;
}

// vycisteni a destrukce zasobniku
void stack_destroy(Stack *s) {
    if (!stack_empty(s)) {
        // naalokujeme dve polozky
        Stack_item *current_item = s->stack_pointer; // priprav si promennou na loopovani
        Stack_item *preceeding_item = NULL;

        // projed vsechny polozky
        while (current_item != NULL) {
            preceeding_item = current_item;
            current_item = current_item->next_item;
            gc_delete(preceeding_item); // odstran predchozi
        }
        gc_delete(current_item); // odstran soucasnou
    } 

    gc_delete(s);
}

// vyber prvni polozku, ktera je terminal
Stack_item * stack_first_terminal(Stack *s) {
    if (!stack_empty(s)) { // jestli ma vubec cenu se divat po polozce
        // polozka pro stack
        Stack_item *loop_item = s->stack_pointer; // priprav si promennou na loopovani
    
        while (loop_item != NULL) {
            // printuj hodnoty
            if (loop_item->data < OFFSET)
                return loop_item;
            
            loop_item = loop_item->next_item;
        }        
    }
    return NULL;
}

// odstraneni ze zasobniku
void stack_pop(Stack *s) {
    if (!stack_empty(s)) { // at mame co popovat
        Stack_item *last_elem = s->stack_pointer; // ulozime polozku pro odstraneni 
        s->stack_pointer = s->stack_pointer->next_item; // posuneme se v seznamu
        gc_delete(last_elem);
    }
}

void stack_push_behind_terminal(Stack *s, int t){
    Stack_item *new_item = (Stack_item *) gc_new(sizeof(Stack_item));

    if (new_item == NULL) // chyba alokace pameti
        return;

    Stack_item * terminal = stack_first_terminal(s);

    if (terminal == NULL) // terminal nenalezen
        return;

    new_item->next_item = terminal->next_item;
    terminal->next_item = new_item;

    new_item->data = t;
}

void stack_push_before_terminal(Stack *s, int t){
    Stack_item *new_item = (Stack_item *) gc_new(sizeof(Stack_item));

    if (!stack_empty(s)) {
        if (new_item == NULL) // chyba alokace pameti
            return;

        Stack_item * terminal = stack_first_terminal(s);

        if (terminal == NULL) // terminal nenalezen
            return;

        Stack_item *loop_item = s->stack_pointer; // priprav si promennou na loopovani
        if (loop_item == NULL){
            return;
        }

        if (loop_item->data < OFFSET) {
            stack_push(s, t);
        } else {
            while (loop_item != NULL && loop_item->next_item->data >= OFFSET) {
                loop_item = loop_item->next_item;
            }
            new_item->next_item = loop_item->next_item;
            loop_item->next_item = new_item;
            new_item->data = t;
        }
    } else {
        stack_push(s, t);
    }
}

// pridani do zasobniku
void stack_push(Stack *s, int t) {
    Stack_item *new_item = (Stack_item *) gc_new(sizeof(Stack_item));

    if (new_item == NULL) // chyba alokace pameti
        return;

    // pridej na stack ukazatel na polozku
    new_item->data = t;
    // pridej ukazatel na dalsi polozku
    new_item->next_item = s->stack_pointer;
    // prirad novou polozku
    s->stack_pointer = new_item;
}

// pomocna funkce pro vytisteni zasobniku
void stack_print(Stack *s) {    
    Stack_item *loop_item = s->stack_pointer; // priprav si promennou na loopovani
    //printf("----------- Tisknu stack -----------\n");
    // projed vsechny polozky
    while (loop_item != NULL) {
        // printuj hodnoty
        printf("|%2d", loop_item->data + 1);
        // nastav dalsi cyklus
        loop_item = loop_item->next_item;
    }
    // vyprintuj konec stacku
    printf("|$|\n");
    //printf("----------- Tisknu stack -----------\n");
}

// pomocna funkce pro zjisteni delky stacku
int stack_len(Stack *s) {    
    Stack_item *loop_item = s->stack_pointer; // priprav si promennou na loopovani
    int len = 0;
    // projed vsechny polozky
    while (loop_item != NULL) {
        // printuj hodnoty
        len += 1;
        // nastav dalsi cyklus
        loop_item = loop_item->next_item;
    }
    return len;
}