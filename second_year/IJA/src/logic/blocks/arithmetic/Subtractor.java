/*
 * Subtractor block, has 2 ports and subtracts the second value from the first
 * @author Patrik Holop
 */
package logic.blocks.arithmetic;

import javafx.scene.Group;
import logic.base.Block;
import logic.base.Port;
import logic.connections.Connection;
import ui.ImageManager;


/**
 * Subtractor block, has 2 ports and subtracts the second value from the first
 * @author Patrik Holop
 */
public class Subtractor extends Block {

    /**
     * Simple constructor
     */
    public Subtractor() {
        super();
        // set id
        this.local_id = Block.id;
        this.group = new Group();
        this.name = "Subtractor";
        // load texture
        this.outlookImage = ImageManager.load_cached_image("subtractor.png");

        // initialize ports
        this.inputPorts.add(new Port(this.group,"PortA", 0, 2, this.local_id));
        this.inputPorts.add(new Port(this.group, "PortB", 0, 30, this.local_id));
        this.outputPorts.add(new Port(this.group, "PortC", 50, 15, this.local_id));
        this.inputPorts.get(0).values.put("value", null);
        this.inputPorts.get(1).values.put("value", null);
        this.outputPorts.get(0).values.put("value", null);

        // init graphics
        this.drawArea = set_block_graphics(8, 0);
        group.getChildren().add(this.drawArea);
        add_ports_to_active_group();
        set_dragging_handlers();
        // update tip
        this.update_tooltip();
    }

    /**
     * Simple constructor used after loading
     * @param x Initialize X coordinate
     * @param y Initialize Y coordinate
     * @param loc Block ID
     */
    public Subtractor(float x, float y, float loc) {
        super();
        // block id
        this.local_id = loc;
        this.group = new Group();
        // set coordinates
        this.group.setTranslateX(x);
        this.group.setTranslateY(y);
        this.name = "subtractor";

        // load coordinates
        this.outlookImage = ImageManager.load_cached_image("subtractor.png");

        // initialize ports
        this.inputPorts.add(new Port(this.group, "PortA", 0, 2, loc));
        this.inputPorts.add(new Port(this.group, "PortB", 0, 30, loc));
        this.outputPorts.add(new Port(this.group,"PortC", 50, 15, loc));
        this.inputPorts.get(0).values.put("value", null);
        this.inputPorts.get(1).values.put("value", null);
        this.outputPorts.get(0).values.put("value", null);

        // set graphics
        this.drawArea = set_block_graphics(8, 0);
        group.getChildren().add(this.drawArea);
        add_ports_to_active_group();
        set_dragging_handlers();
        // update the graphics
        this.update_tooltip();
    }

    /**
     * Execution function, second value is subtracted from the first
     */
    public void update_value() {
        if (inputPorts.get(0).get_value().get("value") != null && inputPorts.get(1).get_value().get("value") != null) {
            double result = this.inputPorts.get(0).get_value().get("value") - this.inputPorts.get(1).get_value().get("value");
            this.outputPorts.get(0).get_value().put("value", result);
            for (Connection x : this.outputPorts.get(0).connections) {
                if (x.inputPort != null && x.outputPort != null) {
                    x.outputPort.get_value().put("value", result);
                }
            }
            // update the value
            this.update_tooltip();
        }
    }
}
