package main;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;
import simulation.Simulation;


/**
 * Main program, currently for testing simulation
 */
public class Main extends Application {

    @Override
    public void start(Stage primaryStage) throws Exception {
        System.out.println("Start");
        FXMLLoader rootF = new FXMLLoader(getClass().getResource("../ui/ui_design.fxml"));
        Parent root = rootF.load();
        primaryStage.setTitle("Block Editor");
        primaryStage.setResizable(false);
        primaryStage.setScene(new Scene(root, 700, 500));
        primaryStage.show();
    }

    /*public static void main(String[] args) {
        Simulation sim = new Simulation();
        sim.execute_simulation();
    }*/
}
