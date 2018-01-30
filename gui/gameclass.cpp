#include "gameclass.h"
#include "ui_gameclass.h"

#include<string>
#include<random>

gameclass::gameclass(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::gameclass)
{
    ui->setupUi(this);
    alive = 0;
    answer=0;

    ui->labelQuestion->setAlignment(Qt::AlignCenter);
}

gameclass::~gameclass()
{
    alive = 0;
    delete ui;
}

void gameclass::resetOption()
{
    this->answer=0;
    ui->LabelOptionPicked->setText(QString("-"));
}

int gameclass::getOption()
{
    return answer;
}

void gameclass::hideObjects()
{
    ui->label_2->hide();
    ui->label_3->hide();
    ui->label_4->hide();
    ui->label_5->hide();
}

void gameclass::showObjects()
{
    ui->label_2->show();
    ui->label_3->show();
    ui->label_4->show();
    ui->label_5->show();
}

void gameclass::setQuestionNumber(int a)
{
    ui->labelQuestionNumber->setText(QString::number(a));
}

void gameclass::updateSecondsLeft(int a)
{
    QString txt = QVariant(a).toString();
    ui->label->setText(txt);
}

void gameclass::updateMyPoints(int a)
{
    ui->label_points->setText(QVariant(a).toString());
}

void gameclass::appendRankingLine(char a[])
{
    ui->RankingText->appendPlainText(QString(a));
}

void gameclass::clearRanking()
{
    ui->RankingText->clear();
}

void gameclass::setUserName(char a[])
{
    ui->label_7->setText(QString(a));
}

void gameclass::appendText(char a[])
{
    ui->plainTextEdit->clear();
    QString txt = QString(a);
    ui->plainTextEdit->appendPlainText(txt);
}

void gameclass::setQuestion(char a[])
{
    ui->labelQuestion->setText(QString(a));
}

void gameclass::updateA1(char a[])
{
    ui->label_2->setText(QString(a));
}
void gameclass::updateA2(char a[])
{
    ui->label_3->setText(QString(a));
}
void gameclass::updateA3(char a[])
{
    ui->label_4->setText(QString(a));
}
void gameclass::updateA4(char a[])
{
    ui->label_5->setText(QString(a));
}

void gameclass::on_pushButton_clicked()
{
    alive = -1;
    this->close();
}

void gameclass::on_label_2_clicked()
{
    ui->LabelOptionPicked->setText(QString("1"));
    this->answer=1;
}

void gameclass::on_label_4_clicked()
{
    ui->LabelOptionPicked->setText(QString("3"));
    this->answer=3;
}

void gameclass::on_label_3_clicked()
{
    ui->LabelOptionPicked->setText(QString("2"));
    this->answer=2;
}

void gameclass::on_label_5_clicked()
{
    ui->LabelOptionPicked->setText(QString("4"));
    this->answer=4;
}
