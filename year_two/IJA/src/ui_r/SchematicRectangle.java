/**
 * Scheme UI
 */

package ui_r;

import javafx.scene.Group;
import javafx.scene.paint.Color;
import javafx.scene.paint.CycleMethod;
import javafx.scene.paint.LinearGradient;
import javafx.scene.paint.Stop;
import javafx.scene.shape.Line;
import javafx.scene.shape.Rectangle;
import main_r.Main;


/**
 * Scheme UI object
 */
public class SchematicRectangle {

    private Rectangle rectangle;

    public SchematicRectangle(Main main) {
        rectangle = new Rectangle(main.schematicWidth, main.schematicHeigth);
        rectangle.setStroke(Color.WHITE);
        rectangle.setFill(Color.rgb(255,255,191));
    }

    /** draws grid to scheme windows */
    public Group drawGrid(Main main) {
        Group g = new Group();
        for (int i = 0; i < main.schematicWidth; i += 20) {
            Line gridLine = new Line(i, 0, i, rectangle.getHeight());
            gridLine.setStroke(Color.LIGHTGRAY);
            gridLine.setStrokeWidth(1);
            g.getChildren().add(gridLine);
        }
        for (int i = 0; i < main.schematicHeigth; i += 20) {
            Line gridLine = new Line(0, i, rectangle.getWidth(), i);
            gridLine.setStroke(Color.LIGHTGRAY);
            gridLine.setStrokeWidth(1);
            g.getChildren().add(gridLine);
        }
        return g;
    }

    public Rectangle getRectangle() {
        return rectangle;
    }

    public void setRectangle(Rectangle rectangle) {
        this.rectangle = rectangle;
    }
}

