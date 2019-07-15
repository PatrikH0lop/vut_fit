/**
 * Implementation of logic - graphic interface
 * @author Patrik Holop
 * @author Matej Hrabal
 */


#include "logic_gui_interface.h"


/**
 * Constructor of the interface
 * @brief Logic_gui_interface::Logic_gui_interface
 */
Logic_gui_interface::Logic_gui_interface() {
    // create logic model
    this->simulation = new Simulation();
    // create graphic model
    this->scheme = new Scheme(this);
    this->to_update = 0;
    this->active_circle = NULL;
    this->count = 0;

    //graphics update job
    QTimer *timer_scene = new QTimer;
    timer_scene->setSingleShot(false);
    connect(timer_scene, SIGNAL(timeout()), this, SLOT(clear_scene()));
    timer_scene->start(10); 
}


/**
 * Help function for graphic block generation
 * @brief generate_arithmetic_gui_block
 * @param block Block to be generated
 * @param id id of the block
 */
void generate_arithmetic_gui_block(GUI_Block *block, int id) {
    GUI_Port *a = new GUI_Port(id, "PortA");
    GUI_Port *b = new GUI_Port(id, "PortB");
    block->inputPorts.push_back(a);
    block->inputPorts.push_back(b);
    a->setParentItem(block);
    b->setParentItem(block);
    a->setX(a->x() - 10);
    b->setX(b->x() - 10);
    b->setY(b->y() + 30);
    GUI_Port *c = new GUI_Port(id, "PortC");
    block->outputPorts.push_back(c);
    c->setParentItem(block);
    c->setY(c->y() + 15);
    c->setX(c->x() + 45);
}

/**
 * This function is used when creating new block
 * @brief Logic_gui_interface::add_block
 * @param type Type of the generated block
 * @param load True is block is generated from save file
 * @param local_id id of the block
 * @param x X coordinate of the block
 * @param y Y coordinate of the block
 * @param values Initial values the block
 */
