#ifndef FUNKCJE_SKLEJANE_PRZEDZIALY_H
#define FUNKCJE_SKLEJANE_PRZEDZIALY_H
#include <QString>
#include <vector>
#include "Interval.h"

interval_arithmetic::Interval<long double> Wartosc_Funkcji_Sklejanej(
                            int n,
                            std::vector<interval_arithmetic::Interval<long double>> x,
                            std::vector<interval_arithmetic::Interval<long double>> f,
                            interval_arithmetic::Interval<long double> f1x0,
                            interval_arithmetic::Interval<long double> f1xn,
                            interval_arithmetic::Interval<long double> xx,
                            int &st);

void Wspolczynniki_Funkcji_Sklejanej(
    int n,
    std::vector<interval_arithmetic::Interval<long double>> x,
    std::vector<interval_arithmetic::Interval<long double>> f,
    interval_arithmetic::Interval<long double> f1x0,
    interval_arithmetic::Interval<long double> f1xn,
    std::vector<std::vector<interval_arithmetic::Interval<long double>>> &a,
    int &st);
class Funkcje_Sklejane_Przedzialy
{
public:
    Funkcje_Sklejane_Przedzialy(int size, std::vector<interval_arithmetic::Interval<long double>> x_in,std::vector<interval_arithmetic::Interval<long double>> f_in,interval_arithmetic::Interval<long double>  f1x0_in,interval_arithmetic::Interval<long double> f1xn_in,interval_arithmetic::Interval<long double> xx_in);

    template <class T>
         void Inicjalizuj_Vector(std::vector<interval_arithmetic::Interval<T>> &tmp,int rozmiar);
    template <class T>
         void Inicjalizuj_Macierz(std::vector<std::vector<interval_arithmetic::Interval<T>>> &macierz,int wiersze,int kolumny);

         int ilosc_elementow;
         std::vector <interval_arithmetic::Interval<long double>> wartosci_x,wartosci_f;
         interval_arithmetic::Interval<long double> wartosci_f1x0,wartosci_f1xn,wartosci_xx;
private:
};

#endif // FUNKCJE_SKLEJANE_PRZEDZIALY_H
