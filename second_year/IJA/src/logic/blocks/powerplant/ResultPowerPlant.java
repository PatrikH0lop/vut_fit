/*
 * Powerplant block representing nuclear power plant.
 * Every nuclear power plant block contains it's accident probability,
 * reactor temperature and distributed energy amount per day.
 * This block is used for optional saving the result of scheme calculations.
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
 * This block is used for optional saving the result of scheme calculations.
 * @author Patrik Holop
 */
public class ResultPowerPlant extends Block {

    /**
     * Simple constructor
     */
    public ResultPowerPlant() {
        super();
        // set the local id
        this.local_id = Block.id;
        this.group = new Group();
        this.name = "PowerplantResult";
        // load the texture
        this.outlookImage = ImageManager.load_cached_image("powerplant_result.png");

        // add new port
        this.inputPorts.add(new Port(this.group, "PortC", 0, 15, this.local_id));
        // set it's value
        this.inputPorts.get(0).get_value().put("risk_probability", null);
        this.inputPorts.get(0).get_value().put("reactor_temperature", null);
        this.inputPorts.get(0).get_value().put("distributed_energy", null);
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
     * @param x Initialize X coordinate
     * @param y Initialize Y coordinate
     * @param loc Saved ID of the block
     */
    public ResultPowerPlant(double x, double y, float loc) {
        super();
        // set the local id
        this.local_id = loc;
        this.group = new Group();
        // initialize the coordinates
        this.group.setTranslateX(x);
        this.group.setTranslateY(y);
        this.name = "PowerplantResult";
        // load the texture
        this.outlookImage = ImageManager.load_cached_image("powerplant_result.png");
        // set the port
        this.inputPorts.add(new Port(this.group, "PortC", 0, 15, this.local_id));
        // set it's value
        this.inputPorts.get(0).get_value().put("risk_probability", null);
        this.inputPorts.get(0).get_value().put("reactor_temperature", null);
        this.inputPorts.get(0).get_value().put("distributed_energy", null);
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
        //update tooltip
        this.update_tooltip();
    }
}