void Logic_gui_interface::add_block(std::string type, bool load, int local_id, int x, int y, std::map<std::string, double> values) {
    if (type == "adder") {
        this->simulation->blocks.push_back(new Adder()); // logic
        int id = this->simulation->blocks.back()->local_id;
        if (load)
            this->reset_id(this->simulation->blocks.back(), local_id);
        else
            local_id = id;
        GUI_Adder *a1 = new GUI_Adder(local_id, this); // gui
        a1->setX(x);
        a1->setY(y);
        this->scheme->canvas->addItem(a1);
        generate_arithmetic_gui_block(a1, local_id);
    } else if (type == "subtractor") {
        this->simulation->blocks.push_back(new Subtractor()); // logic
        int id = this->simulation->blocks.back()->local_id;
        if (load)
            this->reset_id(this->simulation->blocks.back(), local_id);
        else
            local_id = id;
        GUI_Subtractor *a1 = new GUI_Subtractor(local_id, this); // gui
        a1->setX(x);
        a1->setY(y);
        this->scheme->canvas->addItem(a1);
        generate_arithmetic_gui_block(a1, local_id);
    } else if (type == "multiplier") {
        this->simulation->blocks.push_back(new Multiplier()); // logic
        int id = this->simulation->blocks.back()->local_id;
        if (load)
            this->reset_id(this->simulation->blocks.back(), local_id);
        else
            local_id = id;
        GUI_Multiplier *a1 = new GUI_Multiplier(local_id, this); // gui
        a1->setX(x);
        a1->setY(y);
        this->scheme->canvas->addItem(a1);
        generate_arithmetic_gui_block(a1, local_id);
    } else if (type == "divider") {
        this->simulation->blocks.push_back(new Divider()); // logic
        int id = this->simulation->blocks.back()->local_id;
        if (load)
            this->reset_id(this->simulation->blocks.back(), local_id);
        else
            local_id = id;
        GUI_Divider *a1 = new GUI_Divider(local_id, this); // gui
        a1->setX(x);
        a1->setY(y);
        this->scheme->canvas->addItem(a1);
        generate_arithmetic_gui_block(a1, local_id);
    } else if (type == "result") {
        this->simulation->blocks.push_back(new Result()); // logic
        int id = this->simulation->blocks.back()->local_id;
        if (load)
            this->reset_id(this->simulation->blocks.back(), local_id);
        else
            local_id = id;
        GUI_Result *a1 = new GUI_Result(local_id, this); // gui
        a1->setX(x);
        a1->setY(y);
        this->scheme->canvas->addItem(a1);
        GUI_Port *c = new GUI_Port(local_id, "PortC");
        a1->inputPorts.push_back(c);
        c->setParentItem(a1);
        c->setY(c->y() + 15);
        c->setX(c->x() - 10);
    } else if (type == "var") {
        this->simulation->blocks.push_back(new Constant()); // logic
        int id = this->simulation->blocks.back()->local_id;
        if (load)
            this->reset_id(this->simulation->blocks.back(), local_id);
        else
            local_id = id;
        GUI_Constant *a1 = new GUI_Constant(local_id, this); // gui
        this->scheme->canvas->addItem(a1);
        a1->setX(x);
        a1->setY(y);
        GUI_Port *c = new GUI_Port(local_id, "PortC");
        a1->outputPorts.push_back(c);
        c->setParentItem(a1);
        c->setY(c->y() + 15);
        c->setX(c->x() + 45);
        // get value
        if (!load) {
            bool dialogResult;
            QInputDialog *renameDialog = new QInputDialog();
            QString result = renameDialog->getText(0, "Constant value", "New value:", QLineEdit::Normal, "", &dialogResult);
            double value = result.toDouble();
            this->set_model_value(local_id, "PortC", "value", value);
        } else {
            for (std::pair<std::string, double> p: values) {
                this->set_model_value(local_id, "PortC", p.first, p.second);
            }
        }
        a1->update_tooltip();
    } else if (type == "powerplant") {
        this->simulation->blocks.push_back(new Powerplant()); // local_id
        int id = this->simulation->blocks.back()->local_id;
        if (load)
            this->reset_id(this->simulation->blocks.back(), local_id);
        else
            local_id = id;
        GUI_Powerplant *a1 = new GUI_Powerplant(local_id, this); // gui
        this->scheme->canvas->addItem(a1);
        a1->setX(x);
        a1->setY(y);
        GUI_Port *c = new GUI_Port(local_id, "PortC");
        a1->outputPorts.push_back(c);
        c->setParentItem(a1);
        c->setY(c->y() + 15);
        c->setX(c->x() + 45);
        // get values
        if (!load) {
            bool dialogResult;
            QInputDialog *renameDialog = new QInputDialog();
            QString result = renameDialog->getText(0, "Powerplant", "Powerplant risk probability:", QLineEdit::Normal, "", &dialogResult);
            double value = std::max(std::min(result.toDouble(), 1.0), 0.0);
            this->set_model_value(local_id, "PortC", "risk_probability", value);

            result = renameDialog->getText(0, "Powerplant", "Powerplant reactor temperature:", QLineEdit::Normal, "", &dialogResult);
            value = result.toDouble();
            this->set_model_value(local_id, "PortC", "reactor_temperature", value);

            result = renameDialog->getText(0, "Powerplant", "Powerplant energy per day:", QLineEdit::Normal, "", &dialogResult);
            value = result.toDouble();
            this->set_model_value(local_id, "PortC", "distributed_energy", value);
        } else {
            for (std::pair<std::string, double> p: values) {
                 this->set_model_value(local_id, "PortC", p.first, p.second);
            }
        }
        a1->update_tooltip();
    } else if (type == "cooler") {
        this->simulation->blocks.push_back(new ReactorCooler()); // logic
        int id = this->simulation->blocks.back()->local_id;
        if (load)
            this->reset_id(this->simulation->blocks.back(), local_id);
        else
            local_id = id;
        GUI_ReactorCooler *a1 = new GUI_ReactorCooler(local_id, this); // gui
        a1->setX(x);
        a1->setY(y);
        this->scheme->canvas->addItem(a1);
        generate_arithmetic_gui_block(a1, local_id);
    } else if (type == "commonrisk") {
        this->simulation->blocks.push_back(new CommonRisk()); // logic
        int id = this->simulation->blocks.back()->local_id;
        if (load)
            this->reset_id(this->simulation->blocks.back(), local_id);
        else
            local_id = id;
        GUI_CommonRisk *a1 = new GUI_CommonRisk(local_id, this); // gui
        this->scheme->canvas->addItem(a1);        
        a1->setX(x);
        a1->setY(y);
        generate_arithmetic_gui_block(a1, local_id);
    } else if (type == "powerplant_result") {
        this->simulation->blocks.push_back(new ResultPowerplant()); // logic
        int id = this->simulation->blocks.back()->local_id;
        if (load)
            this->reset_id(this->simulation->blocks.back(), local_id);
        else
            local_id = id;
        GUI_ResultPowerplant *a1 = new GUI_ResultPowerplant(local_id, this); // gui
        this->scheme->canvas->addItem(a1);
        a1->setX(x);
        a1->setY(y);
        GUI_Port *c = new GUI_Port(local_id, "PortC");
        a1->inputPorts.push_back(c);
        c->setParentItem(a1);
        c->setY(c->y() + 15);
        c->setX(c->x() - 10);
        a1->update_tooltip();
    } else if (type == "betterplant") {
        this->simulation->blocks.push_back(new BetterPlant()); // logic
        int id = this->simulation->blocks.back()->local_id;
        if (load)
            this->reset_id(this->simulation->blocks.back(), local_id);
        else
            local_id = id;
        GUI_BetterPlant *a1 = new GUI_BetterPlant(local_id, this); // graphics
        a1->setX(x);
        a1->setY(y);
        this->scheme->canvas->addItem(a1);
        generate_arithmetic_gui_block(a1, local_id);
    } else if (type == "company") {
        this->simulation->blocks.push_back(new Company());
        int id = this->simulation->blocks.back()->local_id;
        if (load)
            this->reset_id(this->simulation->blocks.back(), local_id);
        else
            local_id = id;
        GUI_Company *a1 = new GUI_Company(local_id, this); // gui
        a1->setX(x);
        a1->setY(y);
        this->scheme->canvas->addItem(a1);
        GUI_Port *c = new GUI_Port(local_id, "PortC");
        a1->outputPorts.push_back(c);
        c->setParentItem(a1);
        c->setY(c->y() + 15);
        c->setX(c->x() + 45);
        // get values
        if (!load) {
            bool dialogResult;
            QInputDialog *renameDialog = new QInputDialog();
            QString result = renameDialog->getText(0, "Company", "Company stock value:", QLineEdit::Normal, "", &dialogResult);
            double value = result.toDouble();
            this->set_model_value(local_id, "PortC", "stock_value", value);

            result = renameDialog->getText(0, "Company", "Company stock number:", QLineEdit::Normal, "", &dialogResult);
            value = result.toDouble();
            this->set_model_value(local_id, "PortC", "stock_number", value);

            result = renameDialog->getText(0, "Company", "Company average salary:", QLineEdit::Normal, "", &dialogResult);
            value = result.toDouble();
            this->set_model_value(local_id, "PortC", "average_salary", value);
        } else {
            for (std::pair<std::string, double> p: values) {
                this->set_model_value(local_id, "PortC", p.first, p.second);
            }
        }
        a1->update_tooltip();
    } else if (type == "evaluator") {
        this->simulation->blocks.push_back(new Evaluator()); // logic
        int id = this->simulation->blocks.back()->local_id;
        if (load)
            this->reset_id(this->simulation->blocks.back(), local_id);
        else
            local_id = id;
        GUI_Evaluator *a1 = new GUI_Evaluator(local_id, this); // graphics
        a1->setX(x);
        a1->setY(y);
        this->scheme->canvas->addItem(a1);
        GUI_Port *a = new GUI_Port(local_id, "PortA");
        a1->inputPorts.push_back(a);
        a->setParentItem(a1);
        a->setX(a->x() - 10);
        a->setY(a->y() + 15);
        GUI_Port *c = new GUI_Port(local_id, "PortC");
        a1->outputPorts.push_back(c);
        c->setParentItem(a1);
        c->setY(c->y() + 15);
        c->setX(c->x() + 45);
    } else if (type == "company_merge") {
        this->simulation->blocks.push_back(new CompanyMerge()); // logic
        int id = this->simulation->blocks.back()->local_id;
        if (load)
            this->reset_id(this->simulation->blocks.back(), local_id);
        else
            local_id = id;
        GUI_CompanyMerge *a1 = new GUI_CompanyMerge(local_id, this); // gui
        a1->setX(x);
        a1->setY(y);
        this->scheme->canvas->addItem(a1);
        generate_arithmetic_gui_block(a1, local_id);
    } else if (type == "company_result") {
        this->simulation->blocks.push_back(new ResultCompany()); // logic
        int id = this->simulation->blocks.back()->local_id;
        if (load)
            this->reset_id(this->simulation->blocks.back(), local_id);
        else
            local_id = id;
        GUI_ResultCompany *a1 = new GUI_ResultCompany(local_id, this); // gui
        this->scheme->canvas->addItem(a1);
        a1->setX(x);
        a1->setY(y);
        GUI_Port *c = new GUI_Port(local_id, "PortC");
        a1->inputPorts.push_back(c);
        c->setParentItem(a1);
        c->setY(c->y() + 15);
        c->setX(c->x() - 10);
        a1->update_tooltip();
    }
}

