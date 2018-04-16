package logic_r.blocks.arithmetic;

import javafx.scene.Group;
import logic_r.base.Block;
import logic_r.base.Port;
import logic_r.connections.Connection;
import ui_r.ImageManager;


/**
 * Adder blocks, adds N inputs t
 */
public class Result extends Block {

    boolean last = false;

    public Result() {
        super();
        this.local_id = Block.id;
        this.group = new Group();
        this.name = "Result";
        this.outlookImage = ImageManager.getHmImage("result");

        this.inputPorts.add(new Port(group, "PortA", 0, 12, this.local_id));

        this.drawArea = initRectangle(8, 0);
        group.getChildren().add(this.drawArea);
        addPinObjects();
        initGroup();
        this.updateTooltip();
    }

    public Result(float x, float y, float loc) {
        super();
        this.local_id = loc;
        this.group = new Group();
        this.group.setTranslateX(x);
        this.group.setTranslateY(y);
        this.name = "Result";
        this.outlookImage = ImageManager.getHmImage("result");

        this.inputPorts.add(new Port(group, "PortA", 0, 12, loc));

        this.drawArea = initRectangle(8, 0);
        group.getChildren().add(this.drawArea);
        addPinObjects();
        initGroup();
        this.updateTooltip();
    }

    public void updateValue(long deltaTime) {
        this.updateTooltip();
    }

    public boolean isLast() {
        return last;
    }

    public void setLast(boolean last) {
        this.last = last;
    }
}
