/*
 * This class provides method for serialization of created schemes
 * Using the file chooser provides interface for saving and loading schemes
 * @author Patrik Holop
 */
package main;

import javafx.scene.control.Alert;
import javafx.scene.control.Tooltip;
import javafx.scene.paint.Color;
import javafx.scene.shape.Line;
import javafx.stage.FileChooser;
import logic.base.Block;
import logic.base.Port;
import logic.blocks.arithmetic.*;
import logic.blocks.companies.Company;
import logic.blocks.companies.Evaluator;
import logic.blocks.companies.MergeCompany;
import logic.blocks.companies.ResultCompany;
import logic.blocks.powerplant.*;
import logic.connections.Connection;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;


/**
 * This class provides method for serialization of created schemes
 * <p> Using the file chooser provides interface for saving and loading schemes
 * @author Patrik Holop
 */
public class IOSchemeManager {

    /**
     * This function is used for saving schemes using the file chooser
     */
    public static void save_scheme() {
        // initialize file chooser
        FileChooser fileChooser = new FileChooser();
        fileChooser.setTitle("Open scheme file");
        // set scheme extension filter
        FileChooser.ExtensionFilter extFilter = new FileChooser.ExtensionFilter("Scheme file (.scheme)", "*.scheme");
        fileChooser.getExtensionFilters().add(extFilter);
        // apply file chooser
        File file = fileChooser.showSaveDialog(Main.main.root_stage);

        try {
            // set write to the correct file
            PrintWriter writer = new PrintWriter(file.getAbsoluteFile(), "UTF-8");

            // save all blocks
            writer.write(Main.main.blocks.size() + "\n");
            for (Block b : Main.main.blocks) {
                writer.println(b.local_id + " " + b.name + " " + b.group.getTranslateX() + " " + b.group.getTranslateY());
                switch (b.name) {
                    case "Constant":
                        for (Map.Entry<String, Double> m : b.getOutputPorts().get(0).get_value().entrySet()) {
                            writer.print(m.getKey() + " " + m.getValue() + " ");
                        }
                        writer.print("\n");
                        break;
                    case "Powerplant":
                        for (Map.Entry<String, Double> m : b.getOutputPorts().get(0).get_value().entrySet()) {
                            writer.print(m.getKey() + " " + m.getValue() + " ");
                        }
                        writer.print("\n");
                        break;
                    case "Company":
                        for (Map.Entry<String, Double> m : b.getOutputPorts().get(0).get_value().entrySet()) {
                            writer.print(m.getKey() + " " + m.getValue() + " ");
                        }
                        writer.print("\n");
                        break;
                    default:
                        break;
                }
            }

            // save all connections
            writer.write(Main.main.connections.size() + "\n");
            for (Connection c : Main.main.connections) {
                writer.println(c.inputPort.loc + " " + c.inputPort.name + " " + c.outputPort.loc + " " + c.outputPort.name);
            }
            writer.close();
        } catch (Exception e) {
            // show error dialog
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle("Block Editor Error");
            alert.setHeaderText("Error during saving file!");
            alert.setContentText("Please check permissions or filename and try again.");
            alert.showAndWait();
        }
    }

