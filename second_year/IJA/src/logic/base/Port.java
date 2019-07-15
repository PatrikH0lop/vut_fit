/*
 * Port connected to certain Block.
 * This port might represent input or output port.
 * It's coordinates are relative to the group.
 * @author Patrik Holop
 */
package logic.base;


import java.util.*;
import javafx.scene.Group;
import javafx.scene.ImageCursor;
import javafx.scene.control.Tooltip;
import javafx.scene.image.Image;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.paint.ImagePattern;
import javafx.scene.shape.Line;
import javafx.scene.shape.Rectangle;
import logic.connections.Connection;
import main.Main;
import ui.BlockCache;
import ui.ImageManager;


/**
 * Port connected to certain Block.
 * This port might represent input or output port.
 * It's coordinates are relative to the group.
 * @author Patrik Holop
 */
public class Port {

    /** Name of the port */
    public String name;

    /** Actual values of ports propagated by block */
    public Map<String, Double> values;

    /** X coordinate of the block */
    public int x;
    /** Y coordinate of the block */
    public int y;

    /** Specific ID of the block, that this port belongs to */
    public float loc = 0;

    /** Graphic area of the port */
    public Rectangle drawArea;
    /** Texture of the image */
    public Image outlookImage;
    /** Group of the port */
    public Group g;

    /** Connections of the port */
    public ArrayList<Connection> connections;
    /** Temporary connection used in operations */
    public Connection connection;

    /** Constructor
     * @param g Initial group of the block
     * @param n Name of the port
     * @param x X coordinate
     * @param y Y coordinate
     * @param loc ID of the block
     */
    public Port(Group g, String n, int x, int y, float loc) {
        this();
        // initialize values
        this.loc = loc;
        this.g = g;
        this.values = new HashMap();
        //this.values.put("value", 0.0);
        name = n;
        // initialize graphics
        this.x = x;
        this.y = y;
        drawArea.setTranslateX(x);
        drawArea.setTranslateY(y);
        drawArea = create_new_drawing_area(this, g);
    }

    /** Simple Constructor */
    public Port() {
        // load image
        outlookImage = ImageManager.load_cached_image("pin_over.png");
        // initialize values
        connections = new ArrayList<>();
        connection = new Connection();
        drawArea = new Rectangle(10, 10);
    }

    /** Getter for the name
     * @return Name
     */
    public String get_name() {
        return name;
    }
    /** Setter for the name
     * @param name new name
     */
    public void set_name(String name) {
        this.name = name;
    }
    /** Getter for the map of values
     * @return value Values
     */
    public Map<String, Double> get_value() {
        return this.values;
    }
    /** Setter for the values
     * @param value New values
     */
    public void set_value(Map<String, Double> value) {
        this.values = value;
    }
    /**
     * Getter for the connections
      * @return Connections
     */
    public List<Connection> get_connections() {
        return connections;
    }
    /**
     * Setter for the connection
     * @param connections New connections
     */
    public void set_connection(ArrayList<Connection> connections) {
        this.connections = connections;
    }
    /**
     * Get temporary connection
     * @return Temporary connection
     */
    public Connection get_connection() {
        return connection;
    }
    /**
     * Set connection
     * @param connection New connection
     */
    public void set_connection(Connection connection) {
        this.connection = connection;
    }
    /**
     * Getter for the area
     * @return Drawing area
     */
    public Rectangle get_rectangle() {
        return drawArea;
    }

