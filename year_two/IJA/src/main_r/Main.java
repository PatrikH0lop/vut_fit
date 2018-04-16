/**
 * Main Application
 */

package main_r;

import java.awt.*;
import java.io.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javafx.animation.Animation;
import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.application.Application;
import javafx.event.Event;
import javafx.event.EventHandler;
import javafx.geometry.Point2D;
import javafx.scene.Group;
import javafx.scene.ImageCursor;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.MenuBar;
import javafx.scene.control.Tooltip;
import javafx.scene.image.Image;
import javafx.scene.layout.HBox;
import javafx.scene.layout.VBox;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.scene.shape.Line;
import javafx.scene.shape.Rectangle;
import javafx.stage.FileChooser;
import javafx.stage.Stage;
import javafx.util.Duration;
import logic_r.base.Block;
import logic_r.base.Port;
import logic_r.blocks.arithmetic.*;
import logic_r.connections.Connection;
import ui_r.ImageManager;
import ui_r.MenuBarBuilder;
import ui_r.SchematicRectangle;
import ui_r.SideBarBuilder;

import javax.swing.*;

import static javax.script.ScriptEngine.FILENAME;


/**
 * Main class of the Block Editor
 * Runs application
 */
public class Main extends Application {

    public static Main main;

    /** Size of the main window */
    public int mainHeight = 500;
    public int mainWidth = mainHeight * 16 / 9;

    /** Size of the scheme */
    public int schematicWidth = mainWidth - 50;
    public int schematicHeigth = 500;

    /** Initial drop object coordinates */
    public double initX;
    public double initY;
    public Point2D dragAnchor;
    public Circle circ = null;

    /** Every created object in scheme */
    public List<Block> blocks;
    public List<Line> lines;
    public List<Connection> connections;

    /** Update loop objects */
    public Stage primaryStage;
    Timeline timeline;

    /** UI Main containers */
    public VBox rootGroup;          // contains menuBar and rootHBox
    public MenuBar menuBar;         // an most excelent menubar
    public HBox rootHBox;           // contains sideBar and rootVBox
    public VBox rootVBox;           // contains rectangle and console
    public VBox sideBar;            // contains sidebar items
    public Group schematicGroup;    // this is where scematicRectangle and Gates will be grouped
    public Group gateGroup;         // this one is for all gates

    /** Scheme window */
    public SchematicRectangle schematicRectangle;


    /** Initialize application */
    private void init(Stage primaryStage) {
        this.primaryStage = primaryStage;
        primaryStage.setTitle("Block Editor v2");
        primaryStage.setResizable(false);

        rootGroup = new VBox(2);        // contains menuBar and rootHBox
        rootHBox = new HBox(2);         // contains sideBar and rootVBox
        rootHBox.setSpacing(2f);
        rootVBox = new VBox(2);         // contains rectangle and console
        rootVBox.setSpacing(5f);
        schematicGroup = new Group();      // schematicGroup (atm gateGroup)
        gateGroup = new Group();
        schematicGroup.getChildren().add(gateGroup);
        initschematic();

        MenuBarBuilder classyMenuBar = new MenuBarBuilder(this);
        menuBar = classyMenuBar.buildMenuBarWithMenus();
        rootGroup.getChildren().add(menuBar);

        SideBarBuilder classySideBarBuilder = new SideBarBuilder(this);
        sideBar = classySideBarBuilder.buildSideBarWithButtons();
        rootHBox.getChildren().add(sideBar);

        schematicRectangle = new SchematicRectangle(this);
        Rectangle schemrect = schematicRectangle.getRectangle();
        schematicGroup.getChildren().add(schemrect);
        Group grid = schematicRectangle.drawGrid(this);
        schematicGroup.getChildren().add(grid);
        grid.toBack();          // put grid behind everything
        schemrect.toBack();     // put schematic rectangle behind everything

        rootVBox.getChildren().add(schematicGroup);
        rootHBox.getChildren().add(rootVBox);
        rootGroup.getChildren().add(rootHBox);

        Scene scene = new Scene(rootGroup, mainWidth, mainHeight);
        primaryStage.setScene(scene);
        Image defaultCursorImage = ImageManager.getHmImage("cursor");
        ImageCursor imageCursor = new ImageCursor(defaultCursorImage, -defaultCursorImage.getWidth(), -defaultCursorImage.getHeight());
        scene.setCursor(imageCursor);
    }



    /** find cycles in scheme*/
    public boolean containsCycles() {
        for (Block b : blocks) {

            ArrayList<Float> visited = new ArrayList<>();
            ArrayList<Float> closed = new ArrayList<>();

            // add starting point
            visited.add(b.local_id);
            //System.out.println("BFS FOR POINT " + b.local_id);

            // while not every point was visited
            while (!visited.isEmpty()) {

                //System.out.println("Starting for point: " + b.local_id);
                //System.out.println("Visited: " + visited);
                //System.out.println("Closed:  " +closed);

                // get the first point and add it to closed
                float id = visited.get(0);
                visited.remove(0);
                closed.add(id);

                Block processed = null;
                for (Block bl: blocks) {
                    if (bl.local_id == id) {
                        processed = bl;
                    }
                }

                // add all outer connections to visited
                for (Port x: processed.outputPorts) {
                    for (Connection c: x.connections) {
                        float output_loc = c.outputPort.loc;
                        if (closed.contains(output_loc)) {
                            //System.out.println("Found cycle with output block " + output_loc);
                            return true;
                        }
                        visited.add(output_loc);
                    }
                }
            }
        }
        return false;
    }


