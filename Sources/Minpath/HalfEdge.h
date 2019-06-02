#ifndef HALF_EDGE
#define HALF_EDGE

#include "MinpathUtils.h"

// Krawędź skierowanego grafu ważonego. Reprezentacja pozwalająca
// przechowywać graf jako tablicę list tego typu obiektów.
// v-tym elementem tablicy jest kolekcja (np. lista) obiektów HalfEdge,
// reprezentujących krawędzie wychodzące z wierzchołka 'v'.

class HalfEdge {
	private:
		int	w;			  // Numer wierzchołka końcowego.
		ulong weight;	// Waga krawędzi.

	public:
		HalfEdge(): w(0), weight(INF) {}
		HalfEdge(int a, ulong wght): w(a), weight(wght) {}
		int 	getEndingVertexIndex(void) const { return w; }
		ulong getWeight(void) const { return weight; }
		void	setWeight(ulong wt) { weight = wt; }
};

#endif // HALF_EDGE
