#include "funkcje_sklejane_przedzialy.h"
//#include <QObject>

Funkcje_Sklejane_Przedzialy::Funkcje_Sklejane_Przedzialy(int size, std::vector<interval_arithmetic::Interval<long double>> x_in,std::vector<interval_arithmetic::Interval<long double>> f_in,interval_arithmetic::Interval<long double>  f1x0_in,interval_arithmetic::Interval<long double> f1xn_in,interval_arithmetic::Interval<long double> xx_in)
{
    ilosc_elementow = size;
    wartosci_f1x0 = f1x0_in;
    wartosci_f1xn = f1xn_in;
    wartosci_xx = xx_in;

    for (int i = 0; i<=ilosc_elementow;i++)
    {
        wartosci_x.push_back(x_in[i]) ;
    }

    for (int i = 0; i<=ilosc_elementow;i++)
    {
        wartosci_f.push_back(f_in[i]) ;
    }
}

void Wspolczynniki_Funkcji_Sklejanej(
                            int n,
                            std::vector<interval_arithmetic::Interval<long double>> x,
                            std::vector<interval_arithmetic::Interval<long double>> f,
                            interval_arithmetic::Interval<long double> f1x0,
                            interval_arithmetic::Interval<long double> f1xn,
                            std::vector<std::vector<interval_arithmetic::Interval<long double>>> &a,
                            int &st)
{
    int i;
    interval_arithmetic::Interval<long double> u,v,y,z,xi;
    std::vector <interval_arithmetic::Interval<long double>> b,c,d;
    interval_arithmetic::Interval<long double> interval_1 = interval_arithmetic::Interval<long double>::IntRead("1.0");
    interval_arithmetic::Interval<long double> interval_2 = interval_arithmetic::Interval<long double>::IntRead("2.0");
    interval_arithmetic::Interval<long double> interval_3 = interval_arithmetic::Interval<long double>::IntRead("3.0");
    interval_arithmetic::Interval<long double> interval_6 = interval_arithmetic::Interval<long double>::IntRead("6.0");
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

    if (st != 0 ) return;
    b.resize(n,interval_arithmetic::Interval<long double>::IntRead("0.0"));
    c.resize(n,interval_arithmetic::Interval<long double>::IntRead("0.0"));
    d.resize(n,interval_arithmetic::Interval<long double>::IntRead("0.0"));
    st = 0;
    if (st == 0)
    {
        b[0] = interval_1;
        u = x[1] - x[0];

        if (u.a <=0 && u.b>=0)
        {
            st = 3;
            return;
        }
        d[0] = interval_6*((f[1]-f[0])/u - f1x0)/u;

        c[n] = interval_1;
        u = x[n] - x[n-1];


        if (u.a <=0 && u.b>=0)
        {
            st = 3;
            return;
        }
        d[n] = interval_6*(f1xn - (f[n] - f[n-1])/u)/u;

        for (i = 1; i <= n-1;i++)
        {
            z = x[i];
            y = x[i+1] - z;
            z = z - x[i-1];
            u = f[i];

            if ((y+z).a <=0 && (y+z).b>=0)
            {
                st = 3;
                return;
            }
            b[i] = y/(y+z);

            c[i] = interval_1-b[i];


            if ((y+z).a <=0 && (y+z).b>=0)
            {
                st = 3;
                return;
            }
            if (y.a <=0 && y.b>=0)
            {
                st = 3;
                return;
            }
            if (z.a <=0 && z.b>=0)
            {
                st = 3;
                return;
            }
            d[i] = interval_6*((f[i+1]-u)/y - (u - f[i-1])/z)/(y+z);
        }
        u =interval_2;
        i = -1;
        y = d[0] / u;

        d[0] = y;
        do
        {
            i++;
            z = b[i]/u;
            b[i] = z;
            u = interval_2 - z*c[i+1];

            if (u.a <=0 && u.b>=0)
            {
                st = 3;
                return;
            }
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


            if (z.a <=0 && z.b>=0)
            {
                st = 3;
                return;
            }
            v = (f[i+1] - u)/z - (interval_2*y+d[i+1])*z/interval_6;
            z = (d[i+1] - y)/(interval_6*z);
            y = y/interval_2;
            a[0][i] = ((y-z*xi)*xi - v)*xi + u;
            u = interval_3*z*xi;
            a[1][i] = (u-interval_2*y)*xi +v;
            a[2][i] = y-u;
            a[3][i] = z;
        }
    }

}

