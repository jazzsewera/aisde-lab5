#include "Forest.h"
#include "Edge.h"

Forest::Forest(long num_of_vertices) : size(num_of_vertices) {
	subsetIndex = new long[size];
	subsetSize  = new long[size];
	successor   = new long[size];

	for (long i = 0; i < size; i++) {
		subsetIndex[i] = i;
		subsetSize[i]  = 1;
		successor[i]   = i;
	}
}

Forest::~Forest(void) {
	delete [] subsetIndex;
	delete [] subsetSize;
	delete [] successor;
}

bool Forest::inTheSameSubset(long v, long w) const {
	return subsetIndex[v] == subsetIndex[w];
}

// Operacja powszechnie znana pod nazwa 'Find'
long Forest::getSubsetIndex(long v) const {
	return subsetIndex[v];
}

long Forest::getSubsetSize(long v) const {
	return subsetSize[subsetIndex[v]];
}

void Forest::setSubsetSize(long v, long new_size) {
	subsetSize[subsetIndex[v]] = new_size;
}

// Operacja powszechnie znana pod nazwa 'Union'
bool Forest::addEdge(Edge *new_edge) {
	long v, w, subset_v, subset_w;

	v = (*new_edge).v;
	w = (*new_edge).w;
	// Jesli 'v' i 'w' sa w tym samym podzbiorze,
	// to nie dodajemy do niego krawedzi v-w, gdyz powstalby cykl.
	if (inTheSameSubset(v, w))
	   	return false;

	// W przeciwnym przypadku laczymy oba podzbiory dodajac krawedz v-w.
	else {
		// Niech wierzcholek 'v' bedzie tym nalezacym do wiekszego podzbioru
		if(getSubsetSize(w) > getSubsetSize(v))
			exchange(v, w);

		subset_v = subsetIndex[v];	// Indeks wiekszego podzbioru.
		subset_w = subsetIndex[w];	// Indeks mniejszego podzbioru.

		setSubsetSize(v,  getSubsetSize(v) + getSubsetSize(w));
		subsetIndex[subset_w] = subset_v; // Indeksem nowego podzbioru bedzie korzen wiekszego

		// Uaktualnij indeksy wszystkich wierzcholkow dolaczonego podzbioru
		for (long i = successor[subset_w]; i != subset_w; i = successor[i])
			subsetIndex[i] = subset_v;

		exchange(successor[subset_w], successor[subset_v]);
		return true;
	}
}

void Forest::exchange(long &x, long &y) {
	long tmp = x;
	x = y;
	y = tmp;
}
