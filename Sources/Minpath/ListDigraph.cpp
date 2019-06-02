#include <iostream>
#include <fstream>
#include <cstdlib>

#include "ListDigraph.h"
#include "MatrixDigraph.h"
#include "Exception.h"

ListDigraph::ListDigraph(const char *filename) {
	std::ifstream infile;  // Plik z grafem
	V = 0;       // Liczba wierzchołków
	E = 0;       // Liczba krawędzi
	int   v, w;  // Zmienne tymczasowe: początkowy i koncowy wierzchołek krawędzi
 	ulong wt;    // Waga krawędzi;

	infile.open(filename);

	if (!infile.is_open()) {
		throw Exception(FileOpenException, filename);
	}

	// Na razie przeglądamy plik od początku do końca
	// w poszukiwaniu największego indeksu wierzchołka.
	while(infile >> v >> w >> wt) {
		if (v > V) V = v;
		if (w > V) V = w;
	}

	// Numery wierzchołków zaczynaja sie od zera, wiec jeśli maksymalny numer
	// znaleziony w pliku to V, to mamy V+1 wezlow.
	V++;

	// Wreszcie znamy rozmiar tablicy list krawędzi, wiec mozemy ją zaalokować
	edges = new List<HalfEdge *>*[V];

	for (int i = 0; i < V; i++)
		edges[i] = new List<HalfEdge *>();

 	// Wracamy do poczatku pliku
 	infile.clear();
 	infile.seekg(0);

 	// Odczytujemy krawędzie
	while(infile >> v >> w >> wt) {
		insertEdge(v, w, wt);
		E++;
	}
}

ListDigraph::ListDigraph(const MatrixDigraph& g){
	V     = g.getNumVertices();
	edges = new List<HalfEdge *>*[V];
	for (int i = 0; i < V; i++)
		edges[i] = new List<HalfEdge *>();

	for (int v = 0; v < V; v++) {
		for (int w = 0; w < V; w++) {
			ulong weight = g.getWeight(v, w);
			if (weight != INF)
				insertEdge(v, w, weight);
		}
	}
}

ListDigraph::~ListDigraph(void) { delete [] edges; }

ulong ListDigraph::getWeight(int v, int w) const {
	if (v >= 0 and w >=0 and v < V and w < V) {
		HalfEdge* edge;
		edges[v]->rewind();
		while (edges[v]->hasMoreElements()) {
			edge = edges[v]->getNext();
			if (edge->getEndingVertexIndex() == w)
				return edge->getWeight();
		}
		return INF;
	}
	else
		return INF;
}

int ListDigraph::getNumVertices(void) const { return V; }

int ListDigraph::getNumEdges(void) const { return E; }

bool ListDigraph::insertEdge(int v, int w, ulong weight) {
	if (v >= 0 and w >= 0 and v < V and w < V) {
		edges[v]->append(new HalfEdge(w, weight));
		return true;
	}
	return false;
}

void ListDigraph::print(void) {
	// Wagi krawędzi od 'v' do każdego z V wierzchołków (mogą być nieskończone)
	ulong *weights = new ulong[V];
	HalfEdge *edge;

 	for (int v = 0; v < V; v++) {
		edges[v]->rewind();
		// Inicjujemy tablicę nieskończonymi wagami
 		for (int w = 0; w < V; w++)
			weights[w] = INF;

		// Uaktualniamy wagi odpowadające istniejącym krawędziom
		while (edges[v]->hasMoreElements()) {
			edge = edges[v]->getNext();
			weights[edge->getEndingVertexIndex()] = edge->getWeight();
		}

		// Wypisujemy wagi na ekran
  	for (int w = 0; w < V; w++) {
			std::cout << "\t";
 			if (weights[w] != INF)
				std::cout << weights[w];
			else
				std::cout << "-";
		}
		std::cout << "\n";
	}
	std::cout << std::endl;
	delete [] weights;
}
