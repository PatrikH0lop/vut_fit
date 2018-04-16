package ui.menus;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.embed.swing.SwingFXUtils;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.control.*;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.image.Image;
import javafx.scene.image.ImageView;
import javafx.scene.input.MouseButton;
import javafx.scene.input.MouseEvent;
import javafx.scene.layout.AnchorPane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Priority;
import javafx.scene.paint.Color;
import javafx.scene.shape.*;
import javafx.scene.shape.Rectangle;
import javafx.util.Callback;
import org.w3c.dom.events.Event;


import java.awt.*;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;
import java.util.ResourceBundle;


public class Controller implements Initializable {

    @FXML
    private ListView<HBoxCell> constBlocksListView;

    @FXML
    private ListView<HBoxCell> arithBlocksListView;

    @FXML
    private ListView<HBoxCell> logicBlocksListView;

    @FXML
    private ListView<HBoxCell> funcBlocksListView;

    @FXML
    private GridPane gridpane;

    @FXML
    private TabPane tabpane;

    int x_selected = -1;
    int y_selected = -1;
    int x_new = -1;
    int y_new = -1;

    @FXML
    private AnchorPane rooot;

    @Override
    public void initialize(URL url, ResourceBundle rb) {
        List<HBoxCell> constBlockList = new ArrayList<>();
        constBlockList.add(new HBoxCell("Number", new Image(getClass().getResourceAsStream("../images/number.png"))));
        constBlockList.add(new HBoxCell("π", new Image(getClass().getResourceAsStream("../images/pi.png"))));
        ObservableList<HBoxCell> const_data = FXCollections.observableList(constBlockList);
        constBlocksListView.setItems(const_data);

        List<HBoxCell> arithBlockList = new ArrayList<>();
        arithBlockList.add(new HBoxCell("Adder", new Image(getClass().getResourceAsStream("../images/plus.png"))));
        arithBlockList.add(new HBoxCell("Subtractor", new Image(getClass().getResourceAsStream("../images/minus.png"))));
        arithBlockList.add(new HBoxCell("Multiplier", new Image(getClass().getResourceAsStream("../images/x.png"))));
        arithBlockList.add(new HBoxCell("Divider", new Image(getClass().getResourceAsStream("../images/slash.png"))));
        ObservableList<HBoxCell> arith_data = FXCollections.observableList(arithBlockList);
        arithBlocksListView.setItems(arith_data);

        List<HBoxCell> logicBlockList = new ArrayList<>();
        logicBlockList.add(new HBoxCell("AND", new Image(getClass().getResourceAsStream("../images/and.png"))));
        logicBlockList.add(new HBoxCell("OR", new Image(getClass().getResourceAsStream("../images/or.png"))));
        ObservableList<HBoxCell> logic_data = FXCollections.observableList(logicBlockList);
        logicBlocksListView.setItems(logic_data);

        List<HBoxCell> funcBlockList = new ArrayList<>();
        funcBlockList.add(new HBoxCell("Averager", new Image(getClass().getResourceAsStream("../images/average.png"))));
        ObservableList<HBoxCell> func_data = FXCollections.observableList(funcBlockList);
        funcBlocksListView.setItems(func_data);

        gridpane.addEventHandler(MouseEvent.MOUSE_CLICKED, new EventHandler<MouseEvent>() {
            @Override
            public void handle(MouseEvent e) {
                if (e.getButton() == MouseButton.PRIMARY) {
                    int row = (int) e.getX() / 45;
                    int col = (int) e.getY() / 45;
                    int index = tabpane.getSelectionModel().getSelectedIndex();
                    ListView t = new ListView();
                    switch (index) {
                        case 0:
                            t = constBlocksListView;
                            break;

                        case 1:
                            t = arithBlocksListView;
                            break;
                        case 2:
                            t = logicBlocksListView;
                            break;
                        case 3:
                            t = funcBlocksListView;
                            break;
                    }
                    ObservableList<HBoxCell> selected = t.getSelectionModel().getSelectedItems();
                    ImageView image = (ImageView) selected.get(0).button.graphicProperty().getValue();
                    ImageView new_image = new ImageView();
                    new_image.setImage(SwingFXUtils.toFXImage(SwingFXUtils.fromFXImage(image.getImage(), null), null));
                    new_image.setFitWidth(15);
                    new_image.setFitHeight(15);
                    gridpane.add(new_image, row, col);
                }
            }
        });

        gridpane.addEventHandler(MouseEvent.MOUSE_CLICKED, new EventHandler<MouseEvent>() {
                    @Override
                    public void handle(MouseEvent e) {
                        if (e.getButton() == MouseButton.SECONDARY) {
                            System.out.println(e.getX() + " " + e.getY());
                            if (x_selected == -1) {
                                x_selected = (int) e.getX()+230;
                                y_selected = (int) e.getY()+40;
                            } else if (x_new == -1) {
                                x_new = (int) e.getX()+230;
                                y_new = (int) e.getY()+40;
                                // TODO
                                Path path = new Path();
                                MoveTo moveTo = new MoveTo();
                                moveTo.setX(x_selected);
                                moveTo.setY(y_selected);

                                LineTo lineTo = new LineTo();
                                lineTo.setX(x_new);
                                lineTo.setY(y_new);

                                path.getElements().add(moveTo);
                                path.getElements().add(lineTo);
                                path.setStrokeWidth(5);
                                path.setStroke(Color.BLACK);
                                rooot.getChildren().add(path);
                                x_selected = y_selected = y_new = x_new = -1;
                            }
                        }
                    }
                }
        );
    }

    public static class HBoxCell extends HBox {
        Label label = new Label();
        Button button = new Button();

        HBoxCell(String labelText, Image image) {
            super();
            label.setText(labelText);
            label.setMaxWidth(Double.MAX_VALUE);
            HBox.setHgrow(label, Priority.ALWAYS);

            ImageView im = new ImageView(image);
            im.setFitHeight(15);
            im.setFitWidth(15);
            button.setGraphic(im);
            button.setDisable(true);
            this.getChildren().addAll(label, button);
        }
    }
}
