/**
 * Implementation of the graphic scheme
 * @author Patrik Holop
 */


#include "scheme.h"


/**
 * Scheme constructor
 * @brief Scheme::Scheme
 * @param interface logic-gui interface
 */
Scheme::Scheme(Logic_gui_interface *interface) {
    this->interface = interface;
    // create new canvas
    canvas = new QGraphicsScene();
    this->setScene(canvas);
    this->setBackgroundBrush(QBrush(QColor(255,255,191), Qt::SolidPattern));
    // create connections to slots
    QObject::connect(this->canvas, SIGNAL(selectionChanged()), this, SLOT(selected()));
    QObject::connect(this->canvas, SIGNAL(changed(QList<QRectF>)), this, SLOT(moved()));
}


/**
 * Get the key release event to remove the block or connection
 * @brief Scheme::keyReleaseEvent
 * @param event Event trigger
 */
void Scheme::keyReleaseEvent(QKeyEvent *event) {
    // when user pressed delete
    if (event->key() == Qt::Key_Delete) {
        // when the block was selected
        QList<QGraphicsItem*> selectedItems = this->canvas->selectedItems();
        foreach (QGraphicsItem *item, selectedItems) {
            // remove blocks
            GUI_Block *b = dynamic_cast<GUI_Block*> (item);
            if (b != NULL) {
                this->interface->remove_block(b->local_id);
            }
            // remove connections
            else {
                GUI_Connection *c = dynamic_cast<GUI_Connection*> (item);
                if (c != NULL) {
                    this->interface->remove_connection(c->inputPort->id, c->inputPort->name, c->ouputPort->id, c->ouputPort->name);
                }
            }
        }
    }
}


/**
 * When movable object was moved
 * @brief Scheme::moved
 */
void Scheme::moved() {    
    if (this->canvas->selectedItems().size() > 0) {
        // try if the movable object was block
        GUI_Block *p = dynamic_cast<GUI_Block*> (this->canvas->selectedItems()[0]);
        if (p == NULL) {
            return;
        } else {
            // if it was, update lines
            for (GUI_Port *port : p->inputPorts) {
                for (GUI_Connection *connection : port->connections) {
                    QLineF line = connection->line();
                    int x1 = connection->inputPort->x() + connection->inputPort->parentItem()->pos().x();
                    int y1 = connection->inputPort->y() + connection->inputPort->parentItem()->pos().y();
                    int x2 = connection->ouputPort->x() + connection->ouputPort->parentItem()->pos().x();
                    int y2 = connection->ouputPort->y() + connection->ouputPort->parentItem()->pos().y();
                    line.setP1(QPointF(x1, y1));
                    line.setP2(QPointF(x2, y2));
                    line.setP2(QPointF(x2, y2));
                    connection->setLine(line);
                }
            }
            for (GUI_Port *port : p->outputPorts) {
                for (GUI_Connection *connection : port->connections) {
                    QLineF line = connection->line();
                    int x1 = connection->inputPort->x() + connection->inputPort->parentItem()->pos().x();
                    int y1 = connection->inputPort->y() + connection->inputPort->parentItem()->pos().y();
                    int x2 = connection->ouputPort->x() + connection->ouputPort->parentItem()->pos().x();
                    int y2 = connection->ouputPort->y() + connection->ouputPort->parentItem()->pos().y();
                    line.setP1(QPointF(x1, y1));
                    line.setP2(QPointF(x2, y2));
                    line.setP2(QPointF(x2, y2));
                    connection->setLine(line);
                }
            }
        }
    }
}


template <typename Map>
/**
 * Help function to compare the map keys equality
 * @brief compare_keys_equality
 * @param f First map
 * @param s Second map
 * @return True is the key sets are equal
 */
bool compare_keys_equality (Map const &f, Map const &s) {

    auto pred = [] (decltype(*f.begin()) a, decltype(a) b)
                   { return a.first == b.first; };
    return f.size() == s.size() && std::equal(f.begin(), f.end(), s.begin(), pred);
}


/**
 * Get the selection signal to connect 2 ports
 * @brief Scheme::selected
 */
void Scheme::selected() {
    if (this->canvas->selectedItems().size() > 0) {
        // if the selected item is port
        GUI_Port *p = dynamic_cast<GUI_Port*> (this->canvas->selectedItems()[0]);
        if (p == NULL) {
            return;
        } else {
            // if it was cached already
            if (GUI_Port::cached_port != NULL) {
                GUI_Block *b = dynamic_cast<GUI_Block*> (p->parentItem());
                if (b == NULL) {
                    GUI_Port::cached_port = NULL;
                    return ;
                }
                else {
                    // create connection
                    bool found = false;
                    for (GUI_Port *port : b->outputPorts) {
                        if (port == p) {
                            found = true;
                        }
                    }
                    if (!found) {
                        if (p->id == GUI_Port::cached_port->id) {
                            std::cout << "SAME" << std::endl;
                            GUI_Port::cached_port = NULL;
                            return;
                        }
                        if (!compare_keys_equality(this->interface->get_port_values(p->id, p->name), this->interface->get_port_values(GUI_Port::cached_port->id, GUI_Port::cached_port->name))) {
                            std::cout << "DIFF" << std::endl;
                            GUI_Port::cached_port = NULL;
                            return;
                        }
                        if (p->connections.size() != 0) {
                            GUI_Port::cached_port = NULL;
                            return ;
                        }
                        this->interface->add_connection(GUI_Port::cached_port->id, GUI_Port::cached_port->name, p->id, p->name);
                        GUI_Port::cached_port = NULL;
                    }
                }
            } else {
                // else cache the port
                GUI_Block *b = dynamic_cast<GUI_Block*> (p->parentItem());
                if (b == NULL) {
                    return ;
                }
                else {
                    bool found = false;
                    for (GUI_Port *port : b->inputPorts) {
                        if (port == p) {
                            found = true;
                        }
                    }
                    if (!found) {
                        GUI_Port::cached_port = p;
                        std::cout << "INPUT" << std::endl;
                    } else {
                        std::cout << "OUTPUT" << std::endl;
                    }
                }
            }
        }
    }
}
