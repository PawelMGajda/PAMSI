#include "algorytmy.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <math.h>

using namespace std;

#define DataType int
                    // do wyboru int, double oraz float
template<typename Typ>
using SortFncPoint = void (*)(Typ*, Typ*, int, int, int); //wskaznik na funkcje

template<typename Typ>
void wypisz_tabele(Typ *tab, int n)
{
    for (int i = 0; i < n; i++)  cout << tab[i] << " ";
}


template<typename Typ>
Typ *stworz_tabele(int n)
{
	return new Typ[n];
}


template<typename Typ>
void usun_tabele(Typ *tab)
{
	delete[] tab;
}



//wypelnij tabele n iloscia losowych liczb z zakresu 1-1000
template<typename Typ>
void wypelnij_tabele(Typ *tab, int n)
{
	for (int i = 0; i < n; i++) tab[i] = rand() % 1000 + 1;
}


//funkcja sortuje procent liczb w tabeli
template<typename Typ>
void posortuj_czesc(Typ *tab, int n, double procent)
{
    if (procent == 100) //nalezy odwrocic tabele
    {
         sort(tab, tab + n,  greater<Typ>());//sortuje i odwraca
        return;
    }
	 sort(tab, tab + (int)((n*procent) / 100));//sortuje tylko w czesci w zaleznosci od ilosci procent
}


template<typename Typ>
void czy_posortowane(Typ *tab, int n)
{
    for (int i = 0; i < n - 1 ; i++)
    {
        if(tab[i] > tab[i + 1])
        {
             cout << "Tabela nie zostala posortowana!\n";
             cin.get(); //czeka na reakcje uzytkownika
            exit(1);
        }
    }
}


void czy_otwarty ( ofstream &file)
{
    if (file.is_open())
    {
         cout<< "Plik zostal otwarty"<<  endl;
    }
    if (!file.is_open())
     {
         cout << "Plik nie zostal otwarty!\n";
         cin.get();
        exit(1);
     }
}


//sortuje tablice za pomoca funkcji na ktore wskazuje wskaznik
//funkcja zwraca czas posortowania tablicy w [ms]
template<typename Typ>
double posortuj(SortFncPoint<DataType> fp, int n, Typ *tab, Typ *tmp)
{
    auto t_start =  chrono::high_resolution_clock::now(); //poczatek liczenia czasu
    fp(tab, tmp, 0, n-1, log(n) * 2);
    auto t_end =  chrono::high_resolution_clock::now(); //koniec liczenia czasu

    return  chrono::duration<double,  milli>(t_end - t_start).count(); //zwraca roznice czasu w milisekundach
}




int main()
{
     ofstream file("wyniki.txt");
    czy_otwarty(file);

    srand(time(NULL));

	int ile_tabel = 30; //ilosc tablic ktore beda posortowane dla kazdego typu, wielkosci i ilosci procentow
	int  wielkosc_tablicy[5] = {10000, 50000, 100000, 500000, 1000000}; //tabela z rozmiarami tablic
	double procenttab[8] = {0, 25, 50, 75, 95, 99, 99.7, 100 };
	//tabela z procentami okreslajacymi poczatkowe posortowanie tablicy

	SortFncPoint<DataType> mpoint= &mergeWrap, qpoint = &quickWrap, ipoint = &introWrap; //wskazniki na funkcje
    SortFncPoint<DataType> sort_tabela[3] = {qpoint, mpoint, ipoint}; //tabela wskaznikow

	double suma_czasow = 0;
	int nazwa_sort = 0;
	 string sorts[3] = {"QUCIK_SORT", "MERGE_SORT", "INTRO_SORT"};

    for(SortFncPoint<DataType> fp : sort_tabela) //typy sortowania
    {
         cout << sorts[nazwa_sort++] <<  endl;
        for (double procent : procenttab) //procent wstepnego posortowania tablicy
        {
            for (int n :  wielkosc_tablicy) // rozmiar tablicy
            {
                DataType *tab = stworz_tabele<DataType>(n);
                DataType *tmp = stworz_tabele<DataType>(n);

                for (int i = 0; i < ile_tabel; i++) //ilosc tablic
                {

                    wypelnij_tabele<DataType>(tab, n);
                    posortuj_czesc<DataType>(tab, n, procent);
                    suma_czasow += posortuj<DataType>(fp, n, tab, tmp);
                    czy_posortowane<DataType>(tab, n);

                     cout << i << " posortowano" << "\r";//licznik posortowanych tabel
                }

                usun_tabele<DataType>(tab);
                usun_tabele<DataType>(tmp);

                 cout << "Posortowano tabele " <<n<<" elementow"<<  endl;

                file << suma_czasow / ile_tabel << " "; //zapis do pliku sredniej czasow z danego sortowania dla poszczegolnego procentu, wielkosci i typu sortowanie
                suma_czasow = 0;//wyzerowanie sumy, przygotowanie do kolejnego wykonania petli
            }
             cout << procent << " % gotowe!" <<  endl;
        }
        file << "\n";
    }
     cout << "Koniec!";
    file.close();

    cin.get();

	return 0;
}

