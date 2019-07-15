/*
 * SchemeWindow is the class representing board on which blocks and connections operate
 * Represented is mainly by rectangle
 * @author Matěj Hrabal
 */

package ui;

import javafx.scene.paint.Color;
import javafx.scene.shape.Rectangle;


/**
 * SchemeWindow is the class representing board on which blocks and connections operate
 * Represented is mainly by rectangle
 * @author Matěj Hrabal
 */
public class SchemeWindow {

    /** Width of the scheme window */
    public int windows_width = 850;
    /** Height of the scheme window */
    public int window_height = 460;

    /** Rectangle representing the area of window*/
    public Rectangle rectangle;

    /** Simple constructor filling the area */
    public SchemeWindow() {
        rectangle = new Rectangle(windows_width, window_height);
        rectangle.setStroke(Color.WHITE);
        // fill it width coffee color
        rectangle.setFill(Color.rgb(255,255,191));
    }
}

