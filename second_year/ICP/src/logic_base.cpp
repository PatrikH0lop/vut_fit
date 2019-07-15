/**
 * Implementation of the general logic components
 * @author Patrik Holop
 */


#include "logic_base.h"


// initialize block id for the session
int Block::id = 0;


/**
 * Simple constructor of the block
 * @brief Block::Block
 */
Block::Block() {
    // set the id and generic name
    this->local_id = this->id;
    this->name = "the_block";
    this->active = false;
}

/**
 * Execution function is supposed to be overridet
 * in specific blocks
 */
void Block::update_value() {
    // pass
}

/**
 * Update the value, calls the overriding functions
 * @brief Block::update
 */
void Block::update() {
    this->update_value();
}

/**
 * Getter for the ouput ports of the block
 * @brief Block::getOuputPorts
 * @return Vector of output ports
 */
std::vector<Port*> Block::getOuputPorts() {
    return this->outputPorts;
}


/**
 * Constructor of connection.
 * Initializes generic name
 * @brief Connection::Connection
 */
Connection::Connection() {
    this->name = "line";
}

/**
 * Setter for input port
 * @brief Connection::setInputPort
 * @param inputPort New input port
 */
void Connection::setInputPort(Port *inputPort) {
    this->inputPort = inputPort;
}

/**
 * Setter for output port
 * @brief Connection::setOutputPort
 * @param outputPort New output port
 */
void Connection::setOutputPort(Port *outputPort) {
    this->outputPort = outputPort;
}


/**
 * Constructor of the port
 * @brief Port::Port
 * @param name Name of the block (block specific)
 * @param loc Identifier of parent block
 */
Port::Port(std::string name, float loc) {
    this->loc = loc;
    this->name = name;
}

/**
 * Getter for the name
 * @brief Port::get_name
 * @return Name of the port
 */
std::string Port::get_name() {
    return this->name;
}

/**
 * Setter for the name
 * @brief Port::set_name
 * @param name New name
 */
void Port::set_name(std::string name) {
    this->name = name;
}

/**
 * Getter for the values
 * @brief Port::get_value
 * @return Values
 */
std::map<std::string,double> Port::get_value() {
    return this->values;
}

/**
 * Setter for the values
 * @brief Port::set_value
 * @param value New map of values
 */
void Port::set_value(std::map<std::string, double> value) {
    this->values = value;
}


/**
 * Getter for the connections
 * @brief Port::get_connections
 * @return Connection of the current port
 */
std::vector<Connection*> Port::get_connections() {
    return this->connections;
}

/**
 * Creates new connection
 * @brief Port::create_connection
 * @param second Second port to be connected to
 * @return New connection
 */
Connection *Port::create_connection(Port *second) {
    Connection *connection = new Connection();
    // set input and output ports
    connection->name = "con";
    connection->inputPort = this;
    connection->outputPort = second;
    this->connections.push_back(connection);
    second->connections.push_back(connection);
    return connection;
}
