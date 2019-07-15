/*
 * Adder block, has 2 ports and adds their values to output port
 * @author Patrik Holop
 */
package logic.blocks.arithmetic;


import javafx.scene.Group;
import logic.base.Block;
import logic.base.Port;
import logic.connections.Connection;
import ui.ImageManager;


/**
 * Adder block, has 2 ports and adds their values to output port
 * @author Patrik Holop
 */
public class Adder extends Block {

    /** Simple constructor */
    public Adder() {
        super();
        // set local id
        this.local_id = Block.id;
        // create new group for block and ports
        this.group = new Group();
        this.name = "Adder";
        // load the texture
        this.outlookImage = ImageManager.load_cached_image("adder.png");

        // initializing ports
        this.inputPorts.add(new Port(this.group,"PortA", 0, 2, this.local_id));
        this.inputPorts.add(new Port(this.group, "PortB", 0, 30, this.local_id));
        this.outputPorts.add(new Port(this.group, "PortC", 50, 15, this.local_id));
        this.inputPorts.get(0).values.put("value", null);
        this.inputPorts.get(1).values.put("value", null);
        this.outputPorts.get(0).values.put("value", null);

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
    public Adder(float x, float y, float loc) {
        super();
        // set id
        this.local_id = loc;
        this.group = new Group();
        // initialize coordinates
        this.group.setTranslateX(x);
        this.group.setTranslateY(y);
        this.name = "Adder";
        // load texture
        this.outlookImage = ImageManager.load_cached_image("adder.png");
        // set ports
        this.inputPorts.add(new Port(this.group, "PortA", 0, 2, loc));
        this.inputPorts.add(new Port(this.group, "PortB", 0, 30, loc));
        this.outputPorts.add(new Port(this.group,"PortC", 50, 15, loc));
        this.inputPorts.get(0).values.put("value", null);
        this.inputPorts.get(1).values.put("value", null);
        this.outputPorts.get(0).values.put("value", null);
        // init graphics
        this.drawArea = set_block_graphics(8, 0);
        group.getChildren().add(this.drawArea);
        add_ports_to_active_group();
        set_dragging_handlers();
        // update the tooltip
        this.update_tooltip();
    }

    /**
     * Updates function defining the blocks. Adds values from ports
     */
    public void update_value() {
        if (inputPorts.get(0).get_value().get("value") != null && inputPorts.get(1).get_value().get("value") != null) {
            double sum = this.inputPorts.get(0).get_value().get("value") + this.inputPorts.get(1).get_value().get("value");
            this.outputPorts.get(0).get_value().put("value", sum);
            for (Connection x : this.outputPorts.get(0).connections) {
                if (x.inputPort != null && x.outputPort != null) {
                    x.outputPort.get_value().put("value", sum);
                }
            }
            this.update_tooltip();
        }
    }
}
