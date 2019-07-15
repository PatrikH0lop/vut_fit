/**
 * Implementation of the main application
 * @author Matej Hrabal
 */


#include "main.h"


/**
 * Simple constructor for singleton application manager
 * @brief ApplicationManager::ApplicationManager
 */ 
ApplicationManager::ApplicationManager() {
    this->application = NULL;
}


/**
 * Instantiate new application. If the application is already instantiated,
 * return existing instance
 * @brief ApplicationManager::instantiate_main_application
 * @return Instance of the main graphic application
 */
Main *ApplicationManager::instantiate_main_application() {
    if (application == NULL) {
	this->application = new Main();
    }
    return this->application;
}


/**
 * Constructor of the main application window
 * @brief Main::Main
 */
Main::Main() {
    // set basic graphic attributes
    this->setWindowTitle("Block Editor");
    this->setFixedSize(window_width, window_height);

    // set main containers and fill theme
    main_container = new QVBoxLayout();
    rootHBox = new QHBoxLayout();

    this->logic_gui_interface = new Logic_gui_interface();
    // add block menu
    initialize_block_menu();
    rootHBox->addWidget(logic_gui_interface->scheme);

    // init layout
    QLabel *a = new QLabel("Block Menu");
    main_container->addWidget(a);
    main_container->addLayout(rootHBox);

    // init main menu
    initialize_main_manu();

    // set layout
    QWidget *window = new QWidget();
    window->setLayout(main_container);
    setCentralWidget(window);
}


/**
 * Initalizes block menu. Creates all menus required for block generating.
 * @brief Main::initialize_block_menu
 */
void Main::initialize_block_menu() {
    ArithmeticBlockMenu *arithmetic = new ArithmeticBlockMenu(this->logic_gui_interface);
    PowerplantBlockMenu *powerplant = new PowerplantBlockMenu(this->logic_gui_interface);
    MarketBlockMenu *market = new MarketBlockMenu(this->logic_gui_interface);

    QTabWidget *tabBar = new QTabWidget();
    tabBar->addTab(arithmetic, "ART");
    tabBar->addTab(powerplant, "PWT");
    tabBar->addTab(market, "CMP");
    tabBar->setMaximumWidth(170);
    rootHBox->addWidget(tabBar);
}


/**
 * Initializes main menu (top) and connect correct action to their slots
 * @brief Main::initialize_main_manu
 */
void Main::initialize_main_manu() {
    // init file menu
    QMenu *menuFile = this->menuBar()->addMenu(tr("File"));
    QAction *quit = new QAction("New", this);
    menuFile->addAction(quit);
    connect(quit, &QAction::triggered, this->logic_gui_interface, &Logic_gui_interface::clear);

    QAction *exp = new QAction("Export to file", this);
    menuFile->addAction(exp);
    connect(exp, &QAction::triggered, this->logic_gui_interface, &Logic_gui_interface::save_schmeme);

    QAction *load = new QAction("Load from file", this);
    menuFile->addAction(load);
    connect(load, &QAction::triggered, this->logic_gui_interface, &Logic_gui_interface::load_scheme);

    QAction *exit = new QAction("Exit", this);
    menuFile->addAction(exit);
    connect(exit, &QAction::triggered, qApp, QApplication::quit);

    // init simulation
    QMenu *simulation = this->menuBar()->addMenu(tr("Simulation"));
    QAction *run = new QAction("Run", this);
    simulation->addAction(run);
    connect(run, &QAction::triggered, this->logic_gui_interface, &Logic_gui_interface::user_simulation);

    QAction *step = new QAction("Next step", this);
    simulation->addAction(step);
    connect(step, &QAction::triggered, this->logic_gui_interface, &Logic_gui_interface::user_step);

    // init help
    QMenu *aboutMenu = this->menuBar()->addMenu(tr("About"));
    QAction *about = new QAction("About | Tutorial", this);
    aboutMenu->addAction(about);
    connect(about, &QAction::triggered, this, &Main::show_help);
}

/**
 * Show help and tutorial dialog
 * @brief Main::show_help
 */
void Main::show_help() {
    std::string text;
     text = "Welcome to the Block Editor.\n";
     text += "This application is used for creation, editing and saving block schemes.\n\n";
     text += "Left panel: Left panel constists of 3 block types\n";
     text += "  : ART: Arithmetic blocks  -> Basic calculations with values\n";
     text += "  : PWT: Power plant blocks -> Administration of power plant\n";
     text += "  : CMP: Market blocks      -> Management of companies\n";
     text += "Feel free to load example scheme in 'examples' folder\n\n";
     text += "Tutorial:\n";
     text += "  Adding a block     : Left click on preferred block (left panel)\n";
     text += "  Adding connection: Left click on 2 ports (input and output)\n";
     text += "  Show information   : Hold cursor over block or line\n";
     text += "  Removing a block or connection: Select item and press Delete\n";
     text += "  Run a simulation   : Choose Simulation | Run in the menu\n";
     text += "  Step a simulation  : Choose Simulation | Next step in the menu\n";
     text += "  Save a scheme      : Choose File | Export to file\n";
     text += "  Load a scheme      : Choose File | Load from file\n";
     QMessageBox messageBox;
     messageBox.information(this, "About", QString::fromStdString(text));
     return;
}


/**
 * Example function of the logic model (without gui)
 * @brief simulate
 * @param simulation Testing simulation
 */
void simulate() {
    Simulation *simulation = new Simulation();

    std::cout << "Adding constant (5), constant(10), adder and result block" << std::endl;
    simulation->blocks.push_back(new Constant());
    simulation->blocks.push_back(new Constant());
    simulation->blocks.push_back(new Adder());
    simulation->blocks.push_back(new Result());

    auto p1 = simulation->blocks[0]->outputPorts[0]->values.find("value");
    auto p2 = simulation->blocks[1]->outputPorts[0]->values.find("value");
    p1->second = 5;
    p2->second = 10;

    std::cout << "Connecting contants to adder and adder to result" << std::endl;
    simulation->blocks[0]->outputPorts[0]->create_connection(simulation->blocks[2]->inputPorts[0]);
    simulation->blocks[1]->outputPorts[0]->create_connection(simulation->blocks[2]->inputPorts[1]);
    simulation->blocks[2]->outputPorts[0]->create_connection(simulation->blocks[3]->inputPorts[0]);

    std::cout << "Running simulation" << std::endl;
    if (simulation->check_cycles()) {
        std::cout << "Simulation contains cycles" << std::endl;
    } else {
        std::cout << "Simulation does not contain cycles" << std::endl;
    }
    simulation->sort_blocks();
    for (Block *b : simulation->blocks) {
        std::cout << "Updating value: " << b->name << std::endl;
        b->update_value();
    }
    std::cout << "Result value: " << simulation->blocks[3]->inputPorts[0]->values.find("value")->second << std::endl;
}


/**
 * Main function of the application
 * @brief main
 * @param argc number of command line params
 * @param argv params
 * @return graphic application
 */
int main(int argc, char *argv[]) {
    ApplicationManager manager;
    if (argc == 2 && strcmp(argv[1], "--backend") == 0) {
        simulate();
    } else {
        QApplication app (argc, argv);
        Main *main = manager.instantiate_main_application(); //new Main();        
        main->show();
        return app.exec();
    }
    return 0;
}

