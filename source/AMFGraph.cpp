/*
 * AMFGraph.cpp
 *
 *  Created on: 21-dec.-2013
 *      Author: Daan,Max
 */

#include "AMFGraph.h"

//AMFGraph::AMFGraph(AMFunction r1,AMFunction r2) {
//    edges = (AMFInterval(r1, r2)).graph().getEdges;
//}

AMFGraph::AMFGraph(edges_t g) {
    edges = g;
}

AMFGraph::AMFGraph() {
    //nothing
}



AMFGraph::~AMFGraph() {
	// TODO Auto-generated destructor stub
}

long AMFGraph::countConnected(edges_t g){
    set<SmallBasicSet> had;
    long ret =0;
    for (pair<SmallBasicSet, set<SmallBasicSet> > b : g) {
        if ((had.find(b.first)) == had.end()) {
            ret++;
            had.insert(b.first);
            doNode(g, b.first, had);
        }
    }
    return ret;
}

void AMFGraph::doNode(edges_t g, SmallBasicSet n, set<SmallBasicSet> had) {
    set<SmallBasicSet> set = (*(g.find(n))).second;
    for (SmallBasicSet b : set) {
        if (had.insert(b).second) {
            doNode(g, b, had);
        }
    }
}

/* return a pointer to the edges */
edges_t* AMFGraph::getEdges() {
	return &edges;
}
