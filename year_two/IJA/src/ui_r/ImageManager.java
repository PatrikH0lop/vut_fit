package ui_r;

import java.io.File;
import java.util.HashMap;
import javafx.scene.image.Image;

public class ImageManager {

    private static final HashMap<String, Image> imageMap = new HashMap<>();
    private static Image noImage;

    public static Image getHmImage(String name) {
        Image image = imageMap.get(name);
        if (image == null) {
            System.out.println("Missing Texture: " + name);
            return noImage;
        }
        return image;
    }

    public static void initMap() {
        System.out.println("Loading init image");
        noImage = new Image("file:src/data/no_image.png");
        System.out.println("Initializing Textures");
        try {
            File folder = new File("src/data");
            File[] listOfFiles = folder.listFiles();
            for (File file : listOfFiles) {
                if (file.isFile()) {
                    String fileName = file.getName();
                    String name = fileName.substring(0, fileName.lastIndexOf('.'));
                    Image i = new Image("file:src/data/" + fileName);
                    imageMap.put(name, i);
                    System.out.println("Loaded Texture: " + name);
                }
            }
            System.out.println("Textures Successfully Initialized");
        } catch (Exception e) {
            System.out.println("Texture Initialization Failed: " + e);
        }
    }

}