    /** Updates the scheme each 1 second */
    public final void buildAndSetLoop() {

        boolean has_cycle = containsCycles();

        if (has_cycle == true) {
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle("Block Editor Error");
            alert.setHeaderText("Cycle detected");
            alert.setContentText("In current scheme was detected cycle!\nPlease make necessary changes and try again.");
            alert.showAndWait();
        }
        else {
            Timeline timeline = new Timeline(new KeyFrame(Duration.millis(1000), ae -> update_step_scheme()));
            timeline.setCycleCount(this.blocks.size());
            timeline.play();
        }
    }

    /** updates on step */
    public void update_step_scheme() {
        Block to_update = null;
        int pos = 0;
        int index = 0;
        for (Block next : blocks) {
            if (next.active) {
                to_update = next;
                pos = index;
            }
            index++;
        }
        if (to_update == null) {
            to_update = blocks.get(0);
        }

        this.gateGroup.getChildren().remove(circ);
        circ = new Circle((int)to_update.group.getTranslateX(), (int)to_update.group.getTranslateY(), 5, Color.GREEN);
        this.gateGroup.getChildren().add(circ);

        to_update.update(0);
        for (Connection next : connections) {
            if (next != null) {
                if (next.inputPort.loc == to_update.local_id) {
                    next.update(new Long(0));
                    Block output_block= null;
                    double loc = next.outputPort.loc;
                    for (Block output: blocks) {
                        if (loc == output.local_id) {
                            output.update(0);
                        }
                    }
                }
            }
        }
        if (pos+1 < this.blocks.size()) {
            to_update.active = false;
            blocks.get(pos+1).active = true;
        }
        else {
            to_update.active = false;
            blocks.get(0).active = true;
        }
    }

    /** Remove everything from scheme */
    public void initschematic() {
        if (gateGroup.getChildren().size() > 0) {
            gateGroup.getChildren().remove(0, gateGroup.getChildren().size());
        }
        /* reset all lists*/
        blocks = new ArrayList();
        lines = new ArrayList();
        connections = new ArrayList();
    }

    public void destroy() {
        timeline.stop();
    }

    @Override
    public void start(Stage primaryStage) throws Exception {
        main = this;
        ImageManager.initMap();
        init(primaryStage);
        //buildAndSetLoop();
        primaryStage.show();
    }

    public void save_scheme() {
        FileChooser fileChooser = new FileChooser();
        fileChooser.setTitle("Open Resource File");
        FileChooser.ExtensionFilter extFilter = new FileChooser.ExtensionFilter("Scheme file (.scheme)", "*.scheme");
        fileChooser.getExtensionFilters().add(extFilter);
        File file = fileChooser.showSaveDialog(this.primaryStage);

        System.out.println("SAVING SCHEME");
        try {
            PrintWriter writer = new PrintWriter(file.getAbsoluteFile(), "UTF-8");

            /** save blocks */
            writer.write(blocks.size() + "\n");
            for (Block b : blocks) {
                writer.println(b.local_id + " " + b.name + " " + b.group.getTranslateX() + " " + b.group.getTranslateY());
                switch (b.name) {
                    case "Constant":
                        for (Map.Entry<String, Double> m : b.getOutputPorts().get(0).getValue().entrySet()) {
                            writer.print(m.getKey() + " " + m.getValue() + " ");
                        }
                        writer.print("\n");
                        //writer.println(b.getOutputPorts().get(0).getValue());
                        break;
                    default:
                        break;
                }
            }

            /** save connections */
            writer.write(connections.size() + "\n");
            for (Connection c : connections) {
                writer.println(c.getInputPort().loc + " " + c.getInputPort().name + " " + c.getOutputPort().loc + " " + c.getOutputPort().name);
            }

            /*
            for (int i = 0; i < blocks.size(); i++) {
                blocks.get(i).destroy();
            }
            blocks.clear();
            gateGroup.getChildren().clear();*/
            writer.close();
        } catch (Exception e) {
            System.out.println("File not found!");
            e.printStackTrace();
        }
    }

