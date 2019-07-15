/**
 * Projekt IFJ 2017
 * Rozhrani pro chybove stavy
 * @brief Chybove stavy
 * @Author Matej Mitas, xmitas02
 * @Author HanakJiri, xhanak33
 * @file utl_err.h
 */

#ifndef IFJ17_FIT_2017_UTL_ERR_H
#define IFJ17_FIT_2017_UTL_ERR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdint.h>
#include "utl_mem.h"

#define PRG_OK 			            0
#define LEA_ERR 		            1
#define SYA_ERR 		            2
#define SEA_PRG_ERR  	            3
#define SEA_TYPE_ERR	            4
#define SEA_OTHER_ERR	            6
#define GVK_LABEL_ERR	            52
#define GVK_INVALID_OPERAND_ERR	    53
#define GVK_NO_EXIST_OPERAND_ERR	54
#define GVK_NO_DEFINED_FRAME_ERR	55
#define GVK_NO_INIT_OPERAND_ERR	    56
#define GVK_DIVIDE_BY_ZERO_ERR	    57
#define GVK_VALUE_ERR	            58
#define UTL_MEM_ERR		            99
#define INNER_ERROR                 100

/**
 * @brief Korektne ukonci program (at uz s chybou nebo bez)
 * @param exit_code Kod, s jakym ma skoncit program
 */
void utl_exit(int exit_code);

#endif