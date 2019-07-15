#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <string>

/**
 * \defgroup mainwindow GUI
 * Interaction with user such as buttons clicked, keyevents, appearance(positioning).
 * @{
 */

namespace Ui {
class MainWindow;
}

/**
 * Class covering interactions with user
 * @author PetrKapoun
 * @brief Mainwindow interactions
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
/**
     * Sends text from lineEdit to System, expects throw or string, prints it
     * @author PetrKapoun
     * @brief Calculates lineEdit expression
     */
    void on_pushButton_Equality_clicked();

    /**
     * Switch calculator to basic mode, lowers window width, less buttons
     * @author PetrKapoun
     * @brief Disables advanced functions
     * @pre Window is in scientific mode
     */
    void on_action_Basic_triggered();

    /**
     * Switch calculator to scientific mode, wider window, more buttons
     * @author PetrKapoun
     * @brief Adds advanced functions
     * @pre Window is in basic mode
     */
    void on_action_Scientific_triggered();

    /**
     * Prints "1" at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "1" to lineEdit
     */
    void on_pushButton_1_clicked();

    /**
     * Prints "2" at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "2" to lineEdit
     */
    void on_pushButton_2_clicked();

    /**
     * Prints "3" at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "3" to lineEdit
     */
    void on_pushButton_3_clicked();

    /**
     * Prints "4" at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "4" to lineEdit
     */
    void on_pushButton_4_clicked();

    /**
     * Prints "5" at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "5" to lineEdit
     */
    void on_pushButton_5_clicked();

    /**
     * Prints "6" at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "6" to lineEdit
     */
    void on_pushButton_6_clicked();

    /**
     * Prints "7" at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "7" to lineEdit
     */
    void on_pushButton_7_clicked();

    /**
     * Prints "8" at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "8" to lineEdit
     */
    void on_pushButton_8_clicked();

    /**
     * Prints "9" at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "9" to lineEdit
     */
    void on_pushButton_9_clicked();

    /**
     * Prints "0" at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "0" to lineEdit
     */
    void on_pushButton_0_clicked();

    /**
     * Prints "." at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "." to lineEdit
     */
    void on_pushButton_Period_clicked();

    /**
     * Delete all text from lineEdit, no matter what is written in it
     * @author PetrKapoun
     * @brief Delete text from lineEdit
     */
    void on_pushButton_CE_clicked();

    /**
     * Delete all text from lineEdit and lineEdit_Hist, no matter what is written in it
     * @author PetrKapoun
     * @brief Clean lineEdit and lineEdit_Hist
     */
    void on_pushButton_C_clicked();

    /**
     * Prints "+" at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "+" to lineEdit
     */
    void on_pushButton_Plus_clicked();

    /**
     * Prints "-" at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "-" to lineEdit
     */
    void on_pushButton_Minus_clicked();

    /**
     * Prints "*" at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "*" to lineEdit
     */
    void on_pushButton_Multiplication_clicked();

    /**
     * Prints "/" at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "/" to lineEdit
     */
    void on_pushButton_Division_clicked();

    /**
     * Prints "%" at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "%" to lineEdit
     */
    void on_pushButton_Modulo_clicked();

    /**
     * Prints "!" at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "!" to lineEdit
     */
    void on_pushButton_Factorial_clicked();

    /**
     * Prints "groot(" at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "groot(" to lineEdit
     */
    void on_pushButton_Root_clicked();

    /**
     * Prints "^" at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "^" to lineEdit
     */
    void on_pushButton_Power_clicked();

    /**
     * Prints "pi" at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "pi" to lineEdit
     */
    void on_pushButton_Pi_clicked();

    /**
     * Prints "fibo(" at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "fibo(" to lineEdit
     */
    void on_pushButton_FiboNum_clicked();

    /**
     * Prints "(" at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "(" to lineEdit
     */
    void on_pushButton_LeftBracket_clicked();

    /**
     * Prints ")" at the end of lineEdit
     * @author PetrKapoun
     * @brief Add ")" to lineEdit
     */
    void on_pushButton_RightBracket_clicked();

    /**
     * Prints "," at the end of lineEdit
     * @author PetrKapoun
     * @brief Add "," to lineEdit
     */
    void on_pushButton_Comma_clicked();

    /**
     * Show MessageBox with names of authors
     * @author PetrKapoun
     * @brief Show MessageBox
     * @pre No other MessageBox opened
     */
    void on_action_About_triggered();

    /**
     * Prints last result to lineEdit
     * @author PetrKapoun
     * @brief Prints last result
     */
    void on_pushButton_Ans_clicked();

private:
    Ui::MainWindow *ui;

    /**
     * Function allowing user to use keyboard input
     * @author PetrKapoun
     * @brief Key pressed events
     * @pre Key is allowed to be used
     * @param * event Button pushed
     */
    void keyPressEvent(QKeyEvent * event);

    /**
     * Makes lineEdit standart empty if there is error output
     * @author PetrKapoun
     * @brief Makes no error input
     */
    void checkErrorDeactivated();

    bool scientificMode;

    bool errorOutputActive;

    QString ansValue;
};

/**
@}
*/

#endif // MAINWINDOW_H
