/**
* Projekt IFJ 2017
* @brief Implementace fronty (variace seznamu) k semantice
* @Author Petr Kapoun, xkapou04
* @file sea_queue.c
*/

#include "sea_queue.h"
#include "token.h"

Queue_sea * sea_q_init() {
	Queue_sea *q = gc_new(sizeof(Queue_sea));
	q->q_head = NULL;
	q->q_tail = NULL;
	return q;
}

int sea_q_add(Queue_sea *queue, token *new_token, int time_stamp) {
	// pridam na konec
	sea_dbg_expr_msg("---> sea_q_add: Pridam\n");
	sea_q_t *tmp_tail = gc_new(sizeof(struct sea_q_item));
	sea_dbg_expr_msg("---> sea_q_add: Budu plnit hodnoty\n");
	tmp_tail->next = NULL;
	token* tmp_tok = gc_new(sizeof(token));
	tmp_tail->time_stamp = time_stamp;
	tmp_tail->tok = tmp_tok;
	tmp_tail->tok->data = new_token->data;
	tmp_tail->tok->data_length = new_token->data_length;
	tmp_tail->tok->type = new_token->type;
	sea_dbg_expr_msg("---> sea_q_add: Token plny\n");
	if (queue->q_head == NULL) {
		queue->q_tail = tmp_tail;
		queue->q_head = tmp_tail;
	}
	else {
		queue->q_tail->next = tmp_tail;
		queue->q_tail = tmp_tail;
	}
	return 0;
}

int sea_q_s_add(Queue_sea *queue, token *new_token, int time_stamp) {
	// pridam na konec
	sea_dbg_expr_msg("---> sea_q_s_add: Pridam\n");
	sea_q_t *tmp_head = gc_new(sizeof(struct sea_q_item));
	sea_dbg_expr_msg("---> sea_q_s_add: Budu plnit hodnoty\n");
	token* tmp_tok = gc_new(sizeof(token));
	tmp_head->time_stamp = time_stamp;
	tmp_head->tok = tmp_tok;
	tmp_head->tok->data = new_token->data;
	tmp_head->tok->data_length = new_token->data_length;
	tmp_head->tok->type = new_token->type;
	sea_dbg_expr_msg("---> sea_q_s_add: Token plny\n");
	if (queue->q_head == NULL) {
		tmp_head->next = NULL;
		queue->q_tail = tmp_head;
		queue->q_head = tmp_head;
	}
	else {
		tmp_head->next = queue->q_head;
		queue->q_head = tmp_head;
	}
	return 0;
}

token *sea_q_up(Queue_sea *queue) {
	return queue->q_head->tok;
}

int sea_q_up_stamp(Queue_sea *queue) {
	return queue->q_head->time_stamp;
}

bool sea_q_isempty(Queue_sea *queue) {
	if (queue->q_head == NULL) {
		return true;
	}
	return false;
}

void sea_q_delete(Queue_sea *queue) {
	// odstranim prvni
	if (queue->q_head == NULL) {
		return;
	}
	sea_q_t *tmp_head = queue->q_head;
	queue->q_head = queue->q_head->next;
	if (queue->q_head == NULL) {
		queue->q_tail = NULL;
	}
}

void sea_q_delete_all(Queue_sea *queue){
	while (queue->q_head != NULL) {
		sea_q_delete(queue);
	}
}
