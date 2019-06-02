/***************************************************************/
/*                                                             */
/*                 AISDE - ALGORYTMY GRAFOWE                   */
/*   POSZUKIWANIE NAJKRÓTSZEJ ŚCIEŻKI W GRAFIE SKIEROWANYM     */
/*                                                             */
/*   Autor: Dominik Kasprowicz                                 */
/*   Ostatnia modyfikacja: 18 maja 2017                        */
/*                                                             */
/***************************************************************/

#include <iostream>
#include <fstream>
#include <string>

#include "MinpathUtils.h"
#include "MinpathConfig.h"
#include "Exception.h"
#include "List.h"
#include "ListElement.h"
#include "HeapPQ.h"
#include "DistanceTable.h"
#include "AbstractGraph.h"
#include "MatrixDigraph.h"
#include "ListDigraph.h"

// Algorytm znajdujący najkrotszą drogę od 'source' do 'destination' metodą
// pełnego przeglądu przestrzeni rozwiązań. Przegląd jest dokonywany poprzez
// rekurencyjne przeszukiwanie grafu w gląb (ang. Depth-First Search).

ulong DFS(
	MatrixDigraph *graph, // Analizowany graf
	bool    *visited,     // Tablica flag wierzchołków - czy odwiedzony?
	int      current,     // Indeks bieżącego wierzchołka
	int      dest,        // Indeks wierzchołka docelowego
	ulong    currentCost, // Długość ścieżki w bieżącej iteracji
	ulong   &minCost      // Najmniejsza znaleziona długość ścieżki
) {
	ulong edgeCost, newCost = 0;
	visited[current] = true;

	for (int w = 0; w < graph->getNumVertices(); w++) {
		edgeCost = graph->getWeight(current, w);
		newCost = currentCost + edgeCost;
		if (w != current && edgeCost != INF && visited[w] == false) {
			if (w != dest)
   			DFS(graph, visited, w, dest, newCost, minCost);
       	else {
				if (newCost < minCost)
					minCost = newCost;
			}
		}
	}
	visited[current] = false;
	return minCost;
}


ulong RunDFS(AbstractGraph *graph, int source, int destination, bool findAllPaths) {
	ulong minCost = INF;  // Koszt najkrótszej drogi od 'source' do 'destination'.
	bool *visited;        // Jest 'true', jeśli wierzchołek o odpowiednim indeksie został już odwiedzony.
	int   V = graph->getNumVertices();
	visited = new bool[V];

	if (findAllPaths == false) {
		for (int i = 0; i < V; visited[i++] = false);
		return DFS((MatrixDigraph*)graph, visited, source, destination, 0, minCost);
	}
  	else {
		for (int v = 0; v < V; v++) {
 			for (int w = 0; w < V; w++) {
 				// Czyścimy pomocnicze struktury danych
 				minCost = INF;
 				for (int i = 0; i < V; visited[i++] = false);
 				// Wyznaczamy odległość
 				minCost = DFS((MatrixDigraph*)graph, visited, v, w, 0, minCost);

				// Wypisujemy najkrótsze odległości na ekran
 				if (V <= MAX_PRINTABLE_V) {
					if (minCost == INF)
						std::cout << "\t-";
					else
						std::cout << "\t" << minCost;
				}
			}
			if (V <= MAX_PRINTABLE_V)
				std::cout << "\n";
		}
	}
	return 0;
}


// Zwraca macierz odległości między wierzchołkami, wyznaczoną za pomocą algorytmu Floyda-Warshalla.
// Zawartość pola w wierszu "i" i kolumnie "j" odpowiada długości najkrótszej drogi od "i" do "j".

MatrixDigraph* Floyd(MatrixDigraph *graph) {
	int  V;
	ulong pkLength, pskLength; // Długości dróg p->k i p->s->k

	V = graph->getNumVertices();			// Liczba wierzchołków w grafie.

	// Indeksy wierzchołków: p - początek, k - koniec, s - ewentualny wierzchołek pośredni
	for(int s = 0; s < V; s++) {
		for(int p = 0; p < V; p++) {
			for(int k = 0; k < V; k++) {
  				pkLength = graph->getWeight(p, k);
  				pskLength = Add(graph->getWeight(p, s), graph->getWeight(s, k));
	 			// Jeśli dotychczasowy dystans p->k jest większy od sumy p->s i s->k,
 				// zapisywany jest ten nowy.
				if (pskLength < pkLength) {
					graph->setWeight(p, k, pskLength);
				}
			}
		}
	}
	return graph;
}

