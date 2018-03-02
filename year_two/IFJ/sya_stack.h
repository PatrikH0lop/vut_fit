/**
 * Projekt IFJ 2017
 * Rozhrani stacku
 * @brief Rozhrani stacku
 * @Author Matej Mitas, xmitas02
 * @file sya_stack.h
 */

#ifndef STACK_H
#define STACK_H

/**
 * Includes
 */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "sya_terminal.h"
#include "utl_mem.h"

/**
 * Typ stack_item, ktery je pouzit pro ukladani polozek na zasobnik
 */
typedef struct Stack_item {
	int data; // typ pro terminal
	struct Stack_item *next_item; // typ pro dalsi polozku
} Stack_item;

/**
 * Typ stack, samotny zasobnik
 */
typedef struct Stack {
    Stack_item *stack_pointer;
} Stack;


/**
 * @brief Vytvoreni stacku
 * @Returns Ukazatel na nove vytvoreny stack
 */
Stack * stack_init();

/**
 * @brief Uklidi stack po pouziti
 * @param s Struktura pro zasobnik
 */
void stack_destroy(Stack *s);

/**
 * @brief Vrati polozku na vrcholu zasobniku
 * @Returns Polozku, ktera je na vrcholu zasobniku
 * @param s Struktura pro zasobnik
 */
Stack_item * stack_top(Stack *s);

/**
 * @brief Vrati prvni terminal na stacku
 * @Returns Prvni terminal na stacku
 * @param s Struktura pro zasobnik
 */
Stack_item * stack_first_terminal(Stack *s);

/**
 * @brief Vyhodi jednu polozku ze stacku
 * @param s Struktura pro zasobnik
 */
void stack_pop(Stack *s);

/**
 * @brief Prida na stack terminal ci nonterminal
 * @param s Struktura pro zasobnik
 * @param t Terminal
 */
void stack_push(Stack *s, int t);

/**
 * @brief Prida polozku pred terminal
 * @param s Struktura pro zasobnik
 * @param t Terminal
 */
void stack_push_behind_terminal(Stack *s, int t);

/**
 * @brief Prida polozku za terminal
 * @param s Struktura pro zasobnik
 * @param t Terminal
 */
void stack_push_before_terminal(Stack *s, int t);

/**
 * @brief Je stack prazdny?
 * @param s Struktura pro zasobnik
 * @Returns Binarni hodnotu ohledne prazdnosti stacku
 */
bool stack_empty(Stack *s);

/**
 * @brief Vypsani celeho obsahu stacku na obrazovku
 * @param s Struktura pro zasobnik
 */
void stack_print(Stack *s);

/**
 * @brief Delka stacku
 * @param s Struktura pro zasobnik
 */
int stack_len(Stack *s);

#endif