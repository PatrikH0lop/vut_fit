/**
 * Implementation of logic blocks
 * @author Patrik Holop
 */


#include "logic_blocks.h"


/**
 * Initialization of 2-1 aithmetic blocks
 * @brief two_to_one_arithmetic_initializator
 * @param b Block to be initialized
 */
void two_to_one_arithmetic_initializator(Block *b) {
    // value is initialized to maximum (as null)
    double init_val = std::numeric_limits<double>::max();
    // add new ports
    b->inputPorts.push_back(new Port("PortA", b->local_id));
    b->inputPorts.push_back(new Port("PortB", b->local_id));
    b->outputPorts.push_back(new Port("PortC", b->local_id));
    // initials ports with values
    b->inputPorts[0]->values.insert(std::pair<std::string, double>("value", init_val));
    b->inputPorts[1]->values.insert(std::pair<std::string, double>("value", init_val));
    b->outputPorts[0]->values.insert(std::pair<std::string, double>("value", init_val));
}


/**
 * Calculates value of the arithmetic block
 * @brief arithmetic_update_value
 * @param b Block to be updated
 * @param operation Operation to be performed
 */
void arithmetic_update_value(Block *b, char operation) {
    // get values from ports
    auto p1 = b->inputPorts[0]->values.find("value");
    auto p2 = b->inputPorts[1]->values.find("value");
    // if the values are found
    if (p1 != b->inputPorts[0]->values.end() && p2 != b->inputPorts[0]->values.end()) {
        // perform execution
        double result = 0;
        switch (operation) {
            case '+':
                result = p1->second + p2->second;
                break;
            case '-':
                result = p1->second - p2->second;
                break;
            case '*':
                result = p1->second * p2->second;
                break;
            case '/':
                result = p1->second / p2->second;
                break;
        }
        // update value
        auto p3 = b->outputPorts[0]->values.find("value");
        p3->second = result;
        // propagate values through connections
        for (auto x: b->outputPorts[0]->connections) {
            if (x->inputPort != NULL && x->outputPort != NULL) {
                auto p = x->outputPort->values.find("value");
                p->second = result;
            }
        }
    }
}


// Arithmetic blocks

/**
 * Constructor
 * @brief Adder::Adder
 */
Adder::Adder() {
    this->local_id = Block::id;
    Block::id++;
    this->name = "Adder";
    // initialize block
    two_to_one_arithmetic_initializator(this);
}

/**
 * Execution function
 * @brief Adder::update_value
 */
void Adder::update_value() {
    arithmetic_update_value(this, '+');
}

/**
 * Constructor
 * @brief Subtractor::Subtractor
 */
Subtractor::Subtractor() {
    this->local_id = Block::id;
    Block::id++;
    this->name = "Subtractor";
    // initialize block
    two_to_one_arithmetic_initializator(this);
}

/**
 * Execution function
 * @brief Subtractor::update_value
 */
void Subtractor::update_value() {
    arithmetic_update_value(this, '-');
}

/**
 * Constructor
 * @brief Multiplier::Multiplier
 */
Multiplier::Multiplier() {
    this->local_id = Block::id;
    Block::id++;
    this->name = "Multiplier";
    // initialize block
    two_to_one_arithmetic_initializator(this);
}

/**
 * Execution function
 * @brief Multiplier::update_value
 */
void Multiplier::update_value() {
    arithmetic_update_value(this, '*');
}

/**
 * Constructor
 * @brief Divider::Divider
 */
Divider::Divider() {
    this->local_id = Block::id;
    Block::id++;
    this->name = "Divider";
    two_to_one_arithmetic_initializator(this);
}

/**
 * Execution function
 * @brief Divider::update_value
 */
void Divider::update_value() {
    arithmetic_update_value(this, '/');
}

/**
 * Constructor
 * @brief Constant::Constant
 */
Constant::Constant() {
    double init_val = std::numeric_limits<double>::max();
    this->local_id = Block::id;
    Block::id++;
    this->name = "Constant";
    this->outputPorts.push_back(new Port("PortC", this->local_id));
    this->outputPorts[0]->values.insert(std::pair<std::string, double>("value", init_val));
}

/**
 * Execution function
 * @brief Constant::update_value
 */
