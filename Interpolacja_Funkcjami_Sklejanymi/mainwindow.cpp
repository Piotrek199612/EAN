#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "funkcje_sklejane.h"
#include "funkcje_sklejane_przedzialy.h"
#include <fstream>

#include "Interval.h"

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
    QString x_in = ui->x_edit->toPlainText();
    if (!ui->arytmetyka_radioButton->isChecked())
    {
        x_in.replace(QRegExp("[ \t,:;]{2,}")," ");
        x_in.remove(QRegExp("^[ \t,:]{1,}"));
        x_in.remove(QRegExp("[ \t,:]{1,}$"));
        QString f_in = ui->f_edit->toPlainText();
        f_in.replace(QRegExp("[ \t,:;]{2,}")," ");
        f_in.remove(QRegExp("^[ \t,:]{1,}"));
        f_in.remove(QRegExp("[ \t,:]{1,}$"));

        Funkcje_Sklejane tmp(ui->n_edit->value(),
                         x_in,
                         f_in,
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
        ui->x_edit->setPlainText(x_in);
        ui->f_edit->setPlainText(f_in);
    }
    else
    {
        interval_arithmetic::Interval<long double>::Initialize();
        x_in.replace(QRegExp("[ \t,:;]{2,}")," ");
        x_in.remove(QRegExp("^[ \t,:]{1,}"));
        x_in.remove(QRegExp("[ \t,:]{1,}$"));
        QString f_in = ui->f_edit->toPlainText();
        f_in.replace(QRegExp("[ \t,:;]{2,}")," ");
        f_in.remove(QRegExp("^[ \t,:]{1,}"));
        f_in.remove(QRegExp("[ \t,:]{1,}$"));
        Funkcje_Sklejane_Przedzialy tmp(ui->n_edit->value(),
                         x_in,
                         f_in,
                         ui->f1x0_edit->text(),
                         ui->f1xn_edit->text(),
                         ui->xx_edit->text());
        ui->wynik_label->clear();
        ui->listWidget->clear();
        interval_arithmetic::Interval<long double> result;
        result = tmp.Wartosci_Funkcji_Sklejanych();
        string a,b;
        a.clear();
        b.clear();
        result.IEndsToStrings(a,b);
        ui->wynik_label->setText(QString::fromStdString( a+" "+b));
        ui->x_edit->setPlainText(x_in);
        ui->f_edit->setPlainText(f_in);
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

