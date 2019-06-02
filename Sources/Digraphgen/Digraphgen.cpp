/**********************************************************************************
*                                      DIGRAPHGEN
*
*	Generator losowych grafów SKIEROWANYCH.
*	Tworzony jest graf o zadanej liczbie 'V' wierzchołków i 'E' krawędzi.
*	Stopnie wejściowe i wyjściowe wszystkich wierzchołków maja wartości
* w przybliżeniu równe E/V.
* Wagi krawędzi mają rozkład równomierny na przedziale [1, MAXWEIGHT].
*
*	Autor: Dominik Kasprowicz
*	Ostatnia aktualizacja: 17 maja 2017
*
***********************************************************************************/

#include <cstdlib>
#include <climits>
#include <cstring>
#include <iostream>
#include <fstream>
#include <list>
#include <string>

#include "Exception.h"

const int MAXWEIGHT = 100;
const int SEED      = 300499;	// TU WPISUJEMY NUMER ALBUMU

class Edge {
	public:
		Edge(long v, long w, int wt): v(v), w(w), weight(wt) {}
		long v, w;    // Numery wierzchołków
  		int	 weight;  // Waga krawędzi
};

// Lista sąsiedztwa, czyli tablica list obiektów klasy Edge
// Zapisana w obiekcie klasy 'std::list'
class Graph {
	private:
		const long V, E;	// Ile wierzcholkow? Ile krawedzi?
		std::list<Edge> edges; // Lista krawedzi
	public:
		Graph(long V, long E);	// Konstruktor grafu o V wierzcholkach i E krawedziach
		virtual ~Graph(void) { };
		bool save(const std::string& file);
		// Liczba krawedzi w pelnym grafie skierowanym o V wierzcholkach
		static long maxEdgesPossible(long v) { return v*(v-1); }
};


Graph::Graph(long _V, long _E) : V(_V), E(_E < maxEdgesPossible(V) ? _E : maxEdgesPossible(V)) {

  	const double WEIGHT_NORM = (double)MAXWEIGHT / (double)RAND_MAX;
	const double rate = (double)(maxEdgesPossible(V) - E)/maxEdgesPossible(V); // 'Co ktora' krawedz trzeba usunac.
	const double safety_margin = (E < 20) ? 5.0 : (E < 100) ? 2.0 : 1.2;
	long         edge_cnt = 0;
	std::list<Edge>::iterator edge;

	// Wstepnie generujemy graf nieco 'gestszy', niz wynika to z wartosci E.
	// Pozniej go 'przerzedzimy'.
 	for (long v = 0; v < V; v++) {
 		for (long w = 0; w < V; w++) {
			// Nie chcemy petli wlasnych, czyli krawedzi z 'v'do 'v'.
			if (v == w) continue;
			// Chcemy za to miec cykl Hamiltona: 0->1->2->(V-1)->0.
			// Niezaleznie od tego chcemy wstawic mniej wiecej 120% docelowej liczby krawedzi.
			if (w == (v+1) % V  or rand() < safety_margin*(1-rate)*RAND_MAX) {
				edges.push_back(Edge(v, w, (int)(1 + WEIGHT_NORM * rand())));
				edge_cnt++;
			}
		}
	}

	// Z grafu wycinamy tyle krawedzi, by zostalo ich E
	edge = edges.begin();
	while(edge_cnt > E) {
		// Usuwamy krawedzie z prawdopodobienstwem "rate".
		// Musimy uwazac, zeby nie wyciac naszego cyklu Hamiltona
		if ((*edge).w != ((*edge).v+1) % V and rand() < rate*RAND_MAX) {
			edge = edges.erase(edge);  // usuwamy krawedz i...
			--edge_cnt;		             // idziemy do nastepnej.
		}
		else
			++edge; // Ewentualnie nic nie usuwamy i idziemy do nastepnej.

		// Moze sie okazac, ze po jednym przejsciu pozostalo nam wciaz zbyt wiele krawedzi
		if (edge == edges.end())
			edge = edges.begin();
	}
}


bool Graph::save(const std::string& filename) {
	std::list<Edge>::iterator edge;
	std::ofstream	outfile(filename, std::ios::out);

	if (!outfile.is_open()) {
		std::cout << " Nie mogę utworzyć pliku!" << std::endl;
		return false;
	}

	for (edge = edges.begin(); edge != edges.end(); edge++)
		outfile << (*edge).v << " " << (*edge).w << " " << (*edge).weight << "\n";

	outfile.close();
	return true;
}


int main(int argc, char* argv[]) {
	long 	V, E;	// Liczba wierzcholkow i krawedzi
	char  filename[256];

	try {
		if (argc < 3 || atol(argv[1]) < 2 || atol(argv[2]) < 1) {
			std::cout << " Prawidlowe wywołanie: " << argv[0];
	  		std::cout << " ile_wierzchołków ile_krawędzi [plik_wynikowy]" << std::endl;
	  		exit(EXIT_FAILURE);
		}

	 	if (argc > 3)
	 		strcpy(filename, argv[3]);
	 	else
	 		strcpy(filename, "digraf.txt");

		V = atol(argv[1]);
		E = atol(argv[2]);

		if (SEED == 0) throw Exception(BadSeedException, "Digraphgen.cpp");
		if (V <= 0)    throw Exception(ZeroVerticesException, "");
		if (E <= 0)    throw Exception(ZeroEdgesException, "");
		if (E < V)     throw Exception(MessageException, "Liczba krawędzi nie może być mniejsza od liczby wierzchołków - inaczej graf skierowany będzie niespójny!");

		srand(SEED);
		std::cout << "\n Generacja ";
	 	if (E >= Graph::maxEdgesPossible(V)) {
		 	E = Graph::maxEdgesPossible(V);
	 		std::cout << "PEŁNEGO ";
	 	}
	 	std::cout << "grafu skierowanego o " << V << " wierzchołkach i " << E << " krawędziach...";
		Graph g(V, E);
		std::cout << " gotowe.\n";

		std::cout << " Zapisywanie do pliku '" << filename << "'...";
		g.save(filename);
		std::cout << " gotowe.\n" << std::endl;
	} catch (const Exception& e) { HandleException(e); }
	return 0;
}
