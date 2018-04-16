/**
 * Clipboard
 * Temporary usable object used when connecting 2 ports
 * Cache like behaviour
 */

package ui_r;
import javafx.scene.Group;
import javafx.scene.shape.Line;
import logic_r.base.Block;
import logic_r.base.Port;
import logic_r.connections.Connection;


/**
 * Clipboard
 * Temporary usable object used when connecting 2 ports
 * Cache like behaviour
 */
public class ClipBoard {

    private static Block block;
    private static Connection connection;
    private static Line line;
    private static String name;

    private static Object object;
    private static Group group;
    private static Port outputPort;
    private static Port inputPort;
    private static double x = -1;
    private static double y = -1;

    public static Object getObject() {
        return object;
    }

    public static void setObject(Object object) {
        ClipBoard.object = object;
    }

    public static void setBlock(Block go) {
        block = go;
    }

    public static Block getBlock() {
        return block;
    }

    public static void setConnectionLineObject(Connection clo) {
        connection = clo;
    }

    public static Connection getConnectionLineObject() {
        return connection;
    }

    public static Group getGroup() {
        return group;
    }

    public static void setGroup(Group g) {
        group = g;
    }

    public static Line getLine() {
        return line;
    }

    public static void setLine(Line l) {
        line = l;
    }

    public static String getName() {
        return name;
    }

    public static void setName(String n) {
        name = n;
    }

    public static Port getInputPort() {
        return inputPort;
    }

    public static void setInputPort(Port ip) {
        inputPort = ip;
    }

    public static Port getOutputPort() {
        return outputPort;
    }

    public static void setOutputPort(Port op) {
        outputPort = op;
    }

    public static double getX() {
        return x;
    }

    public static double getY() {
        return y;
    }

    public static void setX(double newX) {
        x = newX;
    }

    public static void setY(double newY) {
        y = newY;
    }

    public static void clearClipBoard() {
        block = null;
        connection = null;
        //connectionLineObject2 = null;
        line = null;
        name = null;
        object = null;
        group = null;
        inputPort = null;
        outputPort = null;
        x = -1;
        y = -1;
    }
}

