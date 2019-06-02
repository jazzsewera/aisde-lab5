#ifndef LIST_DIGRAPH_H
#define LIST_DIGRAPH_H

#include "List.h"
#include "HalfEdge.h"
#include "AbstractGraph.h"
#include "MatrixDigraph.h"

class MatrixDigraph;

// Graf skierowany, zaimplementowany jako tablica list sąsiedztw.
// v-ty element tablicy wskazuje na listę obiektów klasy HalfEdge,
// reprezentujących krawędzie wychodzące z wierzchołka 'v'.

class ListDigraph : public AbstractGraph {

	private:
		int 	V, E;               // Liczba wierzchołków i krawędzi
		List<HalfEdge *> **edges; // Rzeczona tablica list krawędzi.
		bool	insertEdge(int v, int w, ulong weight); // Wstawia krawędź.

	public:
		ListDigraph(const char *);           // Konstruuje graf na podstawie opisu w pliku.
		ListDigraph(const MatrixDigraph &);  // A tu - na podstawie grafu w postaci macierzy przyległości.
		~ListDigraph(void);
		// Zwraca listę krawędzi wychodzących z wierzchołka 'v'.
		List<HalfEdge *> *getOutgoingEdges(int v) const { edges[v]->rewind(); return edges[v]; }
 		int   getNumVertices(void) const;
 		int   getNumEdges(void) const;
 		ulong getWeight(int v, int w) const; // Zwraca wagę krawędzi skierowanej od wierzch. 'v' do 'w'.
 		void  print(void);
};

#endif // LIST_DIGRAPH_H
