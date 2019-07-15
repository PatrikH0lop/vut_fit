/**
 * \addtogroup logic_model
 * logic_base.h module provides general definition of logic block, port and connection,
 * the main components of the application model.
 * for schematic design.
 * @author Patrik Holop
 */


#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include <string>
#include <map>


class Block;
class Port;
class Connection;


/**
 * This class represents general logic blocks of the scheme
 * Each blocks contains information about it's ports, has identifier.
 * Each block also has execution function used for transforming input values
 * to output.
 * @brief The Block class
 */
class Block {
    public:
        /** number of created blocks during session */
        static int id;
        /** activity of the blocks, chooses the next to update block */
        bool active;
        /** identifier */
        int local_id = 0;
        /** input blocks of the block */
        std::vector<Port*> inputPorts;
        /** output blocks of the block */
        std::vector<Port*> outputPorts;
        /** name of the block */
        std::string name;

        /** Constructor of the block*/
        Block();

        /** Execution function */
        virtual void update_value();
        /** Updating function */
        virtual void update();
        /** Getter for output ports */
        std::vector<Port*> getOuputPorts();
};


/**
 * This class represents general logic port.
 * Each port contains specific name (in block), logic connections
 * to other ports and map of values.
 * @brief The Port class
 */
class Port {
    public:
        /** identifier name */
        std::string name;
        /** map of values */
        std::map<std::string,double> values;
        /** identifier of the parent block */
        int loc = 0;
        /** connections with other ports */
        std::vector<Connection*> connections;
        /** active connection */
        Connection *connection;

        /** constructor */
        Port(std::string name, float loc);
        /** getter for the name */
        std::string get_name();
        /** setter for the name */
        void set_name(std::string name);
        /** getter for the values */
        std::map<std::string,double> get_value();
        /** setter for the values */
        void set_value(std::map<std::string, double> value);
        /** getter for the connections */
        std::vector<Connection*> get_connections();
        /** creating new connection */
        Connection *create_connection(Port *second);
};


/**
 * This class represents logic connection
 * Connect 2 logic blocks and through this objects are
 * passed values between ports.
 * @brief The Connection class
 */
class Connection {
    public:
        /** identifier of the line */
        std::string name;
        /** input port (from) */
        Port *inputPort;
        /** output port (to) */
        Port *outputPort;

        /** constructor */
        Connection();
        /** setter for input port */
        void setInputPort(Port *inputPort);
        /** getter for input port */
        void setOutputPort(Port *outputPort);
};

#endif // BLOCK_H
