#include "MinpathConfig.h"
#include <cstring>

MinpathConfig::MinpathConfig(int argc, char** argv) {
	if (argc == 2)
		findAllPaths = true;
	else if (argc == 4)
		findAllPaths = false;
	else
		throw Exception(ImproperMinpathCallException, argv[0]);

	strcpy(graphFile, argv[1]);

	// Jeżeli w linii wywołania podaliśmy wierzchołek początkowy i docelowy...
 	if (argc == 4) {
		source      = String2Int(argv[2]);
		destination = String2Int(argv[3]);
	}
	// Jeśli nie, to wyznaczamy długość najkrótszych dróg "od każdego wierzchołka do każdego innego"
	else
		source = destination = 0;
}
