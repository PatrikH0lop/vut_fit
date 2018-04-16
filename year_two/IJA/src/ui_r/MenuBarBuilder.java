/**
 * Main menu bar at the top
 */
package ui_r;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.logging.Level;
import java.util.logging.Logger;
import javafx.application.Platform;
import javafx.event.ActionEvent;
import javafx.geometry.Pos;
import javafx.scene.DepthTest;
import javafx.scene.Group;
import javafx.scene.PerspectiveCamera;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.image.ImageView;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyCodeCombination;
import javafx.scene.input.KeyCombination;
import javafx.scene.layout.VBox;
import javafx.scene.text.Text;
import javafx.stage.FileChooser;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.stage.StageStyle;
import main_r.Main;

/**
 * Main menu of the application
 * Top menu
 */
public class MenuBarBuilder {

    private final Main main;

    public MenuBarBuilder(final Main main) {
        this.main = main;
    }

    /** Declare menu items and submenus*/
    public MenuBar buildMenuBarWithMenus() {
        final MenuBar menuBar = new MenuBar();

        Menu fileMenu = new Menu("File");
        MenuItem fileNew = new MenuItem("New");

        /** Clicking New schme clears the scheme */
        fileNew.setOnAction((ActionEvent event) -> {
            main.initschematic();
            event.consume();
        });

        /** Opening scheme from file */
        MenuItem saveFile = new MenuItem("Export to file");
        saveFile.setOnAction((ActionEvent event) -> {
            main.save_scheme();
        });

        MenuItem fileOpen = new MenuItem("Open from file");
        fileOpen.setOnAction((ActionEvent event) -> {
            main.load_scheme();
        });

        Menu Sim = new Menu("Simulation");
        MenuItem simulation = new MenuItem("Run");
        simulation.setOnAction((ActionEvent event) -> {
            main.buildAndSetLoop();
        });
        MenuItem step = new MenuItem("Next step");
        step.setOnAction((ActionEvent event) -> {
            main.update_step_scheme();
        });

        Sim.getItems().addAll(simulation, step);

        /** Exits the application */
        MenuItem fileExit = new MenuItem("Exit");
        fileExit.setOnAction((ActionEvent event) -> {
            Platform.exit();
            event.consume();
        });

        fileExit.setGraphic(new ImageView(ImageManager.getHmImage("exiticon")));
        /** add Items to menu */
        fileMenu.getItems().addAll(fileNew, fileOpen, saveFile, new SeparatorMenuItem(), fileExit);

        /** Help menu */
        Menu helpMenu = new Menu("Help");

        MenuItem searchMenuItem = new MenuItem("Search");
        searchMenuItem.setDisable(true);

        MenuItem onlineManualMenuItem = new MenuItem("Online Manual");
        onlineManualMenuItem.setVisible(false);

        MenuItem aboutMenuItem = new MenuItem("Help");
        aboutMenuItem.setOnAction((ActionEvent event) -> {
            Alert alert = new Alert(Alert.AlertType.INFORMATION);
            alert.setTitle("About Block Editor");
            alert.setHeaderText("The best solution for the problems you never had");
            alert.setContentText("Welcome to the best Block Editor.\nClick on block (left panel) -> adds block\nClick on two ports -> joins the ports\nTo run simulation choose Simulation|Run\nIzy, gg2\nAnd we are also making the world a better place.");
            alert.showAndWait();
            event.consume();
        });
        aboutMenuItem.setAccelerator(new KeyCodeCombination(KeyCode.F1, KeyCombination.SHORTCUT_ANY));

        helpMenu.getItems().addAll(aboutMenuItem);

        /** Adds all menus to bar */
        menuBar.getMenus().addAll(fileMenu, Sim, helpMenu);
        menuBar.prefWidthProperty().bind(main.primaryStage.widthProperty());
        return menuBar;
    }

    private void init(Group root, Stage primaryStage) {
        root.setDepthTest(DepthTest.ENABLE);
        primaryStage.getScene().setCamera(new PerspectiveCamera());
    }

}

