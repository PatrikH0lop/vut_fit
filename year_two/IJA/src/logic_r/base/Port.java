package logic_r.base;

import java.util.*;

import javafx.scene.Group;
import javafx.scene.ImageCursor;
import javafx.scene.control.Tooltip;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.ImagePattern;
import javafx.scene.shape.Line;
import javafx.scene.shape.Rectangle;
import logic_r.blocks.arithmetic.Constant;
import logic_r.connections.Connection;
import main_r.Main;
import ui_r.ClipBoard;
import ui_r.ImageManager;


/**
 * Port of the block, either Input or Output
 * Ports might be connected
 * width, height = 8
 */
public class Port {

    public String name;

    /** Value */
    public double current_value;
    public Map<String, Double> values;

    /** Coordinates */
    public int x;
    public int y;

    public float loc = 0;

    /** GUI */
    public Rectangle drawArea;
    public Image outlookImage;
    public Group g;

    /** Lines */
    public ArrayList<Connection> connections;
    public Connection connection;

    /** Constructor */
    public Port(Group g, String n, int x, int y, float loc) {
        this();
        this.loc = loc;
        this.g = g;
        current_value = 0;
        this.values = new HashMap();
        this.values.put("value", 0.0);
        name = n;
        this.x = x;
        this.y = y;
        drawArea.setTranslateX(x);
        drawArea.setTranslateY(y);
        drawArea = createPinRectangle(this, g);
    }

    /** Simple Constructor */
    public Port() {
        outlookImage = ImageManager.getHmImage("pin_over");
        connections = new ArrayList<>();
        connection = new Connection();
        drawArea = new Rectangle(8, 8);
    }

    /** getters and setter */
    public String getName() {
        return name;
    }

    public Map<String, Double> getValue() {
        return this.values;
    }

