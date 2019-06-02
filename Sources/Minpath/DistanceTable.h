#ifndef DISTANCE_TABLE_H
#define DISTANCE_TABLE_H

#include "MinpathUtils.h"

// Tablica 'wag' wierzcholkow, czyli ich odleglosci od zrodla,
// wraz z informacja o tym, czy waga jest ustalona, czy tymczasowa.
// Ta informacja jest wykorzystywana przez algorytm Dijkstry.

class DistanceTable {

	private:
		ulong *dists;  // Tablica odleglosci wierzcholka od zrodla
		bool  *fixed;  // Czy waga jest ustalona forever?
		int    size;   // Rozmiar powyzszych tablic, czyli liczba wierzcholkow.

	public:
		DistanceTable(int size);
 		~DistanceTable(void);
		ulong getDistance(int n) const;    // Zwraca wage wierzcholka 'n'.
		void  setDistance(int n, ulong d); // Przypisuje (tymczasowa!) wage 'd' wierzcholkowi 'n'.
		int   getNearest(void) const;  // Pobiera najmniejsza sposrod wag w tablicy.
		bool  isFixed(int n) const;    // Czy waga wierzcholka 'n' jest juz ustalona?
		bool  fix(int n);              // Ustala dotychczasowa wage wierzcholka 'n'.
		void  print(void) const;
		void  check(void) const;
};

#endif // DISTANCE_TABLE_H
