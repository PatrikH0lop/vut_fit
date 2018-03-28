package logic.blocks;

import logic.base.LogicBlock;

public class Adder extends LogicBlock {
    public Adder(int id) {
            this.id = id;
        }

    @Override
    public int execute() {
        int sum = 0;
        for (Integer x: this.arguments) {
            sum += x;
        }
        return sum;
    }

    @Override
    public String toString() {
        String str = "Adder id: " + this.id;
        return str;
    }
}
