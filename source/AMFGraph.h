/*
 * AMFGraph.h
 *
 *  Created on: 21-dec.-2013
 *      Author: Daan, Max
 */

#ifndef AMFGRAPH_H_
#define AMFGRAPH_H_

using namespace std;
#include <set>
#include <list>
#include "SmallBasicSet.h"
#include "AMFunction.h"
#include <iostream>
#include <utility>
#include <map>
#include <unordered_set>
#include <vector>

typedef map<SmallBasicSet,set<SmallBasicSet> > edges_t;
class AMFGraph {
private:
	edges_t edges;
public:
	AMFGraph(const AMFunction & r1, const AMFunction & r2);
	virtual ~AMFGraph();

	long count_connected();
    void do_node(SmallBasicSet* n, set<SmallBasicSet>* had);
};

#endif /* AMFGRAPH_H_ */