interval_arithmetic::Interval<long double> Wartosc_Funkcji_Sklejanej(
        int n,
        std::vector<interval_arithmetic::Interval<long double>> x,
        std::vector<interval_arithmetic::Interval<long double>> f,
        interval_arithmetic::Interval<long double> f1x0,
        interval_arithmetic::Interval<long double> f1xn,
        interval_arithmetic::Interval<long double> xx,
        int &st)
{
int i;
interval_arithmetic::Interval<long double> u,y,z;
bool found;
vector<interval_arithmetic::Interval<long double>> a,b,c,d;

interval_arithmetic::Interval<long double> interval_1 = interval_arithmetic::Interval<long double>::IntRead("1.0");
interval_arithmetic::Interval<long double> interval_2 = interval_arithmetic::Interval<long double>::IntRead("2.0");
interval_arithmetic::Interval<long double> interval_6 = interval_arithmetic::Interval<long double>::IntRead("6.0");
interval_arithmetic::Interval<long double> interval_error = interval_arithmetic::Interval<long double>::IntRead("-100.0");

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
    if (st != 0 ) return interval_error;
    a.resize(3,interval_arithmetic::Interval<long double>::IntRead("0.0"));
    b.resize(n,interval_arithmetic::Interval<long double>::IntRead("0.0"));
    c.resize(n,interval_arithmetic::Interval<long double>::IntRead("0.0"));
    d.resize(n,interval_arithmetic::Interval<long double>::IntRead("0.0"));
    st = 0;
    if (st == 0)
    {
        b[0] = interval_1;
        u = x[1] - x[0];

        if (u.a <=0 && u.b>=0)
        {
            st = 4;
            return interval_error;
        }
        d[0] = interval_6*((f[1]-f[0])/u - f1x0)/u;
        c[n] = interval_1;
        u = x[n] - x[n-1];

        if (u.a <=0 && u.b>=0)
        {
            st = 4;
            return interval_error;
        }
        d[n] = interval_6*(f1xn - (f[n] - f[n-1])/u)/u;
        for (i = 1; i <= n-1;i++)
        {
            z = x[i];
            y = x[i+1] - z;
            z = z-x[i-1];
            u = f[i];

            if ((y+z).a <=0 && (y+z).b>=0)
            {
                st = 4;
                return interval_error;
            }
            b[i] = y/(y+z);
            c[i] = interval_1 - b[i];

            if (u.a <=0 && u.b>=0)
            {
                st = 4;
                return interval_error;
            }

            if (z.a <=0 && z.b>=0)
            {
                st = 4;
                return interval_error;
            }
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

            if (u.a <=0 && u.b>=0)
            {
                st = 4;
                return interval_error;
            }
            y = (d[i+1] - y*c[i+1])/u;
            d[i+1] = y;
        }while( i < n-1);
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

        if (y.a <=0 && y.b>=0)
        {
            st = 4;
            return interval_error;
        }
        a[1] = (f[i+1]-f[i])/y-(interval_2*u+z)*y/interval_6;
        a[2] = u/interval_2;


        if ((interval_6*y).a <=0 && (interval_6*y).b>=0)
        {
            st = 4;
            return interval_error;
        }
        a[3] = (z-u)/(interval_6*y);
        y = a[3];
        z = xx-x[i];
        for (i = 2;i>=0;i--)
            y = y*z+a[i];
        return y;
    }
    else return interval_error;
}

template <class T>
   void Funkcje_Sklejane_Przedzialy::Inicjalizuj_Vector(std::vector <interval_arithmetic::Interval<T>> &tmp,int rozmiar)
{
    tmp.clear();
    for (int i = 0; i <= rozmiar;i++){
        interval_arithmetic::Interval<T> tmp_inter;
        tmp_inter.IntRead("0.0");
        tmp.push_back(tmp_inter);
    }
}

template <class T>
   void Funkcje_Sklejane_Przedzialy::Inicjalizuj_Macierz(std::vector<std::vector<interval_arithmetic::Interval<T>>> &macierz,int wiersze,int kolumny)
{
       for (int i = 0; i <= wiersze;i++)
               macierz.push_back(std::vector<interval_arithmetic::Interval<T>>());
       for (int i = 0; i <= wiersze;i++)
           for (int j = 0; j<= kolumny;j++)
                macierz[i].push_back(interval_arithmetic::Interval<T>::IntRead("0.0") );
}
