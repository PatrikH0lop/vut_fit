/** \addtogroup logic_gui_interface
 * logic_gui_interface.h module provides function that require both graphic and logic model
 * and established communication between them
 * @author Patrik Holop
 * @author Matej Hrabal
 */


#ifndef LOGIC_GUI_INTERFACE_H
#define LOGIC_GUI_INTERFACE_H


#include <QObject>
#include <QFileDialog>
#include <sstream>
#include <QTimer>
#include <QMessageBox>
#include <QInputDialog>
#include <string>
#include <QGraphicsItem>
#include "simulation.h"
#include "scheme.h"
#include "logic_blocks.h"
#include "graphic_blocks.h"


class Scheme;
class GUI_Block;
class GUI_Port;
class GUI_Connection;


/**
 * This class represents graphic and logic interface
 * @brief The Logic_gui_interface class
 */
class Logic_gui_interface : public QObject {
    Q_OBJECT
    public:
        /** logic model */
        Simulation *simulation;
        /** graphic model */
        Scheme *scheme;
        /** to update block */
        unsigned int to_update;
        /** activity circle of active block */
        QGraphicsEllipseItem *active_circle;
        /** timer used for time dependent functions */
        QTimer *timer;
        /** number of finished blocks */
        unsigned int count;

        /** Constructor */
        Logic_gui_interface();
        /** Add block to both models */
        void add_block(std::string type, bool load, int id, int x, int y, std::map<std::string, double> values);
        /** reset the id the block */
        void reset_id(Block *b, int new_id);
        /** find block position in logic model */
        int find_block_position(int local_id);
        /** find block position in graphic model */
        int find_gui_block_position(int local_id);
        /** remove block */
        void remove_block(int local_id);
        /** update block */
        void update_block(int local_id);
        /** update the active block */
        void update_active_block();
        /** create new connection */
        void add_connection(int local_id_in, std::string port_name_in, int local_id_out, std::string port_name_out);
        /** remove connection */
        void remove_connection(int local_id, std::string name, int local_id2, std::string name2);
        /** find connection position in logic model */
        int find_connetion_pos(int local_id, std::string name, int local_id2, std::string name2);
        /** find connection position in port */
        int find_port_connetion_pos(Port *p, int local_id, std::string name, int local_id2, std::string name2);
        /** find connection position in graphic model */
        int find_gui_connection_position(int local_id, std::string name, int local_id2, std::string name2);
        /** find connection postition in graphic port */
        int find_gui_port_connection_position(GUI_Port *p, int local_id, std::string name, int local_id2, std::string name2);
        /** run the simulation */
        void run_simulation();
        /** find block in the logic model */
        Block *find_model_block(int local_id);
        /** find block in graphic model */
        GUI_Block *find_gui_block(int local_id);
        /** find graphic port */
        GUI_Port * find_gui_port(int local_id, std::string name);
        /** find logic port */
        Port *find_model_port(int local_id, std::string name);
        /** find graphic connection */
        GUI_Connection *find_gui_connection(int local_id, std::string name, int local_id2, std::string name2);
        /** get port values */
        std::map<std::string, double> get_port_values(int local_id, std::string name);
        /** get values represented in tooltips */
        std::string get_tooltip_values(int local_id);
        /** generate tooltip for the current port */
        std::string get_port_tooltip(int local_id, std::string name);
        /** set new value in logic model */
        void set_model_value(int local_id, std::string name, std::string key, double value);
        /** ask user for unassigned values */
        void add_unassigned_values();
        /** ask user for one unassigned value */
        void add_unassigned_value(std::pair<std::string, double> &x, std::string name, std::string port);

    public slots:
        /** slot triggered by user to run simulation */
        void user_simulation();        
        /** slot triggered by user to next simulation step */
        void user_step_sim();
        /** slot triggered by user to next simulation step */
        void user_step();
        /** slot triggered by user to clear the current scheme */
        void clear_scene();
        /** save the current scheme */
        void save_schmeme();
        /** load the saved scheme */
        void load_scheme();
        /** clear */
        void clear();
};

#endif // LOGIC_GUI_INTERFACE_H