/**
 * Finds connection position in logic model
 * @brief Logic_gui_interface::find_connetion_pos
 * @param local_id ID of the source block
 * @param name Name of the source port
 * @param local_id2 ID of the dest block
 * @param name2 Name of the dest port
 * @return Index in the logic model
 */
int Logic_gui_interface::find_connetion_pos(int local_id, std::string name, int local_id2, std::string name2) {
    int pos = 0;
    for (Connection *c : this->simulation->connections) {
        if (c->inputPort->loc == local_id && c->outputPort->loc == local_id2 && c->inputPort->name == name && c->outputPort->name == name2)
            return pos;
        pos++;
    }
    return 0;
}

/**
 * Find connection position relative to logic port
 * @brief Logic_gui_interface::find_port_connetion_pos
 * @param p Logic port
 * @param local_id ID of the source port
 * @param name Name of the source name
 * @param local_id2 ID of the dest port
 * @param name2 Name of the dest name
 * @return Relative index of the connection in the logic model
 */
int Logic_gui_interface::find_port_connetion_pos(Port *p, int local_id, std::string name, int local_id2, std::string name2) {
    int pos = 0;
    for (Connection *c : p->connections) {
        if (c->inputPort->loc == local_id && c->outputPort->loc == local_id2 && c->inputPort->name == name && c->outputPort->name == name2)
            return pos;
        pos++;
    }
    return 0;
}

/**
 * Removes connection from the logic and graphic model
 * @brief Logic_gui_interface::remove_connection
 * @param local_id ID of the source block
 * @param name Name of the source port
 * @param local_id2 ID of the dest block
 * @param name2 Name of the dest port
 */