ulong RunFloyd(AbstractGraph *graph, int source, int destination, bool findAllPaths) {

 	MatrixDigraph* distanceMatrix = Floyd((MatrixDigraph*) graph);

	if (findAllPaths == false)
		return distanceMatrix->getWeight(source, destination);
	else {
 		if(distanceMatrix->getNumVertices() <= MAX_PRINTABLE_V)
			distanceMatrix->print();
		return INF;
	}
}

// Znajduje najkrótszą drogę od 'source' do 'destination' wykorzystując algorytm Dijkstry.
// Odległości od źródła do kolejnych wierzchołków są przechowywane w tablicy
// (obiekcie klasy DistanceTable).
// Sam graf jest zapisany jako macierz przyległości (sąsiedztwa).

DistanceTable* DijkstraMatrix(AbstractGraph* graph, int source, int destination, bool findAllPaths) {

	DistanceTable *distances; // Tu pamiętamy wagi wierzchołków oraz to,
	                          // czy są to wagi tymczasowe, czy ustalone.
 	int  activeVertex;        // Numer "aktywnego" wierzchołka, tj. ostatniego,
	                          // któremu przypisaliśmy _stałą_ wagę.
	ulong distSourceToActive; // Odległość od źródła do ostatniego wierzchołka
	                          // o ustalonej wadze ('aktywnego').
	ulong newDist, distActiveToNext;


	distances = new DistanceTable(graph->getNumVertices());
	distances->setDistance(source, 0);

	for(int k = 0; k < graph->getNumVertices(); k++) {
		// Jesli szukamy odleglosci tylko do wierzcholka 'destination'
		// i wlasnie go znalezlismy...
		if (findAllPaths == false && distances->isFixed(destination))
			break;
		// Spośród wierzchołków o jeszcze nieustalonych wagach wybieramy
		// ten najbliższy źródła. Teraz to on będzie wierzchołkiem 'aktywnym'.
  		activeVertex = distances->getNearest();
		// Może się zdarzyć, że żaden wierzchołek nie jest osiągalny z akytwnego.
		if (activeVertex == NO_VERTEX_FOUND)
			break;
		// Ustalamy raz na zawsze wagę aktywnego wierzchołka...
		distances->fix(activeVertex);
		// ...i wyznaczamy jego odległość od źródła.
  		distSourceToActive = distances->getDistance(activeVertex);

		// Uaktualniamy po kolei wagi wszystkich wierzchołków,
		// do których można dojść z wierzchołka aktywnego.
  		for (int nextVertex = 0; nextVertex < graph->getNumVertices(); nextVertex++) {
			distActiveToNext = graph->getWeight(activeVertex, nextVertex);
			if (distActiveToNext != INF && distances->isFixed(nextVertex) == false) {
  			newDist = distSourceToActive + distActiveToNext;
		 		// Jesli dotychczasowy dystans source -> nextVertex jest większy
		 		// od nowo obliczonego, zapisywany jest ten nowy.
				if (newDist < distances->getDistance(nextVertex))
					distances->setDistance(nextVertex, newDist);
			}
		}
	}
	return distances;
}

// Znajduje najkrótszą drogę od 'source' do 'destination' wykorzystując algorytm Dijkstry.
// Odległości od źródła do kolejnych wierzchołków są przechowywane w tablicy
// (obiekcie klasy DistanceTable).
// Sam graf jest zapisany jako tablica list wiązanych.

