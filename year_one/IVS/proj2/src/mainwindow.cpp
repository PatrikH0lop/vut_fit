#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qstring.h"
#include "System.h"
#include <QKeyEvent>
#include <QMessageBox>
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton_Factorial->setVisible(false);
    ui->pushButton_Pi->setVisible(false);
    ui->pushButton_Root->setVisible(false);
    ui->pushButton_Power->setVisible(false);
    ui->pushButton_FiboNum->setVisible(false);
    ui->action_Scientific->setChecked(false);
    ui->action_Basic->setChecked(true);
    this->setFixedWidth(360);
    this->setFixedHeight(300);
    MainWindow::scientificMode = false;
    MainWindow::errorOutputActive = false;
    ansValue = "0";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Equality_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit_Hist->setText(ui->lineEdit->text());
	System::getInstance()->setExpression(std::move(ui->lineEdit->text().toStdString()));
	try
	{
		ui->lineEdit->setText(QString::number(System::getInstance()->evaluate()));
        ansValue = ui->lineEdit->text();
	}
	catch (ParserException& e)
    {
        ui->lineEdit->setStyleSheet("background-color: white;"
                                    "border: none;"
                                    "color:rgb(199, 0, 57);");
        ui->lineEdit->setText(e.what());
        MainWindow::errorOutputActive = true;
	}
}

void MainWindow::on_action_Basic_triggered()
{
    ui->pushButton_Factorial->setVisible(false);
    ui->pushButton_Pi->setVisible(false);
    ui->pushButton_Power->setVisible(false);
    ui->pushButton_Root->setVisible(false);
    ui->pushButton_FiboNum->setVisible(false);
    ui->pushButton_Comma->setVisible(false);
    ui->action_Scientific->setChecked(false);
    this->setFixedWidth(360);
    ui->lineEdit->resize(270, ui->lineEdit->height());
    ui->lineEdit_Hist->resize(270, ui->lineEdit_Hist->height());
    ui->pushButton_C->move(270,0);
    ui->pushButton_CE->move(270,40);
    MainWindow::scientificMode = false;
}

void MainWindow::on_action_Scientific_triggered()
{
    ui->pushButton_Factorial->setVisible(true);
    ui->pushButton_Pi->setVisible(true);
    ui->pushButton_Root->setVisible(true);
    ui->pushButton_Power->setVisible(true);
    ui->pushButton_FiboNum->setVisible(true);
    ui->pushButton_Comma->setVisible(true);
    ui->action_Basic->setChecked(false);
    //ui->MainWindow->setFixedSize(500,500);
    this->setFixedWidth(450);
    ui->lineEdit->resize(360, ui->lineEdit->height());
    ui->lineEdit_Hist->resize(360, ui->lineEdit_Hist->height());
    ui->pushButton_C->move(360,0);
    ui->pushButton_CE->move(360,40);
    MainWindow::scientificMode = true;
    //ui->
}

void MainWindow::on_pushButton_1_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+"1");
}

void MainWindow::on_pushButton_2_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+"2");
}

void MainWindow::on_pushButton_3_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+"3");
}

void MainWindow::on_pushButton_4_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+"4");
}

void MainWindow::on_pushButton_5_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+"5");
}

void MainWindow::on_pushButton_6_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+"6");
}

void MainWindow::on_pushButton_7_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+"7");
}

void MainWindow::on_pushButton_8_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+"8");
}

void MainWindow::on_pushButton_9_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+"9");
}

void MainWindow::on_pushButton_0_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+"0");
}

void MainWindow::on_pushButton_Period_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+".");
}

void MainWindow::on_pushButton_CE_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText("");
}

void MainWindow::on_pushButton_C_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText("");
    ui->lineEdit_Hist->setText("");
}

void MainWindow::on_pushButton_Plus_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+"+");
}

void MainWindow::on_pushButton_Minus_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+"-");
}

void MainWindow::on_pushButton_Multiplication_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+"*");
}

void MainWindow::on_pushButton_Division_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+"/");
}

void MainWindow::on_pushButton_Modulo_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+"%");
}

void MainWindow::on_pushButton_Factorial_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+"!");
}

void MainWindow::on_pushButton_Root_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+"groot(");
}

void MainWindow::on_pushButton_Power_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+"^");
}

void MainWindow::on_pushButton_Pi_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+"pi");//"╥");
}

void MainWindow::on_pushButton_FiboNum_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+"fibo(");
}