void Logic_gui_interface::remove_connection(int local_id, std::string name, int local_id2, std::string name2) {
    std::cout << "Removing connection (" << local_id << name << ", " << local_id2 << name2 << ")" << std::endl;
    Port *p1 = this->find_model_port(local_id, name);
    Port *p2 = this->find_model_port(local_id2, name2);
    if (p1 != NULL && p2 != NULL)
        std::cout << "Logic ports found" << std::endl;

    int to_remove = this->find_port_connetion_pos(p1, local_id, name, local_id2, name2);
    std::cout << p1->loc << p1->name << ": " << p1->connections.size() << ", " << to_remove << std::endl;
    p1->connections.erase(p1->connections.begin() + to_remove);
    std::cout << "Removed connection from the first port " << std::endl;

    to_remove = this->find_port_connetion_pos(p2, local_id, name, local_id2, name2);
    std::cout << p2->loc << p2->name << ": " << p2->connections.size() << ", " << to_remove << std::endl;
    p2->connections.erase(p2->connections.begin() + to_remove);
    std::cout << "Removed connection from the second port " << std::endl;
    std::cout << "output left:" << p2->loc << p2->name << ": " << p2->connections.size() << ", " << to_remove << std::endl;

    to_remove = this->find_connetion_pos(local_id, name, local_id2, name2);
    this->simulation->connections.erase(this->simulation->connections.begin() + to_remove);
    std::cout << "Removed connection from the logic connection " << std::endl;

    std::cout << "Connection: logic erased" << std::endl;

    GUI_Port *gp1 = this->find_gui_port(local_id, name);
    GUI_Port *gp2 = this->find_gui_port(local_id2, name2);

    if (gp1 != NULL && gp2 != NULL)
        std::cout << "Graphic ports found" << std::endl;

    to_remove = this->find_gui_port_connection_position(gp1, local_id, name, local_id2, name2);
    gp1->connections.erase(gp1->connections.begin() + to_remove);

    to_remove = this->find_gui_port_connection_position(gp2, local_id, name, local_id2, name2);
    gp2->connections.erase(gp2->connections.begin() + to_remove);

    std::cout << gp1->connections.size() << ", " << gp2->connections.size() << std::endl;

    to_remove = this->find_gui_connection_position(local_id, name, local_id2, name2);
    std::cout << to_remove << std::endl;
    GUI_Connection *c = this->find_gui_connection(local_id, name, local_id2, name2);
    if (c != NULL)
        delete c;
}

/**
 * Find position of the block in logic model
 * @brief Logic_gui_interface::find_block_position
 * @param local_id ID of the block
 * @return Index in the logic model
 */
int Logic_gui_interface::find_block_position(int local_id) {
    int pos = 0;
    for (Block *p : this->simulation->blocks) {
        if (p->local_id == local_id)
            return pos;
        pos++;
    }
    return 0;
}

/**
 * Finds the index of the block in the graphics model
 * @brief Logic_gui_interface::find_gui_block_position
 * @param local_id ID of the graphics block
 * @return Index int the graphic model
 */
int Logic_gui_interface::find_gui_block_position(int local_id) {
    int pos = 0;
    for (QGraphicsItem *i : this->scheme->canvas->items()) {
        GUI_Block *b = dynamic_cast<GUI_Block*> (i);
        if (b != NULL) {
            if (b->local_id == local_id)
                return pos;
        }
        pos++;
    }
    return 0;
}

/**
 * Find the connection in the graphics model
 * @brief Logic_gui_interface::find_gui_connection
 * @param local_id ID of the source block
 * @param name Name of the source port
 * @param local_id2 ID of the dest block
 * @param name2 Name of the dest port
 * @return Connection the graphic model
 */
GUI_Connection *Logic_gui_interface::find_gui_connection(int local_id, std::string name, int local_id2, std::string name2) {
    for (QGraphicsItem *i : this->scheme->canvas->items()) {
        GUI_Connection *c = dynamic_cast<GUI_Connection*> (i);
        if (c != NULL) {
            if (c->inputPort->id == local_id && c->inputPort->name == name && c->ouputPort->id == local_id2 && c->ouputPort->name == name2)
                return c;
        }
    }
    return 0;
}

/**
 * Find the index of the connection in the graphics model
 * @brief Logic_gui_interface::find_gui_connection_position
 * @param local_id ID of the source block
 * @param name Name of the source port
 * @param local_id2 ID of the dest block
 * @param name2 Name of the dest port
 * @return Index in the graphic model
 */
int Logic_gui_interface::find_gui_connection_position(int local_id, std::string name, int local_id2, std::string name2) {
    int pos = 0;
    for (QGraphicsItem *i : this->scheme->canvas->items()) {
        GUI_Connection *c = dynamic_cast<GUI_Connection*> (i);
        if (c != NULL) {
            if (c->inputPort->id == local_id && c->inputPort->name == name && c->ouputPort->id == local_id2 && c->ouputPort->name == name2)
                return pos;
        }
        pos++;
    }
    return 0;
}

/**
 * Find the index of the connection in the graphic model relative to port
 * @brief Logic_gui_interface::find_gui_port_connection_position
 * @param p Graphic port
 * @param local_id ID of the source block
 * @param name Name of the source port
 * @param local_id2 ID of the dest block
 * @param name2 Name of the dest port
 * @return index relative to port
 */
int Logic_gui_interface::find_gui_port_connection_position(GUI_Port *p, int local_id, std::string name, int local_id2, std::string name2) {
    int pos = 0;
    for (GUI_Connection *c : p->connections) {
        if (c != NULL) {
            if (c->inputPort->id == local_id && c->inputPort->name == name && c->ouputPort->id == local_id2 && c->ouputPort->name == name2)
                return pos;
        }
        pos++;
    }
    return 0;
}

/**
 * Removes the block from the logic and graphic model
 * @brief Logic_gui_interface::remove_block
 * @param local_id ID of the block
 */
