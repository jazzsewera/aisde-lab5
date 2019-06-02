/**********************************************************************************
*                                     GRAPHGEN
*
*	Generator losowych grafów NIESKIEROWANYCH.
*	Tworzony jest graf o zadanej liczbie 'V' wierzchołków i 'E' krawędzi.
*	Wagi wierzchołków to wartości bezwzględne zmiennej losowej
*	o rozkładzie normalnym o wartosci oczekiwanej 0 i odchyleniu standardowym STDDEV.
*
*	Autor: Dominik Kasprowicz
*	Ostatnia aktualizacja: 17 maja 2017
*
***********************************************************************************/

#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>
#include <fstream>
#include <list>
#include <string>

#include "Exception.h"

const int STDDEV = 200;
const int SEED   = 300499;	// TU WPISUJEMY NUMER ALBUMU

// Krawędź grafu nieskierowanego
class Edge {
	public:
		Edge(long v, long w, int wt): v(v), w(w), weight(wt) {}
		long v, w;    // Numery wierzcholkow
  		int  weight;  // Waga krawedzi
};


// Lista sąsiedztwa, czyli tablica list obiektów typu HalfEdge
// Zapisana w obiekcie klasy 'std::list'
class Graph {
	private:
		const long V, E;        // Ile wierzchołków? Ile krawędzi?
		std::list<Edge> edges;  // Lista krawędzi
	public:
		Graph(long V, long E);	// Konstruktor grafu o V wierzchołkach i E krawędziach
		virtual ~Graph(void) {}
		bool save(const std::string& filename);

		// Liczba krawędzi w grafie pełnym o V wierzchołkach
		static long maxEdgesPossible(long v) { return v*(v-1)/2; }
};


// Konstruktor grafu prostego (nieskierowanego)
Graph::Graph(long _V, long _E) : V(_V), E(_E < maxEdgesPossible(V) ? _E : maxEdgesPossible(V)) {
  	// Przypisywanie krawędziom losowych wag. Są one wartościami bezwzględnymi
  	// zmiennej pseudolosowej o rozkładzie normalnym, generowanej
  	// za pomocą algorytmu Marsaglii i Braya
  	const double norm = 2./(double)RAND_MAX;
	// 'Co którą' krawędź trzeba usunąć.
	const double rate = (double)(Graph::maxEdgesPossible(V) - E)/Graph::maxEdgesPossible(V);
	const double safety_margin = (E < 20) ? 5.0 : (E < 100) ? 2.0 : 1.2;
	double x1, x2, r;
	long   edge_cnt = 0;
	std::list<Edge>::iterator edge;

	// Wstępnie generujemy graf nieco 'gęstszy' niż wynika to z wartości E.
	// Później go 'przerzedzimy'.
 	for (long v = 0; v < V-1; v++) {
 		for (long w = v+1; w < V; w++) {
			// Aby zapewnic spójność, wstawiamy do grafu scieżkę Hamiltona: 0->1->2->(V-1).
			// Niezależnie od tego chcemy wstawić mniej więcej 120% docelowej liczby krawędzi.
			if (w == v+1 or rand() < safety_margin*(1-rate)*RAND_MAX) {
 				do {
 					x1 = norm*rand()-1;
 					x2 = norm*rand()-1;
	 				r = x1*x1 + x2*x2;
				} while (r >= 1);
				r = sqrt((-2*log(r))/r);
				edges.push_back(Edge(v, w, (int)(1+STDDEV*fabs(x1*r))));
				edge_cnt++;
			}
		}
	}

	// Z grafu wycinamy tyle krawędzi, by zostało ich E
	edge = edges.begin();
	while(edge_cnt > E) {
		// Usuwamy krawędzie z prawdopodobieństwem "rate".
		// Musimy przy tym uważać, żeby nie wyciać naszej scieżki Hamiltona.
		if (edge->w != (edge->v)+1 and rand() < rate*RAND_MAX) {
			edge = edges.erase(edge);  // usuwamy krawędź i idziemy do nastepnej
			--edge_cnt;
		}
		++edge; // Ewentualnie niczego nie usuwamy i idziemy do następnej.

		// Może się okazać, że po jednym przejściu pozostało nam wciąż zbyt wiele krawędzi
		if (edge == edges.end())
			edge = edges.begin();
	}
}


// Zapisuje graf do pliku.
bool Graph::save(const std::string& filename) {
	std::list<Edge>::iterator edge;
	std::ofstream outfile(filename);

	if (!outfile.is_open()) {
		std::cout << " Nie mogę utworzyć pliku!" << std::endl;
		return false;
	}

	for (edge = edges.begin(); edge != edges.end(); edge++)
		outfile << (*edge).v << " " << (*edge).w << " " << (*edge).weight << std::endl;

	outfile.close();
	return true;
}


int main(int argc, char* argv[]) {
	long 	 V, E;
	char   filename[256];

	if (argc < 3) {
		std::cout << " Prawidłowe wywołanie: " << argv[0];
  		std::cout << " ile_wierzchołkow ile_krawędzi [plik_wynikowy]" << std::endl;
  		return 0;
	}

	V = atol(argv[1]);
	E = atol(argv[2]);

	try {

		if (SEED == 0) throw Exception(BadSeedException, "Graphgen.cpp");
		if (V <= 0)    throw Exception(ZeroVerticesException, "");
		if (E <= 0)    throw Exception(ZeroEdgesException, "");
		if (E < V-1)   throw Exception(MessageException, " Liczba krawędzi nie może być mniejsza od liczby wierzchołków minus jeden - inaczej graf będzie niespójny!");

	 	if (argc > 3)
	 		strcpy(filename, argv[3]);
	 	else
	 	 	strcpy(filename, "graf.txt");

		srand(SEED);

		std::cout << "\n Generacja ";
	 	if (E >= Graph::maxEdgesPossible(V)) {
	 		E = Graph::maxEdgesPossible(V);
	 		std::cout << "PEŁNEGO ";
		}

	 	std::cout << "grafu nieskierowanego o " << V << " wierzchołkach i " << E << " krawędziach...";
		Graph g(V, E);
		std::cout << " gotowe.\n";

		std::cout << " Zapisywanie do pliku '" << filename << "'...";
		g.save(filename);
		std::cout << " gotowe.\n" << std::endl;
	} catch (const Exception& e) { HandleException(e); }

	return 0;
}
