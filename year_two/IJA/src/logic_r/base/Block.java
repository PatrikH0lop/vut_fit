package logic_r.base;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Iterator;

import javafx.geometry.Point2D;
import javafx.scene.Cursor;
import javafx.scene.Group;
import javafx.scene.control.Alert;
import javafx.scene.control.Tooltip;
import javafx.scene.image.Image;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.ImagePattern;
import javafx.scene.shape.Rectangle;
import logic_r.connections.Connection;
import main_r.Main;
import ui_r.ImageManager;
import ui_r.SchematicRectangle;


/**
 * Component block
 * Functional block transforms input to output
 * height, width = 40
 */
public abstract class Block {

    /** Tip */
    public Tooltip t;
    public boolean active = false;
    public static float id = 1;
    public float local_id = 0;

    /**
     * Coordinates
     */
    public double x;
    public double y;

    /**
     * Coordinates used for movement
     */
    public double initX;
    public double initY;

    /**
     * Input and output ports
     */
    public ArrayList<Port> inputPorts;
    public ArrayList<Port> outputPorts;

    /**
     * Other attributes
     */
    public String name;
    public Group group;

    /**
     * Graphic attributes
     */
    public Rectangle drawArea;
    /**
     * rectangle
     */
    public Image outlookImage;
    /**
     * distrinctive gate symbol
     */
    public Point2D dragAnchor;

    /**
     * Simple Constructor
     */
    public Block() {
        this(0, 0);
    }

    /**
     * Constructor
     */
    public Block(double x, double y) {
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
        // init gate
        // init other
        this.drawArea = null;
        this.initX = 0;
        this.initY = 0;
        this.dragAnchor = Point2D.ZERO;
        this.outlookImage = ImageManager.getHmImage("no_image"); /** Textures */
    }

    /**
     * Initializes drawn object with graphics
     */
    public Rectangle initRectangle(double x, double y) {
        this.drawArea = new Rectangle(32, 32);
        this.drawArea.setCursor(Cursor.CLOSED_HAND);
        this.drawArea.setFill(new ImagePattern(outlookImage, 0, 0, 1, 1, true));
        drawArea.setTranslateX(x);  // move 8 to the left because of inputpins on the left
        drawArea.setTranslateY(y);
        return drawArea;
    }

    public void updateTooltip() {
        String message = new String();
        message += "ID: " + this.local_id + "\n";
        for (Port x: this.inputPorts) {
            message += "Input port value: " + x.getValue() + "\n";
        }
        for (Port x: this.outputPorts) {
            message += "Output port value: " + x.getValue() + "\n";
        }
        this.t = new Tooltip(message);
        Tooltip.install(this.drawArea, t);
    }

    /**
     * Set all event handlers
     */
    public void initGroup() {
        /** Store position when mouse is pressed */
        group.setOnMousePressed((MouseEvent me) -> {
            initX = group.getTranslateX();
            initY = group.getTranslateY();
            dragAnchor = new Point2D(me.getSceneX(), me.getSceneY());
        });

        /** Move object on dragging */
        group.setOnMouseDragged((MouseEvent me) -> {
            if (me.getButton() == MouseButton.PRIMARY) {
                if (dragAnchor != null) {
                    double dragX = me.getSceneX() - dragAnchor.getX();
                    double dragY = me.getSceneY() - dragAnchor.getY();
                    double newXPosition = initX + dragX;
                    double newYPosition = initY + dragY;
                    group.setTranslateX(newXPosition - (newXPosition % 32));
                    group.setTranslateY((newYPosition - newYPosition % 32));
                    updateLines();
                }
                me.consume();
            }
        });

        group.setOpacity(0.8f);
        Main.main.gateGroup.getChildren().add(group);
    }