    public void setValue(Map<String, Double> value) {
        this.values = value;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getX() {
        return x;
    }

    public void setX(int x) {
        this.x = x;
    }

    public int getY() {
        return y;
    }

    public void setY(int y) {
        this.y = y;
    }

    public Image getGateImage() {
        return outlookImage;
    }

    public void setGateImage(Image gateImage) {
        this.outlookImage = gateImage;
    }

    public List<Connection> getConnections() {
        return connections;
    }

    public void setConnections(ArrayList<Connection> connections) {
        this.connections = connections;
    }

    public Connection getConnection() {
        return connection;
    }

    public void setConnection(Connection connection) {
        this.connection = connection;
    }

    public void setRectangle(Rectangle r) {
        drawArea = r;
    }

    public Rectangle getRectangle() {
        return drawArea;
    }

    /** Init graphic GUI */
    public void initRectangleStuff() {
        /*Connection connectionLineObject = new Connection();
        connections.add(connectionLineObject);*/
        Image cursorImage = ImageManager.getHmImage("cursor_active");
        ImageCursor imageCursor = new ImageCursor(cursorImage, -cursorImage.getWidth(), -cursorImage.getHeight());
        drawArea.setCursor(imageCursor);
        drawArea.setFill(new ImagePattern(outlookImage, 0, 0, 1, 1, true));
        drawArea.setOnMouseDragged((MouseEvent me) -> {
            me.consume();
        });
        setOnMousePressedReleased(outlookImage);
    }

    /** Set handlers */
    public Rectangle createPinRectangle(Port ipo, final Group g) {
        initRectangleStuff();
        drawArea.setOnMouseClicked((MouseEvent me) -> {
            if (me.getButton() == MouseButton.PRIMARY) {
                System.out.println("Clicked on port " + this.name + " value: " + this.getValue());
                for (Connection x : this.connections) {
                    System.out.println("-> " + x.inputPort + x.outputPort);
                }
                if (ClipBoard.getInputPort() == null && ClipBoard.getOutputPort() == null) {
                    setDragBoard(null, ipo, g);
                } else if (ClipBoard.getOutputPort() != null) {
                    Connection tempConnection = new Connection();
                    tempConnection.setOutputPort(ipo);
                    tempConnection.setInputPort(ClipBoard.getOutputPort());
                    Line line = tempConnection.createLine(g, drawArea, drawArea.getWidth(), drawArea.getHeight());
                    Tooltip t = new Tooltip("Source: " + tempConnection.inputPort.getValue() + "\nDestination: " + tempConnection.outputPort.getValue());
                    Tooltip.install(line, t);
                    System.out.println("Adding con to point final: " + this.name + tempConnection.inputPort + tempConnection.outputPort);
                    this.connections.add(tempConnection);
                    //tempConnection.line = line; MAYBE TODO
                    if (line != null && !Main.main.gateGroup.getChildren().contains(line)) {
                        Main.main.gateGroup.getChildren().add(line);
                    }
                    if (ClipBoard.getOutputPort() != null && !ClipBoard.getOutputPort().connections.contains(tempConnection)) {
                        System.out.println("Adding con to point " + ClipBoard.getOutputPort().getName() + tempConnection.inputPort + tempConnection.outputPort);
                        ClipBoard.getOutputPort().connections.add(tempConnection);
                    }
                    if (!Main.main.connections.contains(tempConnection)) {
                        Main.main.connections.add(tempConnection);
                    }
                    ClipBoard.clearClipBoard();
                } else if (ClipBoard.getInputPort() != null) {
                    Connection tempConnection = new Connection();
                    tempConnection.setOutputPort(ClipBoard.getOutputPort());
                    tempConnection.setInputPort(ipo);
                    connections.add(connection);
                    Line line = tempConnection.createLine(g, drawArea, drawArea.getWidth(), drawArea.getHeight());
                    if (line != null && !Main.main.gateGroup.getChildren().contains(line)) {
                        Main.main.gateGroup.getChildren().add(line);
                    }
                    if (ClipBoard.getInputPort() != null && !ClipBoard.getInputPort().connections.contains(tempConnection)) {
                        ClipBoard.getInputPort().connections.add(tempConnection);
                    }
                    if (!Main.main.connections.contains(tempConnection)) {
                        Main.main.connections.add(tempConnection);
                    }
                    ClipBoard.clearClipBoard();
                } else if (ClipBoard.getInputPort() == ipo) {
                    ClipBoard.clearClipBoard();
                } else if (ClipBoard.getInputPort() != ipo && ClipBoard.getInputPort() != null) {
                    setDragBoard(ipo, null, g);
                } else if (ClipBoard.getOutputPort() == ipo) {
                    ClipBoard.clearClipBoard();
                } else if (ClipBoard.getOutputPort() != ipo && ClipBoard.getOutputPort() != null) {
                    setDragBoard(null, ipo, g);
                }
            }
        });
        return drawArea;
    }

    public void setOnMousePressedReleased(Image i) {
        drawArea.setOnMousePressed((MouseEvent me) -> {
            if (me.getButton() == MouseButton.PRIMARY) {
                drawArea.setFill(new ImagePattern(ImageManager.getHmImage("pin_pressed"), 0, 0, 1, 1, true));
                drawArea.toFront();
                me.consume();
            }
        });
        drawArea.setOnMouseReleased((MouseEvent me) -> {
            drawArea.setFill(new ImagePattern(i, 0, 0, 1, 1, true));
            me.consume();
        });
    }

    public void setDragBoard(Port inputPort, Port outputPort, Group g) {
        ClipBoard.clearClipBoard();
        ClipBoard.setInputPort(inputPort)   ;
        ClipBoard.setOutputPort(outputPort);
        if (ClipBoard.getConnectionLineObject() != null) {
            ClipBoard.setLine(ClipBoard.getConnectionLineObject().getLine());
        }
        System.out.println("Setting group:" + g);
        ClipBoard.setGroup(g);
        ClipBoard.setName(name);
        ClipBoard.setX(drawArea.getTranslateX());     // + Dragboard.pinOver.setGroup.getTranslateX()
        System.out.println(drawArea.getLocalToParentTransform().getTx());
        ClipBoard.setY(drawArea.getTranslateY());      // + Dragboard.pinOver.setGroup.getTranslateY()
        ClipBoard.setOutputPort(outputPort);
        ClipBoard.setInputPort(inputPort);
        ClipBoard.setConnectionLineObject(ClipBoard.getConnectionLineObject());
    }

    public void destroyConnectionLineObjects() {
        Iterator<Connection> ipoclo = connections.iterator();
        while (ipoclo.hasNext()) {
            Connection clo = ipoclo.next();
            clo.destroy();
        }
    }

}
