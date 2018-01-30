#ifndef GAMECLASS_H
#define GAMECLASS_H

#include <QMainWindow>

namespace Ui {
class gameclass;
}

class gameclass : public QMainWindow
{
    Q_OBJECT

public:
    explicit gameclass(QWidget *parent = 0);
    ~gameclass();
    int alive;
    int answer;

    void resetOption();
    int getOption();

    void hideObjects();
    void showObjects();

    void setQuestionNumber(int a);
    void setQuestion(char a[]);
    void updateA1(char a[]);
    void updateA2(char a[]);
    void updateA3(char a[]);
    void updateA4(char a[]);

    void setUserName(char a[]);
    void appendText(char a[]);   //text box for error handling
    void updateSecondsLeft(int a);
    void updateMyPoints(int a);

    void appendRankingLine(char a[]);
    void clearRanking();

private slots:
    void on_pushButton_clicked();  //leave game

    void on_label_2_clicked();

    void on_label_4_clicked();

    void on_label_3_clicked();

    void on_label_5_clicked();

private:
    Ui::gameclass *ui;

};

#endif // GAMECLASS_H
