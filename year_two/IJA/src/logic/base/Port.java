package logic.base;


import java.util.ArrayList;

/**
 * Input or output port of the block
 */
public class Port {
    private double value;

    private ArrayList<Port> connectedPorts;


    public Port(double value) {
        this.value = value;
        connectedPorts = new ArrayList<>();
    }

    public void setValue(double new_value) {
        this.value = new_value;
    }

    public double getValue() {
        return this.value;
    }

    public void addConnection(Port x) {
        connectedPorts.add(x);
    }

    public void propagateResult() {
        for (Port x: connectedPorts) {
            x.setValue(this.value);
        }
    }
}
