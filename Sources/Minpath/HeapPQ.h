#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "Vertex.h"

// Kolejka priorytetowa, zaimplementowana jako kopiec minimalny.
// Dodatkowa tablica 'offsets' pozwala w czasie O(1) zlokalizować w kopcu
// wierzchołek o podanym indeksie (indeksy są unikatowymi etykietami wierzchołków w grafie).

class HeapPQ {

	private:
		Vertex *vertexHeap; // Kopiec minimalny wierzchołków, gdzie kluczem jest odległość od źródła
		int    *offsets;    // 'offsets[n]' to pozycja n-go wierzchołka w 'vertexHeap'.
		int     capacity;   // Maksymalna pojemność kolejki.
		int     size;       // Aktualna liczba elementów w kolejce.
		int     parent(int n) const     { return n/2; }
		int     leftChild(int n) const  { return 2*n; }
		int     rightChild(int n) const { return 2*n+1; }

	public:
		HeapPQ(int capacity);
 		~HeapPQ();
 		bool    notEmpty(void) const;
		bool    contains(int index) const;    // Czy kolejka zawiera wierzchołek 'index'?
		ulong   getDistance(int index) const; // Zwraca wagę wierzchołka 'index'.
		Vertex  removeNearest(void);  // Zwraca wierzchołek o najmniejszej wadze, usuwając go jednocześnie z kolejki
		void    decreaseKey(int index, ulong newWeight);	// Uaktualnia wagę wierzchołka
		void    print(void) const;    // Wypisuje zawartość kolejki.
};

#endif // PRIORITY_QUEUE_H
