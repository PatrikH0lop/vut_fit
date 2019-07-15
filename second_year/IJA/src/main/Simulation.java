/*
 * This class is responsible for simulation management
 * Provides functions to step or run the whole simulation
 * @author Patrik Holop
 */
package main;

import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.scene.control.Alert;
import javafx.scene.control.TextInputDialog;
import javafx.scene.paint.Color;
import javafx.scene.shape.Circle;
import javafx.util.Duration;
import logic.base.Block;
import logic.base.Port;
import logic.connections.Connection;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.Map;
import java.util.Optional;


/**
 * This class is responsible for simulation management
 * Provides functions to step or run the whole simulation
 * @author Patrik Holop
 */
public class Simulation {

    /**
     * Gets all unassigned values for ports
     */
    static void get_unassigned_values() {
        for (Block b: Main.main.blocks) {
            for (Port p: b.inputPorts) {
                if (p.connections == null || p.connections.size() == 0) {
                    boolean stop = false;
                    while (!stop) {
                        stop = true;
                        Iterator it = p.get_value().entrySet().iterator();
                        while (it.hasNext()) {
                            Map.Entry pair = (Map.Entry) it.next();
                            // if the value was unassigned
                            if (pair.getValue() == null) {
                                // get the value from user
                                TextInputDialog dialog = new TextInputDialog("1.0");
                                dialog.setTitle(b.name + " " + p.name + " : " + pair.getKey());
                                dialog.setHeaderText("Please enter valid value");
                                dialog.setContentText("Value:");
                                Optional<String> value = dialog.showAndWait();
                                try {
                                    p.get_value().put(pair.getKey().toString(), Double.parseDouble(value.get()));
                                } catch (NumberFormatException e) {
                                    // pass when wrong number was assigned
                                    stop = false;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    /**
     * Sorts blocks in the main application in order of execution
     */
    static void sort_blocks() {
        ArrayList<Float> ids = new ArrayList<>();

        // while not all blocks are sorted
        while (ids.size() != Main.main.blocks.size()) {
            for (Block b: Main.main.blocks) {
                // if the block has no input ports, it might be inserted
                if (b.inputPorts == null || b.inputPorts.size() == 0) {
                    if (!ids.contains(b.local_id)) {
                        ids.add(b.local_id);
                    }
                }
                else {
                    // if the block has input ports, each of them needs to be part
                    // of already added block to solve dependence
                    boolean add = true;
                    for (Port p: b.inputPorts) {
                        for (Connection c: p.connections) {
                            if (!ids.contains(c.inputPort.loc)) {
                                add = false;
                            }
                        }
                    }
                    if (add) {
                        if (!ids.contains(b.local_id)) {
                            ids.add(b.local_id);
                        }
                    }
                }
            }
        }
        // new sorted array of blocks
        ArrayList<Block> new_main_block_array = new ArrayList<>();
        for (Float id : ids) {
            // find correct block and add it to sorted array
            Block to_add = null;
            for (Block b : Main.main.blocks) {
                if (b.local_id == id) {
                    to_add = b;
                }
            }
            new_main_block_array.add(to_add);
        }
        // reassign the arrays
        Main.main.blocks = new_main_block_array;
    }

    /**
     * This method runs the whole simulation.
     * Before start checks, whether exists any cycle in created scheme
     */
    public static void run_simulation() {

        // find cycles
        boolean has_cycle = SchemeChecker.containsCycles();

        if (has_cycle == true) {
            // error if found
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle("Block Editor Error");
            alert.setHeaderText("Cycle detected");
            alert.setContentText("In current scheme was detected cycle!\nPlease make necessary changes and try again.");
            alert.showAndWait();
        }
        else {
            // sort blocks
            sort_blocks();

            // get missing values
            get_unassigned_values();

            for (Block b: Main.main.blocks) {
                if (b.active == true) {
                    b.active = false;
                }
            }
            // run simulation with delay on every block
            Timeline timeline = new Timeline(new KeyFrame(Duration.millis(1000), ae -> update_step_scheme()));
            timeline.setCycleCount(Main.main.blocks.size());
            timeline.play();
        }
    }

    /**
     * This method updates one block with it's connections and chooses the next block to be active
     */
    public static void update_step_scheme() {
        sort_blocks();

        // find block objects based on activity
        Block to_update = null;
        int pos = 0;
        int index = 0;
        for (Block next : Main.main.blocks) {
            if (next.active) {
                to_update = next;
                pos = index;
            }
            index++;
        }
        // if no object found (first run), set the first to be active
        if (Main.main.blocks.size() == 0)
            return ;
        if (to_update == null) {
            to_update = Main.main.blocks.get(0);
        }

        // remove previous activity circle marker
        Main.main.all_drawable_objects.getChildren().remove(Main.main.active_circle);
        // draw green circle marking the active block
        Main.main.active_circle = new Circle((int)to_update.group.getTranslateX(), (int)to_update.group.getTranslateY(), 5, Color.GREEN);
        Main.main.all_drawable_objects.getChildren().add(Main.main.active_circle);


        // check if no value is needed to add by user
        for (Port p: to_update.inputPorts) {
            if (p.connections == null || p.connections.size() == 0) {
                boolean stop = false;
                while (!stop) {
                    stop = true;
                    Iterator it = p.get_value().entrySet().iterator();
                    while (it.hasNext()) {
                        Map.Entry pair = (Map.Entry) it.next();
                        // if the value was unassigned
                        if (pair.getValue() == null) {
                            // get the value from user
                            TextInputDialog dialog = new TextInputDialog("1.0");
                            dialog.setTitle(to_update.name + " " + p.name + " : " + pair.getKey());
                            dialog.setHeaderText("Please enter valid value");
                            dialog.setContentText("Value:");
                            Optional<String> value = dialog.showAndWait();
                            try {
                                p.get_value().put(pair.getKey().toString(), Double.parseDouble(value.get()));
                            } catch (NumberFormatException e) {
                                // pass when wrong number was assigned
                                stop = false;
                            }
                        }
                    }
                }
            }
        }

        // update_tooltip the block
        to_update.update();

        // update_tooltip all outer connection from the block
        for (Connection next : Main.main.connections) {
            if (next != null) {
                if (next.inputPort.loc == to_update.local_id) {
                    next.update_tooltip();
                    Block output_block= null;
                    double loc = next.outputPort.loc;
                    for (Block output: Main.main.blocks) {
                        if (loc == output.local_id) {
                            output.update();
                        }
                    }
                }
            }
        }

        // set the next active block
        if (pos+1 < Main.main.blocks.size()) {
            to_update.active = false;
            Main.main.blocks.get(pos+1).active = true;
        }
        else {
            to_update.active = false;
            Main.main.blocks.get(0).active = true;
        }
    }
}
