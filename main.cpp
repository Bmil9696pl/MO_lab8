#include <iostream>
#include <math.h>
#include <fstream>
#define PI 3.141592653589793238462643383279502884
#define ilosc 100
#define LWYNIKOW 8

using namespace std;

template<typename T> T f(T x){
    return sin(x);
}
template<typename T> T df(T x)
{
    return cos(x);
}
template <typename T> T roznicaProgresywna2p(T x, T h){
    return (f(x + h) - f(x)) / h;
}
template <typename T> T roznicaCentralna2p(T x, T h){
    return (f(x + h) - f(x - h)) / (2 * h);
}
template <typename T> T roznicaWsteczna2p(T x, T h){
    return (f(x) - f(x - h)) / h;
}

template <typename T> T roznicaProgresywna3p(T x, T h){
    return (-3.0 / 2.0 * f(x) + 2 * f(x + h) - 1.0 / 2.0 * f(x + h + h)) / (h);
}
template <typename T> T roznicaWsteczna3p(T x, T h){
    return (3.0 / 2.0 * f(x) - 2.0 * f(x - h) + 1.0 / 2.0 * f(x - h - h)) / (h);
}

template <typename T> T** obliczanie(T** tab){
    T** blad = new T* [ilosc];
    T h = 0.2;
    T poczatek = 0;
    T koniec = PI/2;
    T srodek = (poczatek + koniec) / 2;
    cout << srodek << endl;

    for(int i = 0; i < ilosc; i++){
        tab[i] = new T[LWYNIKOW];
        blad[i] = new T[LWYNIKOW];

        tab[i][0] = roznicaProgresywna2p(poczatek, h);
        blad[i][0] = fabs(tab[i][0] - df(poczatek));

        tab[i][1] = roznicaCentralna2p(srodek, h);
        blad[i][1] = fabs(tab[i][1] - df(srodek));

        tab[i][2] = roznicaWsteczna2p(koniec, h);
        blad[i][2] = fabs(tab[i][2] - df(koniec));

        tab[i][3] = roznicaProgresywna3p(poczatek, h);
        blad[i][3] = fabs(tab[i][3] - df(poczatek));

        tab[i][4] = roznicaProgresywna3p(srodek, h);
        blad[i][4] = fabs(tab[i][4] - df(srodek));

        tab[i][5] = roznicaWsteczna3p(srodek, h);
        blad[i][5] = fabs(tab[i][5] - df(srodek));

        tab[i][6] = roznicaWsteczna3p(koniec, h);
        blad[i][6] = fabs(tab[i][6] - df(koniec));

        blad[i][7] = h;

        h = h / (1.5);
    }
    return blad;
}

template <typename T> void zapisz(T** tab, string nazwa){
    fstream plik;
    plik << scientific;
    plik.precision(16);
    plik.open(nazwa, ios::out);
    if (!plik.good())
    {
        cout << "Blad otwarcia pliku " << endl;
        return;
    }
    for (int i = 0; i < ilosc; i++)
    {
        plik << log10(tab[i][7]) << " ";
        for (int j =0; j < LWYNIKOW-1; j ++)
        {
            if (fabs(log10(tab[i][j])) > 0)
                plik << log10(tab[i][j]) << " ";
            else
                plik << "0 ";
        }
        plik << endl;
    }
    plik << endl << "Rzedy dokladnosci: "<<endl;
    for (int i = 0; i < 7; i++)
    {
        plik << (log10(tab[1][i]) - log10(tab[0][i])) / (log10(tab[1][7]) - log10(tab[0][7])) << endl;
    }
    plik.close();
}

int main()
{
    cout << scientific;
    cout.precision(10); //ustalamy precyzję

    float** pochodnaFloat = new float* [ilosc]; //tworzymy tablice dwuwymiarową typu float
    pochodnaFloat = obliczanie(pochodnaFloat);
    zapisz(pochodnaFloat, "pochodnaFloat.txt"); //zapisujemy wyniki do pliku

    double** pochodnaDouble = new double* [ilosc];  //tworzymy tablice dwuwymiarową typu double
    pochodnaDouble = obliczanie(pochodnaDouble);
    zapisz(pochodnaDouble, "pochodnaDouble.txt"); //zapisujemy wyniki do pliku

    cout << "Zakonczono.\n";
}