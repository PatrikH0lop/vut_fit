package logic.blocks.arithmetic;

import logic.base.LogicBlock;
import logic.base.Port;

/**
 * Adder Block
 * Adds N numbers and gives one output
 */
public class Adder extends LogicBlock {

    public Adder(int id) {
            this.id = id;
            this.type = 0;
            this.init_output(1);
        }

    @Override
    public void execute() {
        double sum = 0;
        for (Port x: this.inputPorts) {
            sum += x.getValue();
        }
        this.set_output(sum);
        this.outputPorts.get(0).propagateResult();
    }

    @Override
    public String toString() {
        String str = "Adder id: " + this.id;
        return str;
    }
}
