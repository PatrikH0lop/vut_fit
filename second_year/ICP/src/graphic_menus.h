/** \addtogroup graphic_menus
 * graphic_menus.h module contains definition and implementation of Block Menu
 * (left side menu) for instantiating new blocks.
 * @author Matej Hrabal
 */


#ifndef BLOCK_BUTTON_H
#define BLOCK_BUTTON_H


#include <iostream>
#include <QPushButton>
#include <QInputDialog>
#include <QLineEdit>
#include <QVBoxLayout>
#include "simulation.h"
#include "logic_blocks.h"
#include "scheme.h"
#include "graphic_blocks.h"
#include "logic_gui_interface.h"


/**
 * Component of the block menu, which instantiates new block
 * @brief The BlockButton class
 */
class BlockButton : public QPushButton {
    Q_OBJECT

    public:
        /** type of the instatiated blocks */
        std::string type;
        /** interface for access to logic model */
        Logic_gui_interface *interface;

        /** Constructor */
        BlockButton(std::string type, Logic_gui_interface *interface);

    public slots:
        /** slot triggered by user to generate new block */
        void generate_block();
};


/**
 * Left side block menu used for instantiating new blocks
 * @brief The BlockMenu class
 */
class BlockMenu: public QWidget {
    public:
        /** Layout containing block buttons */
        QVBoxLayout *current_layout;
};

/**
 * Block menu containing arithmetic blocks
 * @brief The ArithmeticBlockMenu class
 */
class ArithmeticBlockMenu : public BlockMenu {
    public:
        /** Constructor */
        ArithmeticBlockMenu(Logic_gui_interface *interface);
};

/**
 * Block menu containing power plant blocks
 * @brief The PowerplantBlockMenu class
 */
class PowerplantBlockMenu : public BlockMenu {
    public:
        /** Constructor */
        PowerplantBlockMenu(Logic_gui_interface *interface);
};

/**
 * Block menu containing market blocks
 * @brief The MarketBlockMenu class
 */
class MarketBlockMenu : public BlockMenu {
    public:
        /** Constructor */
        MarketBlockMenu(Logic_gui_interface *interface);
};

#endif // BLOCK_BUTTON_H
