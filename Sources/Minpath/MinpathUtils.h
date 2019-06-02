#ifndef MINPATH_UTILS_H
#define MINPATH_UTILS_H

#include <limits>
#include <chrono>

// Powyżej tej liczby wierzchołków grafy nie będą wypisywane na ekran
#define MAX_PRINTABLE_V 10
// "Nieskończona" waga wierzchołka lub krawędzi
#define INF std::numeric_limits<long>::max()
#define NO_VERTEX_FOUND -1

// Do pomiaru czasu wykonania algorytmów
typedef std::chrono::steady_clock::time_point time_point;

typedef unsigned long ulong;

// Prototypy funkcji (dokładniejsze komentarze w pliku MinpathUtils.cpp)
ulong Add(ulong, ulong);        // Dodaje wagi 2 gałęzi - mogą być 'nieskończone'.
int   String2Int(const char *); // Konwertuje łańcuch na liczbę całkowitą.

time_point CurrentTime(void);   // Zwraca bieżący czas.
long TimeDiffMicroseconds(time_point, time_point); // Różnica czasu w mikrosekundach

#endif // MINPATH_UTILS_H
