/*
 * Main class of the Block Editor project
 * This class represents the main application windows as well as
 * all collection of all active objects of scheme.
 * @author Matěj Hrabal
 */
package main;

import java.util.ArrayList;
import java.util.List;

import javafx.application.Application;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.MenuBar;
import javafx.scene.control.TabPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Rectangle;
import javafx.stage.Stage;
import logic.base.Block;
import logic.connections.Connection;
import ui.ImageManager;
import ui.MainMenu;
import ui.SchemeWindow;
import ui.BlockMenu;


/**
 * Main class of the Block Editor project
 * <p> This class represents the main application windows as well as
 * all collection of all active objects of scheme.
 * @author Matěj Hrabal
 */
public class Main extends Application {

    public static Main main;

    /** Width of the main window */
    int window_width = 900;

    /** Height of the main window */
    int window_height = 500;

    /** Main windows stage */
    public Stage root_stage;

    /** Circle indicating active block during enumeration */
    public Circle active_circle = null;

    /** All created blocks in active scheme */
    public List<Block> blocks;
    /** All created connections in active scheme */
    public List<Connection> connections;

    /** Main container for the menu a scheme with sidebar */
    public VBox main_container;

    /** Schematic drawable group */
    public Group scheme_group;    // this is where scematicRectangle and Gates will be grouped

    /** Group of all drawable objects like blocks and lines */
    public Group all_drawable_objects;         // this one is for all gates

    /** Ignored main method
     * @param args arguments from command line
     **/
    public static void main(String[] args) {
        launch(args);
    }

    /** Implenting Application method starting the application
     * @param root_window Main stage of window
     **/
    @Override
    public void start(Stage root_window) throws Exception {
        main = this;

        //initialize image databaze
        ImageManager.load_cache();

        // assign root stage
        this.root_stage = root_window;

        // set basic attributes of the window
        root_window.setTitle("Block Editor");
        root_window.setResizable(false);

        // for menu and the rest
        main_container = new VBox();
        // for block menu and scheme
        HBox rootHBox = new HBox();

        // creating groups
        scheme_group = new Group();
        all_drawable_objects = new Group();
        scheme_group.getChildren().add(all_drawable_objects);
        clear_scene();

        // menu creation
        MainMenu main_menu_obj = new MainMenu();
        MenuBar main_menu = main_menu_obj.generate_main_menu();
        main_container.getChildren().add(main_menu);

        // block menu creation
        BlockMenu block_menu = new BlockMenu();
        TabPane sideBar = block_menu.generate_block_box();
        rootHBox.getChildren().add(sideBar);

        // scheme window creation
        SchemeWindow schematicRectangle = new SchemeWindow();
        Rectangle scheme_window = schematicRectangle.rectangle;
        scheme_group.getChildren().add(scheme_window);
        scheme_window.toBack();
        rootHBox.getChildren().add(scheme_group);
        main_container.getChildren().add(rootHBox);

        // whole scene object
        Scene scene = new Scene(main_container, window_width, window_height);
        root_window.setScene(scene);

        //show the main window
        root_stage.show();
    }

    /** Remove everything from scheme and create blank scheme */
    public void clear_scene() {
        this.blocks = new ArrayList();
        this.connections = new ArrayList();
        all_drawable_objects.getChildren().clear();
    }
}



