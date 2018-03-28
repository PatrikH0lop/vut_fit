package logic.base;

import java.util.ArrayList;


interface LogicComponent {
    public int execute();
    public void set_arguments(Integer... arg);

    @Override
    public String toString();
}


public abstract class LogicBlock implements LogicComponent {
    public int id;
    public ArrayList<Integer> arguments;

    public int execute(Integer... arg) {
        System.out.println("Execute not implemented!");
        return -1;
    }


    public void set_arguments(Integer... arg) {
        arguments = new ArrayList<>();
        for (Integer argument: arg) {
            arguments.add(argument);
        }
    }

    @Override
    public String toString() {
        System.out.println("Logic compoment");
        return "NOT_IMPLEMENTED";
    }

}