void Logic_gui_interface::remove_block(int local_id) {
    // remove logic
    Block *b = this->find_model_block(local_id);

    int position = this->find_block_position(local_id);

    for (Port *p : b->inputPorts) {
        for (int i=p->connections.size()-1; i>=0; i--) {
            Connection *c = p->connections[i];
            this->remove_connection(c->inputPort->loc, c->inputPort->name, c->outputPort->loc, c->outputPort->name);
        }
    }
    for (Port *p : b->outputPorts) {
        for (int i=p->connections.size()-1; i>=0; i--) {
            Connection *c = p->connections[i];
            this->remove_connection(c->inputPort->loc, c->inputPort->name, c->outputPort->loc, c->outputPort->name);
        }
    }

    this->simulation->blocks.erase(this->simulation->blocks.begin() + position);

    // remove gui
    GUI_Block *bg = this->find_gui_block(local_id);
    if (bg != NULL)
        delete bg;
}

/**
 * Finds the graphic block in the graphic model
 * @brief Logic_gui_interface::find_gui_block
 * @param local_id ID of the block
 * @return Graphic block
 */
GUI_Block *Logic_gui_interface::find_gui_block(int local_id) {
    for (auto b: this->scheme->items()) {
        GUI_Block *gb = dynamic_cast<GUI_Block*> (b);
        if (gb != NULL) {
            if (gb->local_id == local_id)
                return gb;
        }
    }
    return NULL;
}

/**
 * Updates the block
 * @brief Logic_gui_interface::update_block
 * @param local_id ID of the block
 */
void Logic_gui_interface::update_block(int local_id) {
    this->simulation->sort_blocks();
    Block *b = this->find_model_block(local_id);
    b->update_value();
    GUI_Block *gb = this->find_gui_block(local_id);
    gb->update_tooltip();
}

/**
 * Runs the simulation completely without delays
 * @brief Logic_gui_interface::run_simulation
 */
void Logic_gui_interface::run_simulation() {
    this->simulation->sort_blocks();
    for (Block *b : this->simulation->blocks) {
        b->update_value();
        GUI_Block *gb = this->find_gui_block(b->local_id);
        gb->update_tooltip();
    }
}

/**
 * Runs the simulatiom completely with delays to see active blocks
 * @brief Logic_gui_interface::user_simulation
 */
void Logic_gui_interface::user_simulation() {
    if (this->simulation->blocks.size() == 0)
        return ;
    for (QGraphicsItem *item : this->scheme->canvas->items()) {
        QGraphicsEllipseItem *p = dynamic_cast<QGraphicsEllipseItem*> (item);
        if (p != NULL)
            delete p;
    }
    this->active_circle = NULL;
    this->to_update = 0;
    if (this->simulation->check_cycles()) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Current scheme contains cycles!");
        messageBox.setFixedSize(500,200);
        return;
    }
    this->add_unassigned_values();
    //this->run_simulation();
    this->count = 0;
    this->simulation->sort_blocks();
    this->timer = new QTimer;
    this->timer->setSingleShot(false);
    this->timer->setInterval(2000);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(user_step_sim()));
    timer->start(1000);
}

/**
 * Step in the delayed simulation
 * @brief Logic_gui_interface::user_step_sim
 */
void Logic_gui_interface::user_step_sim() {
    if (this->simulation->check_cycles()) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Current scheme contains cycles!");
        messageBox.setFixedSize(500,200);
        timer->stop();
        return;
    }
    this->simulation->sort_blocks();
    std::cout << "Updating " << this->to_update << std::endl;
    Block *b = this->simulation->blocks[to_update];

    //this->add_unassigned_values();

    b->update_value();
    std::cout << "Value updated" << std::endl;
    GUI_Block *gb = this->find_gui_block(b->local_id);
    if (gb == NULL) {
        std::cout << "Graphic not found" << std::endl;
    }

    QPen q;
    if (this->active_circle != NULL) {
        std::cout << "Removing circle" << std::endl;
        delete this->active_circle;
        this->active_circle = NULL;
        //this->scheme->canvas->removeItem(this->active_circle);
        std::cout << "Removed circle" << std::endl;
    }
    q.setColor("green");
    QGraphicsEllipseItem* circle = new QGraphicsEllipseItem(-8, -8, 55, 55);
    circle->setPen(q);
    circle->setParentItem(gb);
    this->active_circle = circle;
    std::cout << "updating tooltip" << std::endl;
    gb->update_tooltip();    
    if (this->to_update + 1 > this->simulation->blocks.size()-1) {
        this->to_update = 0;
    } else {
        this->to_update++;
    }

    if (this->count == this->simulation->blocks.size()-1) {
        this->timer->stop();
    } else {
        this->count++;
    }
    std::cout << "done" << std::endl;
}

/**
 * User's signal to do the next step in simulation
 * @brief Logic_gui_interface::user_step
 */
void Logic_gui_interface::user_step() {    
    if (this->simulation->blocks.size() == 0)
        return;
    if (this->simulation->check_cycles()) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Current scheme contains cycles!");
        messageBox.setFixedSize(500,200);
        return;
    }
    this->simulation->sort_blocks();
    std::cout << "Updating " << this->to_update << std::endl;    

    Block *b = this->simulation->blocks[to_update];
    double init_val = std::numeric_limits<double>::max();

    for (Port *p : b->inputPorts) {
        if (p->connections.size() == 0) {
            for (auto &x: p->values) {
                if (x.second == init_val) {
                    bool dialogResult;
                    QInputDialog *renameDialog = new QInputDialog();
                    std::string title = b->name + " : " + p->name;
                    std::string val = "Please enter new " + x.first + ":";
                    QString result = renameDialog->getText(0, QString::fromStdString(title), QString::fromStdString(val), QLineEdit::Normal, "", &dialogResult);
                    double value = result.toDouble();
                    x.second = value;
                }
            }
        }
    }

    b->update_value();
    GUI_Block *gb = this->find_gui_block(b->local_id);

    QPen q;
    if (this->active_circle != NULL) {
        this->scheme->canvas->removeItem(this->active_circle);
    }
    q.setColor("green");
    QGraphicsEllipseItem* circle = new QGraphicsEllipseItem(-8, -8, 55, 55);
    circle->setPen(q);
    circle->setParentItem(gb);
    this->active_circle = circle;

    gb->update_tooltip();
    if (this->to_update + 1 > this->simulation->blocks.size()-1) {
        this->to_update = 0;
    } else {
        this->to_update++;
    }
}

