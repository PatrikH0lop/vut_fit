/*
 * Company block represents an active a.s. company.
 * Each company is defined by stock number, stock
 * value and average salary.
 * @author Patrik Holop
 */
package logic.blocks.companies;

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
 * Company block represents an active a.s. company.
 * Each company is defined by stock number, stock
 * value and average salary.
 * @author Patrik Holop
 */
public class Company extends Block {

    /**
     * Simple constructor
     */
    public Company() {
        super();
        // set the local id
        this.local_id = Block.id;
        this.group = new Group();
        this.name = "Company";
        // load the texture
        this.outlookImage = ImageManager.load_cached_image("company.png");
        // add new port
        this.outputPorts.add(new Port(this.group, "PortC", 50, 15, this.local_id));

        boolean stop = false;
        Optional<String> result;
        TextInputDialog dialog = null;
        while (!stop) {
            stop = true;
            // request for user to set the value
            dialog = new TextInputDialog("10");
            dialog.setTitle("Company value");
            dialog.setHeaderText("Please enter valid stock number");
            dialog.setContentText("Value:");
            result = dialog.showAndWait();
            // set it's value
            try {
                this.outputPorts.get(0).get_value().put("stock_number", Double.parseDouble(result.get()));
            } catch (NumberFormatException e) {
                stop = false;
            }
        }
        stop = false;
        while (!stop) {
            stop = true;
            dialog.setHeaderText("Please enter stock value");
            dialog.setContentText("Value:");
            result = dialog.showAndWait();
            try {
                this.outputPorts.get(0).get_value().put("stock_value", Double.parseDouble(result.get()));
            } catch (NumberFormatException e) {
                stop = false;
            }
        }
        stop = false;
        while (!stop) {
            stop = true;
            dialog.setHeaderText("Please enter average salary");
            dialog.setContentText("Value:");
            result = dialog.showAndWait();
            try {
                this.outputPorts.get(0).get_value().put("average_salary", Double.parseDouble(result.get()));
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
    public Company(Map<String, Double> value, double x, double y, float loc) {
        super();
        // set the local id
        this.local_id = loc;
        this.group = new Group();
        // initialize the coordinates
        this.group.setTranslateX(x);
        this.group.setTranslateY(y);
        this.name = "Company";
        // load the texture
        this.outlookImage = ImageManager.load_cached_image("company.png");
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
     * Execution function for the constant. Updates the values.
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