    /**
     * This function is used for loading scheme using the file chooser
     */
    public static void load_scheme() {

        // clear scene before loading
        Main.main.clear_scene();

        // initialize filechooser for loading
        FileChooser fileChooser = new FileChooser();
        fileChooser.setTitle("Open Scheme File");
        File filename = fileChooser.showOpenDialog(Main.main.root_stage);

        try {
            // set up buffered reading
            FileReader fr = new FileReader(filename.getAbsoluteFile());
            BufferedReader br = new BufferedReader(fr);

            String sCurrentLine;
            sCurrentLine = br.readLine();

            // get number of blocks
            float number_of_blocks = Float.parseFloat(sCurrentLine);

            // load blocks from file
            for (float i=0; i< number_of_blocks; i++) {
                sCurrentLine = br.readLine();
                String[] splited = sCurrentLine.split("\\s+");
                // coordinates and id of block
                float x, y, loc;
                switch (splited[1]) {
                    // set behaviour for each block individually
                    case "Constant":
                        sCurrentLine = br.readLine();
                        String[] values = sCurrentLine.split("\\s+");
                        Map<String, Double> m = new HashMap<>();
                        // load map of values
                        for (int j = 0; j<values.length; j+=2) {
                            m.put(values[j], Double.parseDouble(values[j+1]));
                        }
                        loc = Float.parseFloat(splited[0]);
                        x = Float.parseFloat(splited[2]);
                        y = Float.parseFloat(splited[3]);
                        // create new constant
                        Main.main.blocks.add(new Constant(m, x, y, loc));
                        break;
                    case "Adder":
                        loc = Float.parseFloat(splited[0]);
                        x = Float.parseFloat(splited[2]);
                        y = Float.parseFloat(splited[3]);
                        // create new adder
                        Main.main.blocks.add(new Adder(x, y, loc));
                        break;
                    case "Multiplier":
                        loc = Float.parseFloat(splited[0]);
                        x = Float.parseFloat(splited[2]);
                        y = Float.parseFloat(splited[3]);
                        // create new multiplier
                        Main.main.blocks.add(new Multiplier(x, y, loc));
                        break;
                    case "Result":
                        loc = Float.parseFloat(splited[0]);
                        x = Float.parseFloat(splited[2]);
                        y = Float.parseFloat(splited[3]);
                        // create new result
                        Main.main.blocks.add(new Result(x, y, loc));
                        break;
                    case "Divider":
                        loc = Float.parseFloat(splited[0]);
                        x = Float.parseFloat(splited[2]);
                        y = Float.parseFloat(splited[3]);
                        // create new divider
                        Main.main.blocks.add(new Divider(x, y, loc));
                        break;
                    case "Subtractor":
                        loc = Float.parseFloat(splited[0]);
                        x = Float.parseFloat(splited[2]);
                        y = Float.parseFloat(splited[3]);
                        // create new subtractor
                        Main.main.blocks.add(new Subtractor(x, y, loc));
                        break;
                    case "Powerplant":
                        sCurrentLine = br.readLine();
                        String[] values_pp = sCurrentLine.split("\\s+");
                        Map<String, Double> m_pp = new HashMap<>();
                        // load map of values
                        for (int j = 0; j<values_pp.length; j+=2) {
                            m_pp.put(values_pp[j], Double.parseDouble(values_pp[j+1]));
                        }
                        loc = Float.parseFloat(splited[0]);
                        x = Float.parseFloat(splited[2]);
                        y = Float.parseFloat(splited[3]);
                        // create new constant
                        Main.main.blocks.add(new Powerplant(m_pp, x, y, loc));
                        break;
                    case "PowerplantResult":
                        loc = Float.parseFloat(splited[0]);
                        x = Float.parseFloat(splited[2]);
                        y = Float.parseFloat(splited[3]);
                        // create new subtractor
                        Main.main.blocks.add(new ResultPowerPlant(x, y, loc));
                        break;
                    case "BetterPlant":
                        loc = Float.parseFloat(splited[0]);
                        x = Float.parseFloat(splited[2]);
                        y = Float.parseFloat(splited[3]);
                        // create new subtractor
                        Main.main.blocks.add(new BetterPlant(x, y, loc));
                        break;
                    case "ReactorCooler":
                        loc = Float.parseFloat(splited[0]);
                        x = Float.parseFloat(splited[2]);
                        y = Float.parseFloat(splited[3]);
                        // create new subtractor
                        Main.main.blocks.add(new ReactorCooler(x, y, loc));
                        break;
                    case "CommonRiskEstimator":
                        loc = Float.parseFloat(splited[0]);
                        x = Float.parseFloat(splited[2]);
                        y = Float.parseFloat(splited[3]);
                        // create new subtractor
                        Main.main.blocks.add(new CommonRiskEstimator(x, y, loc));
                        break;
                    case "Company":
                        sCurrentLine = br.readLine();
                        String[] values_pc = sCurrentLine.split("\\s+");
                        Map<String, Double> m_pc = new HashMap<>();
                        // load map of values
                        for (int j = 0; j<values_pc.length; j+=2) {
                            m_pc.put(values_pc[j], Double.parseDouble(values_pc[j+1]));
                        }
                        loc = Float.parseFloat(splited[0]);
                        x = Float.parseFloat(splited[2]);
                        y = Float.parseFloat(splited[3]);
                        // create new constant
                        Main.main.blocks.add(new Company(m_pc, x, y, loc));
                        break;
                    case "Evaluator":
                        loc = Float.parseFloat(splited[0]);
                        x = Float.parseFloat(splited[2]);
                        y = Float.parseFloat(splited[3]);
                        // create new subtractor
                        Main.main.blocks.add(new Evaluator(x, y, loc));
                        break;
                    case "MergeCompany":
                        loc = Float.parseFloat(splited[0]);
                        x = Float.parseFloat(splited[2]);
                        y = Float.parseFloat(splited[3]);
                        // create new subtractor
                        Main.main.blocks.add(new MergeCompany(x, y, loc));
                        break;
                    case "CompanyResult":
                        loc = Float.parseFloat(splited[0]);
                        x = Float.parseFloat(splited[2]);
                        y = Float.parseFloat(splited[3]);
                        // create new subtractor
                        Main.main.blocks.add(new ResultCompany(x, y, loc));
                        break;
                    default:
                        // other
                        break;
                }
            }

            // load connection from file
            sCurrentLine = br.readLine();

            // get number of connections
            float number_of_con = Float.parseFloat(sCurrentLine);

            // load connections
            for (float i=0; i< number_of_con; i++) {
                sCurrentLine = br.readLine();
                String[] splited = sCurrentLine.split("\\s+");
                // get id of connected blocks (input)
                float id1 = Float.parseFloat(splited[0]);
                // get name of input port
                String port_name1 = splited[1];
                // get id of connected blocks (output)
                float id2 = Float.parseFloat(splited[2]);
                // get name of output port
                String port_name2 = splited[3];

                // find responsible port objects from ids from file
                Port first = null, second = null;
                for (Block c: Main.main.blocks) {
                    if (c.local_id == id1) {
                        for (Port x: c.outputPorts) {
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

                // create new connection based on input and output port
                Connection connection = new Connection();
                connection.setInputPort(first);
                connection.setOutputPort(second);

                // create drawable line object
                Line line = new Line();
                line.setStartX(first.drawArea.getWidth() / 2 + first.x + first.g.getTranslateX());
                line.setStartY(first.drawArea.getHeight() / 2 + first.y + first.g.getTranslateY());
                line.setEndX(first.drawArea.getWidth() / 2 + second.drawArea.getTranslateX() + second.g.getTranslateX());
                line.setEndY(first.drawArea.getHeight() / 2 + second.drawArea.getTranslateY() + second.g.getTranslateY());

                // set line properties
                line.setStroke(Color.RED);
                line.setStrokeWidth(3);
                // initialize tooltip
                Tooltip t = new Tooltip("Source: " + connection.inputPort.get_value() + "\nDestination: " + connection.outputPort.get_value());
                Tooltip.install(line, t);
                // connect line to connection
                connection.setLine(line);

                // add line to main group
                Main.main.all_drawable_objects.getChildren().add(line);

                // add connection to port objects
                first.connections.add(connection);
                second.connections.add(connection);
                Main.main.connections.add(connection);
            }
            fr.close();
        } catch (Exception e) {
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle("Block Editor Error");
            alert.setHeaderText("Error during loading file!");
            alert.setContentText("Please check permissions, filename. Otherwise saved file is corrupted!");
            alert.showAndWait();
            e.printStackTrace();
        }
    }
}
