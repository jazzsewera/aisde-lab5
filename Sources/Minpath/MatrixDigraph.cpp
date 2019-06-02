#include <iostream>
#include <fstream>

#include "MatrixDigraph.h"
#include "Exception.h"
#include "MinpathUtils.h"

MatrixDigraph::MatrixDigraph(const char *filename) {
	V = 0;			// Liczba węzlów
	E = 0;			// Liczba krawędzi
	int v, w;		// Węzly: początkowy i końcowy
 	ulong wt;			// Waga krawędzi
	std::ifstream *ifstr;

	std::cout << " Wczytywanie grafu z pliku '" << filename << "'...";
	ifstr = new std::ifstream(filename);
	if (ifstr->good() == false)
		throw Exception(FileOpenException, filename);

	while(*ifstr >> v >> w >> wt) {
		if (v > V) V = v;
		if (w > V) V = w;
		E++;
	}

	V++;		// Numery węzłów powinny zaczynać się od zera.

	weights = new ulong[V*V];
	for (int i = 0; i < V; i++)
 		for (int j = 0; j < V; j++)
			weights[V*i+j] = INF;

	delete ifstr;
  	ifstr = new std::ifstream(filename);

	while(*ifstr >> v >> w >> wt)
		weights[V*v+w] = wt;

	std::cout << " gotowe" << std::endl;
 	delete ifstr;
}

MatrixDigraph::MatrixDigraph(const MatrixDigraph& g) {
	V = g.getNumVertices();
	E = g.getNumEdges();
	weights = new ulong[V*V];

	for (int i = 0; i < V; i++)
 		for (int j = 0; j < V; j++)
			weights[V*i+j] = g.weights[V*i+j];
}

MatrixDigraph::MatrixDigraph(const ListDigraph& g) {
	List<HalfEdge *> *outgoingEdges;
	HalfEdge *edge;
	int w;

	V = g.getNumVertices();
	E = g.getNumEdges();
	weights = new ulong[V*V];

	for (int i = 0; i < V*V; i++)
		weights[i] = INF;

	for (int v = 0; v < V; v++)  {
		outgoingEdges = g.getOutgoingEdges(v);
		while (outgoingEdges->hasMoreElements()) {
			edge = outgoingEdges->getNext();
			w    = edge->getEndingVertexIndex();
			weights[V*v+w] = edge->getWeight();
		}
	}
}

MatrixDigraph::~MatrixDigraph(void) { delete[] weights; }

ulong MatrixDigraph::getWeight(int v, int w) const {
	if (v >= 0 && w >= 0 && v < V && w < V)
		return weights[V*v + w];
	else
		return INF;
}

void MatrixDigraph::setWeight(int v, int w, ulong weight) {
	weights[V*v + w] = weight;
}

void MatrixDigraph::print(void) {
	ulong weight;

	std::cout << "\n";
 	for (int i = 0; i < V; i++) {
 		for (int j = 0; j < V; j++) {
 			weight = weights[V*i+j];
 			std::cout << "\t";
 			if (weight != INF)
				std::cout << weights[V*i+j];
			else
				std::cout << "-";
		}
		std::cout << "\n";
	}
	std::cout << std::endl;
}

// To w zasadzie tylko dlatego, zże klasa bazowa AbstractGraph
// wymaga takiej funkcji. Tak naprawdę jest ona uzyteczna
// w grafie zaimplementowanym jako tablica

List<HalfEdge *>* MatrixDigraph::getOutgoingEdges(int v) const {
	ulong weight;
	List<HalfEdge *> *edgeList = new List<HalfEdge *>();
	if (v < 0 || v >= V)
		return nullptr;

	for (int w = 0; w < V; w++) {
		weight = weights[V*v+w];
		if (weight != INF) // Dodajemy tylko istniejące gałęzie
			edgeList->append(new HalfEdge(w, weight));
	}
	edgeList->rewind();
	return edgeList;
}
