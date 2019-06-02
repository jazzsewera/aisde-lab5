#include <cstdlib>
#include <iostream>
#include <fstream>
#include <limits>

#include "MST_Algorithms.h"
#include "MST_Utils.h"
#include "Edge.h"
#include "Graph.h"
#include "Forest.h"

using namespace std;

// Algorytm Kruskala, znajdujący najmniejsze drzewo rozpinające (MST)
Edge** Kruskal(const Graph& g) {
	Edge** edges = g.getEdges();
	long V       = g.getNumVertices();
	long E       = g.getNumEdges();
	int  mstSize = 0;

	// Zwracana tablica krawędzi tworzących drzewo rozpinające
	Edge** mst = new Edge*[V-1];

	Forest forest(V);

	qsort(edges, E, sizeof(Edge*), Edge::compareEdgeWeigths);

	for (int i = 0; i < E; i++) {
		// Jeśli kolejna krawędz nie tworzy cyklu z żadnym ze stworzonych dotychczas drzew...
		if (forest.addEdge(edges[i]) == true)
			// ...to dodaj ją do drzewa rozpinającego.
			mst[mstSize++] = edges[i];
	}

	return mst;
}


// Algorytm Borůvki, znajdujący najmniejsze drzewo rozpinające
Edge** Boruvka(const Graph& g) {
	Edge** edges = g.getEdges();
	long V       = g.getNumVertices();
	long E       = g.getNumEdges();

	// Na poczatku mamy V drzew - pojedynczych wierzchołków.
	long  numOfSubsets = V;

	// Długości najkrótszych krawędzi łączących każde z drzew z resztą grafu.
	long shortestOuterEdgeInSubset[V];

	// Indeksy tych krawędzi.
	long edgeToAddToSubset[V];

	// Zwracana tablica krawędzi tworzących drzewo rozpinające
	Edge** mst = new Edge*[V-1];

	// Zmienne tymczasowe
	Edge *candidateEdge; // Krawędź, którą dodamy, o ile nie stworzy cyklu
	long  firstVertexIndex, secondVertexIndex; // Jej początek i koniec
	int   candidateEdgeWeight; // Jej waga
	long  firstSubsetIndex, secondSubsetIndex; // Indeksy drzew, do których należą powyższe dwa wierzchołki
	int   mstSize = 0;

	Forest forest(V);

	while (numOfSubsets > 1) {
		for (int i = 0; i < V; i++) {
			shortestOuterEdgeInSubset[i] = std::numeric_limits<long>::max();
			edgeToAddToSubset[i] = -1;
		}

		for (int i = 0; i < E; i++) {
			candidateEdge = edges[i];
			firstVertexIndex    = candidateEdge->v;
			secondVertexIndex   = candidateEdge->w;
			candidateEdgeWeight = candidateEdge->weight;

			// Indeksy podzbiorów, do których należa wierzchołki połączone 'candidateEdge'
			firstSubsetIndex  = forest.getSubsetIndex(firstVertexIndex);
			secondSubsetIndex = forest.getSubsetIndex(secondVertexIndex);

			// Jeśli 'candidateEdge' łączyłaby wierzchołki należące do tego samego
			// podzbioru, to oczywiście jej nie dodajemy
			if (firstSubsetIndex == secondSubsetIndex)
				continue;

			// Uaktualniamy wagę najkrótszej gałęzi wychodzącej z podzbioru,
			// do którego należy pierwszy wierzchołek.
			if(candidateEdgeWeight < shortestOuterEdgeInSubset[firstSubsetIndex]) {
				shortestOuterEdgeInSubset[firstSubsetIndex]  = candidateEdgeWeight;
				edgeToAddToSubset[firstSubsetIndex] = i;
			}

			// Robimy to samo z podzbiorem, do którego należy drugi wierzchołek.
			if(candidateEdgeWeight < shortestOuterEdgeInSubset[secondSubsetIndex]) {
				shortestOuterEdgeInSubset[secondSubsetIndex] = candidateEdgeWeight;
				edgeToAddToSubset[secondSubsetIndex] = i;
			}
		}

 		for (int i = 0; i < V; i++) {
 			// Może się okazać, ze "i" nie jest indeksem żadnego podzbioru
 			if (edgeToAddToSubset[i] == -1)
 				continue;
 			candidateEdge = edges[edgeToAddToSubset[i]];

 			// Jeśli udało się dodać najkrótszą gałąź (tzn. jesli nie stworzy ona cyklu),
 			// to połaczylismy dwa poddrzewa, czyli dwa podzbiory wierzchołków.
 			if (forest.addEdge(candidateEdge) == true) {
 				mst[mstSize++] = candidateEdge;
 				numOfSubsets--;
 			}
 		}
	}

	return mst;
}
