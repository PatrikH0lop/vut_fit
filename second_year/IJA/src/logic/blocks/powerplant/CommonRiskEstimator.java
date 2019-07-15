/*
 * CommonRiskEstimator block calculates general risk based on 2 input nuclear
 * power plants and propagates calculated value to output
 * @author Patrik Holop
 */
package logic.blocks.powerplant;

import javafx.scene.Group;
import logic.base.Block;
import logic.base.Port;
import logic.connections.Connection;
import ui.ImageManager;


/**
 * CommonRiskEstimator block calculates general risk based on 2 input nuclear
 * power plants and propagates calculated value to output
 * @author Patrik Holop
 */
public class CommonRiskEstimator extends Block {

    /** Simple constructor */
    public CommonRiskEstimator() {
        super();
        // set local id
        this.local_id = Block.id;
        // create new group for block and ports
        this.group = new Group();
        this.name = "CommonRiskEstimator";
        // load the texture
        this.outlookImage = ImageManager.load_cached_image("commonrisk.png");

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
    public CommonRiskEstimator(float x, float y, float loc) {
        super();
        // set id
        this.local_id = loc;
        this.group = new Group();
        // initialize coordinates
        this.group.setTranslateX(x);
        this.group.setTranslateY(y);
        this.name = "CommonRiskEstimator";
        // load texture
        this.outlookImage = ImageManager.load_cached_image("commonrisk.png");
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
     * Updates function defining the blocks. Calculates the risk
     */
    public void update_value() {
        if (inputPorts.get(0).get_value().get("risk_probability") != null && inputPorts.get(1).get_value().get("risk_probability") != null) {
            double sum = 1-(1-this.inputPorts.get(0).get_value().get("risk_probability"))*(1-this.inputPorts.get(1).get_value().get("risk_probability"));
            sum = Math.max(Math.min(sum, 1.0), 0.0);
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
