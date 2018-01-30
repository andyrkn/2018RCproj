#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myheader.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->progressBar->setMaximum(11);
    ui->progressBar->setMinimum(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

gameclass *MainWindow::signalStage()
{
    gc = new gameclass();
    gc->show();
    return gc;
}

void MainWindow::setText(char x[]) //apend text to Window1
{
    QString y = QString(x);
    ui->plainTextEdit->appendPlainText(y);
}

void MainWindow::SetProgressBar(int x)
{
    ui->progressBar->setValue(x);
}

char *MainWindow::getName()
{
    QByteArray ba = ui->lineEdit->text().toLatin1();
    char *ret = ba.data();

    return ret;
}

char *MainWindow::getPW()
{
    QByteArray ba = ui->lineEdit_2->text().toLatin1();
    char *ret = ba.data();

    return ret;
}

void MainWindow::on_pushButton_clicked()  //login
{
    login(this,"login");
}

void MainWindow::on_pushButton_2_clicked()  //register
{
    login(this,"register");
}
