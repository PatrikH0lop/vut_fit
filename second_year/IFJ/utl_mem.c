/**
 * Projekt IFJ 2017
 * implementace GC
 * @brief implementace GC
 * @Author Matej Mitas, xmitas02
 * @file _utl_mem.c
 */

#include "utl_mem.h"

// inicializuj list
void gc_init() {
    // novy list
    GC = (gc_List *) malloc(sizeof(gc_List));
    // zde musime kontrolovat postaru
    if (GC == NULL) {
        dbg_crash_msg("Memory Error\n");
        exit(UTL_MEM_ERR);
    }
    // nastaveni hodnot
    GC->first = NULL;
    GC->last = NULL;
    GC->empty = true;
}

// vycisti, kdyby nahodou
void gc_cleanup() {
    if (GC) {
        // vycisti list
        if (!(GC->empty)) {
            //printf("Leakujeme, ale je to vycistene\n");
            // priprav si promennou na loopovani
            gc_List_item *loop_item = GC->first;
            // projed vsechny polozky
            while (loop_item != NULL) {
                gc_List_item *temp = loop_item;

                loop_item = loop_item->next;

                free(temp->ptr);
                free(temp);
            }
        }
    } else {
        fprintf(stderr, ">Neinicializoval jsi GC\n");
    }

    // odstran list
    free(GC);
}

// pridej polozku do seznamu
void gc_list_add(void *ptr) {

    if (GC) {
        // vytvor novou polozku
        // pridej polozku, bude zaroven prvni i posledni
        gc_List_item *new_item = (gc_List_item *) malloc(sizeof(gc_List_item));

        if (new_item == NULL)
        	gc_error();

        // pridej pointer
        new_item->ptr = ptr;
        new_item->next = NULL;

        // pokud byl list prazdny
        if (GC->empty) {
            // nastav hodnoty
            GC->first = new_item;
            GC->last = new_item;
            // seznam uz neni prazdny
            GC->empty = !(GC->empty);
        } else {
            GC->last->next = new_item;
            GC->last = new_item;
        }
    } else {
        fprintf(stderr, ">Neinicializoval jsi GC\n");
    }
}

gc_List_item * gc_list_find(void *ptr) {
    if (GC) {
        if (!(GC->empty)) {
            gc_List_item *loop_item = GC->first; // priprav si promennou na loopovani
            // projed vsechny polozky
            while (loop_item != NULL) {
                if (loop_item->ptr == ptr) {
                    return loop_item;
                }    
                // nastav dalsi cyklus
                loop_item = loop_item->next;
            }
        }
    } else {
        fprintf(stderr, ">Neinicializoval jsi GC\n");
    }
    return NULL;
}

void gc_list_find_modify(void *ptr, void *new_ptr) {
    // zde budeme volat list find

    gc_List_item *found_item = gc_list_find(ptr);

    if (found_item) {
        printf("Nasli jsme\n");

        // uloz kopii
        void * temp = found_item->ptr;
        // prepis
        found_item->ptr = new_ptr;
        // uvolni
        free(temp);
    }
}

void gc_list_delete(void *ptr) {
    if (GC) {
        // odstran jednu polozku
        if (!(GC->empty)) {
            // pokud je polozka sama
            if ((ptr == GC->first->ptr) && ptr == (GC->last->ptr)) {
                // odstranime pointer a polozku po uchovani, je jedno jestli first nebo last
                // oboje ukazuje tamtez
                free(GC->first->ptr);
                free(GC->first);

                // je to prazdne
                GC->empty = true;

                // nastavime polozky
                GC->first = NULL;
                GC->last = NULL;

            } else if (ptr == GC->first->ptr) {
                // zaloha prvni polozky
                gc_List_item *temp = GC->first;

                GC->first = GC->first->next;

                free(temp->ptr);
                free(temp);

            } else {
                // loopujeme dokud nenajdeme polozku, pokud posledni, nic 
                gc_List_item *prev_item = NULL;
                gc_List_item *loop_item = GC->first; // priprav si promennou na loopovani
                // projed vsechny polozky
                while (loop_item != NULL) {
                    if (loop_item->ptr == ptr) {
                        gc_List_item *temp = loop_item;
                        // premostime

                        if (loop_item->ptr == GC->last->ptr) { // pokud je to posledni polozka
                            prev_item->next = NULL;
                            GC->last = prev_item;
                        } else {
                            prev_item->next = loop_item->next;
                        }

                        free(temp->ptr);
                        free(temp);

                    }    
                    // nastav dalsi cyklus
                    prev_item = loop_item;
                    loop_item = loop_item->next;
                }
            }
        }
    } else {
        fprintf(stderr, ">Neinicializoval jsi GC\n");
    }
}

void gc_error() {
    // zavolej emergency cleanup
    gc_cleanup();
    // ukonci program
    dbg_crash_msg("Program Error\n");
    exit(UTL_MEM_ERR);
}

// vytiskni list
void gc_list_print() {
    gc_List_item *loop_item = GC->first; // priprav si promennou na loopovani
    // projed vsechny polozky
    while (loop_item != NULL) {
        // printuj hodnoty
        printf("| %p ",loop_item->ptr);     
        // nastav dalsi cyklus
        loop_item = loop_item->next;
    }
     // vyprintuj konec stacku
    printf("| $ |\n");
}

// samotne funkce pro praci s pameti
void *gc_new(size_t size) {
    // novy pointer
    void *new_pointer = malloc(size);
    // pokud selze malloc
    if (new_pointer == NULL)
        gc_error();

    gc_list_add(new_pointer);

    return new_pointer; 
}

void * gc_resize(void *ptr, size_t size) {
    // pointer existuje
    if (ptr != NULL) {
        void *temp_ptr = realloc(ptr, size);
        // pokud neni problem se zvetseni
        if (temp_ptr) {
            // musime upravit pointer v listu
            if (ptr != temp_ptr) {
                gc_list_find_modify(ptr, temp_ptr);
            } 
        } else {
            gc_error();
        }
    }
    return ptr;
}

void gc_delete(void *ptr) {
    // pointer existuje
    if (ptr != NULL)
        gc_list_delete(ptr);
}