void Constant::update_value() {
    // propagate values through connections
    double value = this->outputPorts[0]->values.find("value")->second;
    for (auto x: this->outputPorts[0]->connections) {
        if (x->inputPort != NULL && x->outputPort != NULL) {
            auto p = x->outputPort->values.find("value");
            p->second = value;
        }
    }
}

/**
 * Construct
 * @brief Result::Result
 */
Result::Result() {
    // initialize block
    double init_val = std::numeric_limits<double>::max();
    this->local_id = Block::id;
    Block::id++;
    this->name = "Result";
    this->inputPorts.push_back(new Port("PortC", this->local_id));
    this->inputPorts[0]->values.insert(std::pair<std::string, double>("value", init_val));
}

/**
 * Passive execution function
 * @brief Result::update_value
 */
void Result::update_value() {
    // wait for updating
}


// Power Plant blocks

/**
 * Power plant constructor
 * @brief Powerplant::Powerplant
 */
Powerplant::Powerplant() {
    double init_val = std::numeric_limits<double>::max();
    this->local_id = Block::id;
    Block::id++;
    this->name = "PowerPlant";
    // add new ports
    this->outputPorts.push_back(new Port("PortC", this->local_id));
    this->outputPorts[0]->values.insert(std::pair<std::string, double>("risk_probability", init_val));
    this->outputPorts[0]->values.insert(std::pair<std::string, double>("reactor_temperature", init_val));
    this->outputPorts[0]->values.insert(std::pair<std::string, double>("distributed_energy", init_val));
}

/**
 * Execution function
 * @brief Powerplant::update_value
 */
void Powerplant::update_value() {
    // get input power plant values
    double risk_prob = this->outputPorts[0]->values.find("risk_probability")->second;
    double temperature = this->outputPorts[0]->values.find("reactor_temperature")->second;
    double energy = this->outputPorts[0]->values.find("distributed_energy")->second;
    // propagate them through connections
    for (auto x: this->outputPorts[0]->connections) {
        if (x->inputPort != NULL && x->outputPort != NULL) {
            auto rp = x->outputPort->values.find("risk_probability");
            auto t = x->outputPort->values.find("reactor_temperature");
            auto e = x->outputPort->values.find("distributed_energy");
            rp->second = risk_prob;
            t->second = temperature;
            e->second = energy;
        }
    }
}


/**
 * Constructor
 * @brief BetterPlant::BetterPlant
 */
BetterPlant::BetterPlant() {
    double init_val = std::numeric_limits<double>::max();
    this->local_id = Block::id;
    Block::id++;
    this->name = "BetterPlant";
    // add new ports
    this->inputPorts.push_back(new Port("PortA", this->local_id));
    this->inputPorts.push_back(new Port("PortB", this->local_id));
    this->outputPorts.push_back(new Port("PortC", this->local_id));
    // initialize them
    this->inputPorts[0]->values.insert(std::pair<std::string, double>("risk_probability", init_val));
    this->inputPorts[0]->values.insert(std::pair<std::string, double>("reactor_temperature", init_val));
    this->inputPorts[0]->values.insert(std::pair<std::string, double>("distributed_energy", init_val));

    this->inputPorts[1]->values.insert(std::pair<std::string, double>("risk_probability", init_val));
    this->inputPorts[1]->values.insert(std::pair<std::string, double>("reactor_temperature", init_val));
    this->inputPorts[1]->values.insert(std::pair<std::string, double>("distributed_energy", init_val));

    this->outputPorts[0]->values.insert(std::pair<std::string, double>("risk_probability", init_val));
    this->outputPorts[0]->values.insert(std::pair<std::string, double>("reactor_temperature", init_val));
    this->outputPorts[0]->values.insert(std::pair<std::string, double>("distributed_energy", init_val));
}

/**
 * Execution function
 * @brief BetterPlant::update_value
 */
