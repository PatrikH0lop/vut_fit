/*
 * Powerplant block representing nuclear power plant.
 * Every nuclear power plant block contains it's accident probability,
 * reactor temperature and distributed energy amount per day.
 * @author Patrik Holop
 */
package logic.blocks.powerplant;

import javafx.scene.Group;
import javafx.scene.control.TextInputDialog;
import logic.base.Block;
import logic.base.Port;
import logic.connections.Connection;
import ui.ImageManager;
import java.util.Iterator;
import java.util.Map;
import java.util.Optional;


/**
 * Powerplant block representing nuclear power plant.
 * Every nuclear power plant block contains it's accident probability,
 * reactor temperature and distributed energy amount per day.
 * @author Patrik Holop
 */
public class Powerplant extends Block {

    /**
     * Simple constructor
     */
    public Powerplant() {
        super();
        // set the local id
        this.local_id = Block.id;
        this.group = new Group();
        this.name = "Powerplant";
        // load the texture
        this.outlookImage = ImageManager.load_cached_image("powerplant.png");

        // set the port
        this.outputPorts.add(new Port(this.group, "PortC", 50, 15, this.local_id));

        // request for user to set the value
        boolean stop = false;
        Optional<String> result;
        TextInputDialog dialog = null;
        while (!stop) {
            stop = true;
            // request for user to set the value
            dialog = new TextInputDialog("10");
            dialog.setTitle("Power plant value");
            dialog.setHeaderText("Please enter valid risk probability");
            dialog.setContentText("Value:");
            result = dialog.showAndWait();
            // set it's value
            try {
                double sum = Math.max(Math.min(Double.parseDouble(result.get()), 1.0), 0.0);
                this.outputPorts.get(0).get_value().put("risk_probability", sum);
            } catch (NumberFormatException e) {
                stop = false;
            }
        }
        stop = false;
        while (!stop) {
            stop = true;
            dialog.setHeaderText("Please enter valid reactor temperature");
            dialog.setContentText("Value:");
            result = dialog.showAndWait();
            try {
                this.outputPorts.get(0).get_value().put("reactor_temperature", Double.parseDouble(result.get()));
            } catch (NumberFormatException e) {
                stop = false;
            }
        }
        stop = false;
        while (!stop) {
            stop = true;
            dialog.setHeaderText("Please enter distributed energy per day");
            dialog.setContentText("Value:");
            result = dialog.showAndWait();
            try {
                this.outputPorts.get(0).get_value().put("distributed_energy", Double.parseDouble(result.get()));
            } catch (NumberFormatException e) {
                stop = false;
            }
        }
        // update the tooltip
        this.update_tooltip();

        // initialize the graphics
        this.drawArea = set_block_graphics(8, 0);
        group.getChildren().add(this.drawArea);
        add_ports_to_active_group();
        set_dragging_handlers();
        this.update_tooltip();
    }

    /**
     * Simple constructor after loading
     * @param value Saved value of the power plant
     * @param x Initialize X coordinate
     * @param y Initialize Y coordinate
     * @param loc Saved ID of the block
     */
    public Powerplant(Map<String, Double> value, double x, double y, float loc) {
        super();
        // set the local id
        this.local_id = loc;
        this.group = new Group();
        // initialize the coordinates
        this.group.setTranslateX(x);
        this.group.setTranslateY(y);
        this.name = "Powerplant";
        // load the texture
        this.outlookImage = ImageManager.load_cached_image("powerplant.png");
        // set the port
        this.outputPorts.add(new Port(this.group, "PortC", 50, 15, loc));
        // set the value
        this.outputPorts.get(0).set_value(value);
        // update the tooltip
        this.update_tooltip();
        // this graphics
        this.drawArea = set_block_graphics(8, 0);
        group.getChildren().add(this.drawArea);
        add_ports_to_active_group();
        set_dragging_handlers();
        this.update_tooltip();
    }

    /**
     * Execution function for the power plant. Update the values.
     */
    public void update_value() {
        Iterator it = this.outputPorts.get(0).get_value().entrySet().iterator();
        while (it.hasNext()) {
            Map.Entry pair = (Map.Entry)it.next();
            this.outputPorts.get(0).get_value().put(pair.getKey().toString(), this.outputPorts.get(0).get_value().get(pair.getKey()));
            for (Connection x: this.outputPorts.get(0).connections) {
                if (x.inputPort != null && x.outputPort != null) {
                    x.outputPort.get_value().put(pair.getKey().toString(), this.outputPorts.get(0).get_value().get(pair.getKey()));
                }
            }
        }
        //update tooltip
        this.update_tooltip();
    }
}

