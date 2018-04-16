package logic.blocks.constants;

import logic.base.LogicBlock;
import logic.base.Port;


/**
 * Constant block
 */
public class Constant extends LogicBlock {

    public Constant(int id) {
        this.id = id;
        this.init_output(1);
    }

    public void setValue(double number) {
        this.outputPorts.get(0).setValue(number);
    }

    @Override
    public void execute() {
        this.outputPorts.get(0).propagateResult();
    }

    @Override
    public String toString() {
        String str = "Constant id: " + this.id + " value: " + this.outputPorts.get(0).getValue();
        return str;
    }
}