void BetterPlant::update_value() {
    // get input values from input powerplants
    auto r1 = this->inputPorts[0]->values.find("risk_probability");
    auto r2 = this->inputPorts[1]->values.find("risk_probability");
    auto e1 = this->inputPorts[0]->values.find("distributed_energy");
    auto e2 = this->inputPorts[1]->values.find("distributed_energy");
    auto t1 = this->inputPorts[0]->values.find("reactor_temperature");
    auto t2 = this->inputPorts[1]->values.find("reactor_temperature");
    if (r1 != this->inputPorts[0]->values.end() && r2 != this->inputPorts[1]->values.end()) {
        // calculate better statistics
        double r_min = std::min(r1->second, r2->second);
        double e_max = std::max(e1->second, e2->second);
        double t_min = std::min(t1->second, t2->second);        
        auto res_r = this->outputPorts[0]->values.find("risk_probability");
        auto res_t = this->outputPorts[0]->values.find("reactor_temperature");
        auto res_e = this->outputPorts[0]->values.find("distributed_energy");
        res_r->second = r_min;
        res_t->second = t_min;
        res_e->second = e_max;
        // propagate them through connections
        for (auto x: this->outputPorts[0]->connections) {
            if (x->inputPort != NULL && x->outputPort != NULL) {
                auto r = x->outputPort->values.find("risk_probability");
                auto t = x->outputPort->values.find("reactor_temperature");
                auto e = x->outputPort->values.find("distributed_energy");
                r->second = r_min;
                t->second = t_min;
                e->second = e_max;
            }
        }
    }
}

/**
 * Constructor
 * @brief ReactorCooler::ReactorCooler
 */
ReactorCooler::ReactorCooler() {
    double init_val = std::numeric_limits<double>::max();
    this->local_id = Block::id;
    Block::id++;
    this->name = "ReactorCooler";
    // add new ports
    this->inputPorts.push_back(new Port("PortA", this->local_id));
    this->inputPorts.push_back(new Port("PortB", this->local_id));
    this->outputPorts.push_back(new Port("PortC", this->local_id));
    // initialize theme with values
    this->inputPorts[0]->values.insert(std::pair<std::string, double>("risk_probability", init_val));
    this->inputPorts[0]->values.insert(std::pair<std::string, double>("reactor_temperature", init_val));
    this->inputPorts[0]->values.insert(std::pair<std::string, double>("distributed_energy", init_val));

    this->inputPorts[1]->values.insert(std::pair<std::string, double>("value", init_val));

    this->outputPorts[0]->values.insert(std::pair<std::string, double>("risk_probability", init_val));
    this->outputPorts[0]->values.insert(std::pair<std::string, double>("reactor_temperature", init_val));
    this->outputPorts[0]->values.insert(std::pair<std::string, double>("distributed_energy", init_val));
}

/**
 * Execution function
 * @brief ReactorCooler::update_value
 */
void ReactorCooler::update_value() {
    // get powerplant and value
    auto r1 = this->inputPorts[0]->values.find("risk_probability");
    auto t1 = this->inputPorts[0]->values.find("reactor_temperature");
    auto e1 = this->inputPorts[0]->values.find("distributed_energy");
    auto value = this->inputPorts[1]->values.find("value");
    if (r1 != this->inputPorts[0]->values.end() && value != this->inputPorts[1]->values.end()) {
        // cool the reactor
        auto res_r = this->outputPorts[0]->values.find("risk_probability");
        auto res_t = this->outputPorts[0]->values.find("reactor_temperature");
        auto res_e = this->outputPorts[0]->values.find("distributed_energy");
        res_r->second = r1->second;
        res_t->second = t1->second - value->second;
        res_e->second = e1->second;
        // propagate values through connections
        for (auto x: this->outputPorts[0]->connections) {
            if (x->inputPort != NULL && x->outputPort != NULL) {
                auto r = x->outputPort->values.find("risk_probability");
                auto t = x->outputPort->values.find("reactor_temperature");
                auto e = x->outputPort->values.find("distributed_energy");
                r->second = r1->second;
                t->second = t1->second - value->second;
                e->second = e1->second;
            }
        }
    }
}

/**
 * Constructor
 * @brief CommonRisk::CommonRisk
 */
CommonRisk::CommonRisk() {
    double init_val = std::numeric_limits<double>::max();
    this->local_id = Block::id;
    Block::id++;
    this->name = "CommonRisk";
    // add new ports
    this->inputPorts.push_back(new Port("PortA", this->local_id));
    this->inputPorts.push_back(new Port("PortB", this->local_id));
    this->outputPorts.push_back(new Port("PortC", this->local_id));
    // initialize them with values
    this->inputPorts[0]->values.insert(std::pair<std::string, double>("risk_probability", init_val));
    this->inputPorts[0]->values.insert(std::pair<std::string, double>("reactor_temperature", init_val));
    this->inputPorts[0]->values.insert(std::pair<std::string, double>("distributed_energy", init_val));

    this->inputPorts[1]->values.insert(std::pair<std::string, double>("risk_probability", init_val));
    this->inputPorts[1]->values.insert(std::pair<std::string, double>("reactor_temperature", init_val));
    this->inputPorts[1]->values.insert(std::pair<std::string, double>("distributed_energy", init_val));

    this->outputPorts[0]->values.insert(std::pair<std::string, double>("value", init_val));
}

