#ifndef FUNKCJE_SKLEJANE_PRZEDZIALY_H
#define FUNKCJE_SKLEJANE_PRZEDZIALY_H
#include <QString>
#include "Interval.h"

class Funkcje_Sklejane_Przedzialy
{
public:
    Funkcje_Sklejane_Przedzialy(int size, QString x_in,QString f_in,QString  f1x0_in,QString  f1xn_in,QString  xx_in);

    std::vector <interval_arithmetic::Interval<long double>> x,f;
    int n;


    interval_arithmetic::Interval<long double> Wartosci_Funkcji_Sklejanych();
    template <class T>
        void Inicjalizuj_Vector(std::vector <T> &tmp,int rozmiar);

private:
    interval_arithmetic::Interval<long double> f1x0,f1xn,xx;
};

#endif // FUNKCJE_SKLEJANE_PRZEDZIALY_H
