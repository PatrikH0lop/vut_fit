package simulation;

import logic.base.LogicBlock;
import logic.base.Port;
import logic.blocks.*;
import logic.blocks.arithmetic.Adder;
import logic.blocks.constants.Constant;
import logic.blocks.constants.Result;

import java.util.ArrayList;


/**
 * Class representing simulation of constants and adders
 * C (5)
 *     |- A -
 * C (6)    |
 *          | -- A ---A --- R = 20
 * C (1)    |         |
 *     |- A -   C(10) -
 * C (-2)
 */
public class Simulation {
    ArrayList<LogicBlock> blocks;

    public Simulation() {
        blocks = new ArrayList<>();
        blocks.add(new Adder(0));
        blocks.add(new Adder(1));
        blocks.add(new Adder(2));
        blocks.add(new Adder(3));

        blocks.add(new Constant(4));
        blocks.add(new Constant(5));
        blocks.add(new Constant(6));
        blocks.add(new Constant(7));
        blocks.add(new Constant(8));

        blocks.add(new Result(9));

        ((Constant)blocks.get(4)).setValue(5);
        ((Constant)blocks.get(5)).setValue(6);
        ((Constant)blocks.get(6)).setValue(1);
        ((Constant)blocks.get(7)).setValue(-2);
        ((Constant)blocks.get(8)).setValue(10);
        ((Result)blocks.get(9)).setValue(0);

        blocks.get(0).set_arguments(0.0,0.0);
        blocks.get(1).set_arguments(0.0,0.0);
        blocks.get(2).set_arguments(0.0,0.0);
        blocks.get(3).set_arguments(0.0,0.0);

        /** constants to adders connections */
        blocks.get(4).outputPorts.get(0).addConnection(blocks.get(0).inputPorts.get(0));
        blocks.get(5).outputPorts.get(0).addConnection(blocks.get(0).inputPorts.get(1));
        blocks.get(6).outputPorts.get(0).addConnection(blocks.get(1).inputPorts.get(0));
        blocks.get(7).outputPorts.get(0).addConnection(blocks.get(1).inputPorts.get(1));
        blocks.get(8).outputPorts.get(0).addConnection(blocks.get(3).inputPorts.get(1));

        /** adders to adders connections */
        blocks.get(0).outputPorts.get(0).addConnection(blocks.get(2).inputPorts.get(0));
        blocks.get(1).outputPorts.get(0).addConnection(blocks.get(2).inputPorts.get(1));
        blocks.get(2).outputPorts.get(0).addConnection(blocks.get(3).inputPorts.get(0));

        /** adder to result */
        blocks.get(3).outputPorts.get(0).addConnection(blocks.get(9).inputPorts.get(0));
    }

    public void execute_step(int i) {
        for (LogicBlock block : this.blocks) {
            System.out.println("Executing " + i + ": " + block.toString());
            block.execute();
        }
    }

    public void execute_simulation() {
        for (int i = 0; i<20; i++) {
            this.execute_step(i);
            System.out.println("Result: " + blocks.get(9).inputPorts.get(0).getValue());
        }
    }
}