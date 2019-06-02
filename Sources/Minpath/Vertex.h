#ifndef VERTEX_H
#define VERTEX_H

// Wierzchołek jako element kopca (dla klasy HeapPQ)

typedef struct {
	int   index; // Numer (indeks) wierzchołka.
	ulong dist;  // Odległość wierzchołka od 'źródła'.
} Vertex;

#endif // VERTEX_H
