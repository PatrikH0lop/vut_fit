package logic_r.blocks.arithmetic;

import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.control.TextInputDialog;
import javafx.stage.Modality;
import javafx.stage.Stage;
import javafx.stage.StageStyle;
import logic_r.base.Block;
import logic_r.base.Port;
import logic_r.connections.Connection;
import main_r.Main;
import ui_r.ImageManager;

import java.util.Map;
import java.util.Optional;


/**
 * Adder blocks, adds N inputs t
 */
public class Constant extends Block {

    boolean last = false;

    public Constant() {
        super();
        this.local_id = Block.id;
        this.group = new Group();
        this.name = "Constant";
        this.outlookImage = ImageManager.getHmImage("var");

        TextInputDialog dialog = new TextInputDialog("10");
        dialog.setTitle("Constant value");
        dialog.setHeaderText("Please enter value");
        dialog.setContentText("Value:");
        Optional<String> result = dialog.showAndWait();

        this.outputPorts.add(new Port(this.group, "PortC", 40, 12, this.local_id));
        this.outputPorts.get(0).getValue().put("value", Double.parseDouble(result.get()));
        this.updateTooltip();

        this.drawArea = initRectangle(8, 0);
        group.getChildren().add(this.drawArea);
        addPinObjects();
        initGroup();
        this.updateTooltip();
    }

    public Constant(Map<String, Double> value, double x, double y, float loc) {
        super();
        this.local_id = loc;
        this.group = new Group();
        this.group.setTranslateX(x);
        this.group.setTranslateY(y);
        this.name = "Constant";
        this.outlookImage = ImageManager.getHmImage("var");

        /*TextInputDialog dialog = new TextInputDialog("10");
        dialog.setTitle("Constant value");
        dialog.setHeaderText("Please enter value");
        dialog.setContentText("Value:");
        Optional<String> result = dialog.showAndWait();*/

        this.outputPorts.add(new Port(this.group, "PortC", 40, 12, loc));
        this.outputPorts.get(0).setValue(value);
        this.updateTooltip();

        this.drawArea = initRectangle(8, 0);
        group.getChildren().add(this.drawArea);
        addPinObjects();
        initGroup();
        this.updateTooltip();
    }

    public void updateValue(long deltaTime) {
        //System.out.println("Updating constant");
        this.outputPorts.get(0).getValue().put("value", this.outputPorts.get(0).getValue().get("value"));
        for (Connection x: this.outputPorts.get(0).connections) {
            if (x.inputPort != null && x.outputPort != null) {
                x.outputPort.getValue().put("value", this.outputPorts.get(0).getValue().get("value"));
            }
        }
        this.updateTooltip();
    }

    public boolean isLast() {
        return last;
    }

    public void setLast(boolean last) {
        this.last = last;
    }
}