    public void load_scheme() {
        this.connections.clear();
        this.gateGroup.getChildren().clear();
        this.lines.clear();

        FileChooser fileChooser = new FileChooser();
        fileChooser.setTitle("Open Resource File");
        File filename = fileChooser.showOpenDialog(this.primaryStage);


        System.out.println("LOADING SCHEME");
        try {
            FileReader fr = new FileReader(filename.getAbsoluteFile());
            BufferedReader br = new BufferedReader(fr);

            String sCurrentLine;

            sCurrentLine = br.readLine();
            float number_of_blocks = Float.parseFloat(sCurrentLine);
            for (float i=0; i< number_of_blocks; i++) {
                sCurrentLine = br.readLine();
                String[] splited = sCurrentLine.split("\\s+");
                float x, y, loc;
                switch (splited[1]) {
                    case "Constant":
                        sCurrentLine = br.readLine();
                        String[] values = sCurrentLine.split("\\s+");
                        Map<String, Double> m = new HashMap<>();
                        for (int j = 0; j<values.length; j+=2) {
                            m.put(values[j], Double.parseDouble(values[j+1]));
                        }
                        loc = Float.parseFloat(splited[0]);
                        x = Float.parseFloat(splited[2]);
                        y = Float.parseFloat(splited[3]);
                        System.out.println("Loaded constant with value " + m);
                        blocks.add(new Constant(m, x, y, loc));
                        break;
                    case "Adder":
                        loc = Float.parseFloat(splited[0]);
                        x = Float.parseFloat(splited[2]);
                        y = Float.parseFloat(splited[3]);
                        System.out.println("Loaded adder");
                        blocks.add(new Adder(x, y, loc));
                        break;
                    case "Multiplier":
                        loc = Float.parseFloat(splited[0]);
                        x = Float.parseFloat(splited[2]);
                        y = Float.parseFloat(splited[3]);
                        System.out.println("Loaded multiplier");
                        blocks.add(new Multiplier(x, y, loc));
                        break;
                    case "Result":
                        loc = Float.parseFloat(splited[0]);
                        x = Float.parseFloat(splited[2]);
                        y = Float.parseFloat(splited[3]);
                        System.out.println("Loaded result");
                        blocks.add(new Result(x, y, loc));
                        break;
                    case "Divider":
                        loc = Float.parseFloat(splited[0]);
                        x = Float.parseFloat(splited[2]);
                        y = Float.parseFloat(splited[3]);
                        System.out.println("Loaded result");
                        blocks.add(new Divider(x, y, loc));
                        break;
                    case "Subtractor":
                        loc = Float.parseFloat(splited[0]);
                        x = Float.parseFloat(splited[2]);
                        y = Float.parseFloat(splited[3]);
                        System.out.println("Loaded result");
                        blocks.add(new Subtractor(x, y, loc));
                        break;
                }
            }
            /** load connections */
            sCurrentLine = br.readLine();
            float number_of_con = Float.parseFloat(sCurrentLine);
            for (float i=0; i< number_of_con; i++) {
                sCurrentLine = br.readLine();
                String[] splited = sCurrentLine.split("\\s+");
                float id1 = Float.parseFloat(splited[0]);
                String port_name1 = splited[1];
                float id2 = Float.parseFloat(splited[2]);
                String port_name2 = splited[3];

                /** find correct port objects in blocks */
                Port first = null, second = null;
                for (Block c: this.blocks) {
                    if (c.local_id == id1) {
                        for (Port x: c.outputPorts) {
                            System.out.println(x.name + " == " + port_name1);
                            if (x.name.equals(port_name1)) {
                                first = x;
                            }
                        }
                    } else if (c.local_id == id2) {
                        for (Port x: c.inputPorts) {
                            if (x.name.equals(port_name2)) {
                                second = x;
                            }
                        }
                    }
                }
                System.out.println("Creating connection " + first + " " + second);
                Connection connection = new Connection();
                connection.setInputPort(first);
                connection.setOutputPort(second);

                Line line = new Line();line.setStartX(first.drawArea.getWidth() / 2 + first.getX() + first.g.getTranslateX());    // + Dragboard.pinOver.setGroup.getTranslateX()
                line.setStartY(first.drawArea.getHeight() / 2 + first.getY() + first.g.getTranslateY());    // + Dragboard.pinOver.setGroup.getTranslateY()
                line.setEndX(first.drawArea.getWidth() / 2 + second.drawArea.getTranslateX() + second.g.getTranslateX());    // + pinOver.setGroup.getTranslateX()
                line.setEndY(first.drawArea.getHeight() / 2 + second.drawArea.getTranslateY() + second.g.getTranslateY());  // + pinOver.setGroup.getTranslateY()

                line.setStroke(Color.RED);
                line.setStrokeWidth(3);
                System.out.println(line);
                Tooltip t = new Tooltip("Source: " + connection.inputPort.getValue() + "\nDestination: " + connection.outputPort.getValue());
                Tooltip.install(line, t);
                connection.setLine(line);
                this.lines.add(line);
                this.gateGroup.getChildren().add(line);


                first.connections.add(connection);
                second.connections.add(connection);
                this.connections.add(connection);
            }

            fr.close();
        } catch (Exception e) {
            System.out.println("File not found!");
            e.printStackTrace();
        }
    }

    /** ignored */
    public static void main(String[] args) {
        launch(args);
    }
}



