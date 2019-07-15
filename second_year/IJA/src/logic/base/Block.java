/*
 * Block scheme component, which functinally transforms
 * input to output based on internal execution function
 * @author Patrik Holop
 */
package logic.base;


import java.util.ArrayList;
import java.util.Iterator;
import javafx.geometry.Point2D;
import javafx.scene.Cursor;
import javafx.scene.Group;
import javafx.scene.control.Tooltip;
import javafx.scene.image.Image;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.ImagePattern;
import javafx.scene.shape.Rectangle;
import logic.connections.Connection;
import main.Main;


/**
 * Block scheme component, which functinally transforms
 * input to output based on internal execution function
 * @author Patrik Holop
 */
public abstract class Block {

    /** Hint */
    public Tooltip t;

    /** Active block is marked the next to be executed in simulation */
    public boolean active = false;

    /** Marks every generated block with order ID */
    public static float id = 1;

    /** Marked ID used for block identification */
    public float local_id = 0;

    /** X coordinate of the block */
    public double x;
    /** Y coordinate of the block */
    public double y;

    /** Starting X coordinate when dragging */
    public double starting_position_x;
    /** Starting Y coordinate when dragging */
    public double starting_position_y;

    /** Input ports of the block */
    public ArrayList<Port> inputPorts;
    /** Ouput ports of the block */
    public ArrayList<Port> outputPorts;

    /** Name of the block */
    public String name;
    /** Group where block belongs to */
    public Group group;

    /** Graphical representation of the block */
    public Rectangle drawArea;
    /** Texture of the block */
    public Image outlookImage;

    /** dragging point */
    public Point2D dragging_point;

    /**
     * Simple Constructor
     */
    public Block() {
        this(0, 0);
    }

    /**
     * Constructor
     * @param x Initial X coordinate
     * @param y Initial Y coordinate
     */
    public Block(double x, double y) {
        // set local id based on order of generation
        this.local_id = Block.id;
        Block.id += 1;
        // init coordinates
        this.x = x;
        this.y = y;
        // init ports
        this.inputPorts = new ArrayList<>();
        this.outputPorts = new ArrayList<>();
        // init name
        this.name = "noname";
        // init group
        this.group = new Group();
        // init graphics
        this.drawArea = null;
        this.starting_position_x = 0;
        this.starting_position_y = 0;
        this.dragging_point = Point2D.ZERO;
        this.outlookImage = null;
    }

    /**
     * Initializes drawn object with graphics
     * @param x Initialize X coordinate
     * @param y Initialize Y coordinate
     * @return Draw area of the block
     */
    public Rectangle set_block_graphics(double x, double y) {
        this.drawArea = new Rectangle(40, 40);
        this.drawArea.setCursor(Cursor.CLOSED_HAND);
        this.drawArea.setFill(new ImagePattern(outlookImage, 0, 0, 1, 1, true));
        // offset of the block graphic due to ports
        drawArea.setTranslateX(x);
        drawArea.setTranslateY(y);
        return drawArea;
    }

    /** Updates tooltip of the block based on ports */
    public void update_tooltip() {
        String message = new String();
        message += "ID: " + this.local_id + "\n";
        for (Port x: this.inputPorts) {
            message += "Input port value: " + x.get_value() + "\n";
        }
        for (Port x: this.outputPorts) {
            message += "Output port value: " + x.get_value() + "\n";
        }
        this.t = new Tooltip(message);
        Tooltip.install(this.drawArea, t);
    }

