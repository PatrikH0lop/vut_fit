package logic_r.blocks.arithmetic;

import javafx.scene.Group;
import logic_r.base.Block;
import logic_r.base.Port;
import logic_r.connections.Connection;
import ui_r.ImageManager;

/**
 * Adder blocks, adds N inputs t
 */
public class Divider extends Block {

    boolean last = false;

    public Divider() {
        super();
        this.local_id = Block.id;
        this.group = new Group();
        System.out.println("Divider group: " + this.group);
        this.name = "Divider";
        this.outlookImage = ImageManager.getHmImage("divider");

        this.inputPorts.add(new Port(this.group,"PortA", 0, 2, this.local_id));
        this.inputPorts.add(new Port(this.group, "PortB", 0, 22, this.local_id));
        this.outputPorts.add(new Port(this.group, "PortC", 40, 12, this.local_id));

        this.drawArea = initRectangle(8, 0);
        group.getChildren().add(this.drawArea);
        addPinObjects();
        initGroup();
        this.updateTooltip();
    }

    public Divider(float x, float y, float loc) {
        super();
        this.local_id = loc;
        this.group = new Group();
        this.group.setTranslateX(x);
        this.group.setTranslateY(y);
        System.out.println("Divider group: " + this.group);
        this.name = "Divider";
        this.outlookImage = ImageManager.getHmImage("divider");

        this.inputPorts.add(new Port(this.group, "PortA", 0, 2, loc));
        this.inputPorts.add(new Port(this.group, "PortB", 0, 22, loc));
        this.outputPorts.add(new Port(this.group,"PortC", 40, 12, loc));

        this.drawArea = initRectangle(8, 0);
        group.getChildren().add(this.drawArea);
        addPinObjects();
        initGroup();
        this.updateTooltip();
    }

    public void updateValue(long deltaTime) {
        //System.out.println("Updating adder");
        double sum = (double) this.inputPorts.get(0).getValue().get("value") / (double) this.inputPorts.get(1).getValue().get("value");
        this.outputPorts.get(0).getValue().put("value", sum);
        for (Connection x: this.outputPorts.get(0).connections) {
            if (x.inputPort != null && x.outputPort != null) {
                x.outputPort.getValue().put("value", sum);
            }
        }
        this.updateTooltip();
        //return false;
    }

    public boolean isLast() {
        return last;
    }

    public void setLast(boolean last) {
        this.last = last;
    }
}
