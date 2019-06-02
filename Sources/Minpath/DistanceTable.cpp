#include <iostream>

#include "DistanceTable.h"
#include "MinpathUtils.h"
#include "Exception.h"

DistanceTable::DistanceTable(int s) : size(s) {
	if (size < 1)
		throw Exception(WrongCapacityException, "DistanceTable");

	dists = new ulong[s];
	fixed = new bool[s];

	for(int i = 0; i < size; i++) {
		dists[i] = INF;
		fixed[i] = false;
	}
}

DistanceTable::~DistanceTable(void) {
	delete [] dists;
	delete [] fixed;
}

ulong DistanceTable::getDistance(int n) const { return dists[n]; }

// UWAGA! Zakładamy, że wierzchołek 'n' istnieje i że jego waga nie jest jeszcze ustalona.
// Ze względu na wydajność nie testujemy tych warunków wewnątrz funkcji.
void DistanceTable::setDistance(int n, ulong d) { dists[n] = d; }

// Znajduje numer wierzcholka o najmniejszej wadze (odleglosci od wierzch. poczatkowego).
int DistanceTable::getNearest(void) const {
	ulong tempMinDist = INF;
	int nearest = NO_VERTEX_FOUND;
	for (int i = 0; i < size; i++) {
		if (fixed[i] == false && dists[i] < tempMinDist) {
     		nearest = i;
  			tempMinDist = dists[nearest];
		}
	}
	return nearest;
}

// Czy waga wierzcholka 'n' jest ustalona?
bool DistanceTable::isFixed(int n) const { return fixed[n]; }

// Ustala wage wierzcholka 'n'
bool DistanceTable::fix(int n) {
	if(n >= size || fixed[n])
		return false;
	else {
 		fixed[n] = true;
 		return true;
   }
}

void DistanceTable::print(void) const {
	if (size > MAX_PRINTABLE_V)
		return;
	else {
		for (int i = 0; i < size; i++) {
			if (dists[i] != INF)
				std::cout << "\t" << dists[i];
			else
				std::cout << "\t-";
		}
		std::cout << "\n";
	}
}

void DistanceTable::check(void) const {
	for (int i = 0; i < size; i++) {
		std::cout << "Num: " << i << ", distance: " << dists[i];
  		std::cout << ", fixed: " << fixed[i] << "\n";
	}
}
