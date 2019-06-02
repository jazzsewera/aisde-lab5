#ifndef MST_ALGORITHMS_H
#define MST_ALGORITHMS_H

#include "Edge.h"
#include "Graph.h"

// Algorytm Kruskala znajdujący najmniejsze drzewo rozpinające
Edge** Kruskal(const Graph& g);

// Algorytm Borůvki znajdujący najmniejsze drzewo rozpinające
Edge** Boruvka(const Graph& g);

#endif // MST_ALGORITHMS_H
