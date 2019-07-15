/** \addtogroup graphic_interface
 * scheme.h module provides schematic view Scheme, object where the graphic
 * blocks are instantiated and edited by user. This object is also responsible
 * for drawing and updating the scene.
 * @author Patrik Holop
 */


#ifndef SCHEME_H
#define SCHEME_H


#include <iostream>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QLineF>
#include <QPointF>
#include <QLineF>
#include <QGraphicsLineItem>
#include <map>
#include "logic_gui_interface.h"
#include "graphic_blocks.h"


class Logic_gui_interface;


/**
 * Scheme class represent schematic view, the main
 * canvas of the application responsible for updating the scene
 * and storing the graphic objects
 * @brief The Scheme class
 */
class Scheme : public QGraphicsView
{
    Q_OBJECT
    public:
        /** interface for model updating */
        Logic_gui_interface *interface;
        /** graphic drawn canvas */
        QGraphicsScene *canvas;
        /** constructor */
        Scheme(Logic_gui_interface *interface);

    public slots:
        /** slot activated when block is selected */
        void selected();
        /** slot activated when block is moved */
        void moved();
        /** slot activated when removing an element */
        void keyReleaseEvent(QKeyEvent *keyEvent);
};

#endif // SCHEME_H
