#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gameclass.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    gameclass *signalStage(); //signal that the game is starting

    void setText(char x[]);  //append text box error handling
    void SetProgressBar(int x);
    char* getName();
    char* getPW();

private slots:
    void on_pushButton_clicked(); //login

    void on_pushButton_2_clicked();  //register

private:
    Ui::MainWindow *ui;
    gameclass *gc;
};

#endif // MAINWINDOW_H
