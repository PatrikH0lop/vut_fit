/*
 * This class provides method for initialization of block menu and
 * functional instantiation of blocks using menu buttons
 * @author Matej Hrabal
 */
package ui;


import javafx.event.ActionEvent;
import javafx.scene.control.Button;
import javafx.scene.control.Tab;
import javafx.scene.control.TabPane;
import javafx.scene.control.Tooltip;
import javafx.scene.image.ImageView;
import javafx.scene.layout.VBox;
import logic.base.Block;
import logic.blocks.arithmetic.*;
import logic.blocks.companies.Evaluator;
import logic.blocks.companies.Company;
import logic.blocks.companies.MergeCompany;
import logic.blocks.companies.ResultCompany;
import logic.blocks.powerplant.*;
import main.Main;


/**
 * This class provides method for initialization of block menu and
 * functional instantiation of blocks using menu buttons
 * @author Matej Hrabal
 */
public class BlockMenu {

    public BlockMenu() {
    }

    /**
     * Initializes block menu
     * @return generated block menu
     */
    public TabPane generate_block_box() {

        // create new panel
        TabPane panel = new TabPane();
        panel.setTabClosingPolicy(TabPane.TabClosingPolicy.UNAVAILABLE);
        panel.setMinWidth(150);

        // init tabs
        Tab arithmetic = new Tab();
        arithmetic.setText("ART");
        arithmetic.setTooltip(new Tooltip("Arithmetic blocks"));
        Tab powerplant = new Tab();
        powerplant.setText("PWT");
        powerplant.setTooltip(new Tooltip("Powerplant blocks"));
        Tab market = new Tab();
        market.setText("CMP");
        market.setTooltip(new Tooltip("Company blocks"));

        // arithmetic side bar
        VBox arithmetic_box = new VBox();
        Button b1 = generate_button(Adder.class, "Adder", "Adder\nInput: 2 values\nOutput: value (sum)", "adder.png");
        Button b2 = generate_button(Subtractor.class, "Subtractor", "Subtactor\nInput: 2 values\nOutput: value (difference)", "subtractor.png");
        Button b3 = generate_button(Multiplier.class, "Multiplier", "Multiplier\nInput: 2 values\nOutput: value (product)", "multiplier.png");
        Button b4 = generate_button(Divider.class, "Divider", "Divider\nInput: 2 values\nOutput: value (quotient)","divider.png");
        Button b5 = generate_button(Constant.class, "Constant", "Numerical constant\nOutput: value", "var.png");
        Button b6 = generate_button(Result.class, "Result", "Numerical constant\nInput: value", "result.png");

        // powerplant sidebar
        VBox powerplant_box = new VBox();
        Button b7 = generate_button(Powerplant.class, "Power plant", "Nucler power plant\nOutput: power plant (\n  risk probability\n  temperature\n  energy per day)", "powerplant.png");
        Button b8 = generate_button(CommonRiskEstimator.class, "Common Risk", "Common risk calculator\nInput: 2 power plants\nOutput: value (common risk)", "commonrisk.png");
        Button b9 = generate_button(BetterPlant.class, "Better plant", "Calculates superior power plant\nInput: 2 power plants\nOutput: power plant (higher stats)", "betterplant.png");
        Button b10 = generate_button(ReactorCooler.class, "Reactor Cooler", "Reactor cooler\nInput: power plant and value\nOutput: power plant (with cooled reactor by value)", "cooler.png");
        Button b11 = generate_button(ResultPowerPlant.class, "Final power plant", "Result power plant\nInput: power plant", "powerplant_result.png");

        // company sidebar
        VBox companies_box = new VBox();
        Button b12 = generate_button(Company.class, "Company", "Market company (a.s.)\nOutput: company (\n  stock number\n  stock price\n  average salary)", "company.png");
        Button b13 = generate_button(MergeCompany.class, "Merge Companies", "Merging 2 companies\nInput: 2 companies\nOutput: merged company", "adder.png");
        Button b14 = generate_button(Evaluator.class, "Evaluator", "Evaluates value of the company\nInput: company\nOutput: value (market cap)", "evaluator.png");
        Button b15 = generate_button(ResultCompany.class, "Result company",  "Market company (a.s.)\nInput: company", "company_result.png");

        // add buttons to panel
        arithmetic_box.getChildren().addAll(b1, b2, b3, b4, b5, b6);
        powerplant_box.getChildren().addAll(b7, b8, b9, b10, b11);
        companies_box.getChildren().addAll(b12, b13, b14, b15);

        arithmetic.setContent(arithmetic_box);
        powerplant.setContent(powerplant_box);
        market.setContent(companies_box);

        panel.getTabs().add(arithmetic);
        panel.getTabs().add(powerplant);
        panel.getTabs().add(market);
        return panel;
    }

    /** Function to generate new generation buttons
     * @param _class Class linked the to generation button
     * @param name Tooltip name
     * @param hint Tooltip of the button
     * @param image_name Name of the image
     * @return Generated button
     */
    public Button generate_button(Class<? extends Block> _class, String name, String hint, String image_name) {
        Button button = new Button();
        // init button
        button.setMinWidth(150);
        button.setText(name);
        ImageView imageview = new ImageView(ImageManager.load_cached_image(image_name));
        imageview.setFitHeight(32);
        imageview.setFitWidth(32);
        button.setGraphic(imageview);
        button.setTooltip(new Tooltip(hint));
        button.setOnAction((ActionEvent event) -> {
            // try to instantiate new block
            try {
                Main.main.blocks.add(_class.getDeclaredConstructor().newInstance());
                event.consume();
            } catch (Exception e) {
                // error
            }
        });
        return button;
    }
}

