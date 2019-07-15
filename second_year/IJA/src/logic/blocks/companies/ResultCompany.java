/*
 * Result company blocks represents the same company as Company block.
 * This block is used for optional saving of the result.
 * @author Patrik Holop
 */
package logic.blocks.companies;

import javafx.scene.Group;
import logic.base.Block;
import logic.base.Port;
import ui.ImageManager;
import java.util.Map;


/**
 * Result company blocks represents the same company as Company block.
 * This block is used for optional saving of the result.
 * @author Patrik Holop
 */
public class ResultCompany extends Block {

    /**
     * Simple constructor
     */
    public ResultCompany() {
        super();
        // set the local id
        this.local_id = Block.id;
        this.group = new Group();
        this.name = "CompanyResult";
        // load the texture
        this.outlookImage = ImageManager.load_cached_image("company_result.png");

        // add new port
        this.inputPorts.add(new Port(this.group, "PortC", 0, 15, this.local_id));
        // set it's value
        this.inputPorts.get(0).get_value().put("stock_number", null);
        this.inputPorts.get(0).get_value().put("stock_value", null);
        this.inputPorts.get(0).get_value().put("average_salary", null);
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
    public ResultCompany(double x, double y, float loc) {
        super();
        // set the local id
        this.local_id = loc;
        this.group = new Group();
        // initialize the coordinates
        this.group.setTranslateX(x);
        this.group.setTranslateY(y);
        this.name = "CompanyResult";
        // load the texture
        this.outlookImage = ImageManager.load_cached_image("company_result.png");
        // set the port
        this.inputPorts.add(new Port(this.group, "PortC", 0, 15, loc));
        // set it's value
        this.inputPorts.get(0).get_value().put("stock_number", null);
        this.inputPorts.get(0).get_value().put("stock_value", null);
        this.inputPorts.get(0).get_value().put("average_salary", null);
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

