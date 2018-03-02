/**
 * Project: IFJ17
 * @brief Hlavickovy soubor pro frontu semantickeho analyzatoru
 * Pro neomezenost je fronta udelana jako seznam, ktery ma nad sebou
 * funkce pro abstrakci tak, aby se choval jako fronta
 * @Author PetrKapoun, xkapou04
 * @file sea_queue.h
 */


#ifndef IFJ17_FIT_2017_SEM_QUEUE_H
#define IFJ17_FIT_2017_SEM_QUEUE_H

#include <stdlib.h>
#include <stdbool.h>
#include "token.h"
#include "utl_err.h"
#include "utl_debug.h"
#include "utl_mem.h"

/**
 *Struktura pravidla
 */
typedef struct sea_q_item {
	token *tok; // token
	int time_stamp;
	struct sea_q_item *next; // ukazatel na dalsi prvek seznamu
} sea_q_t;

typedef struct Queue_sea {
	sea_q_t *q_head;
	sea_q_t *q_tail;
} Queue_sea;

/**
 * Inicializuje frontu, nutne pred prvnim pouzitim.
 */
Queue_sea * sea_q_init();

/**
* Prida prvek na konec fronty
* @param samotna fronta
* @param new_token token, ktery ma byt pridany
* @param time_stamp casova znamka pro funkce
* @return 0 - uspech, 1 - chyba alokace
*/
int sea_q_add(Queue_sea *queue, token *new_token, int time_stamp);

/**
* Prida prvek na zacatek fronty - udela z ni zasobnik
* @param samotna fronta
* @param new_token token, ktery ma byt pridany
* @param time_stamp casova znamka pro funkce
* @return 0 - uspech, 1 - chyba alokace
*/
int sea_q_s_add(Queue_sea *queue, token *new_token, int time_stamp);

/**
* Koukne na vrh fronty a vrati ukazatel na jeji vrchni prvek(token)
* @param samotna fronta
* @return token z vrchu fronty
*/
token *sea_q_up(Queue_sea *queue);

/**
* Koukne na vrh fronty a vrati time_stamp prvni polozky
* @param samotna fronta
* @return vrati time_stamp prvni polozky
*/
int sea_q_up_stamp(Queue_sea *queue);

/**
* Zjisti jestli je fronta prazdna
* @param samotna fronta
* @return true - fronta je prazdna, false - neco je ve fronte
*/
bool sea_q_isempty(Queue_sea *queue);

/**
* Smaze vrchni prvek fronty
* @param samotna fronta
*/
void sea_q_delete(Queue_sea *queue);

/**
* Smaze vsechny prvky fronty
* @param samotna fronta
*/
void sea_q_delete_all(Queue_sea *queue);

#endif
