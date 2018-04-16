package logic.blocks.functions;

import logic.base.LogicBlock;
import logic.base.Port;

public class Averager extends LogicBlock {
    public Averager(int id) {
        this.id = id;
    }

    @Override
    public void execute() {
        int sum = 0;
        for (Port x: this.inputPorts) {
            sum += x.getValue();
        }
        //return sum/arguments.size()
        // ;
    }

    @Override
    public String toString() {
        String str = "Averager id: " + this.id;
        return str;
    }
}
