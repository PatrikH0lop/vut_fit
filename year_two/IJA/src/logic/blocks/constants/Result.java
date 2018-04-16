package logic.blocks.constants;

import logic.base.LogicBlock;
import logic.base.Port;

import java.util.ArrayList;


/**
 * Result block
 */
public class Result extends LogicBlock {

    public Result(int id) {
        this.id = id;
        this.init_output(1);
        this.set_arguments(0.0);
    }

    public void setValue(double number) {
        this.outputPorts.get(0).setValue(number);
    }

    @Override
    public void execute() {
        /**/
    }

    @Override
    public String toString() {
        String str = "Result id: " + this.id;
        return str;
    }
}

