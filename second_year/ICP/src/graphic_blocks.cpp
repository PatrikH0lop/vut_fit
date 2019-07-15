/**
 * Implementation of graphic components
 * @author Patrik Holop
 * @author Matej Hrabal
 */


#include "graphic_blocks.h"


/** initialize cache port */
GUI_Port *GUI_Port::cached_port = NULL;


/**
 * Constructor of the graphic connection
 * @brief GUI_Connection::GUI_Connection
 * @param interface logic - gui interface
 * @param x1 X coordinate of source port
 * @param y1 Y coordinate of source port
 * @param x2 X coordinate of dest port
 * @param y2 Y coordinate of dest port
 */
GUI_Connection::GUI_Connection(Logic_gui_interface *interface, int x1, int y1, int x2, int y2) {
    this->setLine(x1, y1, x2, y2);
    this->interface = interface;
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    // for better experience with a lot of connections
    this->setZValue(-1);
}


/**
 * Update tooltip over connection.
 * Shown when mouse is over the object.
 * @brief GUI_Connection::update_tooltip
 */
void GUI_Connection::update_tooltip() {
    std::string tip = "Input port: \n" + this->interface->get_port_tooltip(this->inputPort->id, this->inputPort->name);
    tip += "Output port: \n" + this->interface->get_port_tooltip(this->inputPort->id, this->inputPort->name);
    QString tooltip = QString::fromStdString(tip);
    this->setToolTip(tooltip);
}


/**
 * Constructor of the graphic port
 * @brief GUI_Port::GUI_Port
 * @param block_id Parent graphic block
 * @param name Name of the block
 */
GUI_Port::GUI_Port(int block_id, std::string name) {
    this->name = name;
    this->id = block_id;
    // load port image
    this->image = new QImage(":/data/pin_over.png");
    this->setPixmap(QPixmap::fromImage(*(this->image)));
    // set basic attributes for movement
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    // for better experience with a lot of connections
    this->setZValue(1);
}


/**
 * Update tooltip over block.
 * Shown when mouse is over the object.
 * @brief GUI_Block::update_tooltip
 */
void GUI_Block::update_tooltip() {
    std::string tip = "ID: " + std::to_string(this->local_id) + "\n" + this->interface->get_tooltip_values(this->local_id);
    QString tooltip = QString::fromStdString(tip);
    this->setToolTip(tooltip);
    for (GUI_Port *p : this->outputPorts) {
        for (GUI_Connection *c : p->connections) {
            c->update_tooltip();
        }
    }
    for (GUI_Port *p : this->inputPorts) {
        for (GUI_Connection *c : p->connections) {
            c->update_tooltip();
        }
    }
}


/**
 * Help function for initializing graphic blocks with certain image
 * and their basic attributes
 * @brief initialize_gui_block
 * @param b block to be initialized
 * @param local_id id of the block
 * @param interface gui-logic interface
 * @param path_to_image resource image
 */
void initialize_gui_block(GUI_Block *b, int local_id, Logic_gui_interface *interface, QString path_to_image) {
    b->interface = interface;
    b->local_id = local_id;
    b->image = new QImage(path_to_image);
    b->setPixmap(QPixmap::fromImage(*(b->image)));
    b->setFlag(QGraphicsItem::ItemIsSelectable);
    b->setFlag(QGraphicsItem::ItemIsMovable);
    b->setFlag(QGraphicsItem::ItemSendsGeometryChanges);
    b->update_tooltip();
}


// Arithmetic graphic blocks

/**
 * Constructor of the graphic adder
 * @brief GUI_Adder::GUI_Adder
 * @param local_id Id of the block
 * @param interface logic-gui interface
 */
GUI_Adder::GUI_Adder(int local_id, Logic_gui_interface *interface) {
    initialize_gui_block(this, local_id, interface, ":/data/adder.png");
}

/**
 * Constructor of the graphic multiplier
 * @brief GUI_Multiplier::GUI_Multiplier
 * @param local_id Id of the block
 * @param interface logic-gui interface
 */
GUI_Multiplier::GUI_Multiplier(int local_id, Logic_gui_interface *interface) {
    initialize_gui_block(this, local_id, interface, ":/data/multiplier.png");
}

/**
 * Constructor of the graphic subtractor
 * @brief GUI_Subtractor::GUI_Subtractor
 * @param local_id Id of the block
 * @param interface logic-gui interface
 */
GUI_Subtractor::GUI_Subtractor(int local_id, Logic_gui_interface *interface) {
    initialize_gui_block(this, local_id, interface, ":/data/subtractor.png");
}