/**
 * Execution function
 * @brief CommonRisk::update_value
 */
void CommonRisk::update_value() {
    // get input powerplants
    auto r1 = this->inputPorts[0]->values.find("risk_probability");
    auto r2 = this->inputPorts[1]->values.find("risk_probability");
    if (r1 != this->inputPorts[0]->values.end() && r2 != this->inputPorts[1]->values.end()) {
        // calculate common risk
        auto res_r = this->outputPorts[0]->values.find("value");
        res_r->second = std::max(std::min(1 - (1-r1->second) * (1-r2->second), 1.0), 0.0);
        // propage value through connections
        for (auto x: this->outputPorts[0]->connections) {
            if (x->inputPort != NULL && x->outputPort != NULL) {
                auto r = x->outputPort->values.find("value");
                r->second = r1->second + r2->second;
            }
        }
    }
}

/**
 * Constructor
 * @brief ResultPowerplant::ResultPowerplant
 */
ResultPowerplant::ResultPowerplant() {
    double init_val = std::numeric_limits<double>::max();
    this->local_id = Block::id;
    Block::id++;
    this->name = "PowerPlantResult";
    // add new port
    this->inputPorts.push_back(new Port("PortC", this->local_id));
    // initialize it with values
    this->inputPorts[0]->values.insert(std::pair<std::string, double>("risk_probability", init_val));
    this->inputPorts[0]->values.insert(std::pair<std::string, double>("reactor_temperature", init_val));
    this->inputPorts[0]->values.insert(std::pair<std::string, double>("distributed_energy", init_val));
}

/**
 * Passive execution function
 * @brief ResultPowerplant::update_value
 */
void ResultPowerplant::update_value() {
    // pass
}


// Market management blocks

/**
 * Constructor
 * @brief Company::Company
 */
Company::Company() {
    double init_val = std::numeric_limits<double>::max();
    this->local_id = Block::id;
    Block::id++;
    this->name = "Company";
    // add port
    this->outputPorts.push_back(new Port("PortC", this->local_id));
    // initialize ports with values
    this->outputPorts[0]->values.insert(std::pair<std::string, double>("stock_number", init_val));
    this->outputPorts[0]->values.insert(std::pair<std::string, double>("stock_value", init_val));
    this->outputPorts[0]->values.insert(std::pair<std::string, double>("average_salary", init_val));
}

/**
 * Execution function
 * @brief Company::update_value
 */
void Company::update_value() {
    // propagate values through connections
    double stock_num = this->outputPorts[0]->values.find("stock_number")->second;
    double stock_val = this->outputPorts[0]->values.find("stock_value")->second;
    double salary = this->outputPorts[0]->values.find("average_salary")->second;
    for (auto x: this->outputPorts[0]->connections) {
        if (x->inputPort != NULL && x->outputPort != NULL) {
            auto sn = x->outputPort->values.find("stock_number");
            auto sv = x->outputPort->values.find("stock_value");
            auto as = x->outputPort->values.find("average_salary");
            sn->second = stock_num;
            sv->second = stock_val;
            as->second = salary;
        }
    }
}

/**
 * Constructor
 * @brief CompanyMerge::CompanyMerge
 */
CompanyMerge::CompanyMerge() {
    double init_val = std::numeric_limits<double>::max();

    this->local_id = Block::id;
    Block::id++;
    this->name = "CompanyMerge";
    // add new values
    this->inputPorts.push_back(new Port("PortA", this->local_id));
    this->inputPorts.push_back(new Port("PortB", this->local_id));
    this->outputPorts.push_back(new Port("PortC", this->local_id));
    // initialize with init values
    this->inputPorts[0]->values.insert(std::pair<std::string, double>("stock_number", init_val));
    this->inputPorts[0]->values.insert(std::pair<std::string, double>("stock_value", init_val));
    this->inputPorts[0]->values.insert(std::pair<std::string, double>("average_salary", init_val));

    this->inputPorts[1]->values.insert(std::pair<std::string, double>("stock_number", init_val));
    this->inputPorts[1]->values.insert(std::pair<std::string, double>("stock_value", init_val));
    this->inputPorts[1]->values.insert(std::pair<std::string, double>("average_salary", init_val));

    this->outputPorts[0]->values.insert(std::pair<std::string, double>("stock_number", init_val));
    this->outputPorts[0]->values.insert(std::pair<std::string, double>("stock_value", init_val));
    this->outputPorts[0]->values.insert(std::pair<std::string, double>("average_salary", init_val));
}

