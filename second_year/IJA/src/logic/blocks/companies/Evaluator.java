/*
 * Evaluator block takes a company as input and as the output value is
 * calculated market cap of the company
 * @author Patrik Holop
 */
package logic.blocks.companies;

import javafx.scene.Group;
import logic.base.Block;
import logic.base.Port;
import logic.connections.Connection;
import ui.ImageManager;


/**
 * Evaluator block takes a company as input and as the output value is
 * calculated market cap of the company
 * @author Patrik Holop
 */
public class Evaluator extends Block {

    /** Simple constructor */
    public Evaluator() {
        super();
        // set local id
        this.local_id = Block.id;
        // create new group for block and ports
        this.group = new Group();
        this.name = "Evaluator";
        // load the texture
        this.outlookImage = ImageManager.load_cached_image("evaluator.png");

        // initializing ports
        this.inputPorts.add(new Port(this.group,"PortA", 0, 15, this.local_id));
        this.outputPorts.add(new Port(this.group, "PortC", 50, 15, this.local_id));
        // first power plant
        this.inputPorts.get(0).values.put("stock_number", null);
        this.inputPorts.get(0).values.put("stock_value", null);
        this.inputPorts.get(0).values.put("average_salary", null);
        // output value
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
    public Evaluator(float x, float y, float loc) {
        super();
        // set id
        this.local_id = loc;
        this.group = new Group();
        // initialize coordinates
        this.group.setTranslateX(x);
        this.group.setTranslateY(y);
        this.name = "Evaluator";
        // load texture
        this.outlookImage = ImageManager.load_cached_image("evaluator.png");
        // set ports
        this.inputPorts.add(new Port(this.group, "PortA", 0, 15, loc));
        this.outputPorts.add(new Port(this.group,"PortC", 50, 15, loc));
        // first power plant
        this.inputPorts.get(0).values.put("stock_number", null);
        this.inputPorts.get(0).values.put("stock_value", null);
        this.inputPorts.get(0).values.put("average_salary", null);
        // output value
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
     * Updates function defining the blocks. Evaluates company
     */
    public void update_value() {
        if (inputPorts.get(0).get_value().get("stock_number") != null) {
            double sum = this.inputPorts.get(0).get_value().get("stock_number") * this.inputPorts.get(0).get_value().get("stock_value");
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
