package logic_r.connections;

import javafx.scene.Group;
import javafx.scene.control.Tooltip;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.Color;
import javafx.scene.shape.Line;
import javafx.scene.shape.Rectangle;
import logic_r.base.Port;
import main_r.Main;
import ui_r.ClipBoard;


/**
 * Connection object between 2 ports
 */
public class Connection {

    /** Tip */
    Tooltip t;

    /** Basic things*/
    public String name;
    public Line line;
    public Color color = null;
    protected boolean last = false;
    public Port inputPort = null;
    public Port outputPort = null;

    /** Constructor */
    public Connection() {
        name = " Line ";
        line = new Line();
        line.setStrokeWidth(4);
        color = Color.RED;
    }

    /** Update color */
    public void update(Long deltaTime) {
        System.out.println("Updating line");
        line.setStroke(color);
        this.t = new Tooltip("Source: " + this.inputPort.getValue() + "\nDestination: " + this.outputPort.getValue());
        Tooltip.install(this.line, t);
    }

    /** On remove */
    public void destroy() {
        if (Main.main.gateGroup.getChildren().contains(line)) {
            Main.main.gateGroup.getChildren().remove(line);
            line = null;
        }
        if (Main.main.connections.contains(this)) {  // -> connectionlineobject
            Main.main.connections.remove(this);
        }
    }

    /** Create GUI line object */
    public Line createLine(Group g, Rectangle r, double width, double height) {
        if (line == null) {
            return null;
        }
        System.out.println("Creating line");
        line.setStartX(width / 2 + ClipBoard.getX() + ClipBoard.getGroup().getTranslateX());    // + Dragboard.pinOver.setGroup.getTranslateX()
        line.setStartY(height / 2 + ClipBoard.getY() + ClipBoard.getGroup().getTranslateY());    // + Dragboard.pinOver.setGroup.getTranslateY()
        line.setEndX(width / 2 + r.getTranslateX() + g.getTranslateX());    // + pinOver.setGroup.getTranslateX()
        line.setEndY(height / 2 + r.getTranslateY() + g.getTranslateY());  // + pinOver.setGroup.getTranslateY()

        //add mouse listeners
        line.setOnMouseClicked((MouseEvent me) -> {
            if (me.getButton() == MouseButton.MIDDLE) {
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
        line.setOnMouseExited((MouseEvent me) -> {
            //me.consume();
        });
        line.setStroke(color);
        return line;
    }

    public Line getLine() {
        return line;
    }

    public void setLine(Line line) {
        this.line = line;
    }

    public Port getInputPort() {
        return inputPort;
    }

    public void setInputPort(Port inputPort) {
        this.inputPort = inputPort;
    }

    public Port getOutputPort() {
        return outputPort;
    }

    public void setOutputPort(Port outputPort) {
        this.outputPort = outputPort;
    }

}

