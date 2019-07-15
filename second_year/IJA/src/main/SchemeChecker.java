/*
 * This class provides functions to check validity of created scheme
 * before the start of simulation
 * @author Patrik Holop
 */
package main;

import logic.base.Block;
import logic.base.Port;
import logic.connections.Connection;
import java.util.ArrayList;


/**
 * This class provides functions to check validity of created scheme
 * before the start of simulation
 * @author Patrik Holop
 */
public class SchemeChecker {

    /**
     * This method checks, whether given block is part of any cycle using the DFS algorithm
     * @param b Block to be checked
     * @param path Path used for backtracking
     * @return true if cycle was found, otherwise false
     */
    public static boolean check_cycles_block(Block b, ArrayList<Block> path) {
        for (Port p: b.outputPorts) {
            for (Connection c : p.connections) {
                boolean found = false;
                for (Block b_closed : path) {
                    if (b_closed.local_id == c.outputPort.loc)
                        found = true;
                }
                if (found)
                    return true;
                Block new_in_path = null;
                for (Block b_new : Main.main.blocks) {
                    if (b_new.local_id == c.outputPort.loc)
                        new_in_path = b_new;
                }
                path.add(new_in_path);
                boolean sub_cycle = check_cycles_block(new_in_path, path);
                if (sub_cycle)
                    return true;
                path.remove(path.size() -1);
            }
        }
        return false;
    }


    /**
     * This function checks, whether there are loops in created scheme
     * using simple DFS algorithm
     * @return True if scheme contains cycle, else false
     */
    public static boolean containsCycles() {
        ArrayList<Block> path = new ArrayList<>();
        for (Block b : Main.main.blocks) {
            if (check_cycles_block(b, path))
                return true;
        }
        return false;
    }
}

