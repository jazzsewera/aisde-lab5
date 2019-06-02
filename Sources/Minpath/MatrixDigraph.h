#ifndef MATRIX_DIGRAPH_H
#define MATRIX_DIGRAPH_H

#include <iostream>

#include "List.h"
#include "HalfEdge.h"
#include "AbstractGraph.h"
#include "ListDigraph.h"

// Deklaracja zapowiadająca
class ListDigraph;

// Graf skierowany. Implementacja tablicowa - macierz przyległości (sąsiedztwa).

class MatrixDigraph : public AbstractGraph {

	private:
		int   V, E;     // Liczba wierzchołków i krawędzi.
		ulong *weights;	// Tablica wag krawędzi (jednowymiarowa, lecz o długości V*V).

	public:
		MatrixDigraph(const char *filename);
		MatrixDigraph(const MatrixDigraph &);
		MatrixDigraph(const ListDigraph &);
		~MatrixDigraph(void);
		int getNumVertices(void) const { return V; }
		int getNumEdges(void)    const { return E; }
		void  setWeight(int v, int w, ulong dist);
 		ulong getWeight(int v, int w) const;  // Zwraca wagę krawędzi od wierzchołka 'v' do 'w'.
		List<HalfEdge *>* getOutgoingEdges(int v) const;
 		void  print(void); // Wyświetla macierz przyległości
};

#endif // MATRIX_DIGRAPH_H
