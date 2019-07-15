/*
 * Constant block, represented by the certain numeric value.
 * User is asked to enter the value on block creation
 * @author Patrik Holop
 */
package logic.blocks.arithmetic;

import javafx.scene.Group;
import javafx.scene.control.TextInputDialog;
import logic.base.Block;
import logic.base.Port;
import logic.connections.Connection;
import ui.ImageManager;
import java.util.Map;
import java.util.Optional;


/**
 * Constant block, represented by the certain numeric value.
 * User is asked to enter the value on block creation
 * @author Patrik Holop
 */
public class Constant extends Block {

    /**
     * Simple constructor
     */
    public Constant() {
        super();
        // set the local id
        this.local_id = Block.id;
        this.group = new Group();
        this.name = "Constant";
        // load the texture
        this.outlookImage = ImageManager.load_cached_image("var.png");

        // add new port
        this.outputPorts.add(new Port(this.group, "PortC", 50, 15, this.local_id));
        this.outputPorts.get(0).values.put("value", null);

        boolean stop = false;
        Optional<String> result;
        while (!stop) {
            stop = true;
            // request for user to set the value
            TextInputDialog dialog = new TextInputDialog("10");
            dialog.setTitle("Constant value");
            dialog.setHeaderText("Please enter valid value");
            dialog.setContentText("Value:");
            result = dialog.showAndWait();
            try {
                // set it's value
                this.outputPorts.get(0).get_value().put("value", Double.parseDouble(result.get()));
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
     * @param value Saved value of the constant
     * @param x Initialize X coordinate
     * @param y Initialize Y coordinate
     * @param loc Saved ID of the block
     */
    public Constant(Map<String, Double> value, double x, double y, float loc) {
        super();
        // set the local id
        this.local_id = loc;
        this.group = new Group();
        // initialize the coordinates
        this.group.setTranslateX(x);
        this.group.setTranslateY(y);
        this.name = "Constant";
        // load the texture
        this.outlookImage = ImageManager.load_cached_image("var.png");
        // set the port
        this.outputPorts.add(new Port(this.group, "PortC", 50, 15, loc));
        this.outputPorts.get(0).values.put("value", null);
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
     * Execution function for the constant. Updates the values.
     */
    public void update_value() {
        this.outputPorts.get(0).get_value().put("value", this.outputPorts.get(0).get_value().get("value"));
        for (Connection x: this.outputPorts.get(0).connections) {
            if (x.inputPort != null && x.outputPort != null) {
                x.outputPort.get_value().put("value", this.outputPorts.get(0).get_value().get("value"));
            }
        }
        //update tooltip
        this.update_tooltip();
    }
}