/**
 * Execution function
 * @brief CompanyMerge::update_value
 */
void CompanyMerge::update_value() {
    // get input companies
    auto sv1 = this->inputPorts[0]->values.find("stock_value");
    auto sv2 = this->inputPorts[1]->values.find("stock_value");
    auto sn1 = this->inputPorts[0]->values.find("stock_number");
    auto sn2 = this->inputPorts[1]->values.find("stock_number");
    auto as1 = this->inputPorts[0]->values.find("average_salary");
    auto as2 = this->inputPorts[1]->values.find("average_salary");
    if (sv1 != this->inputPorts[0]->values.end() && sv2 != this->inputPorts[1]->values.end()) {
        // calculate average values after companies merging
        double sv_new = (sv1->second + sv2->second) / 2;
        double sn_new = sn1->second + sn2->second;
        double as_new = (as1->second + as2->second) / 2;
        auto res_sv = this->outputPorts[0]->values.find("stock_value");
        auto res_sn = this->outputPorts[0]->values.find("stock_number");
        auto res_as = this->outputPorts[0]->values.find("average_salary");
        res_sv->second = sv_new;
        res_sn->second = sn_new;
        res_as->second = as_new;
        // propagate result through conenctions
        for (auto x: this->outputPorts[0]->connections) {
            if (x->inputPort != NULL && x->outputPort != NULL) {
                auto r = x->outputPort->values.find("stock_value");
                auto t = x->outputPort->values.find("stock_number");
                auto e = x->outputPort->values.find("average_salary");
                r->second = sv_new;
                t->second = sn_new;
                e->second = as_new;
            }
        }
    }
}

/**
 * Constructor
 * @brief Evaluator::Evaluator
 */
Evaluator::Evaluator() {
    double init_val = std::numeric_limits<double>::max();
    this->local_id = Block::id;
    Block::id++;
    this->name = "Evaluator";
    // add new port
    this->inputPorts.push_back(new Port("PortA", this->local_id));
    this->outputPorts.push_back(new Port("PortC", this->local_id));
    // initiale ports
    this->inputPorts[0]->values.insert(std::pair<std::string, double>("stock_value", init_val));
    this->inputPorts[0]->values.insert(std::pair<std::string, double>("stock_number", init_val));
    this->inputPorts[0]->values.insert(std::pair<std::string, double>("average_salary", init_val));

    this->outputPorts[0]->values.insert(std::pair<std::string, double>("value", 0));
}

/**
 * Execution function
 * @brief Evaluator::update_value
 */
void Evaluator::update_value() {
    // get input company
    auto sv = this->inputPorts[0]->values.find("stock_value");
    auto sn = this->inputPorts[0]->values.find("stock_number");
    if (sv != this->inputPorts[0]->values.end()) {
        // calculate it's market value
        auto val = this->outputPorts[0]->values.find("value");
        val->second = sv->second * sn->second;
        // propagate result through connections
        for (auto x: this->outputPorts[0]->connections) {
            if (x->inputPort != NULL && x->outputPort != NULL) {
                auto r = x->outputPort->values.find("value");
                r->second = val->second;
            }
        }
    }
}

/**
 * Constructor
 * @brief ResultCompany::ResultCompany
 */
ResultCompany::ResultCompany() {
    double init_val = std::numeric_limits<double>::max();
    this->local_id = Block::id;
    Block::id++;
    this->name = "ResultCompany";
    // add and initialize ports
    this->inputPorts.push_back(new Port("PortC", this->local_id));
    this->inputPorts[0]->values.insert(std::pair<std::string, double>("stock_number", init_val));
    this->inputPorts[0]->values.insert(std::pair<std::string, double>("stock_value", init_val));
    this->inputPorts[0]->values.insert(std::pair<std::string, double>("average_salary", init_val));
}

/**
 * Passive execution function
 * @brief ResultCompany::update_value
 */
void ResultCompany::update_value() {
    // pass
}
