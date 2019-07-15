/*
 * Temporary usable memory used when connecting 2 ports
 * to cache the first port
 * @author Matěj Hrabal
 */

package ui;
import javafx.scene.Group;
import javafx.scene.shape.Line;
import logic.base.Block;
import logic.base.Port;
import logic.connections.Connection;


/**
 * Temporary usable memory used when connecting 2 ports
 * to cache the first port
 * @author Matěj Hrabal
 */
public class BlockCache {

    /** cached block */
    public static Block block;
    /** cached connection */
    public static Connection connection;
    /** cached line */
    public static Line line;
    /** cached name */
    public static String name;
    /** cached group */
    public static Group group;
    /** cached end point port */
    public static Port outputPort;
    /** cached entry point port */
    public static Port inputPort;
    /** cached X coordinate*/
    public static double x = -1;
    /** cached Y coordinate*/
    public static double y = -1;

    /** This method clears the cache memory */
    public static void clear() {
        block = null;
        connection = null;
        line = null;
        name = null;
        group = null;
        inputPort = null;
        outputPort = null;
        x = -1;
        y = -1;
    }
}

