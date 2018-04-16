package logic.blocks.arithmetic;

import logic.base.LogicBlock;
import logic.base.Port;

import java.util.ArrayList;


public class Divider extends LogicBlock {

    double default_value = 0;

    public Divider(int id) {
        this.id = id;
        this.type = 0;
        this.init_output(1);
    }

    public void set_arguments(Double... arg) {
        this.inputPorts = new ArrayList<>();
        for (Double argument: arg) {
            this.inputPorts.add(new Port(argument));
        }
        default_value = this.inputPorts.get(0).getValue();
        inputPorts.remove(0);
    }

    @Override
    public void execute() {
        double sum = this.default_value;
        for (Port x: this.inputPorts) {
            sum /= x.getValue();
        }
        //return sum;
    }

    @Override
    public String toString() {
        String str = "Divider id: " + this.id;
        return str;
    }
}