    /** Set event handlers for dragging the block */
    public void set_dragging_handlers() {
        // set current position before dragging
        group.setOnMousePressed((MouseEvent me) -> {
            if (me.getButton() == MouseButton.PRIMARY) {
                starting_position_x = group.getTranslateX();
                starting_position_y = group.getTranslateY();
                dragging_point = new Point2D(me.getSceneX(), me.getSceneY());
            } else if (me.getButton() == MouseButton.SECONDARY) {
                this.destroy();
            }
        });

        // move object
        group.setOnMouseDragged((MouseEvent me) -> {
            if (me.getButton() == MouseButton.PRIMARY) {
                if (dragging_point != null) {
                    double dragging_x = me.getSceneX() - dragging_point.getX();
                    double dragging_y = me.getSceneY() - dragging_point.getY();
                    double new_x = starting_position_x + dragging_x;
                    double new_y = starting_position_y + dragging_y;
                    // update_tooltip group position
                    group.setTranslateX(new_x - (new_x % 40));
                    group.setTranslateY((new_y - new_y % 40));
                    // update_tooltip line
                    updateLines();
                }
                me.consume();
            }
        });
        // actualize group in main objects
        Main.main.all_drawable_objects.getChildren().add(group);
    }

    /** Updates lines based on iterator
     * @param iterator Port iterator
     */
    public void updateLinesInt(Iterator<Port> iterator) {
        while (iterator.hasNext()) {
            Port ipo = iterator.next();
            Iterator<Connection> ipoclo = ipo.get_connections().iterator();
            int i = 0;
            while (ipoclo.hasNext()) {
                Connection clo = ipoclo.next();
                i++;
                if (ipo == clo.outputPort && clo.line != null) {
                    clo.line.endXProperty().set(4 + ipo.x + group.getTranslateX());  // pin width 8, mid 8/2=4
                    clo.line.endYProperty().set(4 + ipo.y + group.getTranslateY());
                } else if (clo.line != null) {
                    clo.line.startXProperty().set(4 + ipo.x + group.getTranslateX());
                    clo.line.startYProperty().set(4 + ipo.y + group.getTranslateY());
                }
            }
        }
    }

    /** Update all lines connected to the block during dragging */
    public void updateLines() {
        // update_tooltip input ports
        if (inputPorts != null) {
            Iterator<Port> iterator = this.inputPorts.iterator();
            updateLinesInt(iterator);
        }

        // update_tooltip output ports
        if (outputPorts != null) {
            Iterator<Port> iterator = this.outputPorts.iterator();
            updateLinesInt(iterator);
        }
    }

    /** Add all ports to the current group for relative drawing */
    public void add_ports_to_active_group() {
        // add input ports
        if (this.inputPorts != null) {
            Iterator<Port> iterator = this.inputPorts.iterator();
            while (iterator.hasNext()) {
                group.getChildren().add(iterator.next().get_rectangle());
            }
        }
        // add output ports
        if (this.outputPorts != null) {
            Iterator<Port> iterator = this.outputPorts.iterator();
            while (iterator.hasNext()) {
                group.getChildren().add(iterator.next().get_rectangle());
            }
        }
    }

    /** Removes all ports from based iterator
     * @param to_remove Iterator for the ports
     */
    public void remove_iteratively(Iterator<Port> to_remove) {
        while (to_remove.hasNext()) {
            Port port = to_remove.next();
            if (Main.main.all_drawable_objects.getChildren() != null && port.get_connections() != null && port.get_connections().size() > 0) {
                if (Main.main.all_drawable_objects.getChildren().contains(port.get_connections().get(0).line)) {
                    Main.main.all_drawable_objects.getChildren().remove(port.get_connections().get(0).line);
                }
            }
            port.destroyConnectionLineObjects();
        }
    }

    /** Removes current block from all lists */
    public void destroy() {
        Main.main.all_drawable_objects.getChildren().remove(group);
        Main.main.blocks.remove(this);
        // remove input ports
        if (this.inputPorts != null) {
            Iterator<Port> input_iterator = this.inputPorts.iterator();
            remove_iteratively(input_iterator);
        }
        // remove output ports
        if (this.outputPorts != null) {
            Iterator<Port> output_iterator = this.outputPorts.iterator();
            remove_iteratively(output_iterator);
        }
    }

    /** To be implemented in child classes */
    public void update_value() {
    }

    /** Update value of the current block */
    public void update() {
        x = group.getTranslateX();
        y = group.getTranslateY();
        //if (gate != null) {
        this.update_value();
        //}
    }

    /** Getter of output ports
     * @return output ports list of the block
     */
    public ArrayList<Port> getOutputPorts() {
        return this.outputPorts;
    }
}
