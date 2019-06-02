#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "Graph.h"
#include "Exception.h"

// Tworzy graf nieskierowany na podstawie opisu wczytanego z pliku
Graph::Graph(const char *filename) {
	int   v, w;     // Indeksy wierzchołków
	int   weight;  // Waga krawędzi
	long  index;   // Indeks w tablicy krawędzi
	FILE* infile;

	try {
		if ((infile = fopen(filename, "r")) == nullptr)
			throw Exception(FileOpenException, filename);
	} catch (const Exception& e) {  HandleException(e); }

  	V = 0; E = 0;

	// Na początku liczymy wierzchołki i krawędzie.
	// Indeksy wierzchołków MUSZĄ być kolejnymi liczbami poczynając od zera
	while (fscanf(infile, "%d %d %d\n", &v, &w, &weight) != EOF) {
		if (v > V) V = v;
		if (w > V) V = w;
		E++;
	}

	V++;		// Numery wezłów powinny zaczynać się od zera.
	index = 0;
	edges = new Edge*[E];
	rewind(infile);

	while (fscanf(infile, "%d %d %d\n", &v, &w, &weight) != EOF)
		edges[index++] = new Edge(v, w, weight);

	fclose(infile);

	std::cout << "\"verts\": " << V << ", \"edges\": " << E;
}


Graph::Graph(const Graph& g) : V(g.V), E(g.E) {
	edges = new Edge*[E];
	for (long i = 0; i < E; i++)
		edges[i] = new Edge(*g.edges[i]);
}

Graph::~Graph(void) { delete [] edges; }

Edge** Graph::getEdges(void) const { return edges; }

long Graph::getNumVertices(void) const { return V; }

long Graph::getNumEdges(void) const { return E; }

void Graph::print() const {
	std::cout << " v\tw\twaga\n";
	std::cout << " --------------------\n";
	for (int i = 0; i < E; i++)
		edges[i]->print();
	std::cout << " --------------------\n" << std::endl;
}
