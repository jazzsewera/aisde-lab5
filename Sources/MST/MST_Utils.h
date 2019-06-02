#ifndef MST_UTILS_H
#define MST_UTILS_H

#include <string>
#include <chrono>

#include "Edge.h"
#include "Graph.h"

// Powyżej tej liczby krawędzi grafy nie będą wypisywane na ekran
// Do pomiaru czasu wykonania algorytmów
#define MAX_PRINTABLE_E 10

typedef std::chrono::steady_clock::time_point time_point;

// Uruchamia wybrany algorytm, mierzy czas i ewentualnie wypisuje drzewo
void FindMST (
	const Graph& graph,
	Edge** (* algorithm) (const Graph&),
	const std::string& message,
	bool printTree
);

// Wypisuje drzewo
void printMST(Edge** mst, int mstSize);

#endif // MST_UTILS_H
