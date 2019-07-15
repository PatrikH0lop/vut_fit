/** \addtogroup logic_model
 * logic_blocks.h : Definition of scheme blocks - logic interface
 * Scheme blocks are components, from which is the scheme build.
 * Each block has it's own execution function transforming input
 * map of values from ports to output
 * @author Patrik Holop
 * @author Matej Hrabal
 */


#ifndef ADDER_H
#define ADDER_H


#include "logic_base.h"
#include <limits>
#include <iostream>


// Arithmetic blocks

/**
 * Block representing numeric constant value
 * @brief The Constant class
 */
class Constant : public Block {
    public:
        /** Block constructor */
        Constant();
        /** Execution function : propagates values*/
        void update_value();
};

/**
 * Block representing result value
 * @brief The Result class
 */
class Result : public Block {
    public:
        /** Result block constructor */
        Result();
        /** Execution function : passive */
        void update_value();
};

/**
 * Block representing value adder
 * @brief The Adder class
 */
class Adder : public Block {
    public:
        /** Adder construnctor */
        Adder();
        /** Execution function : add 2 values */
        void update_value();
};

/**
 * Block representing value subtractor
 * @brief The Subtractor class
 */
class Subtractor : public Block {
    public:
        /** Subtractor constructor */
        Subtractor();
        /** Execution function : subtracts 2 values */
        void update_value();
};

/**
 * Block representing value multiplier
 * @brief The Multiplier class
 */
class Multiplier : public Block {
    public:
        /** Multiplier constructor */
        Multiplier();
        /** Execution function : multiplies 2 values */
        void update_value();
};

/**
 * Block representing value divider
 * @brief The Divider class
 */
class Divider : public Block {
    public:
        /** Divider construcor */
        Divider();
        /** Execution function : divides 2 values */
        void update_value();
};

// Power Plant management blocks

/**
 * Block representing Power Plant
 * Each power plant has risk probability,
 * reactor temperature and amount
 * of energy distributed per day.
 * @brief The Powerplant class
 */
class Powerplant : public Block {
    public:
        /** Power plant constructor */
        Powerplant();
        /** Execution function : propagates values */
        void update_value();
};

/**
 * Block representing reactor cooler
 * Cools the reactor by certain value
 * @brief The ReactorCooler class
 */
class ReactorCooler : public Block {
    public:
        /** Cooler constructor */
        ReactorCooler();
        /** Execution function : cools the reactor */
        void update_value();
};


/**
 * Better Plant blocks creates design of the
 * new powerplant with superior stats from the
 * input power plants.
 * @brief The BetterPlant class
 */
class BetterPlant : public Block {
    public:
        /** Better plant constructor */
        BetterPlant();
        /** Execution function : creates better power plant */
        void update_value();
};

/**
 * Block calculating common risk of input powerplants
 * @brief The CommonRisk class
 */
class CommonRisk : public Block {
    public:
        /** Common risk constructor */
        CommonRisk();
        /** Execution function : calculates common risk */
        void update_value();
};

/**
 * Result power plant used for storing power
 * plant desing as result
 * @brief The ResultPowerplant class
 */
class ResultPowerplant : public Block {
    public:
        /** Power plant result constructor */
        ResultPowerplant();
        /** Execution function : passive */
        void update_value();
};


// Market management blocks

/**
 * Block representing company on the market
 * Each company has stock value, number of stocks and
 * average salary of employees
 * @brief The Company class
 */
class Company : public Block {
    public:
        /** Company constructor */
        Company();
        /** Execution function : propagates values */
        void update_value();
};

/**
 * Block representing merging of the 2 companies
 * @brief The CompanyMerge class
 */
class CompanyMerge : public Block {
    public:
        /** Company merge constructor */
        CompanyMerge();
        /** Execution function : merges 2 companies */
        void update_value();
};

/**
 * This block evaluates the market cap of the company
 * @brief The Evaluator class
 */
class Evaluator : public Block {
    public:
        /** Evaluator constructor */
        Evaluator();
        /** Execution function : calculates value of the company */
        void update_value();
};

/**
 * This block is used for saving the company information
 * @brief The ResultCompany class
 */
class ResultCompany : public Block {
    public:
        /** Result company constructor */
        ResultCompany();
        /** Execution function : passive */
        void update_value();
};

#endif // ADDER_H
