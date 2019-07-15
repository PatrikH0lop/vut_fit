/**
 * Implementation of the logic simulation
 * @author Patrik Holop
 */


#include "simulation.h"


/**
 * Check, whether current block is connected to any cycle using DFS
 * @brief Simulation::check_cycles_block
 * @param block_ip block to be checked
 * @param path Current path (for backtracking)
 * @return True is block is connected to cycle, otherwise false
 */
bool Simulation::check_cycles_block(int block_ip, std::vector<int> path) {
    std::cout << "Considering: " << block_ip << ", PATH: "; for (int id: path) std::cout << id; std::cout << std::endl;
    // find the correct block
    Block *b = NULL;
    for (Block *bl : this->blocks) {
        if (block_ip == bl->local_id)
            b = bl;
    }
    // for each connection of the block
    for (Port *p : b->outputPorts) {
        for (Connection *c : p->connections) {
            // if the other block is in already taken path, it's a cycle
            bool find = false;
            for (int id : path) {
                if (id == c->outputPort->loc) {
                    find = true;
                }
            }
            if (find) {
                std::cout << "CYCLE" << std::endl;
                return true;
            }
            path.push_back(c->outputPort->loc);
            // otherwise add it to the path and check the next block
            bool sub_cycle = check_cycles_block(c->outputPort->loc, path);
            if (sub_cycle)
                return true;
            path.pop_back();
        }
    }
    std::cout << "no cycle" << std::endl;
    return false;
}


/**
 * Checks, whether there's at least one cycle in the current scheme
 * @brief Simulation::check_cycles
 * @return True if the scheme contains cycles, otherwise false
 */
bool Simulation::check_cycles() {
    std::vector<int> path;
    // neither of the bloks can contain cycle to be the valid scheme
    for (Block *b : this->blocks) {
        std::cout << "Check cycles for: " << b->local_id << std::endl;
        if (check_cycles_block(b->local_id, path))
            return true;
    }
    return false;
}


/**
 * Sort the block in correct execution order to correctly update the whole scheme.
 * @brief Simulation::sort_blocks
 */
void Simulation::sort_blocks() {
    // valid sorted order
    std::vector<int> ids;
    while (ids.size() != this->blocks.size()) {
        for (Block *b: this->blocks) {
            // propagate all blocks that have no input values
            if (b->inputPorts.size() == 0) {
                bool find = false;
                for (int id : ids) {
                    if (id == b->local_id) {
                        find = true;
                    }
                }
                if (!find) {
                    ids.push_back(b->local_id);
                }
            } else {
                // to add the block, check whether all of it's ancestors are
                // already updated
                bool add = true;                
                std::cout << "CHECKING: " << b->local_id << std::endl;
                for (Port *p: b->inputPorts) {
                    std::cout << "connections to check: " << p->connections.size() << std::endl;
                    for (Connection *c: p->connections) {
                        bool find = false;
                        for (int id : ids) {
                            std::cout << "COMPARE: " << id << " == " << c->inputPort->loc << std::endl;
                            if (id == c->inputPort->loc)
                                find = true;
                        }
                        if (!find) {
                            add = false;
                        } else {
                            std::cout << "CONNECTION OK" << std::endl;
                        }
                    }
                }
                if (add) {
                    std::cout << "ADDING: " << b->local_id << std::endl;
                    bool find = false;
                    for (int id : ids) {
                        if (id == b->local_id)
                            find = true;
                    }
                    if (!find) {
                        ids.push_back(b->local_id);
                    } else {
                        std::cout << "FOUND";
                    }
                }
            }
        }
        for (auto b: ids) {
            std::cout << b << " ";
        }
        std::cout << "IDS: " << ids.size() << " BLOCKS: " << this->blocks.size() << std::endl;
    }

    for (auto f: ids) {
        std::cout << "F: " << f << std::endl;
    }
    // generate new logic block array from the id vector
    std::vector <Block *> new_main_block_array;
    for (float id: ids) {
        Block *to_add = NULL;
        for (Block *b: this->blocks) {
            if (b->local_id == id) {
                to_add = b;
            }
        }
        new_main_block_array.push_back(to_add);
    }
    std::cout << "ORDER" << std::endl;
    for (Block *b : new_main_block_array) {
        std::cout << b->local_id << ", ";
    }
    std::cout<< "\nEND ORDER" << std::endl;
    this->blocks = new_main_block_array;
}
