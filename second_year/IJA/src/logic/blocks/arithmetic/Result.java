/*
 * Result block represents the end value. This block does not have any
 * output values and is used just as representation of ending of the scheme.
 * @author Patrik Holop
 */
package logic.blocks.arithmetic;

import javafx.scene.Group;
import logic.base.Block;
import logic.base.Port;
import ui.ImageManager;


/**
 * Result block represents the end value. This block does not have any
 * output values and is used just as representation of ending of the scheme.
 * @author Patrik Holop
 */
public class Result extends Block {

    /** Simple constructor */
    public Result() {
        super();
        // set id
        this.local_id = Block.id;
        this.group = new Group();
        this.name = "Result";
        // load texture
        this.outlookImage = ImageManager.load_cached_image("result.png");

        // init ports
        this.inputPorts.add(new Port(group, "PortA", 0, 15, this.local_id));
        this.inputPorts.get(0).values.put("value", null);

        // set graphics
        this.drawArea = set_block_graphics(8, 0);
        group.getChildren().add(this.drawArea);
        add_ports_to_active_group();
        set_dragging_handlers();

        // update the tooltip
        this.update_tooltip();
    }

    /**
     * Simple constructor used after loading
     * @param x Initalize X coordinate
     * @param y Initialize Y coordinate
     * @param loc Block id
     */
    public Result(float x, float y, float loc) {
        super();
        // set id
        this.local_id = loc;
        this.group = new Group();
        // set initialize coordinates
        this.group.setTranslateX(x);
        this.group.setTranslateY(y);
        this.name = "Result";
        // load textures
        this.outlookImage = ImageManager.load_cached_image("result.png");

        // init ports
        this.inputPorts.add(new Port(group, "PortA", 0, 15, loc));
        this.inputPorts.get(0).values.put("value", null);

        // set graphics
        this.drawArea = set_block_graphics(8, 0);
        group.getChildren().add(this.drawArea);
        add_ports_to_active_group();
        set_dragging_handlers();
        // set updates
        this.update_tooltip();
    }

    /** Execution function, updates the value */
    public void update_value() {
        this.update_tooltip();
    }
}
