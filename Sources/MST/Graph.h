#ifndef GRAPH_H
#define GRAPH_H

#include "Edge.h"

// Graf nieskierowany jako tablica krawędzi
class Graph {
	private:
		long   V, E;	// Liczba wierzchołków i krawędzi
		Edge **edges;
	public:
		Graph(const char *filename);
		Graph(const Graph& g);
		~Graph(void);
		Edge** getEdges(void) const;
		long   getNumVertices(void) const;
		long   getNumEdges(void) const;
		void   print() const;
};

#endif // GRAPH_H
