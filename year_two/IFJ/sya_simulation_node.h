/**
 * Projekt IFJ 2017
 * @brief Rozhrani pro komunikaci mezi syntaktickym a lexikalnim analyzatorem
 * @Author HanakJiri, xhanak33
 * @file sya_simulation_node.h
 */

#ifndef IFJ17_FIT_2017_SYA_SIMULATION_NODE_H
#define IFJ17_FIT_2017_SYA_SIMULATION_NODE_H

#import "sya_grammar.h"
#import "sya_terminal.h"

typedef struct {
    Rule rule;
    Terminal terminal;
} Sym_node;

#endif //IFJ17_FIT_2017_SYA_SIMULATION_NODE_H
