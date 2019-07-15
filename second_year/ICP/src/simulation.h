/** \addtogroup logic_model
 * simulation.h module provides the basic simulation logic model responsible
 * for storing and validying current scheme model. It's opposite
 * to graphic interface Scheme.
 * @author Patrik Holop
 */


#ifndef SIMULATION_H
#define SIMULATION_H


#include <vector>
#include <algorithm>
#include <iostream>
#include "logic_base.h"


/**
 * Simulation class represent logic model of the applications.
 * Provides method to check the validity of the scheme
 * @brief The Simulation class
 */
class Simulation
{
    public:
        /** Logic blocks in the current simulation */
        std::vector<Block*> blocks;
        /** Logic connections in the current simulation */
        std::vector<Connection*> connections;

        /** Sorts the blocks of simulation in correct order before the simulation is run */
        void sort_blocks();
        /** Checks, whether current block is conencted to cycle */
        bool check_cycles_block(int block_id, std::vector<int> path);
        /** Checks, whether current scheme contains cycles */
        bool check_cycles();
};

#endif // SIMULATION_H
