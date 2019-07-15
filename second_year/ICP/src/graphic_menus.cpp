/** \addtogroup graphic_menus
 * Implementation of the block menu
 * @author Matej Hrabal
 */


#include "graphic_menus.h"


/**
 * Arithmetic blocks menu
 * Contains blocks adder, subtractor, multiplier, divider, var, result
 * @brief ArithmeticBlockMenu::ArithmeticBlockMenu
 * @param interface gui - logic interface
 */
ArithmeticBlockMenu::ArithmeticBlockMenu(Logic_gui_interface *interface) {
    current_layout = new QVBoxLayout();
    // add block buttons
    current_layout->addWidget(new BlockButton("adder", interface));
    current_layout->addWidget(new BlockButton("subtractor", interface));
    current_layout->addWidget(new BlockButton("multiplier", interface));
    current_layout->addWidget(new BlockButton("divider", interface));
    current_layout->addWidget(new BlockButton("var", interface));
    current_layout->addWidget(new BlockButton("result", interface));
    current_layout->setAlignment(Qt::AlignTop);
    this->setLayout(current_layout);
    this->setMaximumWidth(170);
}

/**
 * Power plant block menu
 * @brief PowerplantBlockMenu::PowerplantBlockMenu
 * @param interface gui - logic interface
 */
PowerplantBlockMenu::PowerplantBlockMenu(Logic_gui_interface *interface) {
    current_layout = new QVBoxLayout();
    // adds new block buttons
    current_layout->addWidget(new BlockButton("powerplant", interface));
    current_layout->addWidget(new BlockButton("commonrisk", interface));
    current_layout->addWidget(new BlockButton("cooler", interface));
    current_layout->addWidget(new BlockButton("betterplant", interface));
    current_layout->addWidget(new BlockButton("powerplant_result", interface));
    current_layout->setAlignment(Qt::AlignTop);
    this->setLayout(current_layout);
    this->setMaximumWidth(170);
}

/**
 * Market block menu
 * @brief MarketBlockMenu::MarketBlockMenu
 * @param interface
 */
MarketBlockMenu::MarketBlockMenu(Logic_gui_interface *interface) {
    current_layout = new QVBoxLayout();
    // add current block button
    current_layout->addWidget(new BlockButton("company", interface));
    current_layout->addWidget(new BlockButton("company_merge", interface));
    current_layout->addWidget(new BlockButton("evaluator", interface));
    current_layout->addWidget(new BlockButton("company_result", interface));
    current_layout->setAlignment(Qt::AlignTop);
    this->setLayout(current_layout);
    this->setMaximumWidth(170);
}


/**
 * Block button used for instantiation of the blocks
 * @brief BlockButton::BlockButton
 * @param type Type of the block
 * @param interface gui-logic interface
 */
BlockButton::BlockButton(std::string type, Logic_gui_interface *interface) {
    this->interface = interface;
    std::string path = ":/data/" + type + ".png";
    // initialize block button with the name and help
    if (type == "adder") {
        this->setText(" Adder");
        this->setToolTip("Adder\nInput: 2 values\nOutput: value (sum)");
    } else if (type == "subtractor") {
        this->setText(" Subtractor");
        this->setToolTip("Subtactor\nInput: 2 values\nOutput: value (difference)");
    } else if (type == "multiplier") {
        this->setText(" Subtractor");
        this->setToolTip("Multiplier\nInput: 2 values\nOutput: value (product)");
    } else if (type == "divider") {
        this->setText(" Divider");
        this->setToolTip("Divider\nInput: 2 values\nOutput: value (quotient)");
    } else if (type == "result") {
        this->setText(" Result");
        this->setToolTip("Numerical constant\nInput: value");
    } else if (type == "var") {
        this->setText(" Constant");
        this->setToolTip("Numerical constant\nOutput: value");
    } else if (type == "powerplant") {
        this->setText(" Power Plant");
        this->setToolTip("Nucler power plant\nOutput: power plant (\n  risk probability\n  temperature\n  energy per day)");
    } else if (type == "commonrisk") {
        this->setText(" Common Risk");
        this->setToolTip("Common risk calculator\nInput: 2 power plants\nOutput: value (common risk)");
    } else if (type == "betterplant") {
        this->setText(" Better Plant");
        this->setToolTip("Calculates superior power plant\nInput: 2 power plants\nOutput: power plant (higher stats)");
    } else if (type == "powerplant_result") {
        this->setText(" Result Plant");
        this->setToolTip("Result power plant\nInput: power plant");
    } else if (type == "cooler") {
        this->setText(" Reactor Cooler");
        this->setToolTip("Reactor cooler\nInput: power plant and value\nOutput: power plant (with cooled reactor by value)");
    } else if (type == "company") {
        this->setText(" Company");
        this->setToolTip("Market company (a.s.)\nOutput: company (\n  stock number\n  stock price\n  average salary)");
    } else if (type == "company_merge") {
        this->setText(" Company Merge");
        this->setToolTip("Merging 2 companies\nInput: 2 companies\nOutput: merged company");
    } else if (type == "evaluator") {
        this->setText(" Evaluator");
        this->setToolTip("Evaluates value of the company\nInput: company\nOutput: value (market cap)");
    } else if (type == "company_result") {
        this->setText(" Result Company");
        this->setToolTip("Market company (a.s.)\nInput: company");
    }
    QString qstr = QString::fromStdString(path);
    this->setIcon(QIcon(qstr));
    this->setIconSize(QSize(30,30));
    this->type = type;
    // connect the block button to generation slot
    connect(this, SIGNAL (released()), this, SLOT (generate_block()));
}

/**
 * Slot triggered after used action to generate block
 * @brief BlockButton::generate_block
 */
void BlockButton::generate_block() {
    std::map<std::string, double> map;
    this->interface->add_block(this->type, false, 0, 0, 0, map);
}
