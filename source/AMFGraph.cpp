/*
 * AMFGraph.cpp
 *
 *  Created on: 21-dec.-2013
 *      Author: Daan,Max
 */

#include "AMFGraph.h"

AMFGraph::AMFGraph(const AMFunction & r1, const AMFunction & r2) {
	for (SmallBasicSet r : r2.getSets()) {
		set<SmallBasicSet> cr;
		for (SmallBasicSet s : r2.getSets()) {
			if (!r1.ge(r.setintersect(s))) { cr.insert(s); };
		}
		edges.insert(make_pair(r,cr));
	}
}

AMFGraph::~AMFGraph() {
	// TODO Auto-generated destructor stub
}

long AMFGraph::count_connected() {
    set<SmallBasicSet> had;
    long ret = 0;
    for (pair<SmallBasicSet, set<SmallBasicSet> > b : edges) {
        if ((had.find(b.first)) == had.end()) {
            ++ret;
            had.insert(b.first);
            do_node(&(b.first), &had);
        }
    }
    return ret;
}

void AMFGraph::do_node(SmallBasicSet* n, set<SmallBasicSet>* had) {
    set<SmallBasicSet>& set = (*(edges.find(*n))).second;
    for (SmallBasicSet b : set) {
        if (had->insert(b).second) {
            do_node(&b, had);
        }
    }
}
