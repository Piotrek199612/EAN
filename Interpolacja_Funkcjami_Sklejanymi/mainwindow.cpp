#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "funkcje_sklejane.h"
#include "funkcje_sklejane_przedzialy.h"
#include <fstream>
#include "Interval.h"
#include <QPen>

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
    if (!ui->arytmetyka_radioButton->isChecked()) // Arytmetyka Zwykła
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

        if (tmp.dobre_dane)
        {
        std::vector<std::vector<long double>> wspolczynniki_funkcji_sklejanych;
        wspolczynniki_funkcji_sklejanych = tmp.Wspolczynniki_Funkcji_Sklejanych();
        ui->wynik_label->clear();
        ui->tableWidget->clear();
        ui->tableWidget->setRowCount(tmp.n +1);
        ui->tableWidget->setColumnCount(5);
        ui->wynik_label->setText(QString::number(tmp.Wartosci_Funkcji_Sklejanych(), 'e', 17));
        for (auto r=0; r<tmp.n+1; r++)
             for (auto c=0; c<=4; c++)
             {
                 if (r ==0 && c == 0)
                 {
                     QTableWidgetItem* tmp = new QTableWidgetItem(QString::fromStdString("a[i][j]"));
                     tmp->setTextAlignment(Qt::AlignCenter);
                     ui->tableWidget->setItem( r, c, tmp);
                     QBrush brush(QColor(204,204,204,255/2),Qt::SolidPattern);
                        tmp->setBackground(brush);
                 }
                 else if (r == 0 && c!=0)
                 {
                     QTableWidgetItem* tmp = new QTableWidgetItem(QString::number(c-1,10));
                     tmp->setTextAlignment(Qt::AlignCenter);
                     ui->tableWidget->setItem( r, c, tmp);
                     QBrush brush(QColor(204,204,204,255/2),Qt::SolidPattern);
                        tmp->setBackground(brush);
                 }
                 else if (c ==0 && r!=0)
                 {
                     QTableWidgetItem* tmp = new QTableWidgetItem(QString::number(r-1,10));
                     tmp->setTextAlignment(Qt::AlignCenter);
                     ui->tableWidget->setItem( r, c, tmp);
                     QBrush brush(QColor(204,204,204,255/2),Qt::SolidPattern);
                        tmp->setBackground(brush);
                 }
                 else
                 {
                     QTableWidgetItem* tmp = new QTableWidgetItem(QString::number(wspolczynniki_funkcji_sklejanych[c-1][r-1],'E',17));
                     ui->tableWidget->setItem( r, c, tmp);
                 }
             }
        ui->tableWidget->resizeColumnsToContents();
        ui->x_edit->setPlainText(x_in);
        ui->f_edit->setPlainText(f_in);
        }
    }
    else //Arytmetyka Przedziołwa
    {
        bool dobre_dane = true;
        interval_arithmetic::Interval<long double>::Initialize();
        x_in.replace(QRegExp("[ \t,:;]{2,}")," ");//usuń powtórzenia
        QStringList x_in_list = x_in.split(QRegExp("(\\ |\\,|\\:|\\t)"));
        std::vector<interval_arithmetic::Interval<long double>> wartosci_x;
        if (x_in_list.size()==ui->n_edit->value()+1)
        for (int i = 0;i<=ui->n_edit->value();i++)
        {
                    if (x_in_list[i].startsWith("[") && x_in_list[i].endsWith("]"))
                    {
                        x_in_list[i].remove("[");
                        x_in_list[i].remove("]");
                        QStringList ends = x_in_list[i].split(QRegExp(";"));
                        if (ends[0].contains(QRegularExpression("^-?[0-9]{1,}$")) || ends[0].contains(QRegularExpression("^-?[0-9]{1,}\\.[0-9]{1,}$")))
                        {
                            interval_arithmetic::Interval<long double> tmp;
                            tmp.a = interval_arithmetic::Interval<long double>::LeftRead(ends[0].toStdString());
                            tmp.b = interval_arithmetic::Interval<long double>::RightRead(ends[1].toStdString());
                            wartosci_x.push_back(tmp);
                        }
                        else
                        {
                            QMessageBox Msgbox;
                                 Msgbox.setText("Błędne dane x");
                                 Msgbox.exec();
                                 dobre_dane = false;
                                 return;
                        }
                    }
                    else
                    {
                        if (x_in_list[i].contains(QRegExp("^-?[0-9]{1,}$")) || x_in_list[i].contains(QRegExp("^-?[0-9]{1,}\\.[0-9]{1,}$")))
                        {
                            interval_arithmetic::Interval<long double> tmp;
                            wartosci_x.push_back(tmp = interval_arithmetic::Interval<long double>::IntRead(x_in_list[i].toStdString()));
                        }
                        else
                        {
                            QMessageBox Msgbox;
                                 Msgbox.setText("Błędne dane x");
                                 Msgbox.exec();
                                 dobre_dane = false;
                                 return;
                        }
                    }
        }

        QRegExp rx("(\\ |\\,|\\:|\\t)"); //RegEx for ' ' or ',' or ':' or '\t'
        QString f_in = ui->f_edit->toPlainText();
        f_in.replace(QRegExp("[ \t,:;]{2,}")," ");
        QStringList f_in_list = f_in.split(QRegExp("(\\ |\\,|\\:|\\t)"));
        std::vector<interval_arithmetic::Interval<long double>> wartosci_f;
        if (f_in_list.size()==ui->n_edit->value()+1)
            for (int i = 0;i<=ui->n_edit->value();i++)
            {
                        if (f_in_list[i].startsWith("[") && f_in_list[i].endsWith("]"))
                        {
                            f_in_list[i].remove("[");
                            f_in_list[i].remove("]");
                            QStringList ends = f_in_list[i].split(QRegExp(";"));
                            if (ends[0].contains(QRegularExpression("^-?[0-9]{1,}$")) || ends[0].contains(QRegularExpression("^-?[0-9]{1,}\\.[0-9]{1,}$")))
                            {
                                interval_arithmetic::Interval<long double> tmp;
                                tmp.a = interval_arithmetic::Interval<long double>::LeftRead(ends[0].toStdString());
                                tmp.b = interval_arithmetic::Interval<long double>::RightRead(ends[1].toStdString());
                                wartosci_f.push_back(tmp);

                            }
                            else
                            {
                                QMessageBox Msgbox;
                                     Msgbox.setText("Błędne dane f");
                                     Msgbox.exec();
                                     dobre_dane = false;
                                     return;
                            }
                        }
                        else
                        {
                            if (f_in_list[i].contains(QRegExp("^-?[0-9]{1,}$")) || f_in_list[i].contains(QRegExp("^-?[0-9]{1,}\\.[0-9]{1,}$")))
                            {
                                interval_arithmetic::Interval<long double> tmp;
                                wartosci_f.push_back(tmp = interval_arithmetic::Interval<long double>::IntRead(f_in_list[i].toStdString()));
                            }
                            else
                            {
                                QMessageBox Msgbox;
                                     Msgbox.setText("Błędne dane f");
                                     Msgbox.exec();
                                     dobre_dane = false;
                                     return;
                            }
                        }
            }

        QString f1x0_tmp = ui->f1x0_edit->text();
        interval_arithmetic::Interval<long double> interval_f1x0;
        if (f1x0_tmp.startsWith("[") && f1x0_tmp.endsWith("]"))
        {
            f1x0_tmp.remove("[");
            f1x0_tmp.remove("]");
            QStringList ends = f1x0_tmp.split(QRegExp(";"));
            if (ends[0].contains(QRegularExpression("^-?[0-9]{1,}$")) || ends[0].contains(QRegularExpression("^-?[0-9]{1,}\\.[0-9]{1,}$")))
            {
                interval_f1x0.a = interval_arithmetic::Interval<long double>::LeftRead(ends[0].toStdString());
                interval_f1x0.b = interval_arithmetic::Interval<long double>::RightRead(ends[1].toStdString());
            }
            else
            {
                QMessageBox Msgbox;
                     Msgbox.setText("Błędne dane f1x0");
                     Msgbox.exec();
                     dobre_dane = false;
                     return;
            }
        }
        else
        {
            if (f1x0_tmp.contains(QRegExp("^-?[0-9]{1,}$")) || f1x0_tmp.contains(QRegExp("^-?[0-9]{1,}\\.[0-9]{1,}$")))
            {
                interval_f1x0 = interval_arithmetic::Interval<long double>::IntRead(f1x0_tmp.toStdString());
            }
            else
            {
                QMessageBox Msgbox;
                     Msgbox.setText("Błędne dane f1x0");
                     Msgbox.exec();
                     dobre_dane = false;
                     return;
            }
        }
        QString f1xn_tmp = ui->f1xn_edit->text();
        interval_arithmetic::Interval<long double> interval_f1xn;
        if (f1xn_tmp.startsWith("[") && f1xn_tmp.endsWith("]"))
        {
            f1xn_tmp.remove("[");
            f1xn_tmp.remove("]");
            QStringList ends = f1xn_tmp.split(QRegExp(";"));
            if (ends[0].contains(QRegularExpression("^-?[0-9]{1,}$")) || ends[0].contains(QRegularExpression("^-?[0-9]{1,}\\.[0-9]{1,}$")))
            {
                interval_f1xn.a = interval_arithmetic::Interval<long double>::LeftRead(ends[0].toStdString());
                interval_f1xn.b = interval_arithmetic::Interval<long double>::RightRead(ends[1].toStdString());
            }
            else
            {
                QMessageBox Msgbox;
                     Msgbox.setText("Błędne dane f1xn");
                     Msgbox.exec();
                     dobre_dane = false;
                     return;
            }
        }
        else
        {
            if (f1xn_tmp.contains(QRegExp("^-?[0-9]{1,}$")) || f1xn_tmp.contains(QRegExp("^-?[0-9]{1,}\\.[0-9]{1,}$")))
            {
                interval_f1xn = interval_arithmetic::Interval<long double>::IntRead(f1xn_tmp.toStdString());
            }
            else
            {
                QMessageBox Msgbox;
                     Msgbox.setText("Błędne dane f1xn");
                     Msgbox.exec();
                     dobre_dane = false;
                     return;
            }
        }

        QString xx_tmp = ui->xx_edit->text();
        interval_arithmetic::Interval<long double> interval_xx;
        if (xx_tmp.startsWith("[") && xx_tmp.endsWith("]"))
        {
            xx_tmp.remove("[");
            xx_tmp.remove("]");
            QStringList ends = xx_tmp.split(QRegExp(";"));
            if (ends[0].contains(QRegularExpression("^-?[0-9]{1,}$")) || ends[0].contains(QRegularExpression("^-?[0-9]{1,}\\.[0-9]{1,}$")))
            {
                interval_xx.a = interval_arithmetic::Interval<long double>::LeftRead(ends[0].toStdString());
                interval_xx.b = interval_arithmetic::Interval<long double>::RightRead(ends[1].toStdString());
            }
            else
            {
                QMessageBox Msgbox;
                     Msgbox.setText("Błędne dane xx");
                     Msgbox.exec();
                     dobre_dane = false;
                     return;
            }
        }
        else
        {
            if (xx_tmp.contains(QRegExp("^-?[0-9]{1,}$")) || xx_tmp.contains(QRegExp("^-?[0-9]{1,}\\.[0-9]{1,}$")))
            {
                interval_xx= interval_arithmetic::Interval<long double>::IntRead(xx_tmp.toStdString());
            }
            else
            {
                QMessageBox Msgbox;
                     Msgbox.setText("Błędne dane xx");
                     Msgbox.exec();
                     dobre_dane = false;
                     return;
            }
        }
        Funkcje_Sklejane_Przedzialy tmp(ui->n_edit->value(),
                         wartosci_x,
                         wartosci_f,
                         interval_f1x0,
                         interval_f1xn,
                         interval_xx);
        if (dobre_dane)
        {
        ui->wynik_label->clear();
        interval_arithmetic::Interval<long double> wartosc_w_punkcie;
        int st;
        wartosc_w_punkcie= tmp.Wartosci_Funkcji_Sklejanych(tmp.ilosc_elementow,tmp.wartosci_x,tmp.wartosci_f,tmp.wartosci_f1x0,tmp.wartosci_f1xn,tmp.wartosci_xx,st);
        std::vector<std::vector<interval_arithmetic::Interval<long double>>> wartosci_wspolczynnikow;
        wartosci_wspolczynnikow = tmp.Wspolczynniki_Funkcji_Sklejanych(tmp.ilosc_elementow,tmp.wartosci_x,tmp.wartosci_f,tmp.wartosci_f1x0,tmp.wartosci_f1xn,wartosci_wspolczynnikow,st);
        string a,b;
        a.clear();
        b.clear();
        wartosc_w_punkcie.IEndsToStrings(a,b);
        ui->wynik_label->setText(QString::fromStdString( a+" "+b));
                ui->tableWidget->clear();
                ui->tableWidget->setRowCount(tmp.ilosc_elementow +1);
                ui->tableWidget->setColumnCount(5);

                for (auto r=0; r<tmp.ilosc_elementow+1; r++)
                     for (auto c=0; c<=4; c++)
                     {
                         if (r ==0 && c == 0)
                         {
                             QTableWidgetItem* tmp = new QTableWidgetItem(QString::fromStdString("a[i][j]"));
                             tmp->setTextAlignment(Qt::AlignCenter);
                             ui->tableWidget->setItem( r, c, tmp);
                             QBrush brush(QColor(204,204,204,255/2),Qt::SolidPattern);
                                tmp->setBackground(brush);
                         }
                         else if (r == 0 && c!=0)
                         {
                             QTableWidgetItem* tmp = new QTableWidgetItem(QString::number(c-1,10));
                             tmp->setTextAlignment(Qt::AlignCenter);
                             ui->tableWidget->setItem( r, c, tmp);
                             QBrush brush(QColor(204,204,204,255/2),Qt::SolidPattern);
                                tmp->setBackground(brush);
                         }
                         else if (c ==0 && r!=0)
                         {
                             QTableWidgetItem* tmp = new QTableWidgetItem(QString::number(r-1,10));
                             tmp->setTextAlignment(Qt::AlignCenter);
                             ui->tableWidget->setItem( r, c, tmp);
                             QBrush brush(QColor(204,204,204,255/2),Qt::SolidPattern);
                                tmp->setBackground(brush);
                         }
                         else
                         {
                             interval_arithmetic::Interval<long double> inter;
                             inter = wartosci_wspolczynnikow[c-1][r-1];
                             inter.IEndsToStrings(a,b);
                             QTableWidgetItem* tmp = new QTableWidgetItem("["+QString::fromStdString(a)+" ; "+QString::fromStdString(b)+"]");

                             QLinearGradient gradient(0, 25,interval_arithmetic::LONGDOUBLE_DIGITS *17, 25);
                             gradient.setColorAt(0, QColor::fromRgb(0, 215, 255, 255/2));
                             gradient.setColorAt(1, QColor::fromRgb(255, 0, 0, 255/2));

                             QBrush brush(gradient);
                                tmp->setBackground(brush);

                             ui->tableWidget->setItem( r, c, tmp);
                         }
                     }
        ui->tableWidget->resizeColumnsToContents();
        ui->x_edit->setPlainText(x_in);
        ui->f_edit->setPlainText(f_in);

        }
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