/**
 * Constructor of the graphic divider
 * @brief GUI_Divider::GUI_Divider
 * @param local_id Id of the block
 * @param interface logic-gui interface
 */
GUI_Divider::GUI_Divider(int local_id, Logic_gui_interface *interface) {
    initialize_gui_block(this, local_id, interface, ":/data/divider.png");
}

/**
 * Constructor of the graphic result
 * @brief GUI_Result::GUI_Result
 * @param local_id Id of the block
 * @param interface logic-gui interface
 */
GUI_Result::GUI_Result(int local_id, Logic_gui_interface *interface) {
    initialize_gui_block(this, local_id, interface, ":/data/result.png");
}

/**
 * Constructor of the graphic constant
 * @brief GUI_Constant::GUI_Constant
 * @param local_id Id of the block
 * @param interface logic-gui interface
 */
GUI_Constant::GUI_Constant(int local_id, Logic_gui_interface *interface) {
    initialize_gui_block(this, local_id, interface, ":/data/var.png");
}


// Power plant management blocks

/**
 * Constructor of the graphic power plant
 * @brief GUI_Powerplant::GUI_Powerplant
 * @param local_id Id of the block
 * @param interface logic-gui interface
 */
GUI_Powerplant::GUI_Powerplant(int local_id, Logic_gui_interface *interface) {
    initialize_gui_block(this, local_id, interface, ":/data/powerplant.png");
}

/**
 * Constructor of the graphic better plant
 * @brief GUI_BetterPlant::GUI_BetterPlant
 * @param local_id Id of the block
 * @param interface logic-gui interface
 */
GUI_BetterPlant::GUI_BetterPlant(int local_id, Logic_gui_interface *interface) {
    initialize_gui_block(this, local_id, interface, ":/data/betterplant.png");
}

/**
 * Constructor of the graphic reactor cooler
 * @brief GUI_ReactorCooler::GUI_ReactorCooler
 * @param local_id Id of the block
 * @param interface logic-gui interface
 */
GUI_ReactorCooler::GUI_ReactorCooler(int local_id, Logic_gui_interface *interface) {
    initialize_gui_block(this, local_id, interface, ":/data/cooler.png");
}

/**
 * Constructor of the graphic result power plant
 * @brief GUI_ResultPowerplant::GUI_ResultPowerplant
 * @param local_id Id of the block
 * @param interface logic-gui interface
 */
GUI_ResultPowerplant::GUI_ResultPowerplant(int local_id, Logic_gui_interface *interface){
    initialize_gui_block(this, local_id, interface, ":/data/powerplant_result.png");
}

/**
 * Constructor of the graphic common risk calculator
 * @brief GUI_CommonRisk::GUI_CommonRisk
 * @param local_id Id of the block
 * @param interface logic-gui interface
 */
GUI_CommonRisk::GUI_CommonRisk(int local_id, Logic_gui_interface *interface){
    initialize_gui_block(this, local_id, interface, ":/data/commonrisk.png");
}


// Market management graphic blocks

/**
 * Constructor of the graphic company
 * @brief GUI_Company::GUI_Company
 * @param local_id Id of the block
 * @param interface logic-gui interface
 */
GUI_Company::GUI_Company(int local_id, Logic_gui_interface *interface) {
    initialize_gui_block(this, local_id, interface, ":/data/company.png");
}

/**
 * Constructor of the graphic company merger
 * @brief GUI_CompanyMerge::GUI_CompanyMerge
 * @param local_id Id of the block
 * @param interface logic-gui interface
 */
GUI_CompanyMerge::GUI_CompanyMerge(int local_id, Logic_gui_interface *interface) {
    initialize_gui_block(this, local_id, interface, ":/data/company_merge.png");
}

/**
 * Constructor of the graphic company evaluator
 * @brief GUI_Evaluator::GUI_Evaluator
 * @param local_id Id of the block
 * @param interface logic-gui interface
 */
GUI_Evaluator::GUI_Evaluator(int local_id, Logic_gui_interface *interface) {
    initialize_gui_block(this, local_id, interface, ":/data/evaluator.png");
}

/**
 * Constructor of the graphic result company
 * @brief GUI_ResultCompany::GUI_ResultCompany
 * @param local_id Id of the block
 * @param interface logic-gui interface
 */
GUI_ResultCompany::GUI_ResultCompany(int local_id, Logic_gui_interface *interface){
    initialize_gui_block(this, local_id, interface, ":/data/company_result.png");
}


