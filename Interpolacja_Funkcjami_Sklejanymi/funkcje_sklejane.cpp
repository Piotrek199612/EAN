#include "funkcje_sklejane.h"
#include <fstream>
#include <QMessageBox>


#include "Interval.h"

Funkcje_Sklejane::Funkcje_Sklejane(int size)
{
    //Wczytaj_Dane();
}
Funkcje_Sklejane::Funkcje_Sklejane(int size, QString x_in,QString f_in,QString  f1x0_in,QString  f1xn_in,QString  xx_in)
{
    n = size;
    if (f1x0_in.contains(QRegExp("^-?[0-9]{1,}$")) || f1x0_in.contains(QRegExp("^-?[0-9]{1,}\\.[0-9]{1,}$")))
        f1x0 = f1x0_in.toDouble();
    else
    {
        QMessageBox Msgbox;
             Msgbox.setText("Błędne dane f1x0");
             Msgbox.exec();
             this->dobre_dane = false;
             return;
    }

    if (f1xn_in.contains(QRegExp("^-?[0-9]{1,}$")) || f1xn_in.contains(QRegExp("^-?[0-9]{1,}\\.[0-9]{1,}$")))
        f1xn = f1xn_in.toDouble();
    else
    {
        QMessageBox Msgbox;
             Msgbox.setText("Błędne dane f1xn");
             Msgbox.exec();
             this->dobre_dane = false;
             return;
    }

    if (xx_in.contains(QRegExp("^-?[0-9]{1,}$")) || xx_in.contains(QRegExp("^-?[0-9]{1,}\\.[0-9]{1,}$")))
        xx = xx_in.toDouble();
    else
    {
        QMessageBox Msgbox;
             Msgbox.setText("Błędne dane xx");
             Msgbox.exec();
             this->dobre_dane = false;
             return;
    }
    QRegExp rx("(\\ |\\,|\\:|\\t)"); //RegEx for ' ' or ',' or ':' or '\t'
    QStringList x_in_list = x_in.split(rx);
    QStringList f_in_list = f_in.split(rx);


    if (x_in_list.size() == n+1)
        for (int i = 0;i<=n ;i++)
        if (x_in_list[i].contains(QRegExp("^-?[0-9]{1,}$")) || x_in_list[i].contains(QRegExp("^-?[0-9]{1,}\\.[0-9]{1,}$")))
        {
            x.push_back(x_in_list[i].toDouble());
        }
        else
        {
            QMessageBox Msgbox;
                 Msgbox.setText("Błędne dane x");
                 Msgbox.exec();
                 this->dobre_dane = false;
                 return;
        }


    if (f_in_list.size() == n+1)
        for (int i = 0;i<=n;i++)
            if (f_in_list[i].contains(QRegExp("^-?[0-9]{1,}$")) || f_in_list[i].contains(QRegExp("^-?[0-9]{1,}\\.[0-9]{1,}$")))
            {
                f.push_back(f_in_list[i].toDouble());
            }
            else
            {
                QMessageBox Msgbox;
                     Msgbox.setText("Błędne dane f");
                     Msgbox.exec();
                     this->dobre_dane = false;
                     return;
            }
}

std::vector <std::vector<long double>> Funkcje_Sklejane::Wspolczynniki_Funkcji_Sklejanych()
{
    int i;
    long double u,v,y,z,xi;
    macierz = Inicjalizuj_Macierz<long double>(3,n);

    std::vector <long double> b,c,d;
        Inicjalizuj_Vector(b,n);
        Inicjalizuj_Vector(c,n);
        Inicjalizuj_Vector(d,n);

    if (n < 1) st = 1;
    else
    {
        st = 0;
        i = -1;
        for (int i = 0; i <=n;i++)
            for (int j = 0; j<=n;j++)
                if (i!=j && x[i] == x[j])
                    st = 2;
    }

    if (st == 0)
    {
        b[0] = 1;
        u = x[1] - x[0];
        d[0] = 6*((f[1]-f[0])/u - f1x0)/u;
        c[n] = 1;
        u = x[n] - x[n-1];
        d[n] = 6*(f1xn - (f[n] - f[n-1])/u)/u;
        for (i = 1; i <= n-1;i++)
        {
            z = x[i];
            y = x[i+1] - z;
            z = z - x[i-1];
            u = f[i];
            b[i] = y/(y+z);
            c[i] = 1-b[i];
            d[i] = 6*((f[i+1]-u)/y - (u - f[i-1])/z)/(y+z);
        }
        u =2;
        i = -1;
        y = d[0] / u;
        d[0] = y;
        do
        {
            i++;
            z = b[i]/u;
            b[i] = z;
            u = 2 - z*c[i+1];
            y = (d[i+1] - y*c[i+1])/u;
            d[i+1] = y;
        }while(i < n-1);
        u = d[n];
        for (i = n-1; i >=0;i--)
        {
            u = d[i] - u*b[i];
            d[i] = u;
        }

        for (i = 0; i<= n-1;i++)
        {
            u = f[i];
            xi = x[i];
            z = x[i+1] - xi;
            y = d[i];
            v = (f[i+1] - u)/z - (2*y+d[i+1])*z/6;
            z = (d[i+1] - y)/(6*z);
            y = y/2;
            macierz[0][i] = ((-z*xi+y)*xi - v)*xi + u;
            u = 3*z*xi;
            macierz[1][i] = (u-2*y)*xi +v;
            macierz[2][i] = y-u;
            macierz[3][i] = z;
        }
    }
    return macierz;
}