    /**
     * Updates lines when block was moved
     */
    public void updateLines() {

        /** Update input port lines*/
        if (inputPorts != null) {
            Iterator<Port> iterator = this.inputPorts.iterator();
            while (iterator.hasNext()) {
                Port ipo = iterator.next();
                Iterator<Connection> ipoclo = ipo.getConnections().iterator();
                int i = 0;
                while (ipoclo.hasNext()) {
                    Connection clo = ipoclo.next();
                    System.out.println("IN CONNECTION (" + i + ", " + ipo.getName() + ") " + clo.getInputPort() + clo.getOutputPort());
                    i++;
                    System.out.println(clo.getInputPort() + " == " + ipo);
                    if (ipo == clo.getOutputPort() && clo.getLine() != null) {
                        System.out.println("True");
                        clo.getLine().endXProperty().set(4 + ipo.getX() + group.getTranslateX());  // pin width 8, mid 8/2=4
                        clo.getLine().endYProperty().set(4 + ipo.getY() + group.getTranslateY());
                    } else if (clo.getLine() != null) {
                        System.out.println("False");
                        clo.getLine().startXProperty().set(4 + ipo.getX() + group.getTranslateX());
                        clo.getLine().startYProperty().set(4 + ipo.getY() + group.getTranslateY());
                    }
                }
            }
        }

        /** Update output port lines */
        if (outputPorts != null) {
            Iterator<Port> iterator = this.outputPorts.iterator();
            while (iterator.hasNext()) {
                Port opo = iterator.next();
                Iterator<Connection> opoclo = opo.getConnections().iterator();
                int i = 0;
                while (opoclo.hasNext()) {
                    Connection clo = opoclo.next();
                    System.out.println("OUT CONNECTION (" + i + ", " + opo.getName() + ") " + clo.getInputPort() + clo.getOutputPort());
                    i++;
                    if (opo == clo.getOutputPort() && clo.getLine() != null) {
                        System.out.println("T3");
                        clo.getLine().endXProperty().set(4 + opo.getX() + group.getTranslateX());
                        clo.getLine().endYProperty().set(4 + opo.getY() + group.getTranslateY());
                    } else if (clo.getLine() != null) {
                        System.out.println("T4");
                        clo.getLine().startXProperty().set(4 + opo.getX() + group.getTranslateX());
                        clo.getLine().startYProperty().set(4 + opo.getY() + group.getTranslateY());
                    }
                }
            }
        }
    }

    public void addPinObjects() {
        if (this.inputPorts != null) {
            Iterator<Port> iterator = this.inputPorts.iterator();
            while (iterator.hasNext()) {
                group.getChildren().add(iterator.next().getRectangle());
            }
        }
        if (this.outputPorts != null) {
            Iterator<Port> iterator = this.outputPorts.iterator();
            while (iterator.hasNext()) {
                group.getChildren().add(iterator.next().getRectangle());
            }
        }
    }

    public void destroy() {
        Main.main.gateGroup.getChildren().remove(group);
        Main.main.blocks.remove(this);
        if (this.inputPorts != null) {
            Iterator<Port> ipoIterator = this.inputPorts.iterator();
            while (ipoIterator.hasNext()) {
                Port ipo = ipoIterator.next();
                if (Main.main.gateGroup.getChildren().contains(ipo.getConnections().get(0).getLine())) {
                    Main.main.gateGroup.getChildren().remove(ipo.getConnections().get(0).getLine());
                }
                ipo.destroyConnectionLineObjects();
            }
        }
        if (this.outputPorts != null) {
            Iterator<Port> opoIterator = this.outputPorts.iterator();
            while (opoIterator.hasNext()) {
                Port opo = opoIterator.next();
                if (Main.main.gateGroup.getChildren().contains(opo.getConnections().get(0).getLine())) {
                    Main.main.gateGroup.getChildren().remove(opo.getConnections().get(0).getLine());
                }
                opo.destroyConnectionLineObjects();
            }
        }
    }

    public void updateValue(long deltaTime) {

    }

    public void update(long deltaTime) {
        x = group.getTranslateX();
        y = group.getTranslateY();
        //if (gate != null) {
        this.updateValue(deltaTime);
        //}
    }

    public void updateSymbol(long deltaTime) {
        drawArea.setFill(new ImagePattern(outlookImage, 0, 0, 1, 1, true));
    }

    /** Getters and setters */

    public ArrayList<Port> getInputPorts() {
        return this.inputPorts;
    }

    public void setInputPorts(ArrayList<Port> inputPorts) {
        this.inputPorts = inputPorts;
    }

    public ArrayList<Port> getOutputPorts() {
        return this.outputPorts;
    }

    public void setOutputPorts(ArrayList<Port> outputPorts) {
        this.outputPorts = outputPorts;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
}
