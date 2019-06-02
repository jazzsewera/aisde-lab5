#include <iostream>

#include "MinpathUtils.h"
#include "HeapPQ.h"
#include "Exception.h"

HeapPQ::HeapPQ(int cap) : capacity(cap), size(cap) {
	if (capacity < 1)
		throw Exception(WrongCapacityException, "HeapPQ");

	vertexHeap = new Vertex[capacity+1];
	offsets    = new int[capacity];

	for (int i = 0; i < size; i++) {
		vertexHeap[i+1].index = i;
		vertexHeap[i+1].dist  = INF;
		offsets[i] = i+1;
	}
	vertexHeap[0].index = -1;
	vertexHeap[0].dist  = INF;
}

HeapPQ::~HeapPQ(void) {
	delete [] vertexHeap;
	delete [] offsets;
}

bool HeapPQ::notEmpty(void) const { return (size > 0); }

// Zmieniamy wagę wierzchołka o indeksie 'ind' na 'newWeight'.
void HeapPQ::decreaseKey(int ind, ulong newWeight) {
	int n = offsets[ind];
	while (n > 1 && newWeight < vertexHeap[parent(n)].dist) {
		vertexHeap[n] = vertexHeap[parent(n)];
		offsets[vertexHeap[n].index] = n;
		n = parent(n);
	}
 	vertexHeap[n].dist = newWeight;
 	vertexHeap[n].index = ind;
 	offsets[ind] = n;
}

// Zwraca wierzchołek o najmniejszej wadze i jednocześnie usuwa go z kolejki.
// ZAGADKA DLA DOCIEKLIWYCH: co się dzieje z ostatnim wierzchołkiem kopca ('last')
// podczas odtwarzania jego struktury?
Vertex HeapPQ::removeNearest(void) {
	int n, next;
	Vertex nearest, last;

	if (size == 1) {
		offsets[vertexHeap[size].index] = -1;
		return vertexHeap[size--];
	}

	nearest = vertexHeap[1];       // To jest wierzchołek o najmniejszej wadze...
	offsets[nearest.index] = -1; // ...usuwamy go z kolejki.
	last = vertexHeap[size--];     // A to ostatni wierzchołek w kolejce. Przyda się przy odtwarzaniu kopca.

   // Odtwarzamy strukturę kopca
	for (n = 1; leftChild(n) <= size; n = next) {
		next = leftChild(n);
		if (rightChild(n) <= size && vertexHeap[rightChild(n)].dist < vertexHeap[leftChild(n)].dist) {
			next = rightChild(n);
		}
		if (vertexHeap[next].dist < last.dist) {
			vertexHeap[n] = vertexHeap[next];
			offsets[vertexHeap[n].index] = n;
		}
		else
			break;
	}

	vertexHeap[n] = last;
	offsets[vertexHeap[n].index] = n;
	return nearest;
}

ulong HeapPQ::getDistance(int index) const { return vertexHeap[offsets[index]].dist; }

bool HeapPQ::contains(int ind) const { return (offsets[ind] > 0); }

void HeapPQ::print(void) const {
	for (int i = 1; i <= size; i++) {
		if (vertexHeap[i].dist != INF)
			std::cout << vertexHeap[i].dist << "\t";
		else
			std::cout << "-\t";
	}
	std::cout << std::endl;
}
