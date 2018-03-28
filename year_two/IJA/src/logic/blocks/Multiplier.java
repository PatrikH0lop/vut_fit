package logic.blocks;

import logic.base.LogicBlock;

public class Multiplier extends LogicBlock {
    public Multiplier(int id) {
            this.id = id;
        }

    @Override
    public int execute() {
        int sum = 1;
        for (Integer x: this.arguments) {
            sum *= x;
        }
        return sum;
    }

    @Override
    public String toString() {
        String str = "Subtractor id: " + this.id;
        return str;
    }
}