/**
 * Finds block in the logic model
 * @brief Logic_gui_interface::find_model_block
 * @param local_id ID of the block
 * @return Logic block
 */
Block *Logic_gui_interface::find_model_block(int local_id) {
    for (Block *b : this->simulation->blocks) {
        //std::cout << "SIM blocks: " <<  b->local_id << std::endl;
        if (local_id == b->local_id) {
            return b;
        }
    }
    return NULL;
}

/**
 * Finds the logic ports in the logic block
 * @brief Logic_gui_interface::find_model_port
 * @param local_id ID of the block
 * @param name Name of the port
 * @return Logic port
 */
Port *Logic_gui_interface::find_model_port(int local_id, std::string name) {
    Block *b = this->find_model_block(local_id);
    if (b == NULL) {
        //std::cout << "Block " << local_id << " not found" << std::endl;
        return NULL;
    }
    for (Port *p : b->inputPorts) {
        if (p->name == name) {
            return p;
        }
    }
    for (Port *p : b->outputPorts) {
        if (p->name == name) {
            return p;
        }
    }
    //std::cout << "Port " << name << " not found" << std::endl;
    return NULL;
}

/**
 * Sets the new value new for key in the port values
 * @brief Logic_gui_interface::set_model_value
 * @param local_id Block's id
 * @param name Name of the port
 * @param key Key in the values
 * @param value Value of the block
 */
void Logic_gui_interface::set_model_value(int local_id, std::string name, std::string key, double value) {
    Port *p = this->find_model_port(local_id, name);
    auto p1 = p->values.find(key);
    p1->second = value;
}

/**
 * Creates new connection in both logic and graphic model
 * @brief Logic_gui_interface::add_connection
 * @param local_id_in Source block ID
 * @param port_name_in Source port name
 * @param local_id_out Dest block ID
 * @param port_name_out  Dest port name
 */
void Logic_gui_interface::add_connection(int local_id_in, std::string port_name_in, int local_id_out, std::string port_name_out) {
    Port *l_p1 = this->find_model_port(local_id_in, port_name_in);
    Port *l_p2 = this->find_model_port(local_id_out, port_name_out);    

    for (Block *b : this->simulation->blocks) {
        std::cout << b->local_id << ": " << b->name << ",";
    }
    std::cout << std::endl;
    std::cout << "Creating connection (" << l_p1->loc << l_p1->name << ", " << l_p2->loc << l_p2->name << ")" << std::endl;
    std::cout << "Creating connection (" << local_id_in << port_name_in << ", " << local_id_out << port_name_out << ")" << std::endl;    

    Connection *l_connection = l_p1->create_connection(l_p2);
    this->simulation->connections.push_back(l_connection);


    GUI_Port *p1 = this->find_gui_port(local_id_in, port_name_in);
    GUI_Port *p2 = this->find_gui_port(local_id_out, port_name_out);

    int x1 = p1->pos().x() + p1->parentItem()->x();
    int y1 = p1->pos().y() + p1->parentItem()->y();
    int x2 = p2->pos().x() + p2->parentItem()->x();
    int y2 = p2->pos().y() + p2->parentItem()->y();

    GUI_Connection *connection = new GUI_Connection(this, x1, y1, x2, y2);
    connection->inputPort = p1;
    connection->ouputPort = p2;
    connection->update_tooltip();
    this->scheme->canvas->addItem(connection);
    p1->connections.push_back(connection);
    p2->connections.push_back(connection);
}

/**
 * Get's the port values
 * @brief Logic_gui_interface::get_port_values
 * @param local_id ID of the block
 * @param name Name of the port
 * @return Port values
 */
std::map<std::string, double> Logic_gui_interface::get_port_values(int local_id, std::string name) {
    Port *p = this->find_model_port(local_id, name);
    return p->values;
}

/**
 * Clear the graphic model
 * @brief Logic_gui_interface::clear
 */
void Logic_gui_interface::clear() {
    this->simulation->blocks.clear();
    this->simulation->connections.clear();
    this->scheme->canvas->clear();
    this->scheme->items().clear();
}

/**
 * Get the tooltip value for the block (mouse over block)
 * @brief Logic_gui_interface::get_tooltip_values
 * @param local_id ID of the block
 * @return Tooltip
 */
