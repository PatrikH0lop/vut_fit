/**
 * Projekt IFJ 2017
 * @brief Rozhrani GC
 * @Author Matej Mitas, xmitas02
 * @file _utl_mem.h
 */

#ifndef IFJ17_FIT_2017_UTL_MEM_H
#define IFJ17_FIT_2017_UTL_MEM_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>

#include "utl_err.h"
#include "utl_debug.h"

// polozka listu
typedef struct gc_List_item {
    void *ptr;
    struct gc_List_item *next;
} gc_List_item;

// list
typedef struct {
    gc_List_item *first;
    gc_List_item *last;
    bool empty;
} gc_List;

// globalni objekt pro garbage collector
gc_List *GC;

/**
 * Sestavuje vyrazy na zaklade precedencni tabulky
 */
void gc_init();

/**
 * Prida polozku do zasobniku pointeru
 * @param pointer k pridani
 */
void gc_list_add(void *ptr);

/**
 * Najde pointer ve seznamu pointeru
 * @param pointer
 * @return polozka v linearnim seznamu s pointerem
 */
gc_List_item * gc_list_find(void *ptr);

/**
 * Pomocna funkce pro realloc()
 * @param stary pointer
 * @param novy pointer
 */
void gc_list_find_modify(void *ptr, void *new_ptr);

/**
 * Pomocna funkce pro free()
 * @param pointer k vymazani
 */
void gc_list_delete(void *ptr);

/**
 * Vycisteni celeho GC, zpravidla volano na konci programu nebo pri chybe
 */
void gc_cleanup();

/**
 * Pomocna funkce pro reseni chybovych stavu
 */
void gc_error();

/**
 * Pomocna funkce pro vytisteni celeho obsahu GC
 */
void gc_list_print();

// funkce pro uzivatele
/**
 * Wrapper funkce pro malloc()
 * @param velikost pametoveho mista, velikost v size_t
 * @return pointer na nove naalokave misto
 */
void * gc_new(size_t size);

/**
 * Wrapper funkce pro realloc()
 * @param stary pointer
 * @param velikost noveho pametoveho mista, velikost v size_t
 * @return pointer na nove prealokovane misto, muze byt stejny jako ptr, ale nemusi
 */
void * gc_resize(void *ptr, size_t size);

/**
 * Wrapper funkce pro free()
 * @param pointer k vymazani
 */
void gc_delete(void *ptr);

#endif