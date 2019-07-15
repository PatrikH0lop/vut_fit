/*
 * This class is used as interface with graphic resources and textures.
 * Contains cache filled on start with all image in data folder.
 * During program execution when image is loaded, it's just copied from this cache.
 * Minimalizes number of external loads.
 * @author Matěj Hrabal
 */
package ui;

import java.io.File;
import java.util.HashMap;
import java.util.Map;
import java.util.SimpleTimeZone;

import javafx.scene.image.Image;


/**
 * This class is used as interface with graphic resources and textures.
 * Contains cache filled on start with all image in data folder.
 * During program execution when image is loaded, it's just copied from this cache.
 * Minimalizes number of external loads.
 * @author Matěj Hrabal
 */
public class ImageManager {

    /** Cached images */
    static Map<String, Image> image_cache = new HashMap<>();

    /**
     * This method returns cached image
     * @param name Name of the image (with extension)
     * @return Cached image
     */
    public static Image load_cached_image(String name) {
        return image_cache.get(name);
    }

    /**
     * This method initializes cache memory
     */
    public static void load_cache() {
        try {
            File folder = new File("lib/data");
            File[] files = folder.listFiles();
            for (File file : files) {
                if (file.isFile()) {
                    String fileName = file.getName();
                    image_cache.put(fileName, new Image("file:lib/data/" + fileName));
                }
            }
        } catch (Exception e) {
            // something went wrong
        }
    }

}

