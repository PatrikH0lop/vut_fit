/** \addtogroup graphic_interface
 * graphic_blocks.h module provides definition of graphic entities (blocks, connections, ports)
 * Those objects contain necessary information for drawing the user interface. Communication with
 * model is ensured by interface.
 * @author Patrik Holop
 * @author Matej Hrabal
 */


#ifndef GRAPHIC_BLOCKS_H
#define GRAPHIC_BLOCKS_H


#include <QObject>
#include <QGraphicsObject>
#include <QGraphicsPixmapItem>
#include <QMouseEvent>
#include <QImage>
#include <iostream>
#include <QtGui>
#include <vector>
#include <string>
#include "logic_gui_interface.h"


class Logic_gui_interface;
class GUI_Port;


/**
 * Graphic connection, contains information about graphic ports, that is connecting.
 * @brief The GUI_Connection class
 */
class GUI_Connection : public QGraphicsLineItem {
    public:
        /** graphic source port of the connection */
        GUI_Port *inputPort;
        /** graphics dest port of the connection */
        GUI_Port *ouputPort;
        /** interface for model communication */
        Logic_gui_interface *interface;

        /** construtor */
        GUI_Connection(Logic_gui_interface *interface, int x1, int y1, int x2, int y2);
        /** update tooltip */
        void update_tooltip();
};


/**
 * Graphic port, contains id and graphic connection for updating lines when moved.
 * Does not contain values, they're in the logic model.
 * @brief The GUI_Port class
 */
class GUI_Port : public QGraphicsPixmapItem {
    public:
        /** identification */
        int id;        
        /** name of the port */
        std::string name;
        /** image representing port */
        QImage *image;
        /** cached port when connecting lines */
        static GUI_Port *cached_port;
        /** graphic connections with ports */
        std::vector<GUI_Connection*> connections;

        /** constructor*/
        GUI_Port(int block_id, std::string name);
};


/**
 * Graphic representation of the block
 * @brief The GUI_Block class
 */
class GUI_Block : public QGraphicsPixmapItem {
    public:
        /** identification */
        int local_id;
        /** image loaded from resources */
        QImage *image;        
        /** interface with logic model */
        Logic_gui_interface *interface;

        /** graphic input ports */
        std::vector <GUI_Port*> inputPorts;
        /** graphic output ports */
        std::vector <GUI_Port*> outputPorts;
        /** update tooltip */
        void update_tooltip();
};


// Arithmetic graphic blocks

/**
 * Graphic adder block
 * @brief The GUI_Adder class
 */
class GUI_Adder : public GUI_Block {
    public:
        /** constructor */
        GUI_Adder(int local_id, Logic_gui_interface *interface);
};

/**
 * Graphic subtractor block
 * @brief The GUI_Subtractor class
 */
class GUI_Subtractor : public GUI_Block {
    public:
        /** constructor */
        GUI_Subtractor(int local_id, Logic_gui_interface *interface);
};

/**
 * Graphic multiplier block
 * @brief The GUI_Multiplier class
 */
class GUI_Multiplier : public GUI_Block {
    public:
        /** constructor */
        GUI_Multiplier(int local_id, Logic_gui_interface *interface);
};

/**
 * Graphic divider block
 * @brief The GUI_Divider class
 */
class GUI_Divider : public GUI_Block {
    public:
        /** constructor */
        GUI_Divider(int local_id, Logic_gui_interface *interface);
};

/**
 * Graphic divider block
 * @brief The GUI_Result class
 */
class GUI_Result : public GUI_Block {
    public:
        /** constructor */
        GUI_Result(int local_id, Logic_gui_interface *interface);
};

/**
 * Graphic constant class
 * @brief The GUI_Constant class
 */
class GUI_Constant : public GUI_Block {
    public:
        /** constructor */
        GUI_Constant(int local_id, Logic_gui_interface *interface);
};


// Power plant management block

/**
 * Graphic power plant block
 * @brief The GUI_Powerplant class
 */
class GUI_Powerplant : public GUI_Block {
    public:
        /** constructor */
        GUI_Powerplant(int local_id, Logic_gui_interface *interface);
};

/**
 * Graphic better plant block
 * @brief The GUI_BetterPlant class
 */
class GUI_BetterPlant : public GUI_Block {
    public:
        /** constructor */
        GUI_BetterPlant(int local_id, Logic_gui_interface *interface);
};

/**
 * Graphic reactor cooling class
 * @brief The GUI_ReactorCooler class
 */
class GUI_ReactorCooler : public GUI_Block {
    public:
        /** constructor */
        GUI_ReactorCooler(int local_id, Logic_gui_interface *interface);
};

/**
 * Graphic result power plant
 * @brief The GUI_ResultPowerplant class
 */
class GUI_ResultPowerplant : public GUI_Block {
    public:
        /** constructor */
        GUI_ResultPowerplant(int local_id, Logic_gui_interface *interface);
};

/**
 * Common powerplant risk calculator
 * @brief The GUI_CommonRisk class
 */
class GUI_CommonRisk : public GUI_Block {
    public:
        /** constructor */
        GUI_CommonRisk(int local_id, Logic_gui_interface *interface);
};


// Market management blocks

/**
 * Graphic company block
 * @brief The GUI_Company class
 */
class GUI_Company : public GUI_Block {
    public:
        /** constructor */
        GUI_Company(int local_id, Logic_gui_interface *interface);
};

/**
 * Graphic company merge
 * @brief The GUI_CompanyMerge class
 */
class GUI_CompanyMerge : public GUI_Block {
    public:
        /** constructor */
        GUI_CompanyMerge(int local_id, Logic_gui_interface *interface);
};

/**
 * Graphic evaluator
 * @brief The GUI_Evaluator class
 */
class GUI_Evaluator : public GUI_Block {
    public:
        /** constructor */
        GUI_Evaluator(int local_id, Logic_gui_interface *interface);
};

/**
 * Graphic result company
 * @brief The GUI_ResultCompany class
 */
class GUI_ResultCompany : public GUI_Block {
    public:
        /** constructor */
        GUI_ResultCompany(int local_id, Logic_gui_interface *interface);
};

#endif // GRAPHIC_BLOCKS_H
