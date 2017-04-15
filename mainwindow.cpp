#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "funkcje_sklejane.h"
#include <fstream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    Funkcje_Sklejane tmp(ui->n_edit->value(),
                         ui->x_edit->toPlainText(),
                         ui->f_edit->toPlainText(),
                         ui->f1x0_edit->text(),
                         ui->f1xn_edit->text(),
                         ui->xx_edit->text());
    tmp.Wspolczynniki_Funkcji_Sklejanych();
    ui->wynik_label->clear();
    ui->listWidget->clear();

    ui->wynik_label->setText(QString::number(tmp.Wartosci_Funkcji_Sklejanych(), 'e', 14));
    for (int i = 0; i < tmp.n;i++)
        for (int j = 0; j <=3 ; j++)
        {
            ui->listWidget->addItem("a["+QString::number(j)+"]"+"["+QString::number(i)+"]");
            ui->listWidget->addItem(QString::number(tmp.macierz[j][i], 'e', 14));
        }
}

void MainWindow::on_actionWczytaj_triggered()
{
     Wczytaj_Dane();
}

bool MainWindow::Wczytaj_Dane()
{
    std::fstream plik_wejsciowy;
    plik_wejsciowy.open("./input.txt",std::ios::in);
    if (plik_wejsciowy.good())
    {
        int tmp;
     plik_wejsciowy>>tmp;
     ui->n_edit->setValue(tmp);
     QString tmp_str;
     double tmp_double;
     for (int i = 0; i <=tmp; i++)
        {
            plik_wejsciowy>>tmp_double;
            tmp_str += QString::number(tmp_double);
            if (i != tmp) tmp_str+=' ';
         }
     ui->x_edit->setPlainText(tmp_str);
    tmp_str = "";
     for (int i = 0; i <=tmp; i++)
        {
            plik_wejsciowy>>tmp_double;
            tmp_str += QString::number(tmp_double);
            if (i != tmp) tmp_str+=' ';
         }
     ui->f_edit->setPlainText(tmp_str);

     double f1x0_tmp,f1xn_tmp,xx_tmp;
     plik_wejsciowy>>f1x0_tmp;
     ui->f1x0_edit->setText(QString::number(f1x0_tmp));
     plik_wejsciowy>>f1xn_tmp;
     ui->f1xn_edit->setText(QString::number(f1xn_tmp));
     plik_wejsciowy>>xx_tmp;
     ui->xx_edit->setText(QString::number(xx_tmp));
     return true;
    }
    else return false;
}