    /**
     * Create new drawing area for the port and set handlers
     * @param currect_port Port
     * @param g Current group
     * @return Drawing area
     */
    public Rectangle create_new_drawing_area(Port currect_port, final Group g) {
        // change current cursor to active
        Image cursorImage = ImageManager.load_cached_image("cursor_active.png");
        ImageCursor imageCursor = new ImageCursor(cursorImage, -cursorImage.getWidth(), -cursorImage.getHeight());
        drawArea.setCursor(imageCursor);
        drawArea.setFill(new ImagePattern(outlookImage, 0, 0, 1, 1, true));

        // consume all dragging actions while selecting
        drawArea.setOnMouseDragged((MouseEvent me) -> {
            me.consume();
        });
        setOnMousePressedReleased(outlookImage);

        // creating connections
        drawArea.setOnMouseClicked((MouseEvent me) -> {
            // set on click listener
            if (me.getButton() == MouseButton.PRIMARY) {

                // cache memory is empty, first point was selected
                if (BlockCache.inputPort == null && BlockCache.outputPort == null) {
                    // find correct block object
                    Block to_process_block = null;
                    for (Block b: Main.main.blocks) {
                        if (b.local_id == loc)
                            to_process_block = b;

                    }
                    // start of the line can't be input port
                    if (to_process_block.inputPorts.contains(this)) {
                        return;
                    }
                    // cache the point
                    save_to_cache(null, currect_port, g);
                  // second point was selected
                } else if (BlockCache.outputPort != null) {
                    // find the correct port
                    Block block_to_process = null;
                    for (Block b: Main.main.blocks) {
                        if (b.local_id == loc)
                            block_to_process = b;
                    }
                    // end of the block cant be output port
                    if (block_to_process.outputPorts.contains(this)) {
                        BlockCache.clear();
                        return;
                    }
                    // output block cant have different types of data
                    Set<String> keys_first = BlockCache.outputPort.get_value().keySet();
                    Set<String> keys_second = currect_port.get_value().keySet();
                    if (!(keys_first.containsAll(keys_second) && keys_second.containsAll(keys_first))) {
                        BlockCache.clear();
                        return;
                    }
                    // cant connect block with itself
                    if (block_to_process.local_id == BlockCache.outputPort.loc) {
                        BlockCache.clear();
                        return;
                    }
                    // cant connect block to already assigned port
                    if (currect_port.connections != null && currect_port.connections.size() > 0) {
                        BlockCache.clear();
                        return;
                    }

                    // create the new connection
                    Connection temporary_connection = new Connection();
                    temporary_connection.setOutputPort(currect_port);
                    temporary_connection.setInputPort(BlockCache.outputPort);
                    Line line = temporary_connection.initialize_line(g, drawArea, drawArea.getWidth(), drawArea.getHeight());
                    // set new tooltip
                    Tooltip t = new Tooltip("Source: " + temporary_connection.inputPort.get_value() + "\nDestination: " + temporary_connection.outputPort.get_value());
                    Tooltip.install(line, t);

                    // add new connection to the port and main classes
                    this.connections.add(temporary_connection);
                    // add line to main
                    if (line != null && !Main.main.all_drawable_objects.getChildren().contains(line)) {
                        Main.main.all_drawable_objects.getChildren().add(line);
                    }
                    // add the connecition to the port
                    if (BlockCache.outputPort != null && !BlockCache.outputPort.connections.contains(temporary_connection)) {
                        BlockCache.outputPort.connections.add(temporary_connection);
                    }
                    // add to the main connections
                    if (!Main.main.connections.contains(temporary_connection)) {
                        Main.main.connections.add(temporary_connection);
                    }
                    // clear the cacche
                    BlockCache.clear();
                } else if (BlockCache.outputPort == currect_port) {
                    // the end point and new point cant be the same
                    BlockCache.clear();
                } else if (BlockCache.outputPort != currect_port && BlockCache.outputPort != null) {
                    // save the output port
                    save_to_cache(null, currect_port, g);
                }
            }
        });
        // return correct drawable area
        return drawArea;
    }

    /**
     * Change the graphics from released to normal state
     * @param i Image used after mouse release
     */
    public void setOnMousePressedReleased(Image i) {
        drawArea.setOnMousePressed((MouseEvent me) -> {
            if (me.getButton() == MouseButton.PRIMARY) {
                drawArea.setFill(new ImagePattern(ImageManager.load_cached_image("pin_pressed.png"), 0, 0, 1, 1, true));
                drawArea.toFront();
                me.consume();
            }
        });
        drawArea.setOnMouseReleased((MouseEvent me) -> {
            drawArea.setFill(new ImagePattern(i, 0, 0, 1, 1, true));
            me.consume();
        });
    }

    /**
     * Save pressed port to cache memory
     * @param input_port Input port
     * @param output_port Output port
     * @param g Group of the block
     */
    public void save_to_cache(Port input_port, Port output_port, Group g) {
        // clear the cache
        BlockCache.clear();
        BlockCache.inputPort = input_port;
        BlockCache.outputPort = output_port;
        if (BlockCache.connection != null) {
            BlockCache.line = BlockCache.connection.line;
        }
        BlockCache.group = g;
        BlockCache.name = name;
        BlockCache.x = drawArea.getTranslateX();
        BlockCache.y = drawArea.getTranslateY();
        BlockCache.outputPort = output_port;
        BlockCache.inputPort = input_port;
    }

    /**
     * Destroy all connections
     */
    public void destroyConnectionLineObjects() {
        for (int i = this.connections.size()-1; i>=0; i--) {
            Connection c = this.connections.get(i);
            c.destroy();
        }
    }
}