DistanceTable* DijkstraList(AbstractGraph* graph, int source, int destination, bool findAllPaths) {

	DistanceTable *distances;  // Wagi wierzchołków oraz to, czy są tymczasowe, czy ustalone.
	int  activeVertex;         // Numer "aktywnego" wierzchołka, tj. ostatniego,
                             // któremu przypisaliśmy _stałą_ wagę.
	ulong distSourceToActive;  // Odległość od źródła do ostatniego wierzchołka o ustalonej wadze.
	ulong newDist, distActiveToNext;
	int  nextVertex;
	List<HalfEdge *> *edgesFromActive = nullptr;
	HalfEdge* edge;

	distances = new DistanceTable(graph->getNumVertices());
	distances->setDistance(source, 0);

	for(int k = 0; k < graph->getNumVertices(); k++) {
		// Jeśli szukamy odległości tylko do wierzchołka 'destination' i została ona już znaleziona.
		if (findAllPaths == false && distances->isFixed(destination))
			break;
		// Spośród wierzchołków o jeszcze nieustalonych wagach wybieramy
		// ten najbliższy źródła. Teraz to on będzie wierzchołkiem 'aktywnym'.
		activeVertex = distances->getNearest();
		// Może się zdarzyć, że żaden wierzchołek nie jest osiągalny z akytwnego.
		if (activeVertex == NO_VERTEX_FOUND)		// Uaktualniamy po kolei wagi wszystkich wierzchołków,
		// do których można dojść z wierzchołka aktywnego

			break;
		// Ustalamy raz na zawsze wagę aktywnego wierzchołka...
		distances->fix(activeVertex);
		// ...i wyznaczamy jego odległość od źródła.
		distSourceToActive = distances->getDistance(activeVertex);
		// Pobieramy listę krawędzi wychodzących z wierzchołka aktywnego
		edgesFromActive = graph->getOutgoingEdges(activeVertex);

		// Uaktualniamy po kolei wagi wszystkich wierzchołków,
		// do których można dojść z wierzchołka aktywnego
		while (edgesFromActive->hasMoreElements()) {
			edge = edgesFromActive->getNext();
			nextVertex = edge->getEndingVertexIndex();
	    	distActiveToNext = edge->getWeight();
			// Jeśli wierzchołek, do którego prowadzi krawędź, nie ma jeszcze
			// ustalonej wagi, to sprawdzamy, czy można ją zmniejszyć
			if (!distances->isFixed(nextVertex)) {
				newDist = distSourceToActive + distActiveToNext;
	 			// Jeśli dotychczasowy dystans source -> nextVertex jest większy od nowo
	 			// obliczonego, zapisywany jest ten nowy.
				if (newDist < distances->getDistance(nextVertex))
					distances->setDistance(nextVertex, newDist);
			}
		}
	}
	return distances;
}

// Znajduje najkrotszą drogę od 'source' do 'destination' wykorzystując algorytm Dijkstry.
// Odległości od źródła do kolejnych wierzchołków sa przechowywane w kolejce priorytetowej.
// Sam graf jest zapisany jako macierz przyległości (sąsiedztwa).

DistanceTable* DijkstraMatrixPQ(AbstractGraph* graph, int source, int destination, bool findAllPaths) {

 	Vertex 	activeVertex;       // Ostatni wierzchołek, któremu przypisaliśmy _stałą_ wagę.
	ulong    distActiveToNext;	// Waga krawedzi od 'activeVertex' do następnika
 	ulong    newDist;		    // Zmienna tymczasowa.
	DistanceTable *distances;   // Wagi wierzchołków (tymczasowe i ustalone)

	// Utwórz kolejkę wierzcholków z tymczasowymi wagami jako priorytetem. Wszystkie wagi
	// są nieskonczone, jedynie waga 'source' jest zerowa, jak to w algorytmie Dijkstry.
	// Wierzchołki, których waga zostanie ustalona, są usuwane z kolejki.
	HeapPQ distancePQ(graph->getNumVertices());
	distancePQ.decreaseKey(source, 0);

	distances = new DistanceTable(graph->getNumVertices());
	distances->setDistance(source, 0);

	while (distancePQ.notEmpty()) {
		// Spośród wierzchołków o jeszcze nieustalonych wagach wybieramy
		// ten najbliższy źródła. Teraz to on będzie wierzchołkiem 'aktywnym'.
		activeVertex = distancePQ.removeNearest();
		// Może się zdarzyć, że żaden wierzchołek nie jest osiągalny z aktywnego
		if (activeVertex.dist == INF)
			break;
		// Jesli szukamy odległości tylko do wierzchołka 'destination' i została ona już znaleziona.
		if (findAllPaths == false && activeVertex.index == destination)
			break;

		// Uaktualniamy po kolei wagi wszystkich wierzchołków,
		// do których można dojść z wierzchołka aktywnego.
		for (int nextVertexIndex = 0; nextVertexIndex < graph->getNumVertices(); nextVertexIndex++) {
			distActiveToNext = graph->getWeight(activeVertex.index, nextVertexIndex);
			// *** Dla dociekliwych: jakim przypadkom odpowiada warunek w nawiasach? ***
			if (distActiveToNext != INF && distancePQ.contains(nextVertexIndex)) {
				newDist = activeVertex.dist + distActiveToNext;
	 			// Jeśli dotychczasowy dystans source -> nextVertexIndex jest większy od nowo
	 			// obliczonego, zapisywany jest ten nowy.
				if (newDist < distancePQ.getDistance(nextVertexIndex)) {
					distancePQ.decreaseKey(nextVertexIndex, newDist);
					distances->setDistance(nextVertexIndex, newDist);
				}
			}
		}
	}

	return distances;
}


