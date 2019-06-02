#include <iostream>

#include "Edge.h"

Edge::Edge(const Edge& e) : v(e.v), w(e.w), weight(e.weight) { }

void Edge::print(void) const {
	std::cout << " " << v << "\t" << w << "\t" << weight << "\n";
}

// Funkcja porownująca krawędzie według wagi
int Edge::compareEdgeWeigths(const void *e1, const void *e2) {
	int sign = (**(Edge**)e1).weight - (**(Edge**)e2).weight;
	if (sign > 0)
		return 1;
	if (sign < 0)
		return -1;
	return 0;
}
