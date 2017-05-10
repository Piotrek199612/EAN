#include "funkcje_sklejane_przedzialy.h"
#include <QObject>
Funkcje_Sklejane_Przedzialy::Funkcje_Sklejane_Przedzialy(int size, QString x_in,QString f_in,QString  f1x0_in,QString  f1xn_in,QString  xx_in)
{
    n = size;
    f1x0 = interval_arithmetic::Interval<long double>::IntRead(f1x0_in.toStdString());
    f1xn = interval_arithmetic::Interval<long double>::IntRead(f1xn_in.toStdString());
    xx = interval_arithmetic::Interval<long double>::IntRead(xx_in.toStdString());
    QRegExp rx("(\\ |\\,|\\:|\\t|;)"); //RegEx for ' ' or ',' or ':' or '\t'
    QStringList x_in_list = x_in.split(rx);
    QStringList f_in_list = f_in.split(rx);

    if (x_in_list.size() == n+1)
        for (int i = 0;i<=n;i++)
        {
            interval_arithmetic::Interval<long double> tmp;
            x.push_back(tmp = interval_arithmetic::Interval<long double>::IntRead(x_in_list[i].toStdString()));
        }
    if (f_in_list.size() == n+1)
        for (int i = 0;i<=n;i++)
        {
            interval_arithmetic::Interval<long double> tmp;
            f.push_back(tmp = interval_arithmetic::Interval<long double>::IntRead(f_in_list[i].toStdString()));
        }
}


interval_arithmetic::Interval<long double> Funkcje_Sklejane_Przedzialy::Wartosci_Funkcji_Sklejanych()
{
    int i;
    interval_arithmetic::Interval<long double> u,y,z;
    bool found;
    int st;
    vector<interval_arithmetic::Interval<long double>> a,b,c,d;
    Inicjalizuj_Vector(a,3);
    Inicjalizuj_Vector(b,n);
    Inicjalizuj_Vector(c,n);
    Inicjalizuj_Vector(d,n);

    interval_arithmetic::Interval<long double> interval_1 = interval_arithmetic::Interval<long double>::IntRead("1.0");
    interval_arithmetic::Interval<long double> interval_2 = interval_arithmetic::Interval<long double>::IntRead("2.0");
    interval_arithmetic::Interval<long double> interval_6 = interval_arithmetic::Interval<long double>::IntRead("6.0");
   if (n < 1) st = 1;
   else if (xx<x[0] || xx>x[n]) st = 3;
   else
    {
        st = 0;
        i = -1;
        do
        {
            i++;
            for (int k = i+1;k<=n;k++)
                if (x[i] == x[k]) st = 2;
        }while(! (i <= n-1 || st == 2));
     }
   st = 0;
        if (st == 0)
        {
            b[0] = interval_1;
            u = x[1] - x[0];
            d[0] = interval_6*((f[1]-f[0])/u - f1x0)/u;
            c[n] = interval_1;
            u = x[n] - x[n-1];
            d[n] = interval_6*(f1xn - (f[n] - f[n-1])/u)/u;
            for (i = 1; i <= n-1;i++)
            {
                z = x[i];
                y = x[i+1] - z;
                z = z-x[i-1];
                u = f[i];
                b[i] = y/(y+z);
                c[i] = interval_1 - b[i];
                d[i] = interval_6*((f[i+1]-u)/y - (u-f[i-1])/z)/(y+z);
            }
            u = interval_2;
            i = -1;
            y = d[0]/u;
            d[0] = y;
            do
            {
                i++;
                z = b[i]/u;
                b[i] = z;
                u = interval_2-z*c[i+1];
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
            a[1] = (f[i+1]-f[i])/y-(interval_2*u+z)*y/interval_6;
            a[2] = u/interval_2;
            a[3] = (z-u)/(interval_6*y);
            y = a[3];
            z = xx-x[i];
            for (i = 2;i>=0;i--)
                y = y*z+a[i];

            return y;
        }
return interval_6;
   // return st;
}

template <class T>
    void Funkcje_Sklejane_Przedzialy::Inicjalizuj_Vector(std::vector <T> &tmp,int rozmiar)
{
    tmp.clear();
    for (int i = 0; i <= rozmiar;i++){
        interval_arithmetic::Interval<long double> tmp_inter;
        tmp_inter.IntRead("0.0");
        tmp.push_back(tmp_inter);
    }
}
