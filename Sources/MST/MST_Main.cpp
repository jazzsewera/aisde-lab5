#include <iostream>
#include "MST_Algorithms.h"
#include "MST_Utils.h"
#include "Graph.h"
#include "Exception.h"

int main(int argc, char* argv[]) {

	char *fileName;

	try {
		if (argc == 2)
			fileName = argv[1];
		else
			throw Exception(ImproperMSTCallException, argv[0]);

		std::cout << "{ ";
		Graph graph = Graph(fileName);
		std::cout << ", ";

		//if (graph.getNumEdges() <= MAX_PRINTABLE_E)
		//	graph.print();

		std::cout << "\"times\": {";
		FindMST(graph, Boruvka, "Boruvka",  true);
		std::cout << ", ";
		FindMST(graph, Kruskal, "Kruskal", true);
		std::cout << "}";
		std::cout << " }" << std::endl;
	}  catch (const Exception& e) { HandleException(e); }

	return 0;
}
