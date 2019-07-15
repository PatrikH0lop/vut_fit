/*
 * Merge company block takes 2 companies as arguments
 * and merges them together, simulates company buyout.
 * Result is the new company with new statistics.
 * @author Patrik Holop
 */
package logic.blocks.companies;

import javafx.scene.Group;
import logic.base.Block;
import logic.base.Port;
import logic.connections.Connection;
import ui.ImageManager;


/**
 * Merge company block takes 2 companies as arguments
 * and merges them together, simulates company buyout.
 * Result is the new company with new statistics.
 * @author Patrik Holop
 */
public class MergeCompany extends Block {

    /** Simple constructor */
    public MergeCompany() {
        super();
        // set local id
        this.local_id = Block.id;
        // create new group for block and ports
        this.group = new Group();
        this.name = "MergeCompany";
        // load the texture
        this.outlookImage = ImageManager.load_cached_image("adder.png");

        // initializing ports
        this.inputPorts.add(new Port(this.group,"PortA", 0, 2, this.local_id));
        this.inputPorts.add(new Port(this.group, "PortB", 0, 30, this.local_id));
        this.outputPorts.add(new Port(this.group, "PortC", 50, 15, this.local_id));
        // first power plant
        this.inputPorts.get(0).values.put("stock_number", null);
        this.inputPorts.get(0).values.put("stock_value", null);
        this.inputPorts.get(0).values.put("average_salary", null);
        // second power plant
        this.inputPorts.get(1).values.put("stock_number", null);
        this.inputPorts.get(1).values.put("stock_value", null);
        this.inputPorts.get(1).values.put("average_salary", null);
        // output value
        this.outputPorts.get(0).values.put("stock_number", null);
        this.outputPorts.get(0).values.put("stock_value", null);
        this.outputPorts.get(0).values.put("average_salary", null);

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
    public MergeCompany(float x, float y, float loc) {
        super();
        // set id
        this.local_id = loc;
        this.group = new Group();
        // initialize coordinates
        this.group.setTranslateX(x);
        this.group.setTranslateY(y);
        this.name = "MergeCompany";
        // load texture
        this.outlookImage = ImageManager.load_cached_image("adder.png");
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
        this.outputPorts.get(0).values.put("stock_number", null);
        this.outputPorts.get(0).values.put("stock_value", null);
        this.outputPorts.get(0).values.put("average_salary", null);
        // init graphics
        this.drawArea = set_block_graphics(8, 0);
        group.getChildren().add(this.drawArea);
        add_ports_to_active_group();
        set_dragging_handlers();
        // update the tooltip
        this.update_tooltip();
    }

    /**
     * Updates function defining the blocks. Averages salaries and stock values and adds stock numbers
     */
    public void update_value() {
        if (inputPorts.get(0).get_value().get("stock_value") != null && inputPorts.get(1).get_value().get("stock_value") != null) {
            double stock = this.inputPorts.get(0).get_value().get("stock_number") + this.inputPorts.get(1).get_value().get("stock_number");
            double value = (this.inputPorts.get(0).get_value().get("stock_value") + this.inputPorts.get(1).get_value().get("stock_value"))/2;
            double salary = (this.inputPorts.get(0).get_value().get("average_salary") + this.inputPorts.get(1).get_value().get("average_salary"))/2;
            this.outputPorts.get(0).get_value().put("stock_number", stock);
            this.outputPorts.get(0).get_value().put("stock_value", value);
            this.outputPorts.get(0).get_value().put("average_salary", salary);
            for (Connection x: this.outputPorts.get(0).connections) {
                if (x.inputPort != null && x.outputPort != null) {
                    x.outputPort.values.put("stock_number", stock);
                    x.outputPort.values.put("stock_value", value);
                    x.outputPort.values.put("average_salary", salary);
                }
            }
        }
        this.update_tooltip();
    }
}