void MainWindow::keyPressEvent(QKeyEvent * event)
{
    switch (event->key()) {
    case Qt::Key_0:
        MainWindow::on_pushButton_0_clicked();
        break;
    case Qt::Key_1:
        MainWindow::on_pushButton_1_clicked();
        break;
    case Qt::Key_2:
        MainWindow::on_pushButton_2_clicked();
        break;
    case Qt::Key_3:
        MainWindow::on_pushButton_3_clicked();
        break;
    case Qt::Key_4:
        MainWindow::on_pushButton_4_clicked();
        break;
    case Qt::Key_5:
        MainWindow::on_pushButton_5_clicked();
        break;
    case Qt::Key_6:
        MainWindow::on_pushButton_6_clicked();
        break;
    case Qt::Key_7:
        MainWindow::on_pushButton_7_clicked();
        break;
    case Qt::Key_8:
        MainWindow::on_pushButton_8_clicked();
        break;
    case Qt::Key_9:
        MainWindow::on_pushButton_9_clicked();
        break;
    case Qt::Key_Plus:
        MainWindow::on_pushButton_Plus_clicked();
        break;
    case Qt::Key_Minus:
        MainWindow::on_pushButton_Minus_clicked();
        break;
    case Qt::Key_Period:
        MainWindow::on_pushButton_Period_clicked();
        break;
    case Qt::Key_Slash:
        MainWindow::on_pushButton_Division_clicked();
        break;
    case Qt::Key_Asterisk:
        MainWindow::on_pushButton_Multiplication_clicked();
        break;
    case Qt::Key_Equal:
        MainWindow::on_pushButton_Equality_clicked();
        break;
    case Qt::Key_ParenLeft:
        MainWindow::on_pushButton_LeftBracket_clicked();
        break;
    case Qt::Key_ParenRight:
        MainWindow::on_pushButton_RightBracket_clicked();
        break;
    case Qt::Key_Percent:
        MainWindow::on_pushButton_Modulo_clicked();
        break;
    case Qt::Key_Delete:
        MainWindow::on_pushButton_CE_clicked();
        break;
    case Qt::Key_Enter:
        MainWindow::on_pushButton_Equality_clicked();
        break;
    default:
        break;
    }

    if(MainWindow::scientificMode == true)
    {
        switch (event->key()) {
        case Qt::Key_Exclam:
            MainWindow::on_pushButton_Factorial_clicked();
            break;
        case Qt::Key_Comma:
            MainWindow::on_pushButton_Comma_clicked();
            break;
        case Qt::Key_G:
            MainWindow::on_pushButton_Root_clicked();
            break;
        case Qt::Key_F:
            MainWindow::on_pushButton_FiboNum_clicked();
            break;
        default:
            break;
        }
    }
}

void MainWindow::on_pushButton_LeftBracket_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+"(");
}

void MainWindow::on_pushButton_RightBracket_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+")");
}

void MainWindow::on_pushButton_Comma_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+",");
}

void MainWindow::on_action_About_triggered()
{
    QMessageBox msgBox;
    msgBox.setParent(0);
    msgBox.setWindowTitle("O programu FiboCalc");
    msgBox.setText("<h2><FONT COLOR='#FFFFFF'>IVS - 2. projekt</FONT></h2>"
                   "<h4><FONT COLOR='#FFFFFF'>Tým</h4>"
                   "<p><FONT COLOR='#CCCCFF'>1123581321345589</p>"
                   "<h4><FONT COLOR='#FFFFFF'>Autoři</h4>"
                   "<p><FONT COLOR='#CCCCFF'>Patrik Holop - xholop01<br>"
                   "Petr Kapoun - xkapou04<br>"
                   "Matej Karas - xkaras34<br>"
                   "Tomáš Chocholatý - xchoch07</FONT></p>"
                   "<h4><FONT COLOR='#FFFFFF'>Verze</h4>"
                   "<p><FONT COLOR='#CCCCFF'>1.0, GNU GPLv3</p>");
    msgBox.setStyleSheet("QMessageBox { background-color:rgb(88, 24, 69);  }"
                         "QMessageBox QPushButton {background-color:rgb(199, 0, 57);"
                         "color:rgb(255, 255, 255);"
                         "font-weight: 900;}");
    msgBox.exec();
}

void MainWindow::on_pushButton_Ans_clicked()
{
    MainWindow::checkErrorDeactivated();
    ui->lineEdit->setText(ui->lineEdit->text()+ansValue);
}

void MainWindow::checkErrorDeactivated()
{
    if (MainWindow::errorOutputActive)
    {
        MainWindow::errorOutputActive = false;
        ui->lineEdit->setText("");
        ui->lineEdit->setStyleSheet("background-color: white;"
                                    "border: none;"
                                    "color:rgb(0, 0, 0);");
    }
}
