package ui.base;

import java.awt.Point;


interface ConnectionInterface {
    public void draw();
    public void changeColor(byte r, byte g, byte b);
    public void display();

    @Override
    public String toString();
}


public class Connection implements ConnectionInterface {
    public int id;
    public byte r, g, b;
    public Point start;
    public Point end;

    public Connection(int id) {
        this.id = id;
        this.start = new Point();
        this.end = new Point();
        this.r = this.b = this.g = 0;
    }

    public void draw() {
        System.out.println("Draw not implemented!");
    }

    public void changeColor(byte r, byte g, byte b) {
        this.r = r;
        this.g = g;
        this.b = b;
        this.draw();
    }

    public void display() {
        System.out.println("display not implemented!");
    }

    @Override
    public String toString() {
        System.out.println("Logic compoment");
        return "NOT_IMPLEMENTED";
    }

}
