/** \addtogroup graphic_interface
 * Main application module, provides main function.
 * Generated basic UI interface and initialize the application
 * @author Matej Hrabal
 */


#ifndef MAIN_H
#define MAIN_H


#include <QApplication>
#include <QtGui>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenu>
#include <QObject>
#include <QMainWindow>
#include <QMenuBar>
#include <QTabBar>
#include <QTabWidget>
#include <QLabel>
#include <QToolButton>
#include <QMessageBox>
#include <iostream>
#include <vector>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QObject>
#include "graphic_menus.h"
#include "scheme.h"
#include "simulation.h"
#include "logic_gui_interface.h"


/**
 * Main class represents runnable application
 * @brief The Main class
 */
class Main : public QMainWindow {
    public:
        /** main window width */
        int window_width = 900;
        /** main window height */
        int window_height = 500;
        /** main menu (top) */
        QMenuBar *menu;
        /** main container with menu and other components */
        QVBoxLayout *main_container;
        /** left side block menu and scheme container */
        QHBoxLayout *rootHBox;
        /** logic-gui interface */
        Logic_gui_interface *logic_gui_interface;

        /** Constructor */
        Main();        
        /** Initializes GUI for the block menu */
        void initialize_block_menu();
        /** Initializes top main menu */
        void initialize_main_manu();

    public slots:
        /** slot trigger by tutorial in main menu */
        void show_help();
};


/**
 * This class is used as singleton for instantiation of the main graphic application
 * If graphic window already exists, it does not instantiate new instance
 * @brief The ApplicationManager class
 */ 
class ApplicationManager {
    private:
	Main *application;
    public:
	ApplicationManager();
	Main *instantiate_main_application();
};


#endif // MAIN_H