// Znajduje najkrotszą drogę od 'source' do 'destination' wykorzystując algorytm Dijkstry.
// Odległości od źródła do kolejnych wierzchołków są przechowywane w kolejce priorytetowej.
// Sam graf jest zapisany jako tablica list wiązanych.

DistanceTable* DijkstraListPQ(AbstractGraph* graph, int source, int destination, bool findAllPaths) {

 	Vertex 	activeVertex;      // Ostatni wierzchołek, ktoremu przypisaliśmy _stałą_ wagę.
	DistanceTable    *distances;	// Wagi wierzcholkow (tymczasowe i ustalone)
	List<HalfEdge *> *edgesFromActive;
	HalfEdge* edge;
	ulong newDist, distActiveToNext;
	int   nextVertexIndex;

	// Utwórz kolejkę wierzcholków z tymczasowymi wagami jako priorytetem. Wszystkie wagi
	// są nieskonczone, jedynie waga 'source' jest zerowa, jak to w algorytmie Dijkstry.
	// Wierzchołki, których waga zostanie ustalona, są usuwane z kolejki.
	HeapPQ distancePQ(graph->getNumVertices());
	distancePQ.decreaseKey(source, 0);

	// Inicjalizacja tablicy odległości
	distances = new DistanceTable(graph->getNumVertices());
	distances->setDistance(source, 0);

	while(distancePQ.notEmpty()) {
		// Spośród wierzchołków o jeszcze nieustalonych wagach wybieramy
		// ten najbliższy źródła. Teraz to on będzie wierzchołkiem 'aktywnym'.
		activeVertex = distancePQ.removeNearest();
		// Może się zdarzyć, że żaden wierzchołek nie jest osiągalny z akytwnego.
		if(activeVertex.dist == INF)
			break;
		// Jesli szukamy odległości tylko do wierzchołka 'destination' i została ona już znaleziona.
		if (findAllPaths == false && activeVertex.index == destination)
			break;
		// Pobieramy listę krawędzi wychodzących z wierzchołka o właśnie ustalonej wadze
		edgesFromActive = graph->getOutgoingEdges(activeVertex.index);

		// Uaktualniamy po kolei wagi wszystkich wierzchołków,
		// do których można dojść z ostatnio ustalonego.
		while (edgesFromActive->hasMoreElements()) {
			edge = edgesFromActive->getNext();
			nextVertexIndex = edge->getEndingVertexIndex();
   		distActiveToNext = edge->getWeight();

			// *** Dla dociekliwych: jakiemu przypadkowi odpowiada warunek w nawiasach? ***
			if (distancePQ.contains(nextVertexIndex)) {
  				newDist = activeVertex.dist + distActiveToNext;
	 			// Jeśli dotychczasowy dystans source -> nextVertex jest większy od nowo
	 			// obliczonego, zapisywany jest ten nowy.
				if (newDist < distancePQ.getDistance(nextVertexIndex)) {
					distancePQ.decreaseKey(nextVertexIndex, newDist);
					distances->setDistance(nextVertexIndex, newDist);
				}
			}
		}
	}

	return distances;
}


void FindShortestPath(
		AbstractGraph *graph,
		const MinpathConfig *config,
		ulong (*algorithm) (AbstractGraph*,  int , int, bool)
) {
	time_point from_time, to_time;
	ulong minCost = INF;  // Koszt najkrótszej drogi od 'source' do 'destination'.
 	int   source       = config->source;
 	int   destination  = config->destination;
	bool  findAllPaths = config->findAllPaths;

	from_time = CurrentTime();
	// Wlaściwy algorytm wykonuje się tutaj
	minCost = algorithm(graph, source, destination, findAllPaths);
 	to_time = CurrentTime();

	if (findAllPaths == false) {
		if (minCost != INF) {
 			std::cout << "  Najkrótsza droga od " << source << " do " << destination;
  			std::cout << "  ma koszt " << minCost << std::endl;
		}
		else {
			std::cout << "  Nie istnieje droga z wierzchołka " << source << " do " << destination << std::endl;
		}
	}
  	std::cout << "\n  Obliczenia trwały " << TimeDiffMicroseconds(from_time, to_time) << " mikrosekund.\n";
}


