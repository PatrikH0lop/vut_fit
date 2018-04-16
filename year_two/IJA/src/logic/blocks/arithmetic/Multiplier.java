package logic.blocks.arithmetic;

import logic.base.LogicBlock;
import logic.base.Port;

public class Multiplier extends LogicBlock {
    public Multiplier(int id) {
            this.id = id;
            this.type = 0;
        }

    @Override
    public void execute() {
        double sum = 1;
        for (Port x: this.inputPorts) {
            sum *= x.getValue();
        }
        //
    }

    @Override
    public String toString() {
        String str = "Subtractor id: " + this.id;
        return str;
    }
}
