/************************************************************************************************
*                                          FOREST
*
*   Implementacja abstrakcyjnej struktury danych operującej na rozłącznych podzbiorach,
*   znanej jako 'Disjoint Subsets' lub 'Union-Find'.
*   Umożliwia ona dwie operacje: złaczenie podzbiorów (Union)  oraz okreslenie indeksu podzbioru,
*   do ktorego należy element o podanym indeksie (Find).
*
*   W tej implementacji operujemy na podzbiorach wierzchołków grafu tworzących drzewa.
*   Funkcja addEdge(edge) próbuje dodać krawędź 'edge', o ile połaczy ona dwa drzewa.
*   W przeciwnym wypadku krawędź nie jest dodawana (utworzyłaby ona cykl w którymś
*   z istniejących drzew) i funkcja zwraca 'false'.
*
*   Funkcja getSubsetIndex(v) zwraca indeks poddrzewa, do którego nalezy wierzchołek 'v'.
*
************************************************************************************************/

#ifndef FOREST_H
#define FOREST_H

#include "Edge.h"

class Forest {
	public:
		Forest(long size);
		~Forest(void);
		bool  addEdge(Edge *);
		long  getSubsetIndex(long v) const;

	private:
		long   size;
		long  *subsetIndex;
		long  *successor;
		long  *subsetSize;
		void   exchange(long &x, long &y);
		bool   inTheSameSubset(long v, long w) const;
		long   getSubsetSize(long v) const;
		void   setSubsetSize(long v, long new_size);
};

#endif // FOREST_H
