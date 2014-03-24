/*
 * AMFGraph.cpp
 *
 *  Created on: 21-dec.-2013
 *      Author: Daan
 */

#include "AMFGraph.h"

AMFGraph::AMFGraph() {
	// TODO Auto-generated constructor stub
}

AMFGraph::~AMFGraph() {
	// TODO Auto-generated destructor stub
}

void AMFGraph::doNode(map<SmallBasicSet, set<SmallBasicSet> > g, SmallBasicSet n, set<SmallBasicSet> had) {
}

/* return a pointer to the edges */
edges_t* AMFGraph::getEdges() {
	return &edges;
}
