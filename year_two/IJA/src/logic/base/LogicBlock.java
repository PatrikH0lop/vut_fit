package logic.base;

import java.util.ArrayList;


/**
 * Logical Component interface
 * Basic method that needs to be implemented
 */
interface LogicComponent {
    public void execute();
    public void set_arguments(Double... arg);
    public void init_output(int number);
    public ArrayList<Double> returnOutput();

    @Override
    public String toString();
}


/**
 * Abstract class representing functional Block
 * Encapsulates all of the blocks
 */
public abstract class LogicBlock implements LogicComponent {
    public int id;
    public int type; /**1-output, 0-not output*/
    public ArrayList<Port> inputPorts;
    public ArrayList<Port> outputPorts;

    public void execute() {
        System.out.println("Execute not implemented!");
    }

    public void set_arguments(Double... arg) {
        this.inputPorts = new ArrayList<>();
        for (Double argument: arg) {
            this.inputPorts.add(new Port(argument));
        }
    }

    public void init_output(int number) {
        this.outputPorts = new ArrayList<Port>();
        for (int i=0; i<number; i++) {
            this.outputPorts.add(new Port(0.0));
        }
    }

    public void set_output(Double ... arg) {
        int i = 0;
        for (Double argument : arg) {
            this.outputPorts.get(i).setValue(argument);
            i++;
        }
    }

    @Override
    public ArrayList<Double> returnOutput() {
        ArrayList<Double> toreturn = new ArrayList<Double>();
        for (Port x: this.outputPorts) {
            toreturn.add(x.getValue());
        }
        return toreturn;
    }

    @Override
    public String toString() {
        System.out.println("Logic compoment");
        return "NOT_IMPLEMENTED";
    }
}
