/**
 * Side Bar with Blocks
 */
package ui_r;

import javafx.event.ActionEvent;
import javafx.geometry.Point2D;
import javafx.scene.control.Button;
import javafx.scene.control.Tooltip;
import javafx.scene.image.ImageView;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.VBox;
import logic_r.base.Block;
import logic_r.blocks.arithmetic.*;
import main_r.Main;


/**
 * Side bar with blocks
 */
public class SideBarBuilder {

    private final Main main;

    private double initX;
    private double initY;
    private Point2D dragAnchor;

    public SideBarBuilder(final Main main) {
        this.main = main;
    }

    /**
     * Creates left side menu button for
     * block creation
     */
    public VBox buildSideBarWithButtons() {
        VBox sideBar = new VBox();

        /** Crete buttons on sideBar */
        final Button butt1 = createBlockButton(Adder.class, "Adder", "adder");
        final Button butt2 = createBlockButton(Subtractor.class, "Subtractor", "subtractor");
        final Button butt3 = createBlockButton(Multiplier.class, "Multiplier", "multiplier");
        final Button butt4 = createBlockButton(Divider.class, "Divider", "divider");
        final Button butt5 = createBlockButton(Constant.class, "Constant", "var");
        final Button butt6 = createBlockButton(Result.class, "Result", "result");

        main.rootGroup.setOnKeyPressed((KeyEvent ke) -> {
            if (ke.getCode() == KeyCode.DIGIT1) {
                butt1.fire();
            }
            if (ke.getCode() == KeyCode.DIGIT2) {
                butt2.fire();
            }
            if (ke.getCode() == KeyCode.DIGIT3) {
                butt3.fire();
            }
            if (ke.getCode() == KeyCode.DIGIT4) {
                butt4.fire();
            }
            if (ke.getCode() == KeyCode.DIGIT5) {
                butt5.fire();
            }
            if (ke.getCode() == KeyCode.DIGIT4) {
                butt6.fire();
            }
            ke.consume();
        });

        /** Add buttons to menu */
        sideBar.getChildren().addAll(butt1, butt2, butt3, butt4, butt5, butt6);
        return sideBar;
    }

    /** Creates block button */
    public Button createBlockButton(Class<? extends Block> g, String name, String gateName) {
        final Button button = new Button();
        button.setGraphic(new ImageView(ImageManager.getHmImage(gateName)));
        button.setTooltip(new Tooltip(name));
        button.setOnAction((ActionEvent event) -> {
            /** try to instantiate */
            try {
                main.blocks.add(g.getDeclaredConstructor().newInstance());
                event.consume();
            } catch (Exception e) {
                /** pass */
            }
        });
        return button;
    }
}

