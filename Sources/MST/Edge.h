#ifndef EDGE_H
#define EDGE_H

// Krawędź grafu. Funkcja 'compareEdgeWeigths()' ma postać
// wymaganą przez funkcję standardową 'qsort()'

class Edge {
	public:
		Edge(long _v, long _w, int _weight): v(_v), w(_w), weight(_weight) {}
		Edge(const Edge& e);
  	void print(void) const;
  	static int compareEdgeWeigths(const void *, const void *);

	long  v, w;    // Numery wierzchołków
  	int   weight;  // Waga krawędzi
};

#endif // EDGE_H
