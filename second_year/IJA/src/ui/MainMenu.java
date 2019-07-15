/*
 * This class represents the main menu at the top of the windows.
 * Provides basic functionality over IO operations, simulation and help information.
 * @author Patrik Holop
 * @author Matej Hrabal
 */
package ui;

import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.scene.control.*;
import javafx.scene.layout.Region;
import main.IOSchemeManager;
import main.Main;
import main.Simulation;


/**
 * This class represents the main menu at the top of the windows.
 * Provides basic functionality over IO operations, simulation and help information.
 * @author Patrik Holop
 * @author Matej Hrabal
 */
public class MainMenu {

    /** Simple constructor */
    public MainMenu() {
    }

    /** This method generates the main menu
     * Main tabs are File, Simulation and Help
     * @return Generated main menu
     */
    public MenuBar generate_main_menu() {

        // new menu bar
        MenuBar menuBar = new MenuBar();

        // initializing file tab
        Menu fileMenu = new Menu("File");
        MenuItem fileNew = new MenuItem("New");

        // link new scheme with window clearance
        fileNew.setOnAction((ActionEvent event) -> {
            Main.main.clear_scene();
            event.consume();
        });

        // initialize export to file tab
        MenuItem saveFile = new MenuItem("Export to file");
        // link it with saving the scheme
        saveFile.setOnAction((ActionEvent event) -> {
            IOSchemeManager.save_scheme();
        });

        // initalize scheme opening tab
        MenuItem fileOpen = new MenuItem("Open from file");
        // link it with scheme opening
        fileOpen.setOnAction((ActionEvent event) -> {
            IOSchemeManager.load_scheme();
        });

        // initialize simulation menu
        Menu Sim = new Menu("Simulation");
        // initialize run simulation tab
        MenuItem simulation = new MenuItem("Run");
        // link it with running simulation method
        simulation.setOnAction((ActionEvent event) -> {
            Simulation.run_simulation();
        });
        // initialize one step tab and link it with simulation step
        MenuItem step = new MenuItem("Next step");
        step.setOnAction((ActionEvent event) -> {
            Simulation.update_step_scheme();
        });
        Sim.getItems().addAll(simulation, step);

        //initialize exit button
        MenuItem fileExit = new MenuItem("Exit");
        fileExit.setOnAction((ActionEvent event) -> {
            Platform.exit();
            event.consume();
        });

        // add file menu items to menu
        fileMenu.getItems().addAll(fileNew, fileOpen, saveFile, new SeparatorMenuItem(), fileExit);

        // initialize help menu
        Menu helpMenu = new Menu("Help");

        // initialize about button
        MenuItem aboutMenuItem = new MenuItem("About & Tutorial");
        aboutMenuItem.setOnAction((ActionEvent event) -> {
            // show information dialog
            Alert alert = new Alert(Alert.AlertType.INFORMATION);
            alert.setTitle("About Block Editor");
            alert.setHeaderText("Scheme creation and editing application");
            String text = new String();
            text = "Welcome to the Block Editor.\n";
            text += "This application is used for creation, editing and saving block schemes.\n\n";
            text += "Left panel: Left panel constists of 3 block types\n";
            text += "          : ART: Arithmetic blocks  -> Basic calculations with values\n";
            text += "          : PWT: Power plant blocks -> Administration of power plant\n";
            text += "          : CMP: Market blocks      -> Blocks for management of companies\n";
            text += "Feel free to load example scheme in 'examples' folder\n\n";
            text += "Tutorial:\n";
            text += "   Adding a block     : Left click on preferred block in the left panel\n";
            text += "   Adding a connection: Left click on 2 ports (input and output)\n";
            text += "   Show information   : Hold cursor over block or line\n";
            text += "   Removing a block or connection: Right click in the scheme\n";
            text += "   Run a simulation   : Choose Simulation | Run in the menu\n";
            text += "   Step a simulation  : Choose Simulation | Next step in the menu\n";
            text += "   Save a scheme      : Choose File | Export to file\n";
            text += "   Load a scheme      : Choose File | Open from file\n";
            alert.setContentText(text);
            alert.getDialogPane().setMinHeight(Region.USE_PREF_SIZE);
            alert.getDialogPane().setMinWidth(500);
            alert.showAndWait();
            event.consume();
        });
        helpMenu.getItems().addAll(aboutMenuItem);

        // add main menu item to menu
        menuBar.getMenus().addAll(fileMenu, Sim, helpMenu);
        menuBar.prefWidthProperty().bind(Main.main.root_stage.widthProperty());
        return menuBar;
    }
}