std::string Logic_gui_interface::get_tooltip_values(int local_id) {
    double init_val = std::numeric_limits<double>::max();
    Block *b = this->find_model_block(local_id);
    if (b == NULL) {
        return "Block not found";
    }
    std::string tooltip = "";
    if (b->inputPorts.size() > 0) {
        tooltip = "\nInput values:\n";
        for (Port *p : b->inputPorts) {
            tooltip += "  " + p->name + ":\n";
            for (std::pair<std::string, double> x: p->get_value()) {
                 std::string value;
                 if (x.second == init_val) {
                     value = "unknown";
                 } else {
                     value = std::to_string(x.second);
                 }
                 tooltip += "    " + x.first + " => " += value + "\n";
            }
        }
    }
    if (b->outputPorts.size() > 0) {
        tooltip += "Output values:\n";
        for (Port *p : b->outputPorts) {
            tooltip += "  " + p->name + ":\n";
            for (std::pair<std::string, double> x: p->get_value()) {
                std::string value;
                if (x.second == init_val) {
                    value = "unknown";
                } else {
                    value = std::to_string(x.second);
                }
                tooltip += "   " + x.first + " => " += value + "\n";
            }
        }
    }
    return tooltip;
}

/**
 * Get the tooltip for current port
 * @brief Logic_gui_interface::get_port_tooltip
 * @param local_id ID of the block
 * @param name Name of the port
 * @return Tooltip for port
 */
std::string Logic_gui_interface::get_port_tooltip(int local_id, std::string name) {
    std::string tooltip;
    double init_val = std::numeric_limits<double>::max();
    for (std::pair<std::string, double> x: this->get_port_values(local_id, name)) {
        std::string value;
        if (x.second == init_val) {
            value = "unknown";
        } else {
            value = std::to_string(x.second);
        }
        tooltip += x.first + " => " += value + "\n";
    }
    return tooltip;
}

/**
 * Saves the scheme for later use
 * @brief Logic_gui_interface::save_schmeme
 */
void Logic_gui_interface::save_schmeme() {
    QString filename = QFileDialog::getSaveFileName(this->scheme, "Save file scheme", ".", "*.scheme");
    QFile f(filename);
    if (filename == "")
        return;
    f.open(QIODevice::WriteOnly);

    //save block
    std::string to_write = "";
    to_write += std::to_string(this->simulation->blocks.size()) + "\n";
    for (Block *b : this->simulation->blocks) {
        GUI_Block *gb = this->find_gui_block(b->local_id);
        to_write += std::to_string(b->local_id) + " " + b->name + " " + std::to_string(gb->x()) + " " + std::to_string(gb->y()) + "\n";
        if (b->name == "Constant" || b->name == "PowerPlant" || b->name == "Company") {
            for (std::pair<std::string, double> p : b->outputPorts[0]->values) {
                to_write += p.first + " " + std::to_string(p.second) + " ";
            }
            to_write += "\n";
        }
    }
    f.write(to_write.c_str());

    //save connections
    to_write = "";
    to_write += std::to_string(this->simulation->connections.size()) + "\n";
    for (Connection *c : this->simulation->connections) {
        to_write += std::to_string(c->inputPort->loc) + " " + c->inputPort->name + " ";
        to_write += std::to_string(c->outputPort->loc) + " " + c->outputPort->name + "\n";
    }
    f.write(to_write.c_str());
    f.close();
}

/**
 * Set's the new id for the block
 * @brief Logic_gui_interface::reset_id
 * @param b Current id
 * @param new_id New id
 */
void Logic_gui_interface::reset_id(Block *b, int new_id) {
    if (b!= NULL) {
        b->local_id = new_id;
        for (Port *p : b->inputPorts) {
            p->loc = new_id;
        }
        for (Port *p : b->outputPorts) {
            p->loc = new_id;
        }
    } else {
        std::cout << "NOT FOUND" << std::endl;
    }
}

/**
 * Asks user for one unassigned value
 * @brief Logic_gui_interface::add_unassigned_value
 * @param x Values to be changed
 * @param name Name of the dialog
 * @param port Name of the port
 */
void Logic_gui_interface::add_unassigned_value(std::pair<std::string, double> &x, std::string name, std::string port) {
    double init_val = std::numeric_limits<double>::max();
    if (x.second == init_val) {
        bool dialogResult;
        QInputDialog *renameDialog = new QInputDialog();
        std::string title = name + " : " + port;
        std::string val = "Please enter new " + x.first + ":";
        QString result = renameDialog->getText(0, QString::fromStdString(title), QString::fromStdString(val), QLineEdit::Normal, "", &dialogResult);
        double value;
        if (x.first == "risk_probability")
            value = std::max(std::min(result.toDouble(), 1.0), 0.0);
        else
            value = result.toDouble();
        x.second = value;
    }
}

/**
 * Asks user to fill all unassigned values in the scheme
 * @brief Logic_gui_interface::add_unassigned_values
 */
void Logic_gui_interface::add_unassigned_values() {
    double init_val = std::numeric_limits<double>::max();
    for (Block *b : this->simulation->blocks) {
        for (Port *p : b->inputPorts) {
            if (p->connections.size() == 0) {
                for (auto &x: p->values) {
                    if (x.second == init_val) {
                        bool dialogResult;
                        QInputDialog *renameDialog = new QInputDialog();
                        std::string title = b->name + " : " + p->name;
                        std::string val = "Please enter new " + x.first + ":";
                        QString result = renameDialog->getText(0, QString::fromStdString(title), QString::fromStdString(val), QLineEdit::Normal, "", &dialogResult);
                        double value;
                        if (x.first == "risk_probability")
                            value = std::max(std::min(result.toDouble(), 1.0), 0.0);
                        else
                            value = result.toDouble();
                        x.second = value;
                    }
                }
            }
        }
    }
}

/**
 * Generates block from save file
 * @brief it_generate_block
 * @param interface logic-gui interface
 * @param l_id loaded ID of the block
 * @param type type of the block
 * @param x Init X coordinate
 * @param y Init Y coordinate
 * @param values Loaded values from memory
 */
