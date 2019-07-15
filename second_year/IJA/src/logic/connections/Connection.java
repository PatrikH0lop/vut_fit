/*
 * Connection objects represents line between 2 block.
 * Contains information about ports and distributes the values between
 * blocks.
 * @author Matěj Hrabal
 */
package logic.connections;


import javafx.scene.Group;
import javafx.scene.control.Tooltip;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import javafx.scene.shape.Line;
import javafx.scene.shape.Rectangle;
import logic.base.Port;
import main.Main;
import ui.BlockCache;


/**
 * Connection objects represents line between 2 block.
 * Contains information about ports and distributes the values between
 * blocks.
 * @author Matěj Hrabal
 */
public class Connection {

    /** Connection information */
    Tooltip t;

    /** Name of the connection */
    public String name;
    /** Graphic line representing connection */
    public Line line;
    /** Input port of the connection */
    public Port inputPort = null;
    /** Output port of the connection */
    public Port outputPort = null;

    /** Basic onstructor */
    public Connection() {
        name = " Line ";
        line = new Line();
        line.setStrokeWidth(3);
    }

    /** Update tooltip of the connection */
    public void update_tooltip() {
        this.t = new Tooltip("Source: " + this.inputPort.get_value() + "\nDestination: " + this.outputPort.get_value());
        Tooltip.install(this.line, t);
    }

    /** Destroy the connection */
    public void destroy() {
        this.outputPort.connections.remove(this);
        this.inputPort.connections.remove(this);
        Main.main.all_drawable_objects.getChildren().remove(line);
        line = null;
        Main.main.connections.remove(this);
    }

    /** Initialize the graphical line of the connetion
     * @param g Group of the block
     * @param r Drawing area
     * @param width Maximum width of the block
     * @param height Maximum height of the block
     * @return Initialized line
     */
    public Line initialize_line(Group g, Rectangle r, double width, double height) {
        // check the void line
        if (line == null) {
            return null;
        }
        // calculate connection graphics
        line.setStartX(width / 2 + BlockCache.x + BlockCache.group.getTranslateX());
        line.setStartY(height / 2 + BlockCache.y + BlockCache.group.getTranslateY());
        line.setEndX(width / 2 + r.getTranslateX() + g.getTranslateX());
        line.setEndY(height / 2 + r.getTranslateY() + g.getTranslateY());

        //add mouse listeners
        line.setOnMouseClicked((MouseEvent me) -> {
            if (me.getButton() == MouseButton.SECONDARY) {
                destroy();
                me.consume();
            }
        });
        line.setOnMouseEntered((MouseEvent me) -> {
            if (line != null) {
                line.toFront();
            }
            me.consume();
        });
        /*line.setOnMouseExited((MouseEvent me) -> {
            //me.consume();
        });*/
        line.setStroke(Color.RED);
        return line;
    }

    /** set the line
     *@param line New line
     */
    public void setLine(Line line) {
        this.line = line;
    }

    /** set input port
     *@param inputPort New input port
     */
    public void setInputPort(Port inputPort) {
        this.inputPort = inputPort;
    }

    /** set output port
     *@param outputPort New output port
     */
    public void setOutputPort(Port outputPort) {
        this.outputPort = outputPort;
    }
}