long double Funkcje_Sklejane::Wartosci_Funkcji_Sklejanych()
{
    int i;
    long double u,y,z;
    bool found;
    std::vector <long double> a,b,c,d;
    Inicjalizuj_Vector(a,3);
    Inicjalizuj_Vector(b,n);
    Inicjalizuj_Vector(c,n);
    Inicjalizuj_Vector(d,n);

    if (n < 1) st = 1;
    else if (xx<x[0] || xx>x[n]) st = 3;
   else
    {
        st = 0;
        i = -1;
        for (int i = 0; i <=n;i++)
            for (int j = 0; j<=n;j++)
                if (i!=j && x[i] == x[j])
                    st = 2;
     }
        if (st == 0)
        {
            b[0] = 1.0L;
            u = x[1] - x[0];
            d[0] = 6*((f[1]-f[0])/u - f1x0)/u;
            c[n] = 1.0L;
            u = x[n] - x[n-1];
            d[n] = 6.0L*(f1xn - (f[n] - f[n-1])/u)/u;
            for (i = 1; i <= n-1;i++)
            {
                z = x[i];
                y = x[i+1] - z;
                z = z-x[i-1];
                u = f[i];
                b[i] = y/(y+z);
                c[i] = 1.0L - b[i];
                d[i] = 6.0L*((f[i+1]-u)/y - (u-f[i-1])/z)/(y+z);
            }
            u = 2.0L;
            i = -1;
            y = d[0]/u;
            d[0] = y;
            do
            {
                i++;
                z = b[i]/u;
                b[i] = z;
                u = 2.0L-z*c[i+1];
                y = (d[i+1] - y*c[i+1])/u;
                d[i+1] = y;
            }
            while( i < n-1);
            u = d[n];
            for (i = n-1; i >= 0;i--)
            {
                u = d[i] - u*b[i];
                d[i] = u;
            }
            found = false;
            i = -1;
            do
            {
                i++;
                if ((xx>=x[i]) && (xx<=x[i+1]))
                    found = true;
            }while(!found);
            y = x[i+1] - x[i];
            z = d[i+1];
            u = d[i];
            a[0] = f[i];
            a[1] = (f[i+1]-f[i])/y-(2*u+z)*y/6;
            a[2] = u/2;
            a[3] = (z-u)/(6*y);
            y = a[3];
            z = xx-x[i];
            for (i = 2;i>=0;i--)
                y = y*z+a[i];
            return y;
        }

    return st;
}

template <class T>
    void Funkcje_Sklejane::Inicjalizuj_Vector(std::vector <T> &tmp,int rozmiar)
{
    tmp.clear();
    for (int i = 0; i <= rozmiar;i++)
        tmp.push_back(0);
}

template <class T>
std::vector<std::vector<T>> Funkcje_Sklejane::Inicjalizuj_Macierz(int wiersze,int kolumny)
{
    std::vector<std::vector<T>> vec;
    for (int i = 0; i <= wiersze;i++)
            vec.push_back(std::vector<T>());
    for (int i = 0; i <= wiersze;i++)
        for (int j = 0; j<= kolumny;j++)
             vec[i].push_back(0);
    return vec;
}

bool Funkcje_Sklejane::Wczytaj_Dane()
{
    std::fstream plik_wejsciowy;
    plik_wejsciowy.open("./input.txt",std::ios::in);
    if (plik_wejsciowy.good())
    {
     plik_wejsciowy>>n;
     Inicjalizuj_Vector(x,n);
     Inicjalizuj_Vector(f,n);
     for (int i = 0; i <=n; i++)
         plik_wejsciowy>>x[i];
     for (int i = 0; i <=n; i++)
         plik_wejsciowy>>f[i];
     plik_wejsciowy>>f1x0;
     plik_wejsciowy>>f1xn;
     plik_wejsciowy>>xx;
     return true;
    }
    else return false;
}
