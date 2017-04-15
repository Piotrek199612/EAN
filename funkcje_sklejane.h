#ifndef FUNKCJE_SKLEJANE_H
#define FUNKCJE_SKLEJANE_H

#include <QObject>

class Funkcje_Sklejane
{
public:
    Funkcje_Sklejane(int size);
    Funkcje_Sklejane(int size, QString x_in,QString f_in,QString  f1x0_in,QString  f1xn_in,QString  xx_in);
    long double Wartosci_Funkcji_Sklejanych();
    std::vector <std::vector<long double>> Wspolczynniki_Funkcji_Sklejanych();
    std::vector <long double> x,f;
    int n;
    std::vector<std::vector<long double>> macierz;
    bool Wczytaj_Dane();
private:

    long double f1x0,f1xn,xx;
    template <class T>
        void Inicjalizuj_Vector(std::vector<T> &tmp,int rozmiar);
    template <class T>
        std::vector<std::vector<T>> Inicjalizuj_Macierz(int wiersze,int kolumny);


};

#endif // FUNKCJE_SKLEJANE_H