void FindShortestPathDijkstra(
		AbstractGraph *graph,
		const MinpathConfig *config,
		DistanceTable* (*algorithm) (AbstractGraph*,  int , int, bool)
) {
	time_point from_time, to_time;
	ulong minCost     = INF;  // Koszt najkrótszej drogi od 'source' do 'destination'.
	int  V            = graph->getNumVertices();
	int  source       = config->source;
	int  destination  = config->destination;
	bool findAllPaths = config->findAllPaths;
	DistanceTable* distances;

	if (findAllPaths == false) {
		from_time = CurrentTime();
		// Wlaściwy algorytm wykonuje się tutaj
		distances = algorithm(graph, source, destination, findAllPaths);
		to_time = CurrentTime();
		minCost = distances->getDistance(destination);

		if (minCost != INF)
 			std::cout << "  Najkrótsza droga od " << source << " do " << destination << "  ma koszt " << minCost << std::endl;
		else
			std::cout << "  Nie istnieje droga z wierzchołka " << source << " do " << destination << std::endl;
	}
	else {
		from_time = CurrentTime();
		// Właściwy algorytm wykonuje się tutaj
		for (source = 0; source < V; source++)
			algorithm(graph, source, destination, findAllPaths)->print();
		to_time = CurrentTime();
	}
  std::cout << "\n  Obliczenia trwały " << TimeDiffMicroseconds(from_time, to_time) << " mikrosekund.\n";
}


void present(const std::string& message) {
	std::cout << " ---------------------------------------------------------------\n";
	std::cout << " " << message << std::endl;
}


int main(int argc, char* argv[]) {

	// Identyczne grafy: 2x reprezentacja tablicowa i 1x listowa.
	AbstractGraph *matrixGraph = nullptr, *graphForFloyd = nullptr, *listGraph = nullptr;
	// Struktura zawierająca parametry wywołania algorytmów:
	// indeksy wierzchołka początkowego i końcowego oraz flagę
	// 'findAllPaths' decydującą o tym, czy szukane są drogi
	// między wszystkimi parami wierzchołkow.
	MinpathConfig *config;

	try {
		config = new MinpathConfig(argc, argv);

		listGraph     = new ListDigraph(config->graphFile);
		matrixGraph   = new MatrixDigraph(*(ListDigraph*)listGraph);
		// Algorytm Floyda modyfikuje wczytany graf, więc wymaga własnej kopii.
		graphForFloyd = new MatrixDigraph(*(ListDigraph*)listGraph);

 		if (config->source >= matrixGraph->getNumVertices())
 			throw Exception(UnknownVertexException, argv[2]);
		if (config->destination >= matrixGraph->getNumVertices())
			throw Exception(UnknownVertexException, argv[3]);

		std::cout << "\n Wczytano graf skierowany o " << matrixGraph->getNumVertices() << " wierzchołkach i " << matrixGraph->getNumEdges() << " krawędziach\n";
		if (matrixGraph->getNumVertices() < MAX_PRINTABLE_V)
			matrixGraph->print();

		present("Algorytm Dijkstry\n   graf: macierz przyległości\n   kolejka priorytetowa wierzchołków: tablica\n");
		FindShortestPathDijkstra(matrixGraph, config, DijkstraMatrix);

		present("Algorytm Dijkstry\n   graf: macierz przyległości\n   kolejka priorytetowa wierzchołków: kopiec\n");
		FindShortestPathDijkstra(matrixGraph, config, DijkstraMatrixPQ);

		present("Algorytm Dijkstry\n   graf: tablica list sąsiedztwa\n   kolejka priorytetowa wierzchołków: tablica\n");
		FindShortestPathDijkstra(listGraph, config, DijkstraList);

		present("Algorytm Dijkstry\n   graf: tablica list sąsiedztwa\n   kolejka priorytetowa wierzchołków: kopiec\n");
		FindShortestPathDijkstra(listGraph, config, DijkstraListPQ);

		present("Algorytm Floyda\n");
		FindShortestPath(graphForFloyd, config, RunFloyd);

		// UWAGA!!! TEN ALGORYTM MA TAK DUŻĄ ZŁOŻONOŚĆ, ŻE CZAS OCZEKIWANIA NA WYNIK
		// MOŻE WYNOSIĆ KILKA MINUT JUŻ DLA GRAFÓW O KILKUNASTU (!) WIERZCHOŁKACH.
		// PRZY ANALIZIE WIĘKSZYCH GRAFÓW POZOSTAŁYMI ALGORYTMAMI NALEŻY "WYKOMENTOWAĆ" PONIŻSZE DWA WIERSZE.
		//present("Przeszukiwanie grafu w głąb (DFS) porownujące wszystkie możliwe drogi\n");
		//FindShortestPath(matrixGraph, config, RunDFS);

		delete matrixGraph;
		delete listGraph;
		delete graphForFloyd;
	}
	catch (const Exception& e) {
		delete matrixGraph;
		delete listGraph;
		delete graphForFloyd;
		HandleException(e);
	}
}
