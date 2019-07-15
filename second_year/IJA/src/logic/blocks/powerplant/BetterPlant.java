/*
 * Better plant block takes 2 power plants as input and creates a new
 * one based on the best attributes of each input plant.
 * @author Patrik Holop
 */
package logic.blocks.powerplant;

import javafx.scene.Group;
import logic.base.Block;
import logic.base.Port;
import logic.connections.Connection;
import ui.ImageManager;


/**
 * Better plant block takes 2 power plants as input and creates a new
 * one based on the best attributes of each input plant.
 * @author Patrik Holop
 */
public class BetterPlant extends Block {

    /** Simple constructor */
    public BetterPlant() {
        super();
        // set local id
        this.local_id = Block.id;
        // create new group for block and ports
        this.group = new Group();
        this.name = "BetterPlant";
        // load the texture
        this.outlookImage = ImageManager.load_cached_image("betterplant.png");

        // initializing ports
        this.inputPorts.add(new Port(this.group,"PortA", 0, 2, this.local_id));
        this.inputPorts.add(new Port(this.group, "PortB", 0, 30, this.local_id));
        this.outputPorts.add(new Port(this.group, "PortC", 50, 15, this.local_id));
        // first power plant
        this.inputPorts.get(0).values.put("risk_probability", null);
        this.inputPorts.get(0).values.put("reactor_temperature", null);
        this.inputPorts.get(0).values.put("distributed_energy", null);
        // second power plant
        this.inputPorts.get(1).values.put("risk_probability", null);
        this.inputPorts.get(1).values.put("reactor_temperature", null);
        this.inputPorts.get(1).values.put("distributed_energy", null);
        // output value
        this.outputPorts.get(0).values.put("risk_probability", null);
        this.outputPorts.get(0).values.put("reactor_temperature", null);
        this.outputPorts.get(0).values.put("distributed_energy", null);

        // init grapghics
        this.drawArea = set_block_graphics(8, 0);
        group.getChildren().add(this.drawArea);
        add_ports_to_active_group();
        set_dragging_handlers();
        // update tooltip
        this.update_tooltip();
    }

    /**
     * Simple constructor setting the initial coordinates after loading
     * @param x Initial X coordinate
     * @param y Initial Y coordinate
     * @param loc Initial ID from load file
     */
    public BetterPlant(float x, float y, float loc) {
        super();
        // set id
        this.local_id = loc;
        this.group = new Group();
        // initialize coordinates
        this.group.setTranslateX(x);
        this.group.setTranslateY(y);
        this.name = "BetterPlant";
        // load texture
        this.outlookImage = ImageManager.load_cached_image("betterplant.png");
        // set ports
        this.inputPorts.add(new Port(this.group, "PortA", 0, 2, loc));
        this.inputPorts.add(new Port(this.group, "PortB", 0, 30, loc));
        this.outputPorts.add(new Port(this.group,"PortC", 50, 15, loc));
        // first power plant
        this.inputPorts.get(0).values.put("risk_probability", null);
        this.inputPorts.get(0).values.put("reactor_temperature", null);
        this.inputPorts.get(0).values.put("distributed_energy", null);
        // second power plant
        this.inputPorts.get(1).values.put("risk_probability", null);
        this.inputPorts.get(1).values.put("reactor_temperature", null);
        this.inputPorts.get(1).values.put("distributed_energy", null);
        // output value
        this.outputPorts.get(0).values.put("risk_probability", null);
        this.outputPorts.get(0).values.put("reactor_temperature", null);
        this.outputPorts.get(0).values.put("distributed_energy", null);
        // init graphics
        this.drawArea = set_block_graphics(8, 0);
        group.getChildren().add(this.drawArea);
        add_ports_to_active_group();
        set_dragging_handlers();
        // update the tooltip
        this.update_tooltip();
    }

    /**
     * Updates function defining the blocks. Creates a better plant
     */
    public void update_value() {
        if (inputPorts.get(0).get_value().get("risk_probability") != null && inputPorts.get(1).get_value().get("risk_probability") != null) {
            double risk_prob = Math.min(this.inputPorts.get(0).get_value().get("risk_probability"), this.inputPorts.get(1).get_value().get("risk_probability"));
            double temp = Math.min(this.inputPorts.get(0).get_value().get("reactor_temperature"), this.inputPorts.get(1).get_value().get("reactor_temperature"));
            double energy = Math.max(this.inputPorts.get(0).get_value().get("distributed_energy"), this.inputPorts.get(1).get_value().get("distributed_energy"));
            this.outputPorts.get(0).get_value().put("risk_probability", risk_prob);
            this.outputPorts.get(0).get_value().put("reactor_temperature", temp);
            this.outputPorts.get(0).get_value().put("distributed_energy", energy);
            for (Connection x: this.outputPorts.get(0).connections) {
                if (x.inputPort != null && x.outputPort != null) {
                    x.outputPort.values.put("risk_probability", risk_prob);
                    x.outputPort.values.put("reactor_temperature", temp);
                    x.outputPort.values.put("distributed_energy", energy);
                }
            }
        }
        this.update_tooltip();
    }
}
