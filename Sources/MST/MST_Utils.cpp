#include <iostream>

#include "MST_Utils.h"

// Wypisuje drzewo
void printMST(Edge** mst, int mstSize) {
	if (mst == nullptr)
		return;
	std::cout << " v\tw\twaga\n";
	std::cout << " -------------------\n";
	for (int i = 0; i < mstSize; i++)
		mst[i]->print();
	std::cout << " -------------------\n" << std::endl;
}

// Uruchamia wybrany algorytm, mierzy czas i ewentualnie wypisuje drzewo
void FindMST (
	const Graph& graph,
	Edge** (* algorithm) (const Graph&),
	const std::string& message,
	bool printTree
) {
	time_point from_time, to_time;
	Edge** mst;

	std::cout << "\"" << message << "\"";
	from_time = std::chrono::steady_clock::now();
	// Właściwe wywołanie algorytmu tworzącego MST
	mst = algorithm(graph);
	to_time = std::chrono::steady_clock::now();
	std::cout << ": ";
	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(to_time - from_time).count();

	if (printTree == true) {
		if (graph.getNumVertices() <= MAX_PRINTABLE_E+1)
 			printMST(mst, graph.getNumVertices()-1);
	}
}