void it_generate_block(Logic_gui_interface *interface, int l_id, std::string type, int x, int y, std::map<std::string, double> values) {
    std::cout << type << std::endl;
    if (type == "Adder") {
        interface->add_block("adder", true, l_id, x, y, values);
    } else if (type == "Subtractor") {
        interface->add_block("subtractor", true, l_id, x, y, values);
    } else if (type == "Multiplier") {
        interface->add_block("multiplier", true, l_id, x, y, values);
    } else if (type == "Divider") {
        interface->add_block("divider", true, l_id, x, y, values);
    } if (type == "Result") {
        interface->add_block("result", true, l_id, x, y, values);
    } else if (type == "Constant") {
        interface->add_block("var", true, l_id, x, y, values);
    } else if (type == "PowerPlant") {
        interface->add_block("powerplant", true, l_id, x, y, values);
    } else if (type == "BetterPlant") {
        interface->add_block("betterplant", true, l_id, x, y, values);
    } else if (type == "ReactorCooler") {
        interface->add_block("cooler", true, l_id, x, y, values);
    } else if (type == "CommonRisk") {
        interface->add_block("commonrisk", true, l_id, x, y, values);
    }  else if (type == "PowerPlantResult") {
        interface->add_block("powerplant_result", true, l_id, x, y, values);
    } else if (type == "Company") {
        interface->add_block("company", true, l_id, x, y, values);
    } else if (type == "CompanyMerge") {
        interface->add_block("company_merge", true, l_id, x, y, values);
    } else if (type == "Evaluator") {
        interface->add_block("evaluator", true, l_id, x, y, values);
    } else if (type == "ResultCompany") {
        interface->add_block("company_result", true, l_id, x, y, values);
    }
}

/**
 * Find graphic port
 * @brief Logic_gui_interface::find_gui_port
 * @param local_id ID of the block
 * @param name Name of the block
 * @return Graphic port
 */
GUI_Port *Logic_gui_interface::find_gui_port(int local_id, std::string name) {
    GUI_Block *b = this->find_gui_block(local_id);
    for (GUI_Port *p : b->inputPorts) {
        if (p->name == name) {
            return p;
        }
    }
    for (GUI_Port *p : b->outputPorts) {
        if (p->name == name) {
            return p;
        }
    }
    return NULL;
}

/**
 * Load scheme saved in the file
 * @brief Logic_gui_interface::load_scheme
 */
void Logic_gui_interface::load_scheme() {
    // clear the current scheme
    this->clear();
    // load the filename
    QString filename = QFileDialog::getOpenFileName(this->scheme, "Save file scheme", ".", "*");
    QFile f(filename);
    if (filename == "")
        return;
    f.open(QIODevice::ReadOnly);

    std::string line = "";
    std::string blocks_num = f.readLine().toStdString();
    blocks_num.pop_back();
    std::cout << blocks_num << std::endl;
    // load blocks
    for (int i = 0; i<std::stoi(blocks_num); i++) {
        // blocks line
        line = f.readLine().toStdString();
        std::istringstream iss(line);
        std::string s;
        getline(iss, s, ' ');
        std::string id = s;
        getline(iss, s, ' ');
        std::string name = s;
        getline(iss, s, ' ');
        std::string x = s;
        getline(iss, s, ' ');
        std::string y = s;
        y.pop_back();
        std::cout << name << " : " << id << " (" << x << ", " << y << ")" << std::endl;
        if (name == "Constant" || name == "PowerPlant" || name == "Company") {
            line = f.readLine().toStdString();
            line.pop_back();
            std::istringstream iss(line);
            std::map<std::string, double> values;
            while (getline(iss, s, ' ')) {
                std::string key = s;
                getline(iss, s, ' ');
                std::string value = s;
                std::cout << "KEY: " << key << " VALUE: " << value << std::endl;
                values.insert(std::pair<std::string, double>(key, std::stod(value)));
            }
            it_generate_block(this, std::stoi(id), name, std::stoi(x), std::stoi(y), values);
        }
        else {
            std::map<std::string, double> n;
            it_generate_block(this, std::stoi(id), name, std::stoi(x), std::stoi(y), n);
        }
    }

    line = "";
    std::string cons_num = f.readLine().toStdString();
    cons_num.pop_back();
    std::cout << cons_num << std::endl;
    // load connections
    for (int i = 0; i<std::stod(cons_num); i++) {
        // blocks line
        line = f.readLine().toStdString();
        std::istringstream iss(line);
        std::string s;
        getline(iss, s, ' ');
        std::string id = s;
        getline(iss, s, ' ');
        std::string name = s;
        getline(iss, s, ' ');
        std::string id2 = s;
        getline(iss, s, ' ');
        std::string name2 = s;
        name2.pop_back();
        std::cout << "Connection: (" << id << "," << name << ")(" << id2 << "," << name2 << ")" << std::endl;
        this->add_connection(std::stod(id), name, std::stod(id2), name2);
    }

    f.close();
    std::cout << "CURRENT STATE: " << this->simulation->blocks.size() << " " << this->simulation->connections.size() << std::endl;
}

/**
 * Clears the current scheme
 * @brief Logic_gui_interface::clear_scene
 */
void Logic_gui_interface::clear_scene() {
    this->scheme->canvas->update(this->scheme->canvas->sceneRect());
}